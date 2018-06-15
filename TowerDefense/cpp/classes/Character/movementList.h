#ifndef DEF_movementList
#define DEF_movementList

#include "Movement.h"
#include <vector>

namespace movementList {

  static std::vector<Movement*> list = {
    new Movement( "null" ),
    new Movement( "walk", 1,
                  MoveData* ( int _pos){
                    return new MoveData( _pos );
                  })

  };

MoveData ( *_func )( int )

};

#endif
