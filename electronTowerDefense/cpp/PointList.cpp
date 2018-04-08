#include "PointList.h"

PointList::PointList(){

}
PointList::PointList( std::vector<int> v ){
  vec = v;
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
