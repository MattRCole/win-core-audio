#include "AudioDevice.h"
#include "CallbackNotificationManager.h"



int translateVolume(float rawVolume)
{
   return (int)(100.0 * rawVolume);
}

float translateVolume(double processedVolume)
{
   return (float)((float)processedVolume / 100.0);
}

WinAPIWrap::IMMDevicePtr getSetSetupDevice(v8::Local<v8::Object> wrappedAudioDevice)
{
   WinAPIWrap::InjectionFramework::ComInitialize();
   AudioDevice *_this_ = Nan::ObjectWrap::Unwrap<AudioDevice>(wrappedAudioDevice);
   auto device = WinAPIWrap::getDevice(_this_->id);
   return device;
}

AudioDevice * getThisFromInfo(v8::Local<v8::Object> wrappedAudioDevice)
{
   return Nan::ObjectWrap::Unwrap<AudioDevice>(wrappedAudioDevice);
}

WinAPIWrap::IAudioEndpointVolumePtr getSetSetupEndpointVolume(v8::Local<v8::Object> wrappedAudioDevice)
{
   WinAPIWrap::InjectionFramework::ComInitialize();
   AudioDevice *_this_ = Nan::ObjectWrap::Unwrap<AudioDevice>(wrappedAudioDevice);
   auto endpointVolume = WinAPIWrap::getEndpointVolume(_this_->id);
   return endpointVolume;
}

Nan::Persistent<v8::FunctionTemplate> AudioDevice::constructor;
NAN_MODULE_INIT(AudioDevice::Init)
{
   v8::Local<v8::FunctionTemplate> constructor_ = Nan::New<v8::FunctionTemplate>(AudioDevice::New);
   constructor.Reset(constructor_);
   constructor_->InstanceTemplate()->SetInternalFieldCount(1);
   constructor_->SetClassName(Nan::New("AudioDevice").ToLocalChecked());

   Nan::SetAccessor(constructor_->InstanceTemplate(), Nan::New("name").ToLocalChecked(), AudioDevice::GetName, AudioDevice::ReadOnly);
   Nan::SetAccessor(constructor_->InstanceTemplate(), Nan::New("id").ToLocalChecked(), AudioDevice::GetId, AudioDevice::ReadOnly);
   Nan::SetAccessor(constructor_->InstanceTemplate(), Nan::New("type").ToLocalChecked(), AudioDevice::GetType, AudioDevice::ReadOnly);
   Nan::SetAccessor(constructor_->InstanceTemplate(), Nan::New("volume").ToLocalChecked(), AudioDevice::GetVolume, AudioDevice::SetVolume);
   Nan::SetAccessor(constructor_->InstanceTemplate(), Nan::New("isMuted").ToLocalChecked(), AudioDevice::GetIsMuted, AudioDevice::SetIsMuted);
   Nan::SetAccessor(constructor_->InstanceTemplate(), Nan::New("state").ToLocalChecked(), AudioDevice::GetState, AudioDevice::SetState);

   Nan::SetPrototypeMethod(constructor_, "onVolumeChange", OnVolumeChange);
   Nan::SetPrototypeMethod(constructor_, "onStateChanged", OnStateChanged);
   Nan::SetPrototypeMethod(constructor_, "onMuteChange", OnMuteStatusChanged);
   Nan::SetPrototypeMethod(constructor_, "lockVolume", LockVolume);
   Nan::SetPrototypeMethod(constructor_, "unlockVolume", UnlockVolume);
   Nan::SetPrototypeMethod(constructor_, "lockMute", LockMute);
   Nan::SetPrototypeMethod(constructor_, "unlockMute", UnlockMute);

   target->Set(Nan::New("AudioDevice").ToLocalChecked(), constructor_->GetFunction());
}

