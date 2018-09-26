#pragma once
#include <nan.h>
#include <WinAPIWrap.h>
#include <mmdeviceapi.h>
#include <common.h>
#include <string>

//AudioDevice
    //name: string, read-only
    //id: string, read-only
    //type: string, read-only, possilbe values: ['speaker', 'mic']
    //volume: number, range is 0-100
    //isMuted: boolean
    //state: string, possible values: ['active', 'disabled', 'unplugged', 'not present'];
    //onVolumeChange(callback: (newVolume: number) => void): Function, called whenever there is an attempt to change the volume
    //onStateChanged(callback: (newState: number) => void): Function, called whenever the state of the device has been changed
    //onMuteStatusChanged(callback: (muteStatus: boolean) => void): Function, called whenever there is an attempt to change the mute state of the device.
    //lockVolume(volume: number): Function, locks the volume to the given value. Will be overridden by another call to this function.
    //unlockVolume(): Function, unlocks the volume, allowing it to be changed by other programs.
    //lockMute(toMute: boolean): Function, locks the mute status to the given value. Will be overridden by another call to this function.
    //unlockMute(): Function, unlocks the mute status of the device, allowing it to be changed by other programs.

class AudioDevice : public Nan::ObjectWrap 
{
public:
   conversion::string id;
   std::string name;
   static Nan::Persistent<v8::FunctionTemplate> constructor;
   static NAN_MODULE_INIT(Init);
   static NAN_METHOD(New);
   static NAN_GETTER(GetName);
   static NAN_GETTER(GetId);
   static NAN_GETTER(GetType);
   static NAN_GETTER(GetVolume);
   static NAN_SETTER(SetVolume);
   static NAN_GETTER(GetIsMuted);
   static NAN_SETTER(SetIsMuted);
   static NAN_GETTER(GetState);
   static NAN_SETTER(SetState);
   static NAN_SETTER(ReadOnly);
   static NAN_METHOD(OnVolumeChange);           //(callback: (newVolume: number) => void): Function, called whenever there is an attempt to change the volume
   static NAN_METHOD(OnStateChanged);           //(callback: (newState: number) => void): Function, called whenever the state of the device has been changed
   static NAN_METHOD(OnMuteStatusChanged);      //(callback: (muteStatus: boolean) => void): Function, called whenever there is an attempt to change the mute state of the device.
   static NAN_METHOD(LockVolume);               //(volume: number): Function, locks the volume to the given value. Will be overridden by another call to this function.
   static NAN_METHOD(UnlockVolume);             //(): Function, unlocks the volume, allowing it to be changed by other programs.
   static NAN_METHOD(LockMute);                 //(toMute: boolean): Function, locks the mute status to the given value. Will be overridden by another call to this function.
   static NAN_METHOD(UnlockMute);               //(): Function, unlocks the mute status of the device, allowing it to be changed by other programs.
};