#pragma once
#include "json.hpp"
#include <string>
#include <sstream>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#pragma comment(lib, "rpcrt4.lib") // UuidCreate - Minimum supported OS Win 2000
#include <windows.h>

#define getString(json)      \
   std::stringstream __ss__; \
   __ss__ << json;           \
   return __ss__.str()

class guid
{
 private:
   static UUID *uuid;

 public:
   inline static UUID *get()
   {
      if (uuid)
         return uuid;
      else
      {
         uuid = new UUID;
         UuidCreate(uuid);
         return uuid;
      }
   }
};

namespace conversion
{
class string
{
   std::string data;

   std::string getSkinny(const std::wstring _data) const
   {
      return std::string(_data.cbegin(), _data.cend());
   }
   std::wstring getFat() const
   {
      return std::wstring(data.cbegin(), data.cend());
   }

   bool isEqual(const std::string &other) const
   {
      return data == other;
   }

   bool isEqual(const std::wstring &other) const
   {
      return getFat() == other;
   }

   bool isGreater(const std::string &rhs) const
   {
      return data > rhs;
   }
   bool isGreater(const std::wstring &rhs) const
   {
      return getFat() > rhs;
   }

 public:
   //default constructor:
   string() : data("") {}
   string(const std::string &_data) : data(_data) {}
   const char *c_str() { return data.c_str(); }
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
   string &operator=(const wchar_t *wc_str)
   {
      data = getSkinny(wc_str);
      return *this;
   }

   //conparison operator overloading
   bool operator==(const std::string &other) const
   {
      return isEqual(other);
   }
   bool operator==(const std::wstring &other) const
   {
      return isEqual(other);
   }
   bool operator==(const conversion::string &other) const
   {
      return this->data == other.data;
   }
   bool operator!=(const std::string &other) const
   {
      return !isEqual(other);
   }
   bool operator!=(const std::wstring &other) const
   {
      return !isEqual(other);
   }
   bool operator>(const std::string &rhs) const
   {
      return isGreater(rhs);
   }
   bool operator>(const std::wstring &rhs) const
   {
      return isGreater(rhs);
   }
   bool operator>(const conversion::string &rhs) const
   {
      return (this->data) > rhs.data;
   }
   bool operator<(const std::string &rhs) const
   {
      return !isGreater(rhs) && !isEqual(rhs);
   }
   bool operator<(const std::wstring &rhs) const
   {
      return !isGreater(rhs) && !isEqual(rhs);
   }
   bool operator<(const conversion::string &other) const
   {
      return this->data < other.data;
   }
   bool operator>=(const std::string &rhs) const
   {
      return isGreater(rhs) || isEqual(rhs);
   }
   bool operator>=(const std::wstring &rhs) const
   {
      return isGreater(rhs) || isEqual(rhs);
   }
   bool operator<=(const std::string &rhs) const
   {
      return !isGreater(rhs);
   }
   bool operator<=(const std::wstring &rhs) const
   {
      return !isGreater(rhs);
   }
};
} // namespace conversion

