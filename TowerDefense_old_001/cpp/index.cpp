#include <iostream>
#include <nan.h>
#include "./classes/TowerDefense/TowerDefense.h"



NAN_MODULE_INIT(InitModule) {
  TowerDefense::Init(target);
}

NODE_MODULE(towerdef_module, InitModule);
