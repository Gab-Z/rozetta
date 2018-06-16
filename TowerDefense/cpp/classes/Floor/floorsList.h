#ifndef DEF_floorsList
#define DEF_floorsList

#include "Floor.h"
#include <vector>

namespace floorsList {

  static std::vector<Floor*> floorTypes = {
    new Floor( 0, "null", "null", 0.0 ),
    new Floor( 1, "bareGround", "bareGround.jpg", 1.0 ),
    new Floor( 2, "water", "water.jpg", 4.0 )
  };

  Floor* getFloorTypeById( int _id );

};

#endif
