#include "CtoJS.h"
#include "asyncCBclass.h"
#include "WinAPIWrap.h"
#include <nan.h>
#include <mmdeviceapi.h>
#include <functiondiscoverykeys_devpkey.h>
#include <string>

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

// WinAPIWrap Bindings::wrapper = WinAPIWrap();
std::vector<IUnknown *> Bindings::notifiers();

class DeviceNotificationsManager : public WinAudioNotificationClientBase
{
  Nan::Callback *callback;

  virtual void HandleAsyncCallback(std::queue<__IMMNotificationClient__::Info> notifications)
  {
    namespace IMN = __IMMNotificationClient__;
    Nan::HandleScope scope;

    v8::Local<v8::Array> jsArr = Nan::New<v8::Array>(notifications.size());

    auto size = notifications.size();

    for (size_t i = 0; i < size; i++)
    {
      std::string front = notifications.front().toJsonString();
      notifications.pop();
      jsArr->Set(i, Nan::New(front.c_str()).ToLocalChecked());
    }

    v8::Local<v8::Value> argv[] =
        {
            jsArr};

    Nan::Call(*callback, 1, argv);

    jsArr.Clear();

    return;
  }

  void destroy()
  {
    delete callback;
  }

public:
  DeviceNotificationsManager(Nan::Callback *_callback) : callback(_callback)
  {
  }
  ~DeviceNotificationsManager()
  {
    if(callback)
    {
      delete callback;
    }
  }
};

NAN_MODULE_INIT(Bindings::Init)
{
  Nan::SetMethod(target, "registerCallback", RegisterDeviceNotificationCallback);
}

NAN_METHOD(Bindings::RegisterDeviceNotificationCallback)
{
  using namespace WinAPIWrap;
  
  InjectionFramework::COMInitialize();

  IMMDeviceEnumeratorPtr enumerator = InjectionFramework::getEnumerator();

  DeviceNotificationsManager devManager * = new DeviceNotificationsManager(new Nan::Callback(info[0].As<v8::Function>()));

  HRESULT hr = enumerator->RegisterEndpointNotificationCallback(devManager);

  if (SUCCEEDED(hr))
  {
    info.GetReturnValue().Set(Nan::New("Success!").ToLocalChecked());
  }
  else
  {
    delete devManager;
    info.GetReturnValue().Set(Nan::New("Failed").ToLocalChecked());
  }

  return;
}
