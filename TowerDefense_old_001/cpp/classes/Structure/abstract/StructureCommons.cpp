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
std::vector<int> StructureCommons::getTilePos( int i ){
  return positions.getPos( i );
}
std::vector<int> StructureCommons::getPosition(){
  return std::vector<int> { x, y };
}
int StructureCommons::size(){
  return positions.size();
}
std::vector<int> StructureCommons::getGridPositions(){
  int nbTiles = positions.size();
  std::vector<int> ret ( nbTiles * 2 );
  for( int i = 0; i < nbTiles; i++ ){
    std::vector<int> tilePos = positions.getPos( i );
    //tilePos[ 0 ] += x;
    //tilePos[ 1 ] += y;
    ret[ i * 2 ] = tilePos[ 0 ] + x;
    ret[ i * 2 + 1 ] = tilePos[ 1 ] + y;
  }
  return ret;
}
