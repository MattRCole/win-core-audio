#include "AudioDeviceController.h"
#include "AudioDevice.h"
#include <vector>

//Initialize all static constructors
Nan::Persistent<v8::FunctionTemplate> Default::constructor;
Nan::Persistent<v8::FunctionTemplate> eRole::constructor;

NAN_MODULE_INIT(AudioDeviceController::Init)
{
   Nan::SetAccessor(target, Nan::New("default").ToLocalChecked(), AudioDeviceController::GetDefault, AudioDeviceController::ReadOnly);
   Nan::SetAccessor(target, Nan::New("speakers").ToLocalChecked(), AudioDeviceController::Devices , AudioDeviceController::ReadOnly);
   Nan::SetAccessor(target, Nan::New("mics").ToLocalChecked(), AudioDeviceController::Devices, AudioDeviceController::ReadOnly);
   Nan::SetAccessor(target, Nan::New("allSpeakers").ToLocalChecked(), AudioDeviceController::Devices, AudioDeviceController::ReadOnly);
   Nan::SetAccessor(target, Nan::New("allMics").ToLocalChecked(), AudioDeviceController::Devices, AudioDeviceController::ReadOnly);
}

NAN_GETTER(AudioDeviceController::Devices)
{
   WinAPIWrap::InjectionFramework::ComInitialize();
   EDataFlow flow = eAll;
   DWORD state = DEVICE_STATE_ACTIVE | DEVICE_STATE_DISABLED;
   std::string propName = std::string(*Nan::Utf8String(property));
   std::vector<std::wstring> deviceIds;

   if(propName == "speakers" || "allSpeakers")
   {
      flow = eRender;
   }
   else if(propName == "mics" || "allMics")
   {
      flow = eCapture;
   }
   else
   {
      return;
   }

   if(propName.substr(0,2) == "all")
   {
      state = DEVICE_STATEMASK_ALL;
   }

   deviceIds = WinAPIWrap::getAllIds(flow, state);

   v8::Local<v8::Array> array = Nan::New<v8::Array>(deviceIds.size());
   for(size_t i = 0; i < deviceIds.size(); i++)
   {
      conversion::string id = deviceIds.at(i);
      const int argc = 1;
      auto wrappedId = Nan::New(id.c_str()).ToLocalChecked();
      auto construct = Nan::New(AudioDevice::constructor)->GetFunction();
      v8::Local<v8::Value> argv[argc] = {
          wrappedId
      };

      auto deviceToReturn = Nan::NewInstance(construct, argc, argv).ToLocalChecked();

      Nan::Set(array, i, deviceToReturn);
   }

   info.GetReturnValue().Set(array);

}

NAN_GETTER(AudioDeviceController::GetDefault)
{
   std::string propName = std::string(*Nan::Utf8String(property));

   if (propName == "default")
   {
      v8::Local<v8::Function> construct = Nan::New(Default::constructor)->GetFunction();

      v8::Local<v8::Object> DefaultToReturn = Nan::NewInstance(construct, 0, nullptr).ToLocalChecked();

      info.GetReturnValue().Set(DefaultToReturn);
   } 
}

NAN_SETTER(AudioDeviceController::ReadOnly)
{
   return Nan::ThrowError(Nan::New("property is read-only").ToLocalChecked());
}

NAN_MODULE_INIT(Default::Init)
{
   v8::Local<v8::FunctionTemplate> construct = Nan::New<v8::FunctionTemplate>(Default::New);
   constructor.Reset(construct);
   construct->InstanceTemplate()->SetInternalFieldCount(1);
   construct->SetClassName(Nan::New("Default").ToLocalChecked());

   Nan::SetAccessor(construct->InstanceTemplate(), Nan::New("media").ToLocalChecked(), Default::RoleGetter, Default::ReadOnly);
   Nan::SetAccessor(construct->InstanceTemplate(), Nan::New("communications").ToLocalChecked(), Default::RoleGetter, Default::ReadOnly);

   target->Set(Nan::New("Default").ToLocalChecked(), construct->GetFunction());
}

NAN_METHOD(Default::New)
{
   if (!info.IsConstructCall())
   {
      return Nan::ThrowError(Nan::New("In C++: Default::New - called without new keyword").ToLocalChecked());
   }

   if (info.Length() != 0)
   {
      return Nan::ThrowError(Nan::New("In C++: Default::New - Expected 0 parameters").ToLocalChecked());
   }

   Default *def = new Default();
   def->Wrap(info.Holder());

   info.GetReturnValue().Set(info.Holder());
}

