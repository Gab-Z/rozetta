#include <cmath>
#include "towerdef_PathMap.h"
#include "towerdef_Structures.h"
#ifndef DEF_MAP
#define DEF_MAP
class Map{

  public:

    void init( int w, int h, int depMin, int depMax, std::vector<int> _startPts, std::vector<int> _endPts );

    int getPathMapIndex( int x, int y );

    std::vector<int> getArray();

    int getLength();

    std::vector<int> getFilledMap( int x, int y );

    void fillMap( int x, int y );

    std::vector<int> fillMapStep( PathMap& pathMap,  std::vector<int> searchList );

    std::vector<int>  testTile( int srcX, int srcY, int offsetX, int offsetY, int value, PathMap& pathMap, std::vector<int> _list );

    std::vector<int> getPath( int startx, int starty, int destinationx, int destinationy );

    std::vector<int> fillAccessMap( int _startx, int _starty, std::vector<int>& arrMap );

    std::vector<int> fillAccessMapStep( std::vector<int>& ret, std::vector<int> searchList, std::vector<int>& arrMap );

    std::vector<int> testAccessTile( int srcX, int srcY, int offsetX, int offsetY, int value, std::vector<int>& ret, std::vector<int> _list, std::vector<int>& arrMap );

    int getWidth();

    int getHeight();

    bool addStructure( std::string _className, std::vector<int> positions );

    v8::Local<v8::Array> getStructures();
    std::vector<int> getStartPts();
    std::vector<int> getEndPts();
    std::vector<int> getAccessMap( std::vector<int> positions );
    std::vector<int> getStructuresPos();

    private:

    std::vector<int> arr;
    int arrW;
    int arrH;
    int arrLength;
    int rangeMin;
    int rangeMax;
    std::vector<int> startPts;
    std::vector<int> endPts;
    std::vector<PathMap> paths;
    std::vector<Structure*> structures;

    int to1d( int x, int y );
    std::vector<int> to2d( int p );

};

#endif
