#ifndef DEF_TowerDefense
#define DEF_TowerDefense

#include <nan.h>
#include <cmath>
#include <string>
#include "../Converter/Converter.h"
#include "../Structure/index.h"
#include "../Floor/Floor.h"
#include "../Tile/Tile.h"

class TowerDefense  : public Nan::ObjectWrap {

  int mapWidth;
  int mapHeight;
  int size;
  std::vector<int> startPoints;
  std::vector<int> endPoints;
  std::vector<Structure*> structures;
  //std::vector<int> floorIds;
  static std::vector<Floor*> floorTypes;
  static std::vector<StructureDef*> structureTypes;
  std::vector<Tile*> tiles;
  std::vector<double> moveMap;

  public:

    TowerDefense();

    TowerDefense( int _width, int _height, std::vector<int> _startPts, std::vector<int> _endPts );

    static NAN_MODULE_INIT( Init );

    static NAN_METHOD( New );

    static Nan::Persistent<v8::FunctionTemplate> constructor;

    static NAN_GETTER( HandleGetters );

    static NAN_SETTER( HandleSetters );

    static NAN_METHOD( getFloors );

    v8::Local<v8::Array> getTilesArray();

    static NAN_METHOD( getTiles );

    static v8::Local<v8::Array> structuresDefs();

    static NAN_METHOD( getStructuresDefs );

    static NAN_METHOD( testStructuresPos );

    int width();

    int height();

    int to1d( int _x, int _y );

    std::vector<int> to2d( int _idx );

    void fillMoveMap();

    static Floor* getFloorTypeById( int _id );

    bool testStructurePos( int _x, int _y, std::string _typeName );

    std::vector<bool> testMultipleStructurePos( std::vector<int> _positions, std::string _typeName );

    static StructureDef* getStructureTypeByName( std::string _typeName );

    static NAN_METHOD( getMoveMap );
    std::vector<double> getMoveMap();

    static NAN_METHOD( addStructures );
    void addStructures( std::vector<int> _positions, std::string _typeName );

    static NAN_METHOD( getStructures );
    v8::Local<v8::Array> getStructures();
};


#endif
