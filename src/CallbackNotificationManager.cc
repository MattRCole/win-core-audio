#include "CallbackNotificationManager.h"
#include "common.h"
#include "WinAPIWrap.h"
#include <map>
#include <nan.h>
#include <queue>

std::map<
    CallbackNotificationManager::VolumeKey,
    CallbackNotificationManager::VolumeChangeNotificationClient *,
    bool (*)(
        const CallbackNotificationManager::VolumeKey &,
        const CallbackNotificationManager::VolumeKey &)>
    CallbackNotificationManager::callbacks(&CallbackNotificationManager::compareVolumeKey);
// std::map<
//     conversion::string,
//     std::tuple<DWORD, Nan::Callback *>,
//     bool (*)(
//         const conversion::string &,
//         const conversion::string &)>
//     CallbackNotificationManager::stateChangeWatch(&CallbackNotificationManager::compareConversionString);
// std::map<
//     conversion::string,
//     std::tuple<Nan::Callback *, Nan::Callback *>,
//     bool (*)(
//         const conversion::string &,
//         const conversion::string &)>
//     CallbackNotificationManager::defaultDeviceWatch(&compareConversionString);

// Nan::Callback *CallbackNotificationManager::deviceRemovedCallback = nullptr;

// CallbackNotificationManager::DeviceChangeNotificationClient CallbackNotificationManager::deviceClient = CallbackNotificationManager::DeviceChangeNotificationClient();

// conversion::string CallbackNotificationManager::DeviceChangeNotificationClient::defaultComMicId = "";
// conversion::string CallbackNotificationManager::DeviceChangeNotificationClient::defaultMultiMicId = "";
// conversion::string CallbackNotificationManager::DeviceChangeNotificationClient::defaultComSpeakerId = "";
// conversion::string CallbackNotificationManager::DeviceChangeNotificationClient::defaultMultiSpeakerId = "";
bool initialized = false;

Nan::Callback *& getCorrectCallback(int toGet, std::tuple<Nan::Callback *, Nan::Callback *> & callbacks)
{
   switch(toGet)
   {
    case 0:
      return std::get<0>(callbacks);
    case 1:
      return std::get<1>(callbacks);
    default:
      throw "Incorrect thingy";
   }
}

std::map<__IMMNotificationClient__::MessageType, __IMMNotificationClient__::Info> sortQueue(std::queue<__IMMNotificationClient__::Info> queueToSort)
{
   using namespace __IMMNotificationClient__;
   std::map<MessageType, Info> toReturn; 
   size_t size = queueToSort.size();
   for(size_t i = 0; i < size; i++)
   {
      auto info = queueToSort.front();
      toReturn[info.type] = info;
      queueToSort.pop();
   }
   return toReturn;
}

// void CallbackNotificationManager::DeviceChangeNotificationClient::initDefaults()
// {
//    WinAPIWrap::InjectionFramework::ComInitialize();

//    defaultComMicId = WinAPIWrap::getName(WinAPIWrap::getDefault(eCapture, eCommunications));
//    defaultMultiSpeakerId = WinAPIWrap::getName(WinAPIWrap::getDefault(eCapture, eMultimedia));
//    defaultComSpeakerId = WinAPIWrap::getName(WinAPIWrap::getDefault(eRender, eCommunications));
//    defaultMultiSpeakerId = WinAPIWrap::getName(WinAPIWrap::getDefault(eRender, eMultimedia));
// }

// void CallbackNotificationManager::DeviceChangeNotificationClient::HandleAsyncCallback(std::queue<__IMMNotificationClient__::Info> info)
// {
//    Nan::HandleScope scope;
//    using namespace __IMMNotificationClient__;
//    auto mapInfo = sortQueue(info);
//    for(auto uniqueInfo : mapInfo)
//    {
//       switch (uniqueInfo.first)
//       {
//        case MessageType::DefaultChanged:
//          handleDefaultChangeNotifications(uniqueInfo.second);
//          break;
//        case MessageType::StateChange:  
//          handleStateChangeNotifications(uniqueInfo.second);
//          break;
//        default:
//          break;
//       }
//    }
//    return;
// }

// void CallbackNotificationManager::DeviceChangeNotificationClient::handleDefaultChangeNotifications(__IMMNotificationClient__::Info info)
// {
//    conversion::string &defaultId = getDefault(info.flow, info.role);
//    if (defaultId != info.id)
//    {
//       const int argc = 1;
//       v8::Local<v8::Value> argv[argc] = {
//           Nan::New(info.id.c_str()).ToLocalChecked()};
//       uv_mutex_lock(&defaultMutex);
//       auto isDefault = CallbackNotificationManager::defaultDeviceWatch.find(info.id);
//       auto wasDefault = CallbackNotificationManager::defaultDeviceWatch.find(defaultId);
//       if (isDefault != CallbackNotificationManager::defaultDeviceWatch.end())
//       {
//          auto callbacks = isDefault->second;
//          auto callback = std::get<NotifyWasDefault>(callbacks);
//          if(callback)
//          {
//             Nan::Call(*callback, argc, argv);
//          }
//       }
//       if (wasDefault != CallbackNotificationManager::defaultDeviceWatch.end())
//       {
//          auto callbacks = wasDefault->second;
//          auto callback = std::get<NotifyIsDefault>(callbacks);
//          if(callback)
//          {
//             Nan::Call(*callback, 0, nullptr);
//          }
//       }
//       uv_mutex_unlock(&defaultMutex);;;;;
//    }
// }

// void CallbackNotificationManager::DeviceChangeNotificationClient::handleStateChangeNotifications(__IMMNotificationClient__::Info info)
// {
//    uv_mutex_lock(&stateMutex);
//    auto toNotify = CallbackNotificationManager::stateChangeWatch.find(info.id);
//    uv_mutex_unlock(&stateMutex);
//    if (toNotify == CallbackNotificationManager::stateChangeWatch.end())
//    {
//       return;
//    }
//    auto stateCallbackTuple = toNotify->second;
//    auto state = std::get<DWORD>(stateCallbackTuple);
//    auto *callback = std::get<Nan::Callback *>(stateCallbackTuple);
//    std::string stateAsString = "";
//    switch (info.state)
//    {
//     case DEVICE_STATE_ACTIVE:
//       stateAsString = "active";
//       break;
//     case DEVICE_STATE_DISABLED:
//       stateAsString = "disabled";
//       break;
//     case DEVICE_STATE_NOTPRESENT:
//       stateAsString = "not present";
//       break;
//     case DEVICE_STATE_UNPLUGGED:
//       stateAsString = "unplugged";
//       break;
//     default:
//       stateAsString = "error";
//       break;
//    }
//    const int argc = 1;
//    v8::Local<v8::Value> argv[argc] = {
//        Nan::New(stateAsString.c_str()).ToLocalChecked()};
//    if (info.state == state)
//    {
//       return;
//    }
//    Nan::Call(*callback, argc, argv);
// }

// void CallbackNotificationManager::DeviceChangeNotificationClient::destroyCallback()
// {
//    return;
// }

void CallbackNotificationManager::VolumeChangeNotificationClient::HandleAsyncCallback(std::queue<__IAudioEndpointVolumeCallback__::Info> info)
{
   Nan::HandleScope scope;
   using namespace VolumeChangeCallback;
   switch (type)
   {
    case VolumeChange:
      if (!info.back().fromHere && info.back().volume != currentVol)
      {
         currentVol = info.back().volume;
         const int argc = 1;
         v8::Local<v8::Value> argv[argc] = {
             Nan::New(currentVol * 100)};
         Nan::Call(*callback, argc, argv);
      }
      break;
    case MuteChange:
      if (!info.back().fromHere && info.back().muted != currentMute)
      {
         currentMute = info.back().muted;
         const int argc = 1;
         v8::Local<v8::Value> argv[argc] = {
             Nan::New(currentMute)};
         Nan::Call(*callback, argc, argv);
      }
      break;
    case VolumeLock:
      if (info.back().volume != volumeLock)
      {
         WinAPIWrap::InjectionFramework::ComInitialize();
         volumePtr->SetMasterVolumeLevelScalar(volumeLock, guid::get());
      }
      break;
    case MuteLock:
      if (info.back().muted != muteLock)
      {
         WinAPIWrap::InjectionFramework::ComInitialize();
         volumePtr->SetMute((BOOL)muteLock, guid::get());
      }
   }
}

