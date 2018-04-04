#include "towerdef_Struct_Wall.h"

Wall::Wall( int x, int y):OneTileStructure(x, y ){
}
std::string Wall::getTypeName(){
  return "Wall";
}
