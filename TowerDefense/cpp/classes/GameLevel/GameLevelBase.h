#ifndef DEF_GameLevelBase
#define DEF_GameLevelBase

#include <nan.h>
#include <cmath>
#include <string>
#include "../Floor/FloorsList.h"
#include "../Structure/Structure.h"
#include "../Structure/StructuresDefList.h"
#include "../Tile/Tile.h"
#include "../converter/converter.h"

class GameLevelBase {

  int mapWidth;
  int mapHeight;
  int size;
  std::vector<int> startPoints;
  std::vector<int> endPoints;
  std::vector<Structure*> structures;

  public:

    GameLevelBase();
    GameLevelBase( int _width, int _height, std::vector<int> _startPts, std::vector<int> _endPts );
    int width();
    int height();
    int to1d( int _x, int _y );
    std::vector<int> to2d( int _idx );
    int getSize();
    void pushStructure( Structure* _structure );
    int structuresSize();
    Structure* getStructure( int _i );
    Structure* getStructureById( int _id );
    std::vector<int> getStartByIndex( int _i );
    v8::Local<v8::Object> getWays();
    static v8::Local<v8::Array> getCommonTextures();
    bool isPointOnStructureById( int _id, int _x, int _y );
    int destroyStructById( int _id );


    virtual v8::Local<v8::Array> getTilesArray(){ return Nan::New<v8::Array>(); };
    virtual Tile* getTile( int _i ){ return new Tile(); };
    virtual std::vector<bool> testMultipleStructurePos( std::vector<int> _positions, std::string _typeName, int _rotation ){ return std::vector<bool>(); };
    virtual std::vector<double> getMoveMap(){ return std::vector<double>(); };
    virtual bool addStructures( std::vector<int> _positions, std::string _typeName, int _rotation ){ return false;};
    virtual v8::Local<v8::Array> getStructures(){ return Nan::New<v8::Array>(); };
    virtual bool testMapOpening( std::vector<int> _positions, std::vector<int> _strucDefPositions ){ return false; };
    virtual int removeStructById( int _id ){ return 0; };
    virtual int destroyStructsByZone( int _startx, int _starty, int _endx, int _endy ){ return 0; };

    virtual v8::Local<v8::Array> getStructureGrid( std::string _typeName, int _rotation ){ return Nan::New<v8::Array>(); };
};

#endif
