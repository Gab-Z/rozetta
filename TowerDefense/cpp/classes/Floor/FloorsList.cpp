#include "FloorsList.h"

/*
std::vector<Floor*> FloorsList::floorTypes = {
  new Floor( 0, "null", "null", 0.0 ),
  new Floor( 1, "bareGround", "bareGround.jpg", 1.0 ),
  new Floor( 2, "water", "water.jpg", 2.0 )
}
*/
Floor* FloorsList::getFloorTypeById( int _id ){
  int l = FloorsList::floorTypes.size();
  for( int i = 0; i < l; i ++ ){
    if( FloorsList::floorTypes[ i ]->getId() == _id ){
      return FloorsList::floorTypes[ i ];
    }
  }
  return FloorsList::floorTypes[ 0 ];
}