namespace MockCom
{
class MockDeviceEnumerator : public IMMDeviceEnumerator
{
 public:
   virtual ULONG AddRef() { return -1; }
   virtual ULONG Release() { return -1; }
   virtual HRESULT QueryInterface(const IID &riid, void **ppvObject)
   {
      *ppvObject = NULL;
      return E_NOTIMPL;
   }
   virtual HRESULT EnumAudioEndpoints(EDataFlow dataFlow, DWORD dwStateMask, IMMDeviceCollection **ppDevices)
   {
      *ppDevices = NULL;
      return E_NOTIMPL;
   }
   virtual HRESULT GetDefaultAudioEndpoint(EDataFlow dataFlow, ERole role, IMMDevice **ppEndpoint)
   {
      *ppEndpoint = NULL;
      return E_NOTIMPL;
   }
   virtual HRESULT GetDevice(LPCWSTR pwstrId, IMMDevice **ppDevice)
   {
      *ppDevice = NULL;
      return E_NOTIMPL;
   }
   virtual HRESULT RegisterEndpointNotificationCallback(IMMNotificationClient *pClient) { return E_NOTIMPL; }
   virtual HRESULT UnregisterEndpointNotificationCallback(IMMNotificationClient *pClient) { return E_NOTIMPL; }
};

class MockDeviceCollection : public IMMDeviceCollection
{
 public:
   virtual ULONG AddRef() { return -1; }
   virtual ULONG Release() { return -1; }
   virtual HRESULT QueryInterface(const IID &riid, void **ppvObject)
   {
      *ppvObject = NULL;
      return E_NOTIMPL;
   }
   virtual HRESULT GetCount(UINT *pcDevices) { return E_NOTIMPL; }
   virtual HRESULT Item(UINT nDevice, IMMDevice **ppDevice)
   {
      *ppDevice = NULL;
      return E_NOTIMPL;
   }
};

class MockDevice : public IMMDevice
{
 public:
   virtual ULONG AddRef() { return -1; }
   virtual ULONG Release() { return -1; }
   virtual HRESULT QueryInterface(const IID &riid, void **ppvObject)
   {
      *ppvObject = NULL;
      return E_NOTIMPL;
   }
   virtual HRESULT Activate(REFIID iid, DWORD dwClsCtx, PROPVARIANT *pActivationParams, void **ppInterface)
   {
      *ppInterface = NULL;
      return E_NOTIMPL;
   }
   virtual HRESULT OpenPropertyStore(DWORD stgmAccess, IPropertyStore **ppProperties)
   {
      *ppProperties = NULL;
      return E_NOTIMPL;
   }
   virtual HRESULT GetId(LPWSTR *ppstrId) { return E_NOTIMPL; }
   virtual HRESULT GetState(DWORD *pdwState) { return E_NOTIMPL; }
};

class MockPropertyStore : public IPropertyStore
{
 public:
   virtual ULONG AddRef() { return -1; }
   virtual ULONG Release() { return -1; }
   virtual HRESULT QueryInterface(const IID &riid, void **ppvObject)
   {
      *ppvObject = NULL;
      return E_NOTIMPL;
   }
   virtual HRESULT GetCount(DWORD *cProps) { return E_NOTIMPL; }
   virtual HRESULT GetAt(DWORD iProp, PROPERTYKEY *pkey) { return E_NOTIMPL; }
   virtual HRESULT GetValue(REFPROPERTYKEY key, PROPVARIANT *pv) { return E_NOTIMPL; }
   virtual HRESULT SetValue(REFPROPERTYKEY key, REFPROPVARIANT propvar) { return E_NOTIMPL; }
   virtual HRESULT Commit(void) { return E_NOTIMPL; }
};

class MockAudioEndpointVolume : public IAudioEndpointVolume
{
 public:
   virtual ULONG AddRef() { return -1; }
   virtual ULONG Release() { return -1; }
   virtual HRESULT QueryInterface(const IID &riid, void **ppvObject)
   {
      *ppvObject = NULL;
      return E_NOTIMPL;
   }
   virtual HRESULT RegisterControlChangeNotify(IAudioEndpointVolumeCallback *pNotify) { return E_NOTIMPL; }
   virtual HRESULT UnregisterControlChangeNotify(IAudioEndpointVolumeCallback *pNotify) { return E_NOTIMPL; }
   virtual HRESULT GetChannelCount(UINT *pnChannelCount) { return E_NOTIMPL; }
   virtual HRESULT SetMasterVolumeLevel(float fLevelDB, LPCGUID pguidEventContext) { return E_NOTIMPL; }
   virtual HRESULT SetMasterVolumeLevelScalar(float fLevel, LPCGUID pguidEventContext) { return E_NOTIMPL; }
   virtual HRESULT GetMasterVolumeLevel(float *pfLevelDB) { return E_NOTIMPL; }
   virtual HRESULT GetMasterVolumeLevelScalar(float *pfLevel) { return E_NOTIMPL; }
   virtual HRESULT SetChannelVolumeLevel(UINT nChannel, float fLevelDB, LPCGUID pguidEventContext) { return E_NOTIMPL; }
   virtual HRESULT SetChannelVolumeLevelScalar(UINT nChannel, float fLevel, LPCGUID pguidEventContext) { return E_NOTIMPL; }
   virtual HRESULT GetChannelVolumeLevel(UINT nChannel, float *pfLevelDB) { return E_NOTIMPL; }
   virtual HRESULT GetChannelVolumeLevelScalar(UINT nChannel, float *pfLevel) { return E_NOTIMPL; }
   virtual HRESULT SetMute(BOOL bMute, LPCGUID pguidEventContext) { return E_NOTIMPL; }
   virtual HRESULT GetMute(BOOL *pbMute) { return E_NOTIMPL; }
   virtual HRESULT GetVolumeStepInfo(UINT *pnStep, UINT *pnStepCount) { return E_NOTIMPL; }
   virtual HRESULT VolumeStepUp(LPCGUID pguidEventContext) { return E_NOTIMPL; }
   virtual HRESULT VolumeStepDown(LPCGUID pguidEventContext) { return E_NOTIMPL; }
   virtual HRESULT QueryHardwareSupport(DWORD *pdwHardwareSupportMask) { return E_NOTIMPL; }
   virtual HRESULT GetVolumeRange(float *pflVolumeMindB, float *pflVolumeMaxdB, float *pflVolumeIncrementdB) { return E_NOTIMPL; }
};

} // namespace MockCom