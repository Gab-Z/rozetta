#include "towerdef_oneTileStruct.h"

class Wall: public OneTileStructure {

  public:
    Wall( int x, int y );
    std::string getTypeName();
};
