#ifndef DEF_FloorsList
#define DEF_FloorsList

#include "Floor.h"
#include <vector>

namespace FloorsList {

  static std::vector<Floor*> floorTypes = {
    new Floor( 0, "null", "null", 0.0 ),
    new Floor( 1, "bareGround", "bareGround.jpg", 1.0 ),
    new Floor( 2, "water", "water.jpg", 2.0 )
  };

  Floor* getFloorTypeById( int _id );

};

#endif
