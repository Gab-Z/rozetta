#include "towerdef_Structure.h"

class OneTileStructure: public Structure {

  public:
    OneTileStructure( int x, int y );
    virtual std::string getTypeName(){ return "OneTileStructure"; };
    std::vector<int> getPositions( int x, int y );
};
