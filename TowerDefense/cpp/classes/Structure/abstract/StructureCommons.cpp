#include "StructureCommons.h"

StructureCommons::StructureCommons(){

}
StructureCommons::StructureCommons( int _x, int _y ){
  x = _x;
  y = _y;
}
StructureCommons::StructureCommons( int _x, int _y, GridIndicesList* _points){
  x = _x;
  y = _y;
  setGridPositions( _points );
}
StructureCommons::StructureCommons( int _x, int _y, std::vector<int> _points ){
  x = _x;
  y = _y;
  positions = new GridIndicesList( _points );
}
void StructureCommons::setGridPositions( GridIndicesList* _points ){
  positions = _points;
}
void StructureCommons::setGridPositions( std::vector<int> v ){
  positions = new GridIndicesList( v );
}
void StructureCommons::setPosition( int _x, int _y ){
  x = _x;
  y = _y;
}
