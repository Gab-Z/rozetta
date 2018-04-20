#ifndef DEF_OneTileStructure
#define DEF_OneTileStructure

#include "StructureCommons.h"

class OneTileStructure : public StructureCommons {

  public:

    OneTileStructure( int _x, int _y );
    virtual std::string getTypeName(){ return "OneTileStructure"; };
    
};

#endif