NAN_METHOD(AudioDevice::New)
{
   //because windows:
   WinAPIWrap::InjectionFramework::ComInitialize();

   //If not called as a constructor, there is a clear coding error
   if (!info.IsConstructCall())
   {
      return Nan::ThrowError(Nan::New("In C++: AudioDevice::New - called without new keyword").ToLocalChecked());
   }

   if (info.Length() != 1) //might change this to allow 2 in order to make a persistant default AudioDevice class (AKA a class that always points to the default Media or Console device)
   {
      return Nan::ThrowError(Nan::New("In C++: AudioDevice::New - Expected 1 parameter: id:string").ToLocalChecked());
   }

   if (!info[0]->IsString())
   {
      return Nan::ThrowTypeError(Nan::New("In C++: AudioDevice::New - Expceted param0 to be of type string").ToLocalChecked());
   }

   AudioDevice *device = new AudioDevice();
   device->Wrap(info.Holder());

   device->id = conversion::string(*Nan::Utf8String(info[0]->ToString()));
   device->name = WinAPIWrap::getName(WinAPIWrap::getDevice(device->id));

   info.GetReturnValue().Set(info.Holder());
}

NAN_GETTER(AudioDevice::GetName)
{
   AudioDevice *_this_ = Nan::ObjectWrap::Unwrap<AudioDevice>(info.This());

   info.GetReturnValue().Set(Nan::New(_this_->name.c_str()).ToLocalChecked());
}

NAN_GETTER(AudioDevice::GetId)
{
   AudioDevice *_this_ = Nan::ObjectWrap::Unwrap<AudioDevice>(info.This());

   info.GetReturnValue().Set(Nan::New(_this_->id.c_str()).ToLocalChecked());
}

//NEED TO MAKE THIS ACTUALLY WORK--------------------------------------------------------------------------------
NAN_GETTER(AudioDevice::GetType)
{
   AudioDevice *_this_ = Nan::ObjectWrap::Unwrap<AudioDevice>(info.This());

   info.GetReturnValue().Set(Nan::New(_this_->id.c_str()).ToLocalChecked());
}

NAN_GETTER(AudioDevice::GetVolume)
{
   float rawVolume = -1;
   auto endpointVolume = getSetSetupEndpointVolume(info.This());

   endpointVolume->GetMasterVolumeLevelScalar(&rawVolume);

   info.GetReturnValue().Set(Nan::New(translateVolume(rawVolume)));
}

NAN_SETTER(AudioDevice::SetVolume)
{
   //should we throw errors for this or just silently fail?
   if (!value->IsNumber())
   {
      return Nan::ThrowTypeError(Nan::New("volume is type: number").ToLocalChecked());
   }
   float rawVolume = translateVolume(value->NumberValue());
   auto endpointVolume = getSetSetupEndpointVolume(info.This());

   endpointVolume->SetMasterVolumeLevelScalar(rawVolume, guid::get());

   return;
}

NAN_GETTER(AudioDevice::GetIsMuted)
{
   auto endpointVolume = getSetSetupEndpointVolume(info.This());
   BOOL isMuted;

   endpointVolume->GetMute(&isMuted);

   switch(isMuted)
   {
   case TRUE:
      info.GetReturnValue().Set(Nan::True());
      break;
   case FALSE:
      info.GetReturnValue().Set(Nan::False());
      break;
   default:
      break; //info.GetReturnValue().Set(undefined);
   };

   return;
}

NAN_SETTER(AudioDevice::SetIsMuted)
{
   if (!value->IsBoolean())
   {
      return Nan::ThrowTypeError(Nan::New("isMuted is type: boolean").ToLocalChecked());
   }
   auto endpointVolume = getSetSetupEndpointVolume(info.This());
   bool isMuted = value->BooleanValue();

   endpointVolume->SetMute(isMuted, guid::get());
}

NAN_GETTER(AudioDevice::GetState)
{
   auto device = getSetSetupDevice(info.This());
   DWORD state = 16;
   std::string stateReturn;

   device->GetState(&state);

   switch(state)
   {
   case DEVICE_STATE_ACTIVE:
      stateReturn = "active";
      break;
   case DEVICE_STATE_DISABLED:
      stateReturn = "disabled";
      break;
   case DEVICE_STATE_NOTPRESENT:
      stateReturn = "not present";
      break;
   case DEVICE_STATE_UNPLUGGED:
      stateReturn = "unplugged";
      break;
   default:
      stateReturn  = "error";
      break;
   };

   info.GetReturnValue().Set(Nan::New(stateReturn.c_str()).ToLocalChecked());
}

