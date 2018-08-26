#include "ComTestInstanciations.h"


void TestAudioEndpointVolume::tellEveryone()
{
    AUDIO_VOLUME_NOTIFICATION_DATA notificationData = {
        NULL,
        muted,
        scalarVolume,
        0,
        NULL
    };
    for(auto * callback : callbacks)
    {
        callback->OnNotify(&notificationData);
    }
}

HRESULT TestAudioEndpointVolume::RegisterControlChangeNotify(IAudioEndpointVolumeCallback *pNotify)
{
    HRESULT hr = S_OK;
    
    if(pNotify)
    {
        callbacks.push_back(pNotify);
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
    auto callback = callbacks.find(pNotify);
    if(callback == callbacks.end())
    {
        return E_INVALIDARG;
    }
    callback->Release();
    callbacks.erase(callback);

    return S_OK;
}
HRESULT TestAudioEndpointVolume::SetMasterVolumeLevelScalar(float fLevel, LPCGUID pguidEventContext)
{
    if(fLevel < 0 || flevel > 1)
    {
        return E_INVALIDARG;
    }
    scalarVolume = fLevel;
    float channelVolumes[1] = { scalarVolume };
    tellEveryone();
    return S_OK;
}
HRESULT TestAudioEndpointVolume::GetMasterVolumeLevelScalar(float *pfLevel)
{
    if(pfLevel)
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
    muted = bMute;
    tellEveryone();
    return S_OK;
}
HRESULT TestAudioEndpointVolume::GetMute(BOOL *pbMute)
{
    if(pbMute)
    {
        *pbMute = muted;
        return S_OK;
    }
    else
    {
        return E_POINTER;
    }
}