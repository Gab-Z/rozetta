#ifndef DEF_TowerDefense
#define DEF_TowerDefense

#include <nan.h>
#include "../Grid/GridIntVec.h"
#include "../Structure/index.h"
#include "../Converter/Converter.h"
#include "../PointList/PointList.h"

class TowerDefense  : public Nan::ObjectWrap {

  GridIntVec grd_terrain;
  GridIntVec grd_game;
  std::vector<StructureCommons*> structures;
  bool addStructure( StructureCommons* _structure );
  v8::Local<v8::Array> getStructures();

  public:

    static NAN_MODULE_INIT(Init);

    static NAN_METHOD(New);

    static Nan::Persistent<v8::FunctionTemplate> constructor;

    static NAN_GETTER(HandleGetters);

    static NAN_SETTER(HandleSetters);

    //static NAN_METHOD( addStructure );

    static NAN_METHOD( testClass );

    static NAN_METHOD( addStructures );



};

#endif
