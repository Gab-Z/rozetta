#ifndef DEF_TowerDefense
#define DEF_TowerDefense

#include <nan.h>
#include "../GameLevel/index.h"
#include "../converter/converter.h"

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
    static NAN_METHOD( getIntMap );
    static NAN_METHOD( addStructures );
    static NAN_METHOD( getStructures );
    static NAN_METHOD( getStructureGrid );
    static NAN_METHOD( getWays );
    static NAN_METHOD( getStructureById );
    static NAN_METHOD( getCommonTextures );
    static NAN_METHOD( isPtOnStructById );
    static NAN_METHOD( destroyStructById );
    static NAN_METHOD( destroyStructsByZone );
    static NAN_METHOD( getStructureUpgradesByTypeName );
    static NAN_METHOD( upgradeStructure );
    static NAN_METHOD( getStructureIdByPosition );
    static NAN_METHOD( getPathMap );
    static NAN_METHOD( getPathMapBuffer );

    static NAN_METHOD( isTraversable );
    static NAN_METHOD( getTileSpeed );
    static NAN_METHOD( getTethaPath );


};



#endif
