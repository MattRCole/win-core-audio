#include <nan.h>
#include "CtoJS.h"

NAN_MODULE_INIT(InitModule)
{
   Bindings::Init(target);
}

NODE_MODULE(winCoreAudio, InitModule);