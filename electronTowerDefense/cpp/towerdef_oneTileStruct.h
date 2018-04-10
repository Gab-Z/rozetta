#include "towerdef_Structure.h"

class OneTileStructure: public Structure {

  public:
    OneTileStructure( int _x, int _y );
    virtual std::string getTypeName(){ return "OneTileStructure"; };
    std::vector<int> getPositions( int _x, int _y );
};
