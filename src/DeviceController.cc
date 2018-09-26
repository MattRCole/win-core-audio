#include "DeviceController.h"
#include "AudioDevice.h"
// #include "asyncCBclass.h"
// #include "WinAPIWrap.h"
// #include <nan.h>
// #include <nan_converters.h>
// #include <mmdeviceapi.h>
// #include <functiondiscoverykeys_devpkey.h>
// #include <string>
// #include <vector>

//Initialize all static constructors
Nan::Persistent<v8::FunctionTemplate> Default::constructor;
Nan::Persistent<v8::FunctionTemplate> eRole::constructor;

NAN_MODULE_INIT(Default::Init)
{
   auto construct = Nan::New<v8::FunctionTemplate>(Default::New);
   constructor.Reset(construct);
   construct->InstanceTemplate()->SetInternalFieldCount(1);
   construct->SetClassName(Nan::New("DefaultAudioDeviceSelector").ToLocalChecked());

   Nan::SetAccessor(construct->InstanceTemplate(), Nan::New("media").ToLocalChecked(), Default::RoleGetter, Default::ReadOnly);
   Nan::SetAccessor(construct->InstanceTemplate(), Nan::New("communications").ToLocalChecked(), Default::RoleGetter, Default::ReadOnly);

   target->Set(Nan::New("DefaultAudioDeviceSelector").ToLocalChecked(), construct->GetFunction());
}

NAN_METHOD(Default::New)
{
   if(!info.IsConstructCall())
   {
      return Nan::ThrowError(Nan::New("In C++: Default::New - called without new keyword").ToLocalChecked());
   }

   if(info.Length() != 0)
   {
      return Nan::ThrowError(Nan::New("In C++: Default::New - Expected 0 parameters").ToLocalChecked());
   }

   Default *def = new Default();
   def->Wrap(info.Holder());

   info.GetReturnValue().Set(info.Holder());
}

