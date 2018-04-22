#include "GridIntFK.h"

GridIntFK::GridIntFK( int _w, int _h ):GridDimensions( _w, _h){
  //vec = std::vector<int>( 0 );
  //vec = new int[ 4000000 ];
}

/*
void GridInt::push( int val ){
  vec.push_back( val );
}

std::vector<int> GridInt::get(){
  return vec;
}
*/
int GridIntFK::get( int i ){
  return vec[ i ];
}
int GridIntFK::get( int _x, int _y ){
  return vec[ to1d( _x, _y ) ];
}
void GridIntFK::set( int i, int val ){
  vec[ i ] = val;
}
void GridIntFK::set( int _x, int _y, int val ){
  vec[ to1d( _x, _y ) ] = val;
}
/*
void GridInt::set( std::vector<int> v ){
  vec = v;
}
*/
