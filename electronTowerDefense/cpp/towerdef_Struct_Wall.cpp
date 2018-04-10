#include "towerdef_Struct_Wall.h"

Wall::Wall( int _x, int _y ):OneTileStructure( _x, _y ){
}
std::string Wall::getTypeName(){
  return "Wall";
}
