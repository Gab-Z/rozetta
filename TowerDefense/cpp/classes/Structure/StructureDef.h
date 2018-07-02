#ifndef DEF_StructureDef
#define DEF_StructureDef

#include <nan.h>
#include <vector>
#include <cmath>
#include <string>
#include "../converter/converter.h"
#include "../geometry/Vec2.h"

class StructureDef {

  std::string typeName;
  int level = 1;
  std::vector<int> grid;
  std::string imgUrl;
  std::string upgradeImgUrl = "null";
  int cost;
  int gridWidth;
  int gridHeight;
  bool rotates;
  std::vector<std::string> upgradeList;

  public:

  StructureDef();
  StructureDef( std::string _typeName, std::vector<int> _grid, std::string _imgUrl, int _cost, int _gridWidth, int _gridHeight );
  StructureDef( std::string _typeName, std::vector<int> _grid, std::string _imgUrl, int _cost, int _gridWidth, int _gridHeight, bool _rotates );
  StructureDef( std::string _typeName, std::vector<int> _grid, std::string _imgUrl, int _cost, int _gridWidth, int _gridHeight, bool _rotates, std::vector<std::string> _upgradeList );
  StructureDef( std::string _typeName,  std::vector<int> _grid,
                std::string _imgUrl,    int _cost, int _gridWidth,
                int _gridHeight,        bool _rotates,
                int _level,             std::string _upgradeImgUrl,
                std::vector<std::string> _upgradeList );
  StructureDef( std::string _typeName,  std::vector<int> _grid,
                std::string _imgUrl,    int _cost, int _gridWidth,
                int _gridHeight,        bool _rotates,
                int _level,             std::string _upgradeImgUrl );

  v8::Local<v8::Object> toObj();
  std::string getTypeName();
  std::vector<int> getGrid();
  std::vector<int> getGrid( int _rotation );
  Vec2<int> rotPoint( int _x, int _y, int _rotation );
  int to1d( int _x, int _y );
  int to1d( Vec2<int> _pos );
  int to1d( int _x, int _y, int _rotation );
  int to1d( Vec2<int> _pos, int _rotation );
  Vec2<int> to2d( int _idx );
  Vec2<int> to2d( int _idx, int _rotation );

  std::string getImgUrl();
  int getGridWidth();
  int getGridHeight();
  bool isRotating();
  std::string getUpgradeImgUrl();
  std::vector<std::string> getUpgradeList();
  bool testPoint( int _x, int _y, int _rotation );
  v8::Local<v8::Object> getUpgradeData();
};

#endif
