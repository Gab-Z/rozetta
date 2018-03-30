#include <nan.h>
#include "towerdef_Map.h"

class TowerDefense : public Nan::ObjectWrap {

  public:

    static NAN_MODULE_INIT(Init);

    static NAN_METHOD(New);

    static Nan::Persistent<v8::FunctionTemplate> constructor;

    static NAN_GETTER(HandleGetters);

    static NAN_SETTER(HandleSetters);

    static NAN_METHOD( addStructure );

  private:

    Map store;

};
