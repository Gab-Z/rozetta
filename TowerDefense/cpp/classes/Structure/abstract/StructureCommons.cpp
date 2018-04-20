#include "StructureCommons.h"

StructureCommons::StructureCommons(){

}
StructureCommons::StructureCommons( int _x, int _y ){
  x = _x;
  y = _y;
}
StructureCommons::StructureCommons( int _x, int _y, GridIndicesList _points){
  x = _x;
  y = _y;
  positions = _points;
}
StructureCommons::StructureCommons( int _x, int _y, std::vector<int> _points ){
  x = _x;
  y = _y;
  positions.convertTwoPointsList( _points );
}
StructureCommons::StructureCommons( int _x, int _y, std::vector<int> _points, int _w, int _h ){
  x = _x;
  y = _y;
  positions.setVec( _points, _w, _h );
}
void StructureCommons::setGridPositions( GridIndicesList _points ){
  positions = _points;
}
void StructureCommons::setGridPositions( std::vector<int> v ){
  positions.setVec( v );
}
void StructureCommons::setPosition( int _x, int _y ){
  x = _x;
  y = _y;
}
