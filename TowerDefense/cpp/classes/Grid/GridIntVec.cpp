#include "GridIntVec.h"

GridIntVec::GridIntVec():GridDimensions(){
  vec = std::vector<int>( 0 );
}
GridIntVec::GridIntVec( int _w, int _h ):GridDimensions( _w, _h ){
  vec = std::vector<int>( _w * _h );
}
GridIntVec::GridIntVec( int _w, int _h, int val ):GridDimensions( _w, _h ){
  vec = std::vector<int>( _w * _h, val );
}
void GridIntVec::push( int val ){
  vec.push_back( val );
}
std::vector<int> GridIntVec::get(){
  return vec;
}
int GridIntVec::get( int i ){
  return vec[ i ];
}
int GridIntVec::get( int _x, int _y ){
  return vec[ to1d( _x, _y ) ];
}
void GridIntVec::set( int i, int val ){
  vec[ i ] = val;
}
void GridIntVec::set( int _x, int _y, int val ){
  vec[ to1d( _x, _y ) ] = val;
}
void GridIntVec::set( std::vector<int> v ){
  vec = v;
}
void GridIntVec::reset( int val ){
  vec = std::vector<int>( size(), val );
}
v8::Local<v8::Array> GridIntVec::getJsArray(){
  int sl = size();
  v8::Local<v8::Array> jsArr = Nan::New<v8::Array>( sl );
  for( int i = 0; i < sl; i++ ){
    v8::Local<v8::Value> jsElement = Nan::New( vec[ i ] );
    jsArr->Set( i, jsElement );
  }
  return jsArr;
}
void GridIntVec::paste( GridIndicesList _gridpoints, int _x, int _y, int _val ){
  int l = _gridpoints.size();
  for( int i = 0; i < l; i++ ){
    std::vector<int> v = _gridpoints.getPos( i );
    set( _x + v[ 0 ], _y + v[ 1 ], _val );
  }

}
