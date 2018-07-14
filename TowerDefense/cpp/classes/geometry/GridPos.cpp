#include "GridPos.h"

GridPos::GridPos( int& _gridWidth ):gridWidth( _gridWidth ){};
GridPos::GridPos( int& _gridWidth, int _x, int _y, int _idx ):gridWidth( _gridWidth ), pos( Vec2<int>( _x, _y ) ), idx( _idx ){};
GridPos::GridPos( int& _gridWidth, int _x, int _y ):gridWidth( _gridWidth ), pos( Vec2<int>( _x, _y ) ), idx( to1d( _x, _y ) ){};
GridPos::GridPos( int& _gridWidth, Vec2<int> _pos ):gridWidth( _gridWidth ), pos( _pos ){
    idx = to1d( _pos );
};
GridPos::GridPos( int& _gridWidth, int _idx ):gridWidth( _gridWidth ), pos( to2d( _idx ) ), idx( _idx ){};
GridPos::~GridPos(){};
GridPos& GridPos::operator=( const GridPos& copy){
  idx = std::move( copy.idx );
  pos = std::move( copy.pos );
  gridWidth = std::move( copy.gridWidth );
  return *this;
}
GridPos operator+( const GridPos& gridpos, const Vec2<int>& vec2 ){
  return GridPos( gridpos.gridWidth, gridpos.pos + vec2 );
}

int GridPos::x(){ return pos.x; };
int GridPos::y(){ return pos.y; };
int GridPos::to1d( int _x, int _y ){
  return _y * gridWidth + _x;
}
int GridPos::to1d( Vec2<int> _pos ){
  return _pos.y * gridWidth + _pos.x;
}
Vec2<int> GridPos::to2d( int _idx ){
  return Vec2<int>( _idx % gridWidth, (int) _idx / gridWidth );
}
