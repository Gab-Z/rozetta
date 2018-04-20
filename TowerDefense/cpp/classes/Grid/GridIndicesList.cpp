#include "GridIndicesList.h"

GridIndicesList::GridIndicesList():GridDimensions(){

}
GridIndicesList::GridIndicesList( std::vector<int> _v ):GridDimensions(){
  convertTwoPointsList( _v );
}
GridIndicesList::GridIndicesList( std::vector<int> _v, int _w, int _h ):GridDimensions( _w, _h ){
  vec = _v;
}
void GridIndicesList::setVec( std::vector<int> _v ){
  vec = _v;
}
void GridIndicesList::setVec( std::vector<int> _v, int _w, int _h ){
  vec = _v;
  setDimensions( _w, _h );
}
void GridIndicesList::convertTwoPointsList( std::vector<int> _v ){
  int l = _v.size() / 2;
  vec = std::vector<int>( l );
  int w = 0;
  int h = 0;
  for( int j = 0; j < l; j++ ){
    if( _v[ j * 2 ] > w ){
      w = _v[ j * 2 ];
    }
    if( _v[ j * 2 + 1 ] > h ){
      h = _v[ j * 2 + 1 ];
    }
  }
  w++;
  h++;
  setDimensions( w, h );
  for( int i = 0; i < l; i++ ){
    int x = _v[ i * 2 ];
    int y = _v[ i * 2 + 1 ];
    vec[ i ] = to1d( x, y );
  }
}
int GridIndicesList::getIndice( int i ){
  return vec[ i ];
}
std::vector<int> GridIndicesList::getPos( int i ){
  return to2d( vec[ i ] );
}
