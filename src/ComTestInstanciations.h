#pragma once
#include "common.h"
#include <string>
#include <set>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <functiondiscoverykeys_devpkey.h>
#include "../src/WinAPIWrap.h"

namespace ComTest
{
    class TestAudioEndpointVolume : public MockCom::MockAudioEndpointVolume {
        float scalarVolume;
        BOOL muted;
        std::set<IAudioEndpointVolumeCallback *> callbacks;
        void tellEveryone();
        MockCom::MockAudioEndpointVolume dummyEndpoint;
        public:
        HRESULT RegisterControlChangeNotify(IAudioEndpointVolumeCallback *pNotify);
        HRESULT UnregisterControlChangeNotify(IAudioEndpointVolumeCallback *pNotify);
        HRESULT SetMasterVolumeLevelScalar(float fLevel, LPCGUID pguidEventContext);
        HRESULT GetMasterVolumeLevelScalar(float *pfLevel);
        HRESULT SetMute(BOOL bMute, LPCGUID pguidEventContext);
        HRESULT GetMute(BOOL *pbMute);

        IAudioEndpointVolume * operator->() { if (WinAPIWrap::InjectionFramework::initCount) return this; else return &dummyEndpoint; }
    };

    class TestPropertyStore : public MockCom::MockPropertyStore {
        std::wstring name;
    public:
       HRESULT GetValue(REFPROPERTYKEY key, PROPVARIANT *pv);
    };

    class TestDevice;

    class TestDeviceCollection : public MockCom::MockDeviceCollection {
       std::vector<TestDevice> devices;
    };

    class TestDeviceEnumerator : public MockCom::MockDeviceEnumerator {

    };
}