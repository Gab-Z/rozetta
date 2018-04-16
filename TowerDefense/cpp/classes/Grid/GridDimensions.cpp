#include "GridDimensions.h"

GridDimensions::GridDimensions(){
  width = 0;
  height = 0;
  length = 0;
}
GridDimensions::GridDimensions( int _w, int _h ){
  width = _w;
  height = _h;
  length = width * height;
}
int GridDimensions::to1d( int _x, int _y ){
  int pos = _y * width + _x;
  return pos;
}
std::vector<int> GridDimensions::to2d( int i ){
  int _y = std::floor( i / width );
  int _x = i - (_y * width );
  std::vector<int> pos { _x, _y };
  return pos;
}
int GridDimensions::size(){
  return length;
}
int GridDimensions::getWidth(){
  return width;
}
int GridDimensions::getHeight(){
  return height;
}
void GridDimensions::setDimensions( int _w, int _h ){
  width = _w;
  height = _h;
  length = width * height;
}
