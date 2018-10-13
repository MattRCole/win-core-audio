#include "json.hpp"
#include "asyncCBclass.h"
#include "WinAPIWrap.h"


// void WinAudioNotificationClientBase::_SendNotifications()
// {
//     std::queue<std::string> toSend;
//     uv_mutex_lock(&_mutex);
//     for (auto notification : notificationBuffer)
//     {
//         toSend.push(notification);
//     }
//     notificationBuffer.clear();
//     uv_mutex_unlock(&_mutex);

//     //there is a slight chance that the object will be destoryed by the time we get to it.
//     uv_mutex_lock(&_enrolledMutex);
//     if (this->isEnrolled)
//     {
//         this->HandleAsyncCallback(toSend);
//     }
//     uv_mutex_unlock(&_enrolledMutex);
// }
//{ DefaultChanged, DeviceAdded, DeviceRemoved, StateChange, PropertyChange}
// void WinAudioNotificationClientBase::SendNotification(Info info)
// {
//     std::string notification;
//     json obj;
//     switch(info.type)
//     {
//     case(DefaultChanged):
//         obj = {
//             {"type", "defaultChanged"},
//             {"id", info.id},
//             {"flow", info.flow},
//             {"role", info.role}
//         };
//         break;
//     case(DeviceAdded):
//         obj = {
//             {"type", "deviceAdded"},
//             {"id", info.id}
//         };
//         break;
//     case(DeviceRemoved):
//         obj = {
//             {"type", "deviceRemoved"},
//             {"id", info.id}
//         };
//         break;
//     case(StateChange):
//         obj = {
//             {"type", "stateChanged"},
//             {"id", info.id},
//             {"state", info.state}
//         }
//     case(PropertyChange):
//         obj = {
//             {"type", "propertyChanged"},
//             { `}
//         }
//     }
//     uv_mutex_lock(&_mutex);
//     notificationBuffer.push_back(notification);
//     uv_mutex_unlock(&_mutex);

//     //signals node's event loop that there is data to be processed
//     uv_async_send(&this->asyncHandle);
// }

WinAudioNotificationClientBase::~WinAudioNotificationClientBase()
{
    if (_pEnumerator != NULL)
    {
        _pEnumerator->Release();
        _pEnumerator = NULL;
    }
}

ULONG WinAudioNotificationClientBase::Release()
{
    ULONG ulRef = InterlockedDecrement(&_cRef);
    if (0 == ulRef)
    {
        delete this;
    }
    return ulRef;
}

HRESULT WinAudioNotificationClientBase::QueryInterface(REFIID riid, VOID **ppvInterface)
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
   
   auto endpointVolume = WinAPIWrap::getEndpointVolume(deviceId);

   endpointVolume->RegisterControlChangeNotify(this);
}

WinVolumeNotificationClientBase::~WinVolumeNotificationClientBase()
{
   WinAPIWrap::InjectionFramework::ComInitialize();

   auto endpointVolume = WinAPIWrap::getEndpointVolume(deviceId);

   endpointVolume->UnregisterControlChangeNotify(this);
}
