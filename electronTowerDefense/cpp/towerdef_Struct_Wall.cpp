#include "towerdef_Struct_Wall.h"

Wall::Wall( int _x, int _y ){
  setx(_x);
  sety(_y);
}
std::string Wall::getTypeName(){
  return "Wall";
}
std::vector<int> Wall::getPositions( int _x, int _y ){
  return OneTileStructure::getPositions( _x, _y );
}
int Wall::getx(){
  return OneTileStructure::getx();
}
int Wall::gety(){
  return OneTileStructure::gety();
}
