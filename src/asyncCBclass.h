#pragma once
#include <uv.h>
#include <mmdeviceapi.h>
#include <functiondiscoverykeys_devpkey.h>
#include <string>
#include <list>
#include <queue>
#include "common.h"

template <typename T>
class Async
{
   uv_async_t _handle;
   uv_mutex_t _mutex;
   std::list<T> _buffer;

   static inline void uv_callback(uv_async_t *handle)
   {
      //get context from handle and call _sendNotifications();
      auto *worker = static_cast<Async *>(handle->data);
      worker->_sendNotifications();
   }

   static inline void _destroy(uv_async_t *handle)
   {
      auto *worker = static_cast<Async *>(handle->data);
      worker->destroy();
      delete worker;
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
   //should be called as the destructor for every class.
   //think of this as a thread-safe destructor
   virtual void destroy() = 0;

   virtual void HandleAsyncCallback(std::queue<T>) = 0;

   virtual void SendInfo(T t)
   {
      uv_mutex_lock(&_mutex);
      _buffer.push_back(t);
      uv_mutex_unlock(&_mutex);

      //signals node's event loop that there is data to be processed
      uv_async_send(&this->_handle);
   }

 public:
   Async()
   {
      uv_mutex_init(&_mutex);
      _handle.data = this;
      uv_async_init(uv_default_loop(), &_handle, uv_callback);
   }

   void Destroy()
   {
      uv_close(&_handle, _destroy)
   }
};

// typedef struct _GUID {
//     unsigned long  Data1;
//     unsigned short Data2;
//     unsigned short Data3;
//     unsigned char  Data4[ 8 ];
// } GUID;

// typedef struct _tagpropertykey
//     {
//     GUID fmtid;
//     DWORD pid;
//     } 	PROPERTYKEY;

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
class Info
{
 private:
   std::string getType()
   {
      switch (type)
      {
      case DefaultChanged:
         return "defaultChanged";
      case DeviceAdded:
         return "deviceAdded";
      case DeviceRemoved:
         return "deviceRemoved";
      case StateChange:
         return "stateChange";
      case PropertyChange:
         return "propertyChange";
      }
      return "undefined";
   }

 public:
   conversion::string id;
   EDataFlow flow;
   ERole role;
   DWORD state;
   MessageType type;
   PROPERTYKEY key;

   std::string toJsonString()
   {
      using json = nlohmann::json;
      json j = {
          {"id", id},
          {"name", IWinAPI::getDefault().} {"flow", flow},
          {"role", role},
          {"state", state},
          {"type", getType()},
          {"pid", key.pid},
          {"data1", key.fmtid.Data1},
          {"data2", key.fmtid.Data2},
          {"data3", key.fmtid.Data3},
          {"data4", key.fmtid.Data4},
      };

      return j.dump(0);
   }
};
} // namespace __IMMNotificationClient__

class WinAudioNotificationClientBase : public IMMNotificationClient, protected Async<__IMMNotificationClient__::Info>
{

   LONG _cRef;
   IMMDeviceEnumerator *_pEnumerator;
   //uv_async_t asyncHandle;
   //uv_mutex_t _mutex;
   //std::list<std::string> notificationBuffer;

   //called by node's event loop
   //static inline void asyncCallback(uv_async_t *handle)
   //{
   //    auto *worker = static_cast<WinAudioNotificationClientBase*>(handle->data);
   //    worker->_SendNotifications(); //since this is static, we must get the context (object) and call its _SendNotifications()
   //}

   //void _SendNotifications();

   enum MessageType
   {
      DefaultChanged,
      DeviceAdded,
      DeviceRemoved,
      StateChange,
      PropertyChange
   };

 protected:
   //needs to instanciate
   //virtual void HandleAsyncCallback(std::queue<std::string> notifications) = 0;
   //bool isEnrolled;
   //uv_mutex_t _enrolledMutex;
   //void SendNotification(Info info);

 public:
   WinAudioNotificationClientBase() : _cRef(1),
                                      _pEnumerator(NULL) //,
   //                                   isEnrolled(true)
   {

      //    auto success = uv_mutex_init(&_mutex);
      //    success = uv_mutex_init(&_enrolledMutex);
      //    asyncHandle.data = this;
      //    uv_async_init(uv_default_loop(), &asyncHandle, asyncCallback);
   }

   ~WinAudioNotificationClientBase();

   void setEnum(IMMDeviceEnumerator *pEnum) { _pEnumerator = pEnum; }

   // IUnknown methods -- AddRef, Release, and QueryInterface

   ULONG STDMETHODCALLTYPE AddRef() { return InterlockedIncrement(&_cRef); }

   ULONG STDMETHODCALLTYPE Release();

   HRESULT STDMETHODCALLTYPE QueryInterface(
       REFIID riid, VOID **ppvInterface);

   // Callback methods for device-event notifications.

   HRESULT STDMETHODCALLTYPE OnDefaultDeviceChanged(EDataFlow flow, ERole role, LPCWSTR pwstrDeviceId)
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

   HRESULT STDMETHODCALLTYPE OnDeviceAdded(LPCWSTR pwstrDeviceId)
   {
      namespace IMN = __IMMNotificationClient__;
      IMN::Info toSend = {};
      toSend.type = IMN::DeviceAdded;
      toSend.id = pwstrDeviceId;

      SendInfo(toSend);

      return S_OK;
   }

   HRESULT STDMETHODCALLTYPE OnDeviceRemoved(LPCWSTR pwstrDeviceId)
   {
      namespace IMN = __IMMNotificationClient__;
      IMN::Info toSend = {};
      toSend.type = IMN::DeviceRemoved;
      toSend.id = pwstrDeviceId;

      SendInfo(toSend);

      return S_OK;
   }

   HRESULT STDMETHODCALLTYPE OnDeviceStateChanged(LPCWSTR pwstrDeviceId, DWORD dwNewState)
   {
      namespace IMN = __IMMNotificationClient__;
      IMN::Info toSend = {};
      toSend.type = IMN::StateChange;
      toSend.id = pwstrDeviceId;
      toSend.state = dwNewState;

      SendInfo(toSend);

      return S_OK;
   }

   HRESULT STDMETHODCALLTYPE OnPropertyValueChanged(LPCWSTR pwstrDeviceId, const PROPERTYKEY key)
   {
      namespace IMN = __IMMNotificationClient__;
      IMN::Info toSend = {};
      toSend.type = IMN::PropertyChange;
      toSend.id = pwstrDeviceId;
      toSend.key = key;

      SendInfo(toSend);

      return S_OK;
   }
};