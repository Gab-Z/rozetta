#include "Grid.h"

Grid::Grid(){
  width = 0;
  height = 0;
  length = 0;
}
Grid::Grid( int _w, int _h ){
  width = _w;
  height = _h;
  length = width * height;
  std::vector<int> v( length );
  vec = v;
}
int Grid::to1d( int _x, int _y ){
  int pos = _y * width + _x;
  return pos;
}
std::vector<int> Grid::to2d( int i ){
  std::vector<int> pos;
  int _y = std::floor( i / width );
  int nbCellRows = _y * width;
  int _x = i - nbCellRows;
  pos.push_back( _x );
  pos.push_back( _y );
  return pos;
}
void Grid::push( int val ){
  vec.push_back( val );
}
int Grid::get( int i ){
  return vec[ i ];
}
int Grid::get( int _x, int _y ){
  return vec[ to1d( _x, _y ) ];
}
void Grid::set( int i, int val ){
  vec[ i ] = val;
}
void Grid::set( int _x, int _y, int val ){
  vec[ to1d( _x, _y ) ] = val;
}
int Grid::size(){
  return length;
}
