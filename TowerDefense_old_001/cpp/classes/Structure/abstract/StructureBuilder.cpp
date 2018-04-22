#include "StructureBuilder.h"

StructureCommons* StructureBuilder::create( std::string typeName, int _x, int _y ){
  StructureCommons *struc;
  if( typeName ==  "Wall"  ){
    struc = new Wall( _x, _y );
  }
  return struc;
}
