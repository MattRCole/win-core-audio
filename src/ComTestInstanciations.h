#pragma once
#include "common.h"
#include <string>
#include <set>
#include <mmdeviceapi.h>

namespace ComTest
{
    class TestAudioEndpointVolume : public MockCom::MockAudioEndpointVolume {
        float scalarVolume;
        BOOL muted;
        std::set<IAudioEndpointVolumeCallback *> callbacks;
        void tellEveryone();
        public:
        HRESULT RegisterControlChangeNotify(IAudioEndpointVolumeCallback *pNotify);
        HRESULT UnregisterControlChangeNotify(IAudioEndpointVolumeCallback *pNotify);
        HRESULT SetMasterVolumeLevelScalar(float fLevel, LPCGUID pguidEventContext);
        HRESULT GetMasterVolumeLevelScalar(float *pfLevel);
        HRESULT SetMute(BOOL bMute, LPCGUID pguidEventContext);
        HRESULT GetMute(BOOL *pbMute);
    };

    class TestPropertyStore : public MockCom::MockPropertyStore {
        wstring name;
    };

    class TestDevice;

    class TestDeviceCollection : public MockCom::MockDeviceCollection {
        vector<TestDevice>
    };

    class TestDeviceEnumerator : public MockCom::MockDeviceEnumerator {

    }
}