#include <nan.h>
#include <map>
#include <asyncCBclass.h>

namespace VolumeChangeCallback
{
enum Type
{
   MuteChange,
   VolumeChange,
   MuteLock,
   VolumeLock
};
}

class CallbackNotificationManager
{
 private:
   class VolumeChangeNotificationClient : protected WinVolumeNotificationClientBase
   {
      Nan::Callback *callback;
      VolumeChangeCallback::Type type;
      bool muteLock;
      float volumeLock;
      float currentVol;
      bool currentMute;

    protected:
      virtual void HandleAsyncCallback(std::queue<__IAudioEndpointVolumeCallback__::Info> info);
      inline virtual void destroyCallback()
      {
         if (callback)
            delete callback;
      }

    public:
      VolumeChangeNotificationClient(Nan::Callback *_callback, conversion::string _deviceId, VolumeChangeCallback::Type _type) : WinVolumeNotificationClientBase(_deviceId),
                                                                                                                                 callback(_callback),
                                                                                                                                 type(_type)
      {
         using namespace WinAPIWrap;

         InjectionFramework::ComInitialize();
         auto endpointVolume = getEndpointVolume(deviceId);
         BOOL BOOLcurrentMute = FALSE;
         currentVol = -1;
         endpointVolume->GetMute(&BOOLcurrentMute);
         currentMute = (bool)BOOLcurrentMute;
         endpointVolume->GetMasterVolumeLevelScalar(&currentVol);
      }

      VolumeChangeNotificationClient(bool _muteLock, conversion::string _deviceId, VolumeChangeCallback::Type _type) : WinVolumeNotificationClientBase(_deviceId),
                                                                                                                       callback(nullptr), type(_type),
                                                                                                                       muteLock(_muteLock)
      {
      }

      VolumeChangeNotificationClient(float _volumeLock, conversion::string _deviceId, VolumeChangeCallback::Type _type) : WinVolumeNotificationClientBase(_deviceId),
                                                                                                                          callback(nullptr), type(_type),
                                                                                                                          volumeLock(_volumeLock)
      {
      }
   };
   struct VolumeKey
   {
      conversion::string id;
      VolumeChangeCallback::Type type;
   };
   static std::map<VolumeKey, VolumeChangeNotificationClient *, bool (*)(const VolumeKey &, const VolumeKey &)> callbacks;
   static inline void deleteDupes(VolumeKey toDelete)
   {
      auto dupe = callbacks.find(toDelete);
      if (dupe != callbacks.end())
      {
         auto *callback = dupe->second;
         callbacks.erase(dupe);
         delete callback;
      }
   }

 public:
   inline static bool compare(const VolumeKey &lhs, const VolumeKey &rhs) { return lhs.id == rhs.id ? lhs.type < rhs.type : lhs.id < rhs.id; }
   CallbackNotificationManager() = delete;
   static inline void AddVolumeCallback(Nan::Callback *callback, conversion::string id, VolumeChangeCallback::Type typeOfCallback)
   {
      VolumeKey key = {id, typeOfCallback};
      auto *toAdd = new CallbackNotificationManager::VolumeChangeNotificationClient(callback, id, typeOfCallback);

      deleteDupes(key);

      callbacks[key] = toAdd;
   }

   static inline void AddVolumeCallback(bool muteLockState, conversion::string id, VolumeChangeCallback::Type typeOfCallback)
   {
      VolumeKey key = {id, typeOfCallback};
      auto *toAdd = new CallbackNotificationManager::VolumeChangeNotificationClient(muteLockState, id, typeOfCallback);

      deleteDupes(key);

      callbacks[key] = toAdd;
   }

   static inline void AddVolumeCallback(float volumeLockState, conversion::string id, VolumeChangeCallback::Type typeOfCallback)
   {
      VolumeKey key = {id, typeOfCallback};
      auto *toAdd = new CallbackNotificationManager::VolumeChangeNotificationClient(volumeLockState, id, typeOfCallback);

      deleteDupes(key);

      callbacks[key] = toAdd;
   }

   static inline void DeleteVolumeCallback(conversion::string id, VolumeChangeCallback::Type typeOfCallback)
   {
      auto toDelete = callbacks.find({id, typeOfCallback});

      if (toDelete != callbacks.end())
      {
         auto *callback = toDelete->second;
         callbacks.erase(toDelete);
         delete callback;
      }
   }
};