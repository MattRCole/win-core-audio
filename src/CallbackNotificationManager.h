#include <nan.h>
#include <map>
#include <asyncCBclass.h>
#include <common.h>
#include <tuple>
#include <uv.h>

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
   static const int NotifyWasDefault = 0;
   static const int NotifyIsDefault = 1;
   static void addGenericDefaultNotification(int toOrWas, Nan::Callback * callback, conversion::string id);
   static void removeGenericDefaultNotification(int toOrWas, conversion::string id);
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
   // class DeviceChangeNotificationClient : protected WinDeviceNotificationClientBase
   // {
   //  private:
   //    static conversion::string defaultComMicId;
   //    static conversion::string defaultMultiMicId;
   //    static conversion::string defaultComSpeakerId;
   //    static conversion::string defaultMultiSpeakerId;
   //    static bool initialized;
   //    WinAPIWrap::IMMDeviceEnumeratorPtr enumerator;

   //    inline static conversion::string & getDefault(EDataFlow flow, ERole role) 
   //    {
   //       return role == eCommunications ?
   //          flow == eRender ? defaultComSpeakerId : defaultComMicId
   //       :
   //          flow == eRender ? defaultMultiSpeakerId : defaultMultiMicId; 
   //    }

   //    static void initDefaults();

   //    void handleDefaultChangeNotifications(__IMMNotificationClient__::Info info);
   //    void handleStateChangeNotifications(__IMMNotificationClient__::Info info);
   //  protected:
   //    inline virtual void HandleAsyncCallback(std::queue<__IMMNotificationClient__::Info> info);
   //    inline virtual void destroyCallback();
   //  public:
   //    uv_mutex_t stateMutex;
   //    uv_mutex_t defaultMutex;
   //    DeviceChangeNotificationClient()
   //    {
   //       using namespace WinAPIWrap;
   //       InjectionFramework::ComInitialize();
   //       enumerator = InjectionFramework::getEnumerator();
   //       enumerator->RegisterEndpointNotificationCallback(this);
   //       uv_mutex_init(&stateMutex);
   //       uv_mutex_init(&defaultMutex);
   //    }
   //    ~DeviceChangeNotificationClient()
   //    {
   //       enumerator->UnregisterEndpointNotificationCallback(this);
   //    }
   // };
   // static DeviceChangeNotificationClient deviceClient;
   static std::map<VolumeKey, VolumeChangeNotificationClient *, bool (*)(const VolumeKey &, const VolumeKey &)> callbacks;
   // static std::map<conversion::string, std::tuple<DWORD, Nan::Callback *>, bool (*)(const conversion::string &, const conversion::string &)> stateChangeWatch;
   // static std::map<conversion::string, std::tuple<Nan::Callback *, Nan::Callback *>, bool (*)(const conversion::string &, const conversion::string &)> defaultDeviceWatch;
   // static Nan::Callback * deviceRemovedCallback;

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
   inline static bool compareVolumeKey(const VolumeKey &lhs, const VolumeKey &rhs) { return lhs.id == rhs.id ? lhs.type < rhs.type : lhs.id < rhs.id; }
   // inline static bool compareConversionString(const conversion::string &lhs, const conversion::string &rhs) { return lhs < rhs;}
   CallbackNotificationManager() = delete;
   static inline void AddVolumeCallback(Nan::Callback *callback, conversion::string id, VolumeChangeCallback::Type typeOfCallback)
   {
      VolumeKey key = {id, typeOfCallback};
      auto *toAdd = new CallbackNotificationManager::VolumeChangeNotificationClient(callback, id, typeOfCallback);

      deleteDupes(key);

      callbacks[key] = toAdd;
   }

   // static void AddWasDefaultDeviceNotification(Nan::Callback * callback, conversion::string id);
   // static void AddIsDefaultDeviceNotification(Nan::Callback * callback, conversion::string id);
   // static void RemoveWasDefaultDeviceNotification(conversion::string id);
   // static void RemoveIsDefaultDeviceNotification(conversion::string id);
   // static void AddStateChangeNotification(Nan::Callback * callback, conversion::string id);
   // static void RemoveStateChangeNotification(conversion::string id);

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