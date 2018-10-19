#include "CallbackNotificationManager.h"
#include <map>
#include <nan.h>
#include <queue>

std::map<
    CallbackNotificationManager::VolumeKey,
    CallbackNotificationManager::VolumeChangeNotificationClient *,
    bool (*)(
        const CallbackNotificationManager::VolumeKey &,
        const CallbackNotificationManager::VolumeKey &)>
    CallbackNotificationManager::callbacks(&CallbackNotificationManager::compare);

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
