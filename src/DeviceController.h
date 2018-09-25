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
  static NAN_METHOD(UnregisterDeviceNotificationCallback);
  static NAN_METHOD(RegisterVolumeChangeCallback);
  static NAN_METHOD(UnregisterVolumeChangeCallback);
  static NAN_METHOD(GetDefaultName);
  static NAN_METHOD(GetDefaultId);
  static NAN_METHOD(GetDefaultVolume);
  static NAN_METHOD(SetDefaultVolume);
  static NAN_METHOD(GetId);
  static NAN_METHOD(SetVolume);
  static NAN_METHOD(GetVolume);
  static NAN_METHOD(SetDefault);
  static NAN_METHOD(GetAllNames);
  static NAN_METHOD(GetAllIDs);


  static NAN_METHOD(HelperGetFlowRenderConst);
  static NAN_METHOD(HelperGetFlowCaptureConst);
  static NAN_METHOD(HelperGetRoleConsoleConst);
  static NAN_METHOD(HelperGetRoleCommunicationsConst);
};