#include "asyncCBclass.h"
#include "WinAPIWrap.h"

WinDeviceNotificationClientBase::~WinDeviceNotificationClientBase()
{
   if (_pEnumerator != NULL)
   {
      _pEnumerator->Release();
      _pEnumerator = NULL;
   }
}

ULONG WinDeviceNotificationClientBase::Release()
{
   ULONG ulRef = InterlockedDecrement(&_cRef);
   if (0 == ulRef)
   {
      delete this;
   }
   return ulRef;
}

HRESULT WinDeviceNotificationClientBase::QueryInterface(REFIID riid, VOID **ppvInterface)
{
   if (IID_IUnknown == riid)
   {
      AddRef();
      *ppvInterface = (IUnknown *)this;
   }
   else if (__uuidof(IMMNotificationClient) == riid)
   {
      AddRef();
      *ppvInterface = (IMMNotificationClient *)this;
   }
   else
   {
      *ppvInterface = NULL;
      return E_NOINTERFACE;
   }
   return S_OK;
}

WinVolumeNotificationClientBase::WinVolumeNotificationClientBase(conversion::string _deviceId)
{
   deviceId = _deviceId;

   WinAPIWrap::InjectionFramework::ComInitialize();

   volumePtr = WinAPIWrap::getEndpointVolume(deviceId);

   auto hr = volumePtr->RegisterControlChangeNotify(this);
}

WinVolumeNotificationClientBase::~WinVolumeNotificationClientBase()
{
   WinAPIWrap::InjectionFramework::ComInitialize();

   volumePtr->UnregisterControlChangeNotify(this);
}
