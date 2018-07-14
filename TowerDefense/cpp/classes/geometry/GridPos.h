#ifndef DEF_GridPos
#define DEF_GridPos

#include "./Vec2.h"
#include <utility>
#include <algorithm>

class GridPos {
public:
  int& gridWidth;
  Vec2<int> pos;
  int idx;

  GridPos( int& _gridWidth );
  GridPos( int& _gridWidth, int _x, int _y, int _idx );
  GridPos( int& _gridWidth, int _x, int _y );
  GridPos( int& _gridWidth, Vec2<int> _pos );
  GridPos( int& _gridWidth, int _idx );

  ~GridPos();

  GridPos& operator=( const GridPos& copy );
  friend GridPos operator+( const GridPos& gridpos, const Vec2<int>& vec2 );

  int x();
  int y();

  int to1d( int _x, int _y );
  int to1d( Vec2<int> _pos );
  Vec2<int> to2d( int _idx );


};

#endif
