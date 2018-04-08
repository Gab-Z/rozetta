#include "towerdef_Structure.h"

Structure::Structure( int _x, int _y, int _gridWidth, int _gridHeight)
  : x( _x ), y( _y ), gridWidth( _gridWidth ), gridHeight( _gridHeight ), gridLength( _gridWidth * _gridHeight ), rotation( 0 )
{
}
int Structure::getx(){
  return x;
}
int Structure::gety(){
  return y;
}
int Structure::getGridWidth(){
  return gridWidth;
}
int Structure::getGridHeight(){
  return gridHeight;
}
int Structure::getGridValue( int i ){
  return grid[ i ];
}
void Structure::setGridValue( int i, int val ){
  grid[ i ] = val;
}
void Structure::setRotation( int rot ){
  rotation = rot;
}
int Structure::getRotation(){
  return rotation;
}
std::vector<int> Structure::getPositions( int _x, int _y ){
  std::vector<int> ret;
  for( int i = 0; i < gridLength; i++ ){
    if( grid[ i ] != 1 ){ continue; }
    int dx;
    int dy;
    std::vector<int> basePos = to2d( i );
    if( rotation == 0 ){
      dx = _x + basePos[ 0 ];
      dy = _y + basePos[ 1 ];
    }else if(rotation == 1 ){
      dx = _x + gridHeight - basePos[ 1 ];
      dy = _y + basePos[ 0 ];
    }else if(rotation == 2 ){
      dx = _x + gridWidth - basePos[ 0 ];
      dy = _y + gridHeight - basePos[ 1 ];
    }else if(rotation == 3 ){
      dx = _x + basePos[ 1 ];
      dy = _y + gridWidth - basePos[ 0 ];
    }else{
      dx = _x + basePos[ 0 ];
      dy = _y + basePos[ 1 ];
    }
    ret.push_back( dx );
    ret.push_back( dy );
  }
  return ret;
}
int Structure::to1d( int _x, int _y ){
  int pos = _y * gridWidth + _x;
  return pos;
}

std::vector<int> Structure::to2d( int p ){
  std::vector<int> pos;
  int y = std::floor( p / gridWidth );
  int nbCellRows = y * gridWidth;
  int x = p - nbCellRows;
  pos.push_back(x);
  pos.push_back(y);
  return pos;
}
void Structure::setx( int _x ){
  x = _x;
}
void Structure::sety( int _y ){
  y = _y;
}
void setGridWidth( int _w ){
  gridWidth = _w;
}
void setGridHeight( int _h ){
  gridHeight = _h;
}
void setGrid( std::vector<int> _grid ){
  grid = _grid;
}
void setGridLength(){
  gridLength = gridWidth * gridHeight;
}
