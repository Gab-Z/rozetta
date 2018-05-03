#include "StructureDef.h"

StructureDef::StructureDef(){}

StructureDef::StructureDef( std::string _typeName, std::vector<int> _grid, std::string _imgUrl, int _cost, int _gridWidth, int _gridHeight ){
  typeName = _typeName;
  grid = _grid;
  imgUrl = _imgUrl;
  cost = _cost;
  gridWidth = _gridWidth;
  gridHeight = _gridHeight;
}
