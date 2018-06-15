#ifndef DEF_Movement
#define DEF_Movement

#include "MoveData.h"
#include <string>

class Movement {

  float speedFactor;
  std::string name;
  MoveData* ( *func )( int );

  public:

    Movement( std::string _name );
    Movement( std::string _name, float _speedFactor, MoveData* ( *_func )( int ) );


};

#endif
