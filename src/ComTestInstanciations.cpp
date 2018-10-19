#include "ComTestInstanciations.h"
#include "../src/WinAPIWrap.h"
#define TEST_CO_INTIALIZED                             \
   if (WinAPIWrap::InjectionFramework::initCount == 0) \
      return CO_E_NOTINITIALIZED;

namespace ComTest
{

void TestAudioEndpointVolume::tellEveryone()
{
   AUDIO_VOLUME_NOTIFICATION_DATA notificationData = {
       NULL,
       muted,
       scalarVolume,
       0,
       NULL};
   for (auto *callback : callbacks)
   {
      callback->OnNotify(&notificationData);
   }
}

HRESULT TestAudioEndpointVolume::RegisterControlChangeNotify(IAudioEndpointVolumeCallback *pNotify)
{
   TEST_CO_INTIALIZED
   HRESULT hr = S_OK;
   if (pNotify)
   {
      callbacks.insert(pNotify);
      pNotify->AddRef();
   }
   else
   {
      hr = E_POINTER;
   }

   return hr;
}

HRESULT TestAudioEndpointVolume::UnregisterControlChangeNotify(IAudioEndpointVolumeCallback *pNotify)
{
   TEST_CO_INTIALIZED
   if (pNotify == NULL)
   {
      return E_POINTER;
   }
   auto callback = callbacks.find(pNotify);
   if (callback == callbacks.end())
   {
      return E_INVALIDARG;
   }
   (*callback)->Release();
   callbacks.erase(callback);

   return S_OK;
}
HRESULT TestAudioEndpointVolume::SetMasterVolumeLevelScalar(float fLevel, LPCGUID pguidEventContext)
{
   TEST_CO_INTIALIZED
   if (fLevel < 0 || fLevel > 1)
   {
      return E_INVALIDARG;
   }
   scalarVolume = fLevel;
   float channelVolumes[1] = {scalarVolume};
   tellEveryone();
   return S_OK;
}
HRESULT TestAudioEndpointVolume::GetMasterVolumeLevelScalar(float *pfLevel)
{
   TEST_CO_INTIALIZED
   if (pfLevel)
   {
      *pfLevel = scalarVolume;
      return S_OK;
   }
   else
   {
      return E_POINTER;
   }
}
HRESULT TestAudioEndpointVolume::SetMute(BOOL bMute, LPCGUID pguidEventContext)
{
   TEST_CO_INTIALIZED
   muted = bMute;
   tellEveryone();
   return S_OK;
}
HRESULT TestAudioEndpointVolume::GetMute(BOOL *pbMute)
{
   TEST_CO_INTIALIZED
   if (pbMute)
   {
      *pbMute = muted;
      return S_OK;
   }
   else
   {
      return E_POINTER;
   }
}

HRESULT TestPropertyStore::GetValue(REFPROPERTYKEY key, PROPVARIANT *pv)
{
   TEST_CO_INTIALIZED
   if (key != PKEY_Device_FriendlyName)
   {
      return E_NOTIMPL;
   }
   if (pv == NULL)
   {
      return E_POINTER;
   }
   else
   {
      unsigned long long length = name.length();
      pv->pwszVal = new wchar_t[length + 1];
      wcsncpy_s((wchar_t *)pv->pwszVal, length + 1, name.c_str(), length);
      return S_OK;
   }
}

HRESULT TestDevice::GetState(DWORD *pdwState)
{
   TEST_CO_INTIALIZED
   if (!pdwState)
      return E_POINTER;
}

} // namespace ComTest

#undef TEST_CO_INTIALIZED