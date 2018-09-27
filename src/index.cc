#include <nan.h>
#include "AudioDevice.h"
#include "DeviceController.h"


NAN_MODULE_INIT(InitModule)
{
   DeviceController::Init(target);
   Default::Init(target);
   eRole::Init(target);
   AudioDevice::Init(target);
}

NODE_MODULE(winCoreAudio, InitModule);