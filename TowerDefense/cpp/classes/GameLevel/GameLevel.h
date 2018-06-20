#ifndef DEF_GameLevel
#define DEF_GameLevel

#include <nan.h>
#include <limits>
#include "GameLevelBase.h"

class GameLevel : public GameLevelBase {

  std::vector<Tile*> tiles;
  std::vector<float> moveMap;
  //std::vector<int> intMap;


  std::vector<DestinationPt> destinationPoints;

  static DestinationPt nullDestinationPt;

  public:

    GameLevel();
    GameLevel( int _width, int _height, std::vector<int> _startPts, std::vector<int> _endPts );
    GameLevel( int _width, int _height, std::vector<int> _startPts, std::vector<int> _endPts, std::vector<int> _floorIds );
    v8::Local<v8::Array> getTilesArray();
    //void fillMoveMap();
    bool testStructurePos( int _x, int _y, std::string _typeName );
    std::vector<bool> testMultipleStructurePos( std::vector<int> _positions, std::string _typeName, int _rotation );
    //std::vector<float> getMoveMap();
    //std::vector<int> getIntMap();
    bool addStructures( std::vector<int> _positions, std::string _typeName, int _rotation );
    v8::Local<v8::Array> getStructures();
    Tile* getTile( int _i );
    Tile* getTileByPosition( int _x, int _y );
    v8::Local<v8::Array> getStructureGrid( std::string _typeName, int _rotation );
    bool testMapOpening();
    bool newStructuresBlockingTest(  std::vector<int> _positions, std::vector<int> _strucDefPositions );
    int removeStructById( int _id );
    int destroyStructsByZone( int _startx, int _starty, int _endx, int _endy );
    int getStructureIdByPosition( int _x, int _y );
    std::vector<float> pathMap( int _startx, int _starty);
    //char * pathMapBuffer( int _startx, int _starty );
    std::vector<char> pathMapChar( int _startx, int _starty);



    //bool isTraversable( int _x, int _y );
    float getTileSpeed( int _x, int _y );

    void tethaCheck( int tx, int ty, TethaSearchTile& t, int &neighbx, int &neighby, int &parentx, int &parenty, float &parentVal, float &nv, float hDist, std::vector<int> &newList );
    void tethaSearch( int _startx, int _starty );
    float lineSight( int x0, int y0, int x1, int y1 );
    void updateAllTethaPaths();
    std::vector<int> getTethaPath( int _startx, int _starty, int _destinationx, int _destinationy );

    void addDestinationPoint( int _x, int _y );
    void removeDestinationPoint( int _x, int _y );
    DestinationPt& getOrAddDestinationPt( int _x, int _y );
    DestinationPt& getDestinationPt( int _x, int _y );
};

#endif
