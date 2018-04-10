#include "towerdef_oneTileStruct.h"
const std::vector<int> oneTileGrid { 1 };
OneTileStructure::OneTileStructure( int _x, int _y):Structure( _x, _y, 1, 1 ){
  setGrid(oneTileGrid);
  setGridLength();
}