NAN_SETTER(AudioDevice::SetState)
{
   if (!value->IsString())
   {
      return Nan::ThrowTypeError(Nan::New("State is type: string").ToLocalChecked());
   }
   //ToDo: get this working
}

NAN_SETTER(AudioDevice::ReadOnly)
{
    return Nan::ThrowError(Nan::New("Property is read only").ToLocalChecked());
}

//(callback: (newVolume: number) => void): Function, called whenever there is an attempt to change the volume
NAN_METHOD(AudioDevice::OnVolumeChange)
{
   auto * _this_ = getThisFromInfo(info.This());
   const auto callbackType = VolumeChangeCallback::VolumeChange;
   if(info.Length() == 0)
   {
      CallbackNotificationManager::DeleteVolumeCallback(_this_->id, callbackType);
      return;
   }
   else if (!info[0]->IsFunction())
   {
      return Nan::ThrowTypeError(Nan::New("Expected param0: function").ToLocalChecked());
   }
   CallbackNotificationManager::AddVolumeCallback(new Nan::Callback(info[0].As<v8::Function>()), _this_->id, callbackType);

   return;
}

//(callback: (newState: number) => void): Function, called whenever the state of the device has been changed
NAN_METHOD(AudioDevice::OnStateChanged)
{
}

//(callback: (muteStatus: boolean) => void): Function, called whenever there is an attempt to change the mute state of the device.
NAN_METHOD(AudioDevice::OnMuteStatusChanged)
{
   auto * _this_ = getThisFromInfo(info.This());
   const auto callbackType = VolumeChangeCallback::MuteChange;
   if(info.Length() == 0)
   {
      CallbackNotificationManager::DeleteVolumeCallback(_this_->id, callbackType);
      return;
   }
   else if (!info[0]->IsFunction())
   {
      return Nan::ThrowTypeError(Nan::New("Expected param0: function").ToLocalChecked());
   }
   CallbackNotificationManager::AddVolumeCallback(new Nan::Callback(info[0].As<v8::Function>()), _this_->id, callbackType);

   return;
}

//(volume: number): Function, locks the volume to the given value. Will be overridden by another call to this function.
NAN_METHOD(AudioDevice::LockVolume)
{
   auto * _this_ = getThisFromInfo(info.This());
   const auto callbackType = VolumeChangeCallback::VolumeLock;
   if(info.Length() == 0 || !info[0]->IsNumber()  || info[0]->NumberValue() > 100 || info[0]->NumberValue() < 0)
   {
      return Nan::ThrowError(Nan::New("Expected param0: int between 0 and 100").ToLocalChecked());
   }

   CallbackNotificationManager::AddVolumeCallback(translateVolume(info[0]->NumberValue()), _this_->id, callbackType);
}

//(): Function, unlocks the volume, allowing it to be changed by other programs.
NAN_METHOD(AudioDevice::UnlockVolume)
{
   auto * _this_ = getThisFromInfo(info.This());

   CallbackNotificationManager::DeleteVolumeCallback(_this_->id, VolumeChangeCallback::VolumeLock); 
}

//(toMute: boolean): Function, locks the mute status to the given value. Will be overridden by another call to this function.
NAN_METHOD(AudioDevice::LockMute)
{
   auto * _this_ = getThisFromInfo(info.This());
   const auto callbackType = VolumeChangeCallback::MuteLock;
   if(info.Length() == 0 || !info[0]->IsBoolean())
   {
      return Nan::ThrowError(Nan::New("Expected param0: Boolean").ToLocalChecked());
   }

   CallbackNotificationManager::AddVolumeCallback(info[0]->BooleanValue(), _this_->id, callbackType);
}

//(): Function, unlocks the mute status of the device, allowing it to be changed by other programs.
NAN_METHOD(AudioDevice::UnlockMute)
{
   auto * _this_ = getThisFromInfo(info.This());

   CallbackNotificationManager::DeleteVolumeCallback(_this_->id, VolumeChangeCallback::MuteLock); 
}

