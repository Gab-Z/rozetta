#include "FloorsList.h"

Floor* FloorsList::getFloorTypeById( int _id ){
  int l = FloorsList::floorTypes.size();
  for( int i = 0; i < l; i ++ ){
    if( FloorsList::floorTypes[ i ]->getId() == _id ){
      return FloorsList::floorTypes[ i ];
    }
  }
  return FloorsList::floorTypes[ 0 ];
}