// void CallbackNotificationManager::AddWasDefaultDeviceNotification(Nan::Callback * callback, conversion::string id)
// {
//    addGenericDefaultNotification(NotifyWasDefault, callback, id);
// }

// void CallbackNotificationManager::AddIsDefaultDeviceNotification(Nan::Callback * callback, conversion::string id)
// {
//    addGenericDefaultNotification(NotifyIsDefault, callback, id);
// }

// void CallbackNotificationManager::addGenericDefaultNotification(int toOrWas, Nan::Callback * callback, conversion::string id)
// {
//    uv_mutex_lock(&deviceClient.defaultMutex);
//    auto maybeExists = defaultDeviceWatch.find(id);
//    if(maybeExists != defaultDeviceWatch.end())
//    {
//       std::tuple<Nan::Callback *, Nan::Callback *> & callbacks = maybeExists->second;
//       Nan::Callback *& toChange = getCorrectCallback(toOrWas, callbacks);
//       if(toChange)
//       {
//          delete toChange;
//       }
//       toChange = callback;
//    }
//    else
//    {
//       std::tuple<Nan::Callback *, Nan::Callback *> toAdd;
//       getCorrectCallback(toOrWas, toAdd) = callback;
//       getCorrectCallback(!toOrWas, toAdd) = nullptr;
//       defaultDeviceWatch[id] = toAdd;
//    }
//    uv_mutex_unlock(&deviceClient.defaultMutex);
// }

// void CallbackNotificationManager::AddStateChangeNotification(Nan::Callback * callback, conversion::string id)
// {
//    WinAPIWrap::InjectionFramework::ComInitialize();
//    DWORD state = DEVICE_STATEMASK_ALL;
//    WinAPIWrap::getDevice(id)->GetState(&state);
//    uv_mutex_lock(&deviceClient.stateMutex);
//    auto dupe = stateChangeWatch.find(id);
//    if(dupe != stateChangeWatch.end())
//    {
//       auto stateCallbackTuple = dupe->second;
//       auto * callback = std::get<Nan::Callback *>(stateCallbackTuple);
//       if(callback)
//       {
//          delete callback;
//       }
//       stateChangeWatch.erase(dupe);
//    }
//    std::tuple<DWORD, Nan::Callback *> toAdd = { state, callback };
//    stateChangeWatch[id] = toAdd;
//    uv_mutex_unlock(&deviceClient.stateMutex);
// }


// void CallbackNotificationManager::RemoveWasDefaultDeviceNotification(conversion::string id)
// {
//    removeGenericDefaultNotification(NotifyWasDefault, id);
// }

// void CallbackNotificationManager::RemoveIsDefaultDeviceNotification(conversion::string id)
// {
//    removeGenericDefaultNotification(NotifyIsDefault, id);
// }

// void CallbackNotificationManager::removeGenericDefaultNotification(int toOrWas, conversion::string id)
// {
//    uv_mutex_lock(&deviceClient.defaultMutex);
//    auto mapToRemove = defaultDeviceWatch.find(id);
//    if(mapToRemove != defaultDeviceWatch.end())
//    {
//       auto & tupleCallbacks = mapToRemove->second;
//       auto *& callbackToRemove = getCorrectCallback(toOrWas, tupleCallbacks);
//       if(callbackToRemove)
//       {
//          delete callbackToRemove;
//       }
//       callbackToRemove = nullptr;
//    }
//    uv_mutex_unlock(&deviceClient.defaultMutex);
// }

// void CallbackNotificationManager::RemoveStateChangeNotification(conversion::string id)
// {
//    uv_mutex_lock(&deviceClient.stateMutex);
//    auto mapToRemove = stateChangeWatch.find(id);
//    if(mapToRemove != stateChangeWatch.end())
//    {
//       auto tupleToRemove = mapToRemove->second;
//       auto callbackToRemove = std::get<Nan::Callback *>(tupleToRemove);
//       if(callbackToRemove)
//       {
//          delete callbackToRemove;
//       }
//       stateChangeWatch.erase(mapToRemove);
//    }
//    uv_mutex_unlock(&deviceClient.stateMutex);
// }
