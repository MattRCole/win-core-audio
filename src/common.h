#pragma once
#include "json.hpp"
#include <string>
#include <sstream>
#include <mmdeviceapi.h>

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
         virtual HRESULT QueryInterface(const IID &riid, void ** ppvObject) { *ppvObject = NULL; return E_POINTER; }
         virtual HRESULT EnumAudioEndpoints(EDataFlow dataFlow, DWORD dwStateMask, IMMDeviceCollection **ppDevices) { *ppDevices = NULL; return E_POINTER; }
         virtual HRESULT GetDefaultAudioEndpoint(EDataFlow dataFlow, ERole role, IMMDevice **ppEndpoint) { *ppEndpoint = NULL; return E_POINTER; }
         virtual HRESULT GetDevice(LPCWSTR pwstrId, IMMDevice **ppDevice) { *ppDevice = NULL; return E_POINTER; }
         virtual HRESULT RegisterEndpointNotificationCallback( IMMNotificationClient *pClient) { return E_POINTER; }
         virtual HRESULT UnregisterEndpointNotificationCallback( IMMNotificationClient *pClient) { return E_POINTER; }
      };

      class MockDeviceCollection : public IMMDeviceCollection {
      public:
         virtual ULONG AddRef() { return -1; }
         virtual ULONG Release() { return -1; }
         virtual HRESULT QueryInterface(const IID &riid, void ** ppvObject) { *ppvObject = NULL; return E_POINTER; }
         virtual HRESULT GetCount( UINT *pcDevices) { return E_POINTER; }
         virtual HRESULT Item(UINT nDevice, IMMDevice **ppDevice) { *ppDevice = NULL; return E_POINTER; }
      };

      class MockDevice : public IMMDevice {
      public:
         virtual ULONG AddRef() { return -1; }
         virtual ULONG Release() { return -1; }
         virtual HRESULT QueryInterface(const IID &riid, void ** ppvObject) { *ppvObject = NULL; return E_POINTER; }
         virtual HRESULT Activate(REFIID iid, DWORD dwClsCtx, PROPVARIANT *pActivationParams, void **ppInterface) { *ppInterface = NULL; return E_POINTER; }
         virtual HRESULT OpenPropertyStore(DWORD stgmAccess, IPropertyStore **ppProperties) { *ppProperties = NULL; return E_POINTER; }
         virtual HRESULT GetId( LPWSTR *ppstrId) { return E_POINTER; }
         virtual HRESULT STDMETHODCALLTYPE GetState( DWORD *pdwState) { return E_POINTER; }
      };

      class MockPropertyStore : public IPropertyStore {
      public:
         virtual ULONG AddRef() { return -1; }
         virtual ULONG Release() { return -1; }
         virtual HRESULT QueryInterface(const IID &riid, void ** ppvObject) { *ppvObject = NULL; return E_POINTER; }
         virtual HRESULT GetCount( DWORD *cProps) { return E_POINTER; }
         virtual HRESULT GetAt( DWORD iProp, PROPERTYKEY *pkey) { return E_POINTER; }
         virtual HRESULT GetValue( REFPROPERTYKEY key, PROPVARIANT *pv) { return E_POINTER; }
         virtual HRESULT SetValue( REFPROPERTYKEY key, REFPROPVARIANT propvar) { return E_POINTER; }
         virtual HRESULT Commit(void) { return E_POINTER; }
      };

      class MockAudioEndpointVolume : public IAudioEndpointVolume {
      public:
         virtual ULONG AddRef() { return -1; }
         virtual ULONG Release() { return -1; }
         virtual HRESULT QueryInterface(const IID &riid, void ** ppvObject) { *ppvObject = NULL; return E_POINTER; }
         virtual HRESULT RegisterControlChangeNotify(IAudioEndpointVolumeCallback *pNotify) { return E_POINTER; }
         virtual HRESULT UnregisterControlChangeNotify(IAudioEndpointVolumeCallback *pNotify) { return E_POINTER; }
         virtual HRESULT GetChannelCount(UINT *pnChannelCount) { return E_POINTER; }
         virtual HRESULT SetMasterVolumeLevel(float fLevelDB, LPCGUID pguidEventContext) { return E_POINTER; }
         virtual HRESULT SetMasterVolumeLevelScalar(float fLevel, LPCGUID pguidEventContext) { return E_POINTER; }
         virtual HRESULT GetMasterVolumeLevel(float *pfLevelDB) { return E_POINTER; }
         virtual HRESULT GetMasterVolumeLevelScalar(float *pfLevel) { return E_POINTER; }
         virtual HRESULT SetChannelVolumeLevel(UINT nChannel, float fLevelDB, LPCGUID pguidEventContext) { return E_POINTER; }
         virtual HRESULT SetChannelVolumeLevelScalar(UINT nChannel, float fLevel, LPCGUID pguidEventContext) { return E_POINTER; }
         virtual HRESULT GetChannelVolumeLevel(UINT nChannel, float *pfLevelDB) { return E_POINTER; }
         virtual HRESULT GetChannelVolumeLevelScalar(UINT nChannel, float *pfLevel) { return E_POINTER; }
         virtual HRESULT SetMute(BOOL bMute, LPCGUID pguidEventContext) { return E_POINTER; }
         virtual HRESULT GetMute(BOOL *pbMute) { return E_POINTER; }
         virtual HRESULT GetVolumeStepInfo(UINT *pnStep, UINT *pnStepCount) { return E_POINTER; }
         virtual HRESULT VolumeStepUp(LPCGUID pguidEventContext) { return E_POINTER; }
         virtual HRESULT VolumeStepDown(LPCGUID pguidEventContext) { return E_POINTER; }
         virtual HRESULT QueryHardwareSupport(DWORD *pdwHardwareSupportMask) { return E_POINTER; }
         virtual HRESULT GetVolumeRange(float *pflVolumeMindB, float *pflVolumeMaxdB, float *pflVolumeIncrementdB) { return E_POINTER; }
      };
   }