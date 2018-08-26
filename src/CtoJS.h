#pragma once
#include <nan.h>
#include "WinAPIWrap.h"
#include <vector>
class Bindings
{
public:
  static std::vector<IUnknown *> notifiers;
  static NAN_MODULE_INIT(Init);
  static NAN_METHOD(RegisterDeviceNotificationCallback);
};