#ifndef DEF_StructureDef
#define DEF_StructureDef

#include <vector>
#include <string>

struct StructureDef {

  StructureDef();

  StructureDef( std::string _typeName, std::vector<int> _grid, std::string _imgUrl, int _cost, int _gridWidth, int _gridHeight );

  std::string typeName;
  std::vector<int> grid;
  std::string imgUrl;
  int cost;
  int gridWidth;
  int gridHeight;

};

#endif
