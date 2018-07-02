#ifndef DEF_GameLevelBase
#define DEF_GameLevelBase

#include <nan.h>
#include <cmath>
#include <string>
#include "../Floor/floorsList.h"
#include "../Structure/Structure.h"
#include "../Structure/structuresDefList.h"
#include "../Tile/Tile.h"
#include "../converter/converter.h"
#include "DestinationPt.h"
#include "TethaSearchTile.h"
#include "../Character/Character.h"
#include "../Character/characterTypeList.h"

class GameLevelBase {

  int mapWidth;
  int mapHeight;
  int size;
  std::vector<int> startPoints;
  std::vector<int> endPoints;
  std::vector<Structure*> structures;
  std::vector<Character*> ennemies;

  public:

    std::vector<DestinationPt> nullDestPt{ DestinationPt() };
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
    Structure*& getStructureRefById( int _id );
    std::vector<int> getStartByIndex( int _i );
    v8::Local<v8::Object> getWays();
    static v8::Local<v8::Array> getCommonTextures();
    bool isPointOnStructureById( int _id, int _x, int _y );
    int destroyStructById( int _id );
    v8::Local<v8::Array> getStructureUpgradesByTypeName( std::string _typeName );
    bool upgradeStructure( int _id, std::string _typeName );

    virtual v8::Local<v8::Array> getTilesArray(){ return Nan::New<v8::Array>(); };
    virtual Tile* getTile( int _i ){ return new Tile(); };
    virtual Tile* getTileByPosition( int _x, int _y ){ return new Tile(); };
    virtual std::vector<bool> testMultipleStructurePos( std::vector<int> _positions, std::string _typeName, int _rotation ){ return std::vector<bool>(); };
    //virtual std::vector<float> getMoveMap(){ return std::vector<float>(); };
    //virtual std::vector<int> getIntMap(){ return std::vector<int>(); };
    virtual bool addStructures( std::vector<int> _positions, std::string _typeName, int _rotation ){ return false;};
    virtual v8::Local<v8::Array> getStructures(){ return Nan::New<v8::Array>(); };
    //virtual bool testMapOpening( std::vector<int> _positions, std::vector<int> _strucDefPositions ){ return false; };
    virtual int removeStructById( int _id ){ return 0; };
    virtual int destroyStructsByZone( int _startx, int _starty, int _endx, int _endy ){ return 0; };
    virtual int getStructureIdByPosition( int _x, int _y ){ return 0; };
    virtual std::vector<float> pathMap( int _startx, int _starty ){ return std::vector<float>(); };
    //virtual char * pathMapBuffer( int _startx, int _starty ){ return new char[ 0 ]; };
    virtual std::vector<char> pathMapChar( int _startx, int _starty ){ return std::vector<char>(); };

    virtual v8::Local<v8::Array> getStructureGrid( std::string _typeName, int _rotation ){ return Nan::New<v8::Array>(); };

    virtual std::vector<int> lineOfSight4View( int x0, int y0, int x1, int y1 ){ return std::vector<int>(); };
    //virtual bool isTraversable( int _x, int _y ){ return false; };
    virtual float getTileSpeed( int _x, int _y ){ return 0.0; };

    //virtual void tethaCheck( int tx, int ty, TethaSearchTile& t, int &neighbx, int &neighby, int &parentx, int &parenty, float &parentVal, float &nv, float hDist, std::vector<int> &newList ){};
    //virtual void tethaCheckN( Tile* testedTile, TethaSearchTile& t, int &neighbx, int &neighby, int &parentx, int &parenty, float &parentVal, float &nv, float hDist, std::vector<Tile*> &newList ){};
    virtual void tethaSearch( int _startx, int _starty ){};
    virtual void tethaSearchN( int _startx, int _starty ){};
    virtual float lineSight( int x0, int y0, int x1, int y1 ){ return 0.0; };
    virtual void updateAllTethaPaths(){};
    virtual std::vector<int> getTethaPath( int _startx, int _starty, int _destinationx, int _destinationy ){ return std::vector<int>(); };


    virtual void addDestinationPoint( int _x, int _y ){};
    virtual void removeDestinationPoint( int _x, int _y ){};
    virtual DestinationPt& getOrAddDestinationPt( int _x, int _y ){ DestinationPt& dp = nullDestPt[ 0 ]; return dp; };
    virtual DestinationPt& getDestinationPt( int _x, int _y ){ DestinationPt& dp = nullDestPt[ 0 ]; return dp; };
};

#endif
