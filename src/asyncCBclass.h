#pragma once
#include <uv.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <functiondiscoverykeys_devpkey.h>
#include <string>
#include <list>
#include <queue>
#include "common.h"
#include "WinAPIWrap.h"
#pragma comment(lib, "rpcrt4.lib") // UuidCompare - Minimum supported OS Win 2000
#include <windows.h>

template <typename T>
class Async
{
   uv_async_t _handle;
   uv_mutex_t _mutex;
   std::list<T> _buffer;
   bool notReady;

   static inline void uv_callback(uv_async_t *handle)
   {
      //get context from handle and call _sendNotifications();
      auto *worker = static_cast<Async *>(handle->data);
      worker->_sendNotifications();
   }

   static inline void _destroy(uv_handle_t *handle)
   {
      //kill your callbacks
      auto *worker = static_cast<Async *>(handle->data);
      worker->destroyCallback();
      worker->notReady = false;
   }

   void _sendNotifications()
   {
      std::queue<T> toSend;
      uv_mutex_lock(&_mutex);
      for (T t : _buffer)
      {
         toSend.push(t);
      }
      _buffer.clear();
      uv_mutex_unlock(&_mutex);

      this->HandleAsyncCallback(toSend);
   }

 protected:
   virtual void HandleAsyncCallback(std::queue<T>) = 0;
   virtual void destroyCallback() = 0;

   virtual void SendInfo(T t)
   {
      uv_mutex_lock(&_mutex);
      _buffer.push_back(t);
      uv_mutex_unlock(&_mutex);

      //signals node's event loop that there is data to be processed
      uv_async_send(&this->_handle);
   }

 public:
   Async() : notReady(true)
   {
      uv_mutex_init(&_mutex);
      _handle.data = this;
      uv_async_init(uv_default_loop(), &_handle, uv_callback);
   }

   ~Async()
   {
      uv_close((uv_handle_t *)&_handle, _destroy);

      while (notReady)
         ; // we will spin our wheels for a while
   }
};

namespace __IMMNotificationClient__
{
enum MessageType
{
   DefaultChanged,
   DeviceAdded,
   DeviceRemoved,
   StateChange,
   PropertyChange
};
struct Info
{
   conversion::string id;
   EDataFlow flow;
   ERole role;
   DWORD state;
   MessageType type;
   PROPERTYKEY key;
};
} // namespace __IMMNotificationClient__

class WinDeviceNotificationClientBase : public IMMNotificationClient, protected Async<__IMMNotificationClient__::Info>
{

   LONG _cRef;
   IMMDeviceEnumerator *_pEnumerator;

 protected:

 public:
   WinDeviceNotificationClientBase() : _cRef(1),
                                      _pEnumerator(NULL) 
   {
   }

   ~WinDeviceNotificationClientBase();

   void setEnum(IMMDeviceEnumerator *pEnum) { _pEnumerator = pEnum; }

   // IUnknown methods -- AddRef, Release, and QueryInterface

   ULONG AddRef() { return InterlockedIncrement(&_cRef); }

   ULONG Release();

   HRESULT QueryInterface(
       REFIID riid, VOID **ppvInterface);

   // Callback methods for device-event notifications.

   HRESULT OnDefaultDeviceChanged(EDataFlow flow, ERole role, LPCWSTR pwstrDeviceId)
   {
      namespace IMN = __IMMNotificationClient__;
      IMN::Info toSend = {};
      toSend.type = IMN::DefaultChanged;
      toSend.flow = flow;
      toSend.role = role;
      toSend.id = pwstrDeviceId;

      //SendNotification(toSend);
      SendInfo(toSend);
      return S_OK;
   }

   HRESULT OnDeviceAdded(LPCWSTR pwstrDeviceId)
   {

      return S_OK;
   }

   HRESULT OnDeviceRemoved(LPCWSTR pwstrDeviceId)
   {
      return S_OK;
   }

   HRESULT OnDeviceStateChanged(LPCWSTR pwstrDeviceId, DWORD dwNewState)
   {
      namespace IMN = __IMMNotificationClient__;
      IMN::Info toSend = {};
      toSend.type = IMN::StateChange;
      toSend.id = pwstrDeviceId;
      toSend.state = dwNewState;

      SendInfo(toSend);

      return S_OK;
   }

   HRESULT OnPropertyValueChanged(LPCWSTR pwstrDeviceId, const PROPERTYKEY key)
   {
      return S_OK;
   }
};

namespace __IAudioEndpointVolumeCallback__
{
struct Info
{
   bool muted;
   bool fromHere;
   float volume;
};
} // namespace __IAudioEndpointVolumeCallback__

class WinVolumeNotificationClientBase : public IAudioEndpointVolumeCallback, protected Async<__IAudioEndpointVolumeCallback__::Info>
{
 protected:
   WinAPIWrap::IAudioEndpointVolumePtr volumePtr;

 public:
   conversion::string deviceId;
   WinVolumeNotificationClientBase(conversion::string _deviceId);
   ~WinVolumeNotificationClientBase();

   ULONG AddRef()
   {
      return (ULONG)0;
   }

   ULONG STDMETHODCALLTYPE Release()
   {
      return 0;
   }

   HRESULT QueryInterface(REFIID riid, VOID **ppvInterface)
   {
      return E_NOTIMPL;
   }

   HRESULT OnNotify(PAUDIO_VOLUME_NOTIFICATION_DATA data)
   {
      if (!data)
         return E_POINTER;
      bool originatesFromThisProcess = false;

      RPC_STATUS status;
      if (UuidCompare(&data->guidEventContext, guid::get(), &status) == 0)
         originatesFromThisProcess = true;

      __IAudioEndpointVolumeCallback__::Info info = {(bool)data->bMuted, originatesFromThisProcess, data->fMasterVolume};

      SendInfo(info);
      return S_OK;
   }
};