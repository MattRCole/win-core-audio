#include <nan.h>
#include "AudioDevice.h"
#include "DeviceController.h"


NAN_MODULE_INIT(InitModule)
{
   AudioDeviceController::Init(target);
   Default::Init(target);
   eRole::Init(target);
   AudioDevice::Init(target);
}

NODE_MODULE(winCoreAudio, InitModule);