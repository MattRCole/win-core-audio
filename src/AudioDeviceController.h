#pragma once
#include <nan.h>
#include "WinAPIWrap.h"
#include <mmdeviceapi.h>
#include <vector>
// class Bindings
// {
// public:
//   static std::vector<IUnknown *> notifiers;
//   static NAN_MODULE_INIT(Init);
//   static NAN_METHOD(RegisterDeviceNotificationCallback);
//   static NAN_METHOD(UnregisterDeviceNotificationCallback);
//   static NAN_METHOD(RegisterVolumeChangeCallback);
//   static NAN_METHOD(UnregisterVolumeChangeCallback);
//   static NAN_METHOD(GetDefaultName);
//   static NAN_METHOD(GetDefaultId);
//   static NAN_METHOD(GetDefaultVolume);
//   static NAN_METHOD(SetDefaultVolume);
//   static NAN_METHOD(GetId);
//   static NAN_METHOD(SetVolume);
//   static NAN_METHOD(GetVolume);
//   static NAN_METHOD(SetDefault);
//   static NAN_METHOD(GetAllNames);
//   static NAN_METHOD(GetAllIDs);

//   static NAN_METHOD(HelperGetFlowRenderConst);
//   static NAN_METHOD(HelperGetFlowCaptureConst);
//   static NAN_METHOD(HelperGetRoleConsoleConst);
//   static NAN_METHOD(HelperGetRoleCommunicationsConst);
// };

class AudioDeviceController
{
 public:
   static IMMNotificationClient *lock;
   static IMMNotificationClient *onDefaultSpeakerChange;
   static IMMNotificationClient *onDefaultMicChange;

   static NAN_MODULE_INIT(Init);
   static NAN_GETTER(Devices);
   // static NAN_GETTER(AllDevices);
   // static NAN_METHOD(LockDefaultTo);
   // static NAN_METHOD(UnlockDefault);
   // static NAN_METHOD(OnDefaultDeviceSpeakerChanged);
   // static NAN_METHOD(OnDefaultDeviceMicChanged);
   static NAN_GETTER(GetDefault);
   static NAN_SETTER(ReadOnly);
};

class Default : public Nan::ObjectWrap
{
 public:
   static Nan::Persistent<v8::FunctionTemplate> constructor;
   static NAN_MODULE_INIT(Init);
   static NAN_METHOD(New);
   static NAN_GETTER(RoleGetter);
   static NAN_SETTER(ReadOnly);
};

class eRole : public Nan::ObjectWrap
{
 public:
   ERole role;
   static Nan::Persistent<v8::FunctionTemplate> constructor;
   static NAN_MODULE_INIT(Init);
   static NAN_METHOD(New);
   static NAN_GETTER(GetDefaultDevice);
   static NAN_SETTER(SetDefaultDevice);
};