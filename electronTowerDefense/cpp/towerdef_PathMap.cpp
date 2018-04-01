#include "towerdef_PathMap.h"

void PathMap::setStart( int x, int y ){
  startx = x;
  starty = y;
}
void PathMap::clearMap(){
  map.clear();
}
void PathMap::setValue( int index, int value ){
  map[ index ] = value;
}
void PathMap::push( int value ){
  map.push_back( value );
}
std::vector<int> PathMap::getStart(){
  std::vector<int> pos { startx, starty };
  return pos;
}
int PathMap::getValue( int index ){
  return map[ index ];
}
std::vector<int> PathMap::getMap(){
  return map;
}
int PathMap::getLength(){
  return map.size();
}
bool PathMap::isOpen(){
  for( std::vector<int>::size_type i = 0; i < map.size(); i++ ){
    if(map[i] == 0-1 ){
      return false;
    }
  }
  return true;
}