NAN_GETTER(Default::RoleGetter)
{
   Default * _this_ = Nan::ObjectWrap::Unwrap<Default>(info.This());

   std::string propName = std::string(*Nan::Utf8String(property));

   if(propName == "media" || propName == "communications")
   {
      auto construct = Nan::New(Default::constructor)->GetFunction();

      const int argc = 1;
      v8::Local<v8::Value> argv[argc] = {
         property
      };

      auto eRoleToReturn = Nan::NewInstance(construct, argc, argv).ToLocalChecked();

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
   auto construct = Nan::New<v8::FunctionTemplate>(eRole::New);
   constructor.Reset(construct);
   construct->InstanceTemplate()->SetInternalFieldCount(1);
   construct->SetClassName(Nan::New("ERole").ToLocalChecked());

   Nan::SetAccessor(construct->InstanceTemplate(), Nan::New("speaker").ToLocalChecked(), eRole::GetDefaultDevice, eRole::SetDefaultDevice);
   Nan::SetAccessor(construct->InstanceTemplate(), Nan::New("mic").ToLocalChecked(), eRole::GetDefaultDevice, eRole::SetDefaultDevice);

   target->Set(Nan::New("ERole").ToLocalChecked(), construct->GetFunction());
}

NAN_METHOD(eRole::New)
{
   if(!info.IsConstructCall())
   {
      return Nan::ThrowError(Nan::New("In C++: eRole::New - called without new keyword").ToLocalChecked());
   }

   if(info.Length() != 1)
   {
      return Nan::ThrowError(Nan::New("In C++: eRole::New - Expected 1 parameter: string").ToLocalChecked());
   }

   if(!info[0]->IsString())
   {
      return Nan::ThrowTypeError(Nan::New("In C++ eRole::New - Expected param0 to be of type string").ToLocalChecked());
   }

   std::string theRole = std::string(*Nan::Utf8String(info[0]));
   eRole *role = new eRole();

   if(theRole == "media")
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

}

NAN_SETTER(eRole::SetDefaultDevice)
{

}


/*
// assume info[0]->IsArray()
v8::Local<v8::Array> jsArr = v8::Local<v8::Array>::Cast(info[0]);

std::vector<double> array;
for (int i = 0; i < jsArr->Length(); i++) {
  v8::Local<v8::Value> jsElement = jsArr->Get(i);
  double number = jsElement->NumberValue();
  vec.push_back(number);
}

v8::Local<v8::Value> argv[] = {
      Nan::New(this->ErrorMessage()).ToLocalChecked(), // return error message
      Nan::Null()
    };
*/


// // WinAPIWrap Bindings::wrapper = WinAPIWrap();
// std::vector<IUnknown *> Bindings::notifiers = std::vector<IUnknown *>();

// int translateVolume(float rawVolume)
// {
//    return (int)(100.0 * rawVolume);
// }

// float translateVolume(int processedVolume)
// {
//    return ((float)processedVolume/100.0);
// }

// std::string getStringFromJS(v8::Local<v8::Value> wrappedString)
// {
//    return std::string(*Nan::Utf8String(wrappedString->ToString()));
// }

// v8::Local<v8::Value> wrapString(std::string toWrap)
// {
//    return Nan::New(toWrap.c_str).ToLocalChecked();
// }

// v8::Local<v8::Array> getJSarrayFromStringVector(std::vector<std::string> strings)
// {
//    v8::Local<v8::Array> array;
//    for(uint32_t i = 0; i < strings.size(); i++)
//    {
//       Nan::Set(array, i, wrapString(strings.at(i)));
//    }

//    return array;
// }

// int getInt(v8::Local<v8::Value> wrappedInt)
// {
//    return Nan::To<int>(wrappedInt).FromJust();
// }

// EDataFlow getDataFlow(v8::Local<v8::Value> wrappedFlow)
// {
//    return (EDataFlow)getInt(wrappedFlow);
// }

// ERole getRole(v8::Local<v8::Value> wrappedRole)
// {
//    return (ERole)getInt(wrappedRole);
// }

// class DeviceNotificationsManager : public WinAudioNotificationClientBase
// {
//    Nan::Callback *callback;

//    virtual void HandleAsyncCallback(std::queue<__IMMNotificationClient__::Info> notifications)
//    {
//       namespace IMN = __IMMNotificationClient__;
//       Nan::HandleScope scope;

//       v8::Local<v8::Array> jsArr = Nan::New<v8::Array>(notifications.size());

//       auto size = notifications.size();

//       for (size_t i = 0; i < size; i++)
//       {
//          std::string front = notifications.front().toJsonString();
//          notifications.pop();
//          jsArr->Set(i, Nan::New(front.c_str()).ToLocalChecked());
//       }

//       v8::Local<v8::Value> argv[] =
//           {
//               jsArr};

//       Nan::Call(*callback, 1, argv);

//       jsArr.Clear();

//       return;
//    }

//    void destroy()
//    {
//       delete callback;
//    }

//  public:
//    DeviceNotificationsManager(Nan::Callback *_callback) : callback(_callback)
//    {
//    }
//    ~DeviceNotificationsManager()
//    {
//       if (callback)
//       {
//          delete callback;
//       }
//    }
// };


// //Unless otherwise specified, the return values of the following functions will conform to these standards:
//    //On success: a JavaScript value of true (In cases where other return values are expected, the return value of true will be replaced with the expected return information)
//    //On failur: a JavaScript string object that reports an error and, optionally, describing why the action could not be completed.
// NAN_MODULE_INIT(Bindings::Init)
// {
//    //Register a general callback that will notify the callback function of various hardware changes to audio devices
//    //This method expects a unique id to identify the callback that has been registered.
//    //Input parameters:
//       //param0: callback function that will receive a JSON object. See asyncCBclass for documentation on the JSON objects passed to the callback function
//       //param1: unique integer ID that corresponds to the callback function passed in. (Used, among other things to unregister the callback);
//    Nan::SetMethod(target, "registerCallback", RegisterDeviceNotificationCallback);
//    //Unregisters a callback as specified by the unique id passed into the function
//    //Input parameters:
//       //param0: unique ID that corresponds to the desired registered callback function.
//    Nan::SetMethod(target, "unregisterCallback", UnregisterDeviceNotificationCallback);
//    //Registers a callback that will registered to a specific audio device and will recieve notifications on different volume changes made to that device in the form of a JSON object.
//    //Input parameters:
//       //param0: callback that will asynchronously recieve a JSON object with information about the Volume of the registered device
//       //param1: unique integer ID that corresponds to the callback function passed in. (Used, among other things to unregister the callback);
//    Nan::SetMethod(target, "registerVolumeCallback", RegisterVolumeChangeCallback);
//    //Unregisters a callback as specified by the unique id passed into the function
//    //Input parameters:
//       //param0: unique ID that corresponds to the desired registered callback function.
//    Nan::SetMethod(target, "unregisterVolumeCallback", UnregisterVolumeChangeCallback);
//    //Returns the friendly name of the specified default device 
//    //Input parameters:
//       //param0: EDataFlow flow (integer) that represents the flow of the device: render for speakers, and capture for microphones
//       //param1: ERole role (integer) that represents the responsibility of the device: console for media playback and communications for... communications
//    //Return value:
//       //On success: A JavaScript String representing the friendly name of the device.
//    Nan::SetMethod(target, "getDefaultName", GetDefaultName);
//    //Returns the device ID of the specified default device 
//    //Input parameters:
//       //param0: EDataFlow flow (integer) that represents the flow of the device: render for speakers, and capture for microphones
//       //param1: ERole role (integer) that represents the responsibility of the device: console for media playback and communications for... communications
//    //Return value:
//       //On success: A JavaScript String representing the ID of the device.
//    Nan::SetMethod(target, "getDefaultId", GetDefaultId);
//    //Sets the specified device as a default for the specified role
//    //Input parameters:
//       //param0: Device ID (String) corresponding to an active device
//       //param1: ERole role (integer) that represents the new default of the stated speaker.
//    Nan::SetMethod(target, "setDefault", SetDefault);
//    //Returns the volume of the specified default device 
//    //Input parameters:
//       //param0: EDataFlow flow (integer) that represents the flow of the device: render for speakers, and capture for microphones
//       //param1: ERole role (integer) that represents the responsibility of the device: console for media playback and communications for... communications
//    //Return value:
//       //On success: An integer (from 0-100) that represents the current volume of the device
//    Nan::SetMethod(target, "getDefaultVolume", GetDefaultVolume);
//    //Sets the volume of the specified default device 
//    //Input parameters:
//       //param0: EDataFlow flow (integer) that represents the flow of the device: render for speakers, and capture for microphones
//       //param1: ERole role (integer) that represents the responsibility of the device: console for media playback and communications for... communications
//       //param2: New Volume (integer within range of 0-100)
//    Nan::SetMethod(target, "setDefaultVolume", SetDefaultVolume);
//    //Returns the ID of the specified device
//    //Input parameters:
//       //param0: Device Name Identifier (String) that can uniquely identify the desired device's friendly name
//       //param1: EDataFlow flow (integer) that represents the flow of the device: render for speakers, and capture for microphones
//       //param2: DEVICE_STATE (integer, range 1-7) of different state flags that may be bitwise or'd together to include many device states. DEFAULT VALUE: DEVICE_STATE_ACTIVE
//          //This one needs a short example: 
//          //DEVICE_STATE_ACTIVE      1
//          //DEVICE_STATE_DISABLED    2
//          //DEVICE_STATE_NOTPRESENT  4
//          //DEVICE_STATE_UNPLUGGED   8
//          //DEVICE_STATEMASK_ALL     15
//    //Return value:
//       //On success: a JavaScript Array of strings that represent the ID's of all matching devices
//    Nan::SetMethod(target, "getId", GetId);
//    //Returns the volume of the specified device
//    //Input parameters:
//       //param0: Device ID (String) obtained from getId or getDefaultId functions
//    //Return value:
//       //On success: An integer (from 0-100) that represents the current volume of the device
//    Nan::SetMethod(target, "getVolume", GetVolume);
//    //Sets the volume of the specified device
//    //Input parameters:
//       //param0: Device ID (String) obtained from getId or getDefaultId functions
//       //param1: New Volume (integer within range of 0-100)
//    Nan::SetMethod(target, "setVolume", SetVolume);
//    //Returns a JavaScript array of all device names meeting the "state" requirement
//    //Input parameters:
//       //param0: DEVICE_STATE (integer, range 1-7) of different state flags that may be bitwise or'd together to include many device states.
//    Nan::SetMethod(target, "getAllNames", GetAllNames);
//    //Returns a JavaScript array of all device IDs meeting the "state" requirement
//    //Input parameters:
//       //param0: DEVICE_STATE (integer, range 1-7) of different state flags that may be bitwise or'd together to include many device states.
//    Nan::SetMethod(target, "getAllIDs", GetAllIDs);

//    //The following functions are helper functions to abstract away from the Windows enums of EDataFlow and ERole
//    //EDataFlow members: eRender, eCapture
//    //ERole members: eConsole, eCommunications
//    Nan::SetMethod(target, "helperGetFlowRenderConst", HelperGetFlowRenderConst);
//    Nan::SetMethod(target, "helperGetFlowCaptureConst", HelperGetFlowCaptureConst);
//    Nan::SetMethod(target, "helperGetRoleConsoleConst", HelperGetRoleConsoleConst);
//    Nan::SetMethod(target, "helperGetRoleCommunicationsConst", HelperGetRoleCommunicationsConst);
// }

// //param0: callback function that will receive a JSON object. See asyncCBclass for documentation on the JSON objects passed to the callback function
// //param1: unique integer ID that corresponds to the callback function passed in. (Used, among other things to unregister the callback);
// NAN_METHOD(Bindings::RegisterDeviceNotificationCallback)
// {
//    using namespace WinAPIWrap;

//    InjectionFramework::ComInitialize();

//    IMMDeviceEnumeratorPtr enumerator = InjectionFramework::getEnumerator();

//    DeviceNotificationsManager *devManager  = new DeviceNotificationsManager(new Nan::Callback(info[0].As<v8::Function>()));

//    HRESULT hr = enumerator->RegisterEndpointNotificationCallback(devManager);

//    if (SUCCEEDED(hr))
//    {
//       info.GetReturnValue().Set(Nan::New("Success!").ToLocalChecked());
//    }
//    else
//    {
//       delete devManager;
//       info.GetReturnValue().Set(Nan::New("Failed").ToLocalChecked());
//    }

//    return;
// }

// //param0: unique ID that corresponds to the desired registered callback function.
// NAN_METHOD(Bindings::UnregisterDeviceNotificationCallback)
// {
// }

// //param0: callback that will asynchronously recieve a JSON object with information about the Volume of the registered device
// //param1: unique integer ID that corresponds to the callback function passed in. (Used, among other things to unregister the callback);
// NAN_METHOD(Bindings::RegisterVolumeChangeCallback)
// {
// }

// //param0: unique ID that corresponds to the desired registered callback function.
// NAN_METHOD(Bindings::UnregisterVolumeChangeCallback)
// {
// }

// //param0: EDataFlow flow (integer) that represents the flow of the device: render for speakers, and capture for microphones
// //param1: ERole role (integer) that represents the responsibility of the device: console for media playback and communications for... communications
// NAN_METHOD(Bindings::GetDefaultName)
// {
//    //validate input
//    if (!info[0]->IsNumber() || !info[1]->IsNumber())
//    {
//       //return null, we don't have the info we need.
//       return;
//    }
//    WinAPIWrap::IMMDeviceEnumeratorPtr deviceEnumerator = WinAPIWrap::InjectionFramework::getEnumerator();
//    WinAPIWrap::IMMDevicePtr device;
//    EDataFlow flow = getDataFlow(info[0]);
//    ERole role = getRole(info[1]);
//    deviceEnumerator->GetDefaultAudioEndpoint(flow, role, (IMMDevice **)device.getPointerToPointerToNull());

//    std::string name = WinAPIWrap::getName(device);
//    info.GetReturnValue().Set(Nan::New(name.c_str()).ToLocalChecked());
//    return;
// }

// //param0: EDataFlow flow (integer) that represents the flow of the device: render for speakers, and capture for microphones
// //param1: ERole role (integer) that represents the responsibility of the device: console for media playback and communications for... communications
// NAN_METHOD(Bindings::GetDefaultId)
// {
//    //validate input
//    if (!info[0]->IsNumber() || !info[1]->IsNumber())
//    {
//       //return null, we don't have the info we need.
//       return;
//    }
//    WinAPIWrap::IMMDeviceEnumeratorPtr deviceEnumerator = WinAPIWrap::InjectionFramework::getEnumerator();
//    WinAPIWrap::IMMDevicePtr device;
//    EDataFlow flow = getDataFlow(info[0]);
//    ERole role = getRole(info[1]);
//    deviceEnumerator->GetDefaultAudioEndpoint(flow, role, (IMMDevice **)device.getPointerToPointerToNull());

//    conversion::string id = WinAPIWrap::getId(device);
//    info.GetReturnValue().Set(Nan::New(id.c_str()).ToLocalChecked());
//    return;
// }

// //param0: Device ID (String) corresponding to an active device
// //param1: ERole role (integer) that represents the new default of the stated speaker.
// NAN_METHOD(Bindings::GetDefaultVolume)
// {
//    //validate input
//    if (!info[0]->IsNumber() || !info[1]->IsNumber())
//    {
//       //return null, we don't have the info we need.
//       return;
//    }
//    WinAPIWrap::IMMDeviceEnumeratorPtr deviceEnumerator = WinAPIWrap::InjectionFramework::getEnumerator();
//    WinAPIWrap::IMMDevicePtr device;
//    EDataFlow flow = getDataFlow(info[0]);
//    ERole role = getRole(info[1]);
//    deviceEnumerator->GetDefaultAudioEndpoint(flow, role, (IMMDevice **)device.getPointerToPointerToNull());

//    WinAPIWrap::IAudioEndpointVolumePtr endpoint = WinAPIWrap::getEndpointVolume(device);
//    float rawVolume;
//    auto hr = endpoint->GetMasterVolumeLevelScalar(&rawVolume);
//    if(!SUCCEEDED(hr))
//    {
//       rawVolume = -1;
//    }
//    info.GetReturnValue().Set(Nan::New(translateVolume(rawVolume)));
//    return;
// }

// //param0: EDataFlow flow (integer) that represents the flow of the device: render for speakers, and capture for microphones
// //param1: ERole role (integer) that represents the responsibility of the device: console for media playback and communications for... communications
// //param2: New Volume (integer within range of 0-100)
// NAN_METHOD(Bindings::SetDefaultVolume)
// {
//    //validate input
//    if (!info[0]->IsNumber() || !info[1]->IsNumber() || !info[2]->IsNumber())
//    {
//       //return null, we don't have the info we need.
//       return;
//    }
//    WinAPIWrap::IMMDeviceEnumeratorPtr deviceEnumerator = WinAPIWrap::InjectionFramework::getEnumerator();
//    WinAPIWrap::IMMDevicePtr device;
//    EDataFlow flow = getDataFlow(info[0]);
//    ERole role = getRole(info[1]);
//    int processedVolume = getInt(info[2]);

//    deviceEnumerator->GetDefaultAudioEndpoint(flow, role, (IMMDevice **)device.getPointerToPointerToNull());

//    WinAPIWrap::IAudioEndpointVolumePtr endpoint = WinAPIWrap::getEndpointVolume(device);

//    endpoint->SetMasterVolumeLevelScalar(translateVolume(processedVolume), /*Change this in the future?*/NULL);
// }

// //param0: Device Name Identifier (String) that can uniquely identify the desired device's friendly name
// //param1: EDataFlow flow (integer) that represents the flow of the device: render for speakers, and capture for microphones
// //param2: DEVICE_STATE (integer, range 1-7) of different state flags that may be bitwise or'd together to include many device states. DEFAULT VALUE: DEVICE_STATE_ACTIVE
// NAN_METHOD(Bindings::GetId)
// {
//    if(!info[0]->IsString() || !info[1]->IsNumber() || !info[2]->IsNumber())
//    {
//       //return right now, we don't have what we need
//       return;
//    }
//    DWORD state;
//    if(info[2]->IsNumber())
//    {
//       state = (unsigned long)getInt(info[2]);
//    }
//    else
//    {
//       state = DEVICE_STATE_ACTIVE;
//    }
//    auto deviceName = getStringFromJS(info[0]);
//    auto flow = getDataFlow(info[1]);
   
//    conversion::string id = WinAPIWrap::getId(WinAPIWrap::getDevice(deviceName, flow, state));

//    info.GetReturnValue().Set(Nan::New(id.c_str()).ToLocalChecked());
// }

// //param0: Device ID (String) obtained from getId or getDefaultId functions
// NAN_METHOD(Bindings::GetVolume)
// {
//    if(!info[0]->IsString())
//    {
//       return;
//    }
//    auto audioPtr = WinAPIWrap::getEndpointVolume(WinAPIWrap::getDevice(conversion::string(getStringFromJS(info[0]))));
//    float rawVolume = -1;
//    audioPtr->GetMasterVolumeLevelScalar(&rawVolume);
//    info.GetReturnValue().Set(Nan::New(translateVolume(rawVolume)));
// }

// //param0: Device ID (String) obtained from getId or getDefaultId functions
// //param1: New Volume (integer within range of 0-100)
// NAN_METHOD(Bindings::SetVolume)
// {
//    if(!info[0]->IsString() || !info[1]->IsNumber())
//    {
//       return;
//    }

//    auto audioPtr = WinAPIWrap::getEndpointVolume(WinAPIWrap::getDevice(conversion::string(getStringFromJS(info[0]))));

//    float newVolume = translateVolume(getInt(info[1]));

//    audioPtr->SetMasterVolumeLevelScalar(newVolume, NULL /*Change...?*/);

//    return;
// }

// //param0: Device ID (String) corresponding to an active device
// //param1: ERole role (integer) that represents the new default of the stated speaker.
// NAN_METHOD(Bindings::SetDefault)
// {
//    //To be implemented
//    return;
// }

// //param0: EDataFlow flow (integer) that represents the flow of the device: render for speakers, and capture for microphones
// //param1: DEVICE_STATE (integer, range 1-7) of different state flags that may be bitwise or'd together to include many device states.
// NAN_METHOD(Bindings::GetAllNames)
// {
//    if(!info[0]->IsNumber() || !info[1]->IsNumber())
//    {
//       return;
//    }
//    auto flow = getDataFlow(info[0]);
//    DWORD state = (unsigned long)getInt(info[1]);

//    auto names = WinAPIWrap::getAllNames(flow, state);

//    info.GetReturnValue().Set(getJSarrayFromStringVector(names));
// }
// //param0: DEVICE_STATE (integer, range 1-7) of different state flags that may be bitwise or'd together to include many device states.
// NAN_METHOD(Bindings::GetAllIDs)
// {
//    if(!info[0]->IsNumber() || !info[1]->IsNumber())
//    {
//       return;
//    }
//    auto flow = getDataFlow(info[0]);
//    DWORD state = (unsigned long)getInt(info[1]);

//    auto idsWide = WinAPIWrap::getAllIds(flow, state);
//    std::vector<std::string> ids;

//    for(auto idWide : idsWide)
//    {
//       ids.push_back(conversion::string(idWide));
//    }

//    info.GetReturnValue().Set(getJSarrayFromStringVector(ids));
// }

// NAN_METHOD(Bindings::HelperGetFlowRenderConst)
// {
//    info.GetReturnValue().Set(Nan::New(eRender));
// }
// NAN_METHOD(Bindings::HelperGetFlowCaptureConst)
// {
//    info.GetReturnValue().Set(Nan::New(eCapture));
// }
// NAN_METHOD(Bindings::HelperGetRoleConsoleConst)
// {
//    info.GetReturnValue().Set(Nan::New(eConsole));
// }
// NAN_METHOD(Bindings::HelperGetRoleCommunicationsConst)
// {
//    info.GetReturnValue().Set(Nan::New(eCommunications));
// }