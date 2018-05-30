#ifndef DEF_GameLevel
#define DEF_GameLevel

#include <nan.h>
#include "GameLevelBase.h"

class GameLevel : public GameLevelBase {

  std::vector<Tile*> tiles;
  std::vector<double> moveMap;
  std::vector<int> intMap;

  public:

    GameLevel();
    GameLevel( int _width, int _height, std::vector<int> _startPts, std::vector<int> _endPts );
    v8::Local<v8::Array> getTilesArray();
    void fillMoveMap();
    bool testStructurePos( int _x, int _y, std::string _typeName );
    std::vector<bool> testMultipleStructurePos( std::vector<int> _positions, std::string _typeName, int _rotation );
    std::vector<double> getMoveMap();
    bool addStructures( std::vector<int> _positions, std::string _typeName, int _rotation );
    v8::Local<v8::Array> getStructures();
    Tile* getTile( int _i );
    v8::Local<v8::Array> getStructureGrid( std::string _typeName, int _rotation );
    bool testMapOpening();
    bool newStructuresBlockingTest(  std::vector<int> _positions, std::vector<int> _strucDefPositions );
    int removeStructById( int _id );
    int destroyStructsByZone( int _startx, int _starty, int _endx, int _endy );
};

#endif
