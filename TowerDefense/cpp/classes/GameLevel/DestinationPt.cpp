#include "DestinationPt.h"

DestinationPt::DestinationPt(){};
DestinationPt::DestinationPt( int _pos1d ){
  pos1d = _pos1d;
};
DestinationPt::DestinationPt( int _pos1d, int _l ){
  pos1d = _pos1d;
  pathPoints = std::vector<TethaSearchTile>( _l );
};
void DestinationPt::init( int _l ){
  pathPoints = std::vector<TethaSearchTile>( _l );
};
void DestinationPt::setPointTarget( int _pos, int _targetPos  ){
  pathPoints[ _pos ].parentPos = _targetPos;
};
TethaSearchTile& DestinationPt::getPointTarget( int _pos ){
  return pathPoints[ _pos ];
};
std::vector<TethaSearchTile>& DestinationPt::getPathPoints(){
  return pathPoints;
};
