#ifndef DEF_StructureDef
#define DEF_StructureDef

#include <nan.h>
#include <vector>
#include <string>
#include "../Converter/Converter.h"

class StructureDef {

  std::string typeName;
  std::vector<int> grid;
  std::string imgUrl;
  int cost;
  int gridWidth;
  int gridHeight;

  public:

  StructureDef();

  StructureDef( std::string _typeName, std::vector<int> _grid, std::string _imgUrl, int _cost, int _gridWidth, int _gridHeight );

  v8::Local<v8::Object> toObj();


};

#endif
