#ifndef DEF_StructureData
#define DEF_StructureData

struct StructureData {

  StructureData( std::string _typeName, std::string _img_url, std::vector<int> _grid, int _cost, int _gridWidth, int _gridHeight );

  std::string typeName;

  std::string img_url;

  std::vector<int> grid;

  int cost;

  int gridWidth;

  int gridHeight;

};

#endif
