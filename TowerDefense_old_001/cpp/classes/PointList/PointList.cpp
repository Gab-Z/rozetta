#include "PointList.h"

PointList::PointList(){

}
PointList::PointList( std::vector<int> v ){
  vec = v;
}
PointList::PointList( v8::Local<v8::Array> _arr ){
  vec = Converter::jsArrayToVectorInt( _arr );
}
void PointList::set( std::vector<int> v ){
  vec = v;
}
void PointList::set( int i, int val ){
  vec[ i ] = val;
}
std::vector<int> PointList::get(){
  return vec;
}
int PointList::get( int i ){
  return vec[ i ];
}
void PointList::push( int val ){
  vec.push_back( val );
}
int PointList::size(){
  return vec.size() / 2;
}
void PointList::add( int i, int val ){
  vec[ i ] = vec[ i ] + val;
}
