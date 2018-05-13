#ifndef DEF_GameLevelBase
#define DEF_GameLevelBase

#include <nan.h>
#include <cmath>
#include <string>
#include "../Floor/FloorsList.h"
#include "../Structure/Structure.h"
#include "../Structure/StructuresDefList.h"
#include "../Tile/Tile.h"
#include "../Converter/Converter.h"

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

    virtual v8::Local<v8::Array> getTilesArray(){ return Nan::New<v8::Array>(); };
    virtual Tile* getTile( int _i ){ return new Tile(); };
    virtual std::vector<bool> testMultipleStructurePos( std::vector<int> _positions, std::string _typeName, int _rotation ){ return std::vector<bool>(); };
    virtual std::vector<double> getMoveMap(){ return std::vector<double>(); };
    virtual void addStructures( std::vector<int> _positions, std::string _typeName, int _rotation ){};
    virtual v8::Local<v8::Array> getStructures(){ return Nan::New<v8::Array>(); };

    virtual v8::Local<v8::Array> getStructureGrid( std::string _typeName, int _rotation ){ return Nan::New<v8::Array>(); };
};

#endif
