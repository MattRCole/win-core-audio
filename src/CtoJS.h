#pragma once
#include <nan.h>
#include "WinAPIWrap.h"
class Bindings
{
 public:
   static NAN_MODULE_INIT(Init);
   static NAN_METHOD(RegisterNotificationCallback);
};