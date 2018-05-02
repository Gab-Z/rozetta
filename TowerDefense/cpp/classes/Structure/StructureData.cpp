#include "StructureData.h"

StructureData::StructureData( std::string _typeName, std::string _img_url, std::vector<int> _grid, int _cost, int _gridWidth, int _gridHeight ){
  typeName = _typeName;
  img_url = _img_url;
  grid = _grid;
  cost = _cost;
  gridWidth = _gridWidth;
  gridHeight= _gridHeight;
}
