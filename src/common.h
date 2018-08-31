#pragma once
#include "json.hpp"
#include <string>
#include <sstream>
#include <mmdeviceapi.h>
#include <endpointvolume.h>

#define getString(json)       \
    std::stringstream __ss__; \
    __ss__ << json;           \
    return __ss__.str()

namespace conversion
{
   class string
   {
      std::string data;

      std::string getSkinny(std::wstring _data)
      {
         return std::string(_data.begin(), _data.end());
      }
      std::wstring getFat() const
      {
         return std::wstring(data.begin(), data.end());
      }

      bool isEqual(const std::string &other)
      {
         return data == other;
      }

      bool isEqual(const std::wstring &other)
      {
         return getFat() == other;
      }

      bool isGreater(const std::string &rhs)
      {
         return data > rhs;
      }
      bool isGreater(const std::wstring &rhs)
      {
         return getFat() > rhs;
      }

   public:
      //default constructor:
      string() : data("") {}
      string(const std::string &_data) : data(_data) {}
      string &operator=(const std::string _data)
      {
         data = _data;
         return *this;
      }
      operator std::string() const { return data; }
      string &operator=(const char c_str[])
      {
         data = c_str;
         return *this;
      }

      string(const std::wstring &_data) : data(getSkinny(_data)) {}
      string &operator=(const std::wstring _data)
      {
         data = getSkinny(_data);
         return *this;
      }
      operator std::wstring() const { return getFat(); }
      string &operator=(const wchar_t * wc_str)
      {
         data = getSkinny(wc_str);
         return *this;
      }

      //conparison operator overloading
      bool operator==(const std::string &other)
      {
         return isEqual(other);
      }
      bool operator==(const std::wstring &other)
      {
         return isEqual(other);
      }
      bool operator!=(const std::string &other)
      {
         return !isEqual(other);
      }
      bool operator!=(const std::wstring &other)
      {
         return !isEqual(other);
      }
      bool operator>(const std::string &rhs)
      {
         return isGreater(rhs);
      }
      bool operator>(const std::wstring &rhs)
      {
         return isGreater(rhs);
      }
      bool operator<(const std::string &rhs)
      {
         return !isGreater(rhs) && !isEqual(rhs);
      }
      bool operator<(const std::wstring &rhs)
      {
         return !isGreater(rhs) && !isEqual(rhs);
      }
      bool operator>=(const std::string &rhs)
      {
         return isGreater(rhs) || isEqual(rhs);
      }
      bool operator>=(const std::wstring &rhs)
      {
         return isGreater(rhs) || isEqual(rhs);
      }
      bool operator<=(const std::string &rhs)
      {
         return !isGreater(rhs);
      }
      bool operator<=(const std::wstring &rhs)
      {
         return !isGreater(rhs);
      }
   };
} // namespace conversion