NAN_GETTER(Default::RoleGetter)
{
   Default *_this_ = Nan::ObjectWrap::Unwrap<Default>(info.This());

   std::string propName = std::string(*Nan::Utf8String(property));

   if (propName == "media" || propName == "communications")
   {
      v8::Local<v8::Function> construct = Nan::New(eRole::constructor)->GetFunction();

      const int argc = 1;
      v8::Local<v8::Value> argv[argc] = {
          Nan::New(propName.c_str()).ToLocalChecked()};

      v8::Local<v8::Object> eRoleToReturn = Nan::NewInstance(construct, argc, argv).ToLocalChecked();

      info.GetReturnValue().Set(eRoleToReturn);
   }
}

NAN_SETTER(Default::ReadOnly)
{
   return Nan::ThrowError(Nan::New("Property is read only").ToLocalChecked());
}

//Begin eRole methods
NAN_MODULE_INIT(eRole::Init)
{
   v8::Local<v8::FunctionTemplate> construct = Nan::New<v8::FunctionTemplate>(eRole::New);
   constructor.Reset(construct);
   construct->InstanceTemplate()->SetInternalFieldCount(1);
   construct->SetClassName(Nan::New("ERole").ToLocalChecked());

   Nan::SetAccessor(construct->InstanceTemplate(), Nan::New("speaker").ToLocalChecked(), eRole::GetDefaultDevice, eRole::SetDefaultDevice);
   Nan::SetAccessor(construct->InstanceTemplate(), Nan::New("mic").ToLocalChecked(), eRole::GetDefaultDevice, eRole::SetDefaultDevice);

   target->Set(Nan::New("ERole").ToLocalChecked(), construct->GetFunction());
}

NAN_METHOD(eRole::New)
{
   if (!info.IsConstructCall())
   {
      return Nan::ThrowError(Nan::New("In C++: eRole::New - called without new keyword").ToLocalChecked());
   }

   if (info.Length() != 1)
   {
      return Nan::ThrowError(Nan::New("In C++: eRole::New - Expected 1 parameter: string").ToLocalChecked());
   }

   if (!info[0]->IsString())
   {
      return Nan::ThrowTypeError(Nan::New("In C++ eRole::New - Expected param0 to be of type string").ToLocalChecked());
   }

   std::string theRole = std::string(*Nan::Utf8String(info[0]));
   eRole *role = new eRole();

   if (theRole == "media")
   {
      role->role = eConsole;
   }
   else if (theRole == "communications")
   {
      role->role = eCommunications;
   }
   else
   {
      delete role;
      return Nan::ThrowTypeError(Nan::New("In C++ eRole:New - Expected param0 to equal: 'media' or 'communications").ToLocalChecked());
   }

   role->Wrap(info.Holder());

   info.GetReturnValue().Set(info.Holder());
}

NAN_GETTER(eRole::GetDefaultDevice)
{
   WinAPIWrap::InjectionFramework::ComInitialize();
   eRole *_this_ = Nan::ObjectWrap::Unwrap<eRole>(info.This());

   std::string propName = std::string(*Nan::Utf8String(property));
   WinAPIWrap::IMMDevicePtr device;
   WinAPIWrap::IMMDeviceEnumeratorPtr enumerator = WinAPIWrap::InjectionFramework::getEnumerator();
   EDataFlow flow;
   LPWSTR idPtr = NULL;
   conversion::string id;

   if (propName == "speaker")
   {
      flow = eRender;
   }
   else if (propName == "mic")
   {
      flow = eCapture;
   }
   else
   {
      return Nan::ThrowError(Nan::New("Invalid property, expected 'speaker' or 'mic'").ToLocalChecked());
   }

   enumerator->GetDefaultAudioEndpoint(flow, _this_->role, (IMMDevice **)device.getPointerToPointerToNull());

   device->GetId(&idPtr);

   if (idPtr)
   {
      id = std::wstring(idPtr);
   }
   auto wrappedId = Nan::New(id.c_str()).ToLocalChecked();

   auto construct = Nan::New(AudioDevice::constructor)->GetFunction();
   const int argc = 1;
   v8::Local<v8::Value> argv[argc] = {
       wrappedId
   };

   auto deviceToReturn = Nan::NewInstance(construct, argc, argv).ToLocalChecked();

   info.GetReturnValue().Set(deviceToReturn);
}

NAN_SETTER(eRole::SetDefaultDevice)
{
   //do nothing yet
}
