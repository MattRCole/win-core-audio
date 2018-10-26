#pragma once
#include <nan.h>
#include "WinAPIWrap.h"
#include <mmdeviceapi.h>
#include <vector>

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