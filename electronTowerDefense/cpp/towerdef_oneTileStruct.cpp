#include "towerdef_oneTileStruct.h"
const std::vector<int> oneTileGrid { 1 };
OneTileStructure::OneTileStructure( int _x, int _y):Structure( _x, _y, 1, 1, oneTileGrid ){
  setGridWidth(1);
  setGridHeight(1);
  setGrid(oneTileGrid);
  setGridLength();
}
std::vector<int> OneTileStructure::getPositions( int _x, int _y ){
  return Structure::getPositions( _x, _y );
}
int OneTileStructure::getx(){
  return Structure::getx();
}
int OneTileStructure::gety(){
  return Structure::gety();
}
