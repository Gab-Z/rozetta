#ifndef DEF_TowerDefense
#define DEF_TowerDefense

#include <nan.h>
#include "../GameLevel/index.h"
#include "../Converter/Converter.h"

class TowerDefense  : public Nan::ObjectWrap {

  GameLevelBase* level;

  public:

    void setLevel( GameLevelBase* _level );

    static NAN_MODULE_INIT( Init );

    static NAN_METHOD( New );

    static Nan::Persistent<v8::FunctionTemplate> constructor;

    static NAN_GETTER( HandleGetters );

    static NAN_SETTER( HandleSetters );

    static NAN_METHOD( getFloors );

    static NAN_METHOD( getTiles );

    static NAN_METHOD( getStructuresDefs );

    static NAN_METHOD( testStructuresPos );

    static NAN_METHOD( getMoveMap );

    static NAN_METHOD( addStructures );

    static NAN_METHOD( getStructures );

};

#endif
