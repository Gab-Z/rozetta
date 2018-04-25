#include "Tile.h"

Tile::Tile(){

}
Tile::Tile( int _x, int _y ){
  x = _x;
  y = _y;
}
Tile::Tile( int _x, int _y, int _floorTypeId ){
  x = _x;
  y = _y;
  floorTypeId = _floorTypeId;
}
std::vector<int> Tile::getPosition(){
  return std::vector<int> { x, y };
}
int Tile::getStructureId(){
  return structureId;
}
int Tile::getFloorId(){
  return floorTypeId;
}
void Tile::setPosition( int _x, int _y ){
  x = _x;
  y = _y;
}
void Tile::setStructureId( int _id ){
  structureId = _id;
}
void Tile::setFloorTypeId( int _id ){
  floorTypeId = _id;
}
