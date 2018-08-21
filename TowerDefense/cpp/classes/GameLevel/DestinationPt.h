#ifndef DEF_DestinationPt
#define DEF_DestinationPt

#include <vector>
#include "TethaSearchTile.h"

class DestinationPt {
  public:
    int pos1d;
    std::vector<TethaSearchTile> pathPoints;
    DestinationPt();
    DestinationPt( int _pos1d );
    DestinationPt( int _pos1d, int _l );
    void init( int _l );
    void setPointTarget( int _pos, int _targetPos );
    TethaSearchTile& getPointTarget( int _pos );
    std::vector<TethaSearchTile>& getPathPoints();

};

#endif
