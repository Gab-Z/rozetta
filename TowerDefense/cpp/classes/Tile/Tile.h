#ifndef DEF_Tile
#define DEF_Tile

#include <nan.h>
#include <vector>

class Tile {

  int x;
  int y;
  int structureId = 0;
  int floorTypeId = 0;

  public:

    Tile();
    Tile( int _x, int _y );
    Tile( int _x, int _y, int _floorTypeId );
    std::vector<int> getPosition();
    int getx();
    int gety();
    int getStructureId();
    int getFloorId();
    void setPosition( int _x, int _y );
    void setStructureId( int _id );
    void setFloorTypeId( int _id );
    v8::Local<v8::Object> toObj();
};

#endif