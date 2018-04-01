#include "towerdef_Structure.h"

class Wall: public Structure {

  public:
    Wall( int x, int y );
    std::string getTypeName();
};
