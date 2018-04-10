#include "towerdef_oneTileStruct.h"

class Wall: public OneTileStructure {

  public:
    Wall( int _x, int _y );
    std::string getTypeName();
  
};
