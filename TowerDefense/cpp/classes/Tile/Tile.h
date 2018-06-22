#ifndef DEF_Tile
#define DEF_Tile

#include <nan.h>
#include <vector>
#include "../Floor/Floor.h"
#include "../Floor/floorsList.h"
#include "../Structure/Structure.h"
#include "../Structure/structuresDefList.h"

class Tile {

  int x;
  int y;
  //int structureId = 0;
  int floorTypeId = 0;
  int isWayInOrOut = 0;
  Structure* structure = nullptr;


  public:

    Tile();
    Tile( int _x, int _y );
    Tile( int _x, int _y, int _floorTypeId );
    std::vector<int> getPosition();
    int getx();
    int gety();
    int getStructureId();
    int getFloorTypeId();
    void setPosition( int _x, int _y );
    //void setStructureId( int _id );
    void setStructure( Structure* _structure );
    Structure* getStructure();
    void setFloorTypeId( int _id );
    v8::Local<v8::Object> toObj();
    void setWayInOrOut( std::string _wayType );
    int getWayType();
    bool isBuildable();
    bool isTraversable();
    float getSpeed();
};

#endif
