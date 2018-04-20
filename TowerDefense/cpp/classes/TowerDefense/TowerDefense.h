#ifndef DEF_TowerDefense
#define DEF_TowerDefense

#include <nan.h>
#include "../Grid/GridIntVec.h"
#include "../Structure/index.h"

class TowerDefense  : public Nan::ObjectWrap {

  GridIntVec grd_terrain;
  std::vector<StructureCommons*> structures;

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
