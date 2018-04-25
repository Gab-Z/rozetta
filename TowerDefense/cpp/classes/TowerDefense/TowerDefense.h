#ifndef DEF_TowerDefense
#define DEF_TowerDefense

#include <nan.h>
#include "../Converter/Converter.h"
#include "../Structure/index.h"
#include "../Floor/Floor.h"

class TowerDefense  : public Nan::ObjectWrap {

  int mapWidth;
  int mapHeight;
  std::vector<int> startPoints;
  std::vector<int> endPoints;
  std::vector<Structure*> structures;

  public:

    TowerDefense( int _width, int _height, std::vector<int> _startPts, std::vector<int> _endPts );

    static NAN_MODULE_INIT(Init);

    static NAN_METHOD(New);

    static Nan::Persistent<v8::FunctionTemplate> constructor;

    static NAN_GETTER(HandleGetters);

    static NAN_SETTER(HandleSetters);

    int width();

    int height();

};


#endif
