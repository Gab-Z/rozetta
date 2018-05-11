#ifndef DEF_GameLevel
#define DEF_GameLevel

#include <nan.h>
#include "GameLevelBase.h"

class GameLevel : public GameLevelBase {

  std::vector<Tile*> tiles;
  std::vector<double> moveMap;

  public:

    GameLevel();
    GameLevel( int _width, int _height, std::vector<int> _startPts, std::vector<int> _endPts );
    v8::Local<v8::Array> getTilesArray();
    void fillMoveMap();
    bool testStructurePos( int _x, int _y, std::string _typeName );
    std::vector<bool> testMultipleStructurePos( std::vector<int> _positions, std::string _typeName );
    std::vector<double> getMoveMap();
    void addStructures( std::vector<int> _positions, std::string _typeName );
    v8::Local<v8::Array> getStructures();
    Tile* getTile( int _i );
};

#endif
