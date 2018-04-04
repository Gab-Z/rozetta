#include "towerdef_Structure.h"

Structure::Structure( int _x, int _y, int _gridWidth, int _gridHeight, std::vector<int> _grid ){
  x = _x;
  y = _y;
  gridWidth = _gridWidth;
  gridHeight = _gridHeight;
  grid = _grid;
  gridLength = _gridWidth * _gridHeight;
  rotation = 0;
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
std::vector<int> Structure::getPositions( int x, int y ){
  std::vector<int> ret;
  for( int i = 0; i < gridLength; i++ ){
    if( grid[ i ] != 1 ){ continue; }
    int dx;
    int dy;
    std::vector<int> basePos = to2d( i );
    if( rotation == 0 ){
      dx = x + basePos[ 0 ];
      dy = y + basePos[ 1 ];
    }else if(rotation == 1 ){
      dx = x + gridHeight - basePos[ 1 ];
      dy = y + basePos[ 0 ];
    }else if(rotation == 2 ){
      dx = x + gridWidth - basePos[ 0 ];
      dy = y + gridHeight - basePos[ 1 ];
    }else if(rotation == 3 ){
      dx = x + basePos[ 1 ];
      dy = y + gridWidth - basePos[ 0 ];
    }
    ret.push_back( dx );
    ret.push_back( dy );
  }
  return ret;
}
int Structure::to1d( int x, int y ){
  int pos = y * gridWidth + x;
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
