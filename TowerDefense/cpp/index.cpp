#include <iostream>
#include <nan.h>
#include <vector>
#include "./classes/TowerDefense/TowerDefense.h"
#include "./classes/Tile/Tile.h"

NAN_MODULE_INIT(InitModule) {
  TowerDefense<std::vector<Tile*>>::Init(target);
}


NODE_MODULE(towerdef_module, InitModule);
