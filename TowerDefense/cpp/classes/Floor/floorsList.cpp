#include "floorsList.h"

Floor* floorsList::getFloorTypeById( int _id ){
  int l = floorsList::floorTypes.size();
  for( int i = 0; i < l; i ++ ){
    if( floorsList::floorTypes[ i ]->getId() == _id ){
      return floorsList::floorTypes[ i ];
    }
  }
  return floorsList::floorTypes[ 0 ];
}
