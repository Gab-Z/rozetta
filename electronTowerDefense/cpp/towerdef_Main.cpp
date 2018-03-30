#include <nan.h>
#include "towerdef.h"

NAN_MODULE_INIT(InitModule) {
  TowerDefense::Init(target);
}

NODE_MODULE(towerdef_module, InitModule);
