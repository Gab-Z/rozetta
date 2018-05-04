#ifndef DEF_Structure
#define DEF_Structure

#include <nan.h>
#include <vector>
#include <string>
#include "./StructureDef.h"


class Structure {

  int id;
  int x;
  int y;
  StructureDef* structureDef;

  public:

    Structure();
    Structure( int _x, int _y,  StructureDef* _structureDef );
    std::vector<int> getPosition();
    void setPosition( int _x, int _y );
    void setX( int _x );
    void setY( int _y );

};


#endif