namespace MockCom
   {
      class MockDeviceEnumerator : public IMMDeviceEnumerator {
      public:
         virtual ULONG AddRef() { return -1; }
         virtual ULONG Release() { return -1; }
         virtual HRESULT QueryInterface(const IID &riid, void ** ppvObject) { *ppvObject = NULL; return CO_E_NOTINITIALIZED; }
         virtual HRESULT EnumAudioEndpoints(EDataFlow dataFlow, DWORD dwStateMask, IMMDeviceCollection **ppDevices) { *ppDevices = NULL; return CO_E_NOTINITIALIZED; }
         virtual HRESULT GetDefaultAudioEndpoint(EDataFlow dataFlow, ERole role, IMMDevice **ppEndpoint) { *ppEndpoint = NULL; return CO_E_NOTINITIALIZED; }
         virtual HRESULT GetDevice(LPCWSTR pwstrId, IMMDevice **ppDevice) { *ppDevice = NULL; return CO_E_NOTINITIALIZED; }
         virtual HRESULT RegisterEndpointNotificationCallback( IMMNotificationClient *pClient) { return CO_E_NOTINITIALIZED; }
         virtual HRESULT UnregisterEndpointNotificationCallback( IMMNotificationClient *pClient) { return CO_E_NOTINITIALIZED; }
      };

      class MockDeviceCollection : public IMMDeviceCollection {
      public:
         virtual ULONG AddRef() { return -1; }
         virtual ULONG Release() { return -1; }
         virtual HRESULT QueryInterface(const IID &riid, void ** ppvObject) { *ppvObject = NULL; return CO_E_NOTINITIALIZED; }
         virtual HRESULT GetCount( UINT *pcDevices) { return CO_E_NOTINITIALIZED; }
         virtual HRESULT Item(UINT nDevice, IMMDevice **ppDevice) { *ppDevice = NULL; return CO_E_NOTINITIALIZED; }
      };

      class MockDevice : public IMMDevice {
      public:
         virtual ULONG AddRef() { return -1; }
         virtual ULONG Release() { return -1; }
         virtual HRESULT QueryInterface(const IID &riid, void ** ppvObject) { *ppvObject = NULL; return CO_E_NOTINITIALIZED; }
         virtual HRESULT Activate(REFIID iid, DWORD dwClsCtx, PROPVARIANT *pActivationParams, void **ppInterface) { *ppInterface = NULL; return CO_E_NOTINITIALIZED; }
         virtual HRESULT OpenPropertyStore(DWORD stgmAccess, IPropertyStore **ppProperties) { *ppProperties = NULL; return CO_E_NOTINITIALIZED; }
         virtual HRESULT GetId( LPWSTR *ppstrId) { return CO_E_NOTINITIALIZED; }
         virtual HRESULT STDMETHODCALLTYPE GetState( DWORD *pdwState) { return CO_E_NOTINITIALIZED; }
      };

      class MockPropertyStore : public IPropertyStore {
      public:
         virtual ULONG AddRef() { return -1; }
         virtual ULONG Release() { return -1; }
         virtual HRESULT QueryInterface(const IID &riid, void ** ppvObject) { *ppvObject = NULL; return CO_E_NOTINITIALIZED; }
         virtual HRESULT GetCount( DWORD *cProps) { return CO_E_NOTINITIALIZED; }
         virtual HRESULT GetAt( DWORD iProp, PROPERTYKEY *pkey) { return CO_E_NOTINITIALIZED; }
         virtual HRESULT GetValue( REFPROPERTYKEY key, PROPVARIANT *pv) { return CO_E_NOTINITIALIZED; }
         virtual HRESULT SetValue( REFPROPERTYKEY key, REFPROPVARIANT propvar) { return CO_E_NOTINITIALIZED; }
         virtual HRESULT Commit(void) { return CO_E_NOTINITIALIZED; }
      };

      class MockAudioEndpointVolume : public IAudioEndpointVolume {
      public:
         virtual ULONG AddRef() { return -1; }
         virtual ULONG Release() { return -1; }
         virtual HRESULT QueryInterface(const IID &riid, void ** ppvObject) { *ppvObject = NULL; return CO_E_NOTINITIALIZED; }
         virtual HRESULT RegisterControlChangeNotify(IAudioEndpointVolumeCallback *pNotify) { return CO_E_NOTINITIALIZED; }
         virtual HRESULT UnregisterControlChangeNotify(IAudioEndpointVolumeCallback *pNotify) { return CO_E_NOTINITIALIZED; }
         virtual HRESULT GetChannelCount(UINT *pnChannelCount) { return CO_E_NOTINITIALIZED; }
         virtual HRESULT SetMasterVolumeLevel(float fLevelDB, LPCGUID pguidEventContext) { return CO_E_NOTINITIALIZED; }
         virtual HRESULT SetMasterVolumeLevelScalar(float fLevel, LPCGUID pguidEventContext) { return CO_E_NOTINITIALIZED; }
         virtual HRESULT GetMasterVolumeLevel(float *pfLevelDB) { return CO_E_NOTINITIALIZED; }
         virtual HRESULT GetMasterVolumeLevelScalar(float *pfLevel) { return CO_E_NOTINITIALIZED; }
         virtual HRESULT SetChannelVolumeLevel(UINT nChannel, float fLevelDB, LPCGUID pguidEventContext) { return CO_E_NOTINITIALIZED; }
         virtual HRESULT SetChannelVolumeLevelScalar(UINT nChannel, float fLevel, LPCGUID pguidEventContext) { return CO_E_NOTINITIALIZED; }
         virtual HRESULT GetChannelVolumeLevel(UINT nChannel, float *pfLevelDB) { return CO_E_NOTINITIALIZED; }
         virtual HRESULT GetChannelVolumeLevelScalar(UINT nChannel, float *pfLevel) { return CO_E_NOTINITIALIZED; }
         virtual HRESULT SetMute(BOOL bMute, LPCGUID pguidEventContext) { return CO_E_NOTINITIALIZED; }
         virtual HRESULT GetMute(BOOL *pbMute) { return CO_E_NOTINITIALIZED; }
         virtual HRESULT GetVolumeStepInfo(UINT *pnStep, UINT *pnStepCount) { return CO_E_NOTINITIALIZED; }
         virtual HRESULT VolumeStepUp(LPCGUID pguidEventContext) { return CO_E_NOTINITIALIZED; }
         virtual HRESULT VolumeStepDown(LPCGUID pguidEventContext) { return CO_E_NOTINITIALIZED; }
         virtual HRESULT QueryHardwareSupport(DWORD *pdwHardwareSupportMask) { return CO_E_NOTINITIALIZED; }
         virtual HRESULT GetVolumeRange(float *pflVolumeMindB, float *pflVolumeMaxdB, float *pflVolumeIncrementdB) { return CO_E_NOTINITIALIZED; }
      };


   }