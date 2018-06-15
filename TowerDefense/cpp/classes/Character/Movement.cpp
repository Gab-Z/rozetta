#include "Movement.h"

Movement::Movement( std::string _name ){
  name = _name;
}
Movement::Movement( std::string _name, float _speedFactor, MoveData* ( *_func )( int ) ){
  name = _name;
  speedFactor = _speedFactor;
  func = _func;
}
