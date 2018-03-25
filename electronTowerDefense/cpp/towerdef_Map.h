#include <cmath>
#include "towerdef_PathMap.cpp"
#ifndef DEF_MAP
#define DEF_MAP
class Map{

  public:

    void init( int w, int h, int depMin, int depMax );

    int getPathMapIndex( int x, int y );

    std::vector<int> getArray();

    int getLength();

    std::vector<int> getFilledMap( int x, int y );

    void fillMap( int x, int y );

    std::vector<int> fillMapStep( int id, PathMap& pathMap,  std::vector<int> searchList );

    std::vector<int>  testTile( int srcX, int srcY, int offsetX, int offsetY, int value, PathMap& pathMap, std::vector<int> _list );

    std::vector<int> getPath( int startx, int starty, int destinationx, int destinationy );

    private:

    std::vector<int> arr;
    int arrW;
    int arrH;
    int arrLength;
    int rangeMin;
    int rangeMax;
    std::vector<PathMap> paths;

    int to1d( int x, int y );
    std::vector<int> to2d( int p );

};

#endif
