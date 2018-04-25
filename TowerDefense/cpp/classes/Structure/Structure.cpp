#include "Structure.h"

Structure::Structure(){

}
Structure::Structure( int _gridWidth, int _gridHeight ){
  gridWidth = _gridWidth;
  gridHeight = _gridHeight;
}
Structure::Structure( int _x, int _y, int _gridWidth, int _gridHeight ){
  x = _x;
  y = _y;
  gridWidth = _gridWidth;
  gridHeight = _gridHeight;
}
std::vector<int> Structure::getDimensions(){
  return std::vector<int> { gridWidth, gridHeight };
}
void Structure::setDimensions( int _gridWidth, int _gridHeight ){
  gridWidth = _gridWidth;
  gridHeight = _gridHeight;
}
void Structure::setGridWidth( int _gridWidth ){
  gridWidth = _gridWidth;
}
void Structure::setGridHeight( int _gridHeight ){
  gridHeight = _gridHeight;
}
void Structure::setPosition( int _x, int _y ){
  x = _x;
  y = _y;
}
void Structure::setX( int _x ){
  x = _x;
}
void Structure::setY( int _y ){
  y = _y;
}
