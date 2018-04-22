#ifndef DEF_Wall
#define DEF_Wall

#include "./abstract/OneTileStructure.h"

class Wall : public OneTileStructure {

  public:

  Wall();
  Wall( int _x, int _y );
  virtual std::string getTypeName(){ return "Wall"; };

};

#endif
