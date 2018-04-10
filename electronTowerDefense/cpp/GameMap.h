#include "Grid.h"
#include "PointList.h"
class GameMap : public Grid {
  PointList startPts;
  PointList endPts;

  public:
    GameMap();
    GameMap( int _w, int _h );
    void setStartPts( std::vector<int> _v );
    std::vector<int> getStartPts();
    void setStartPt( int i, int val );
    void setEndPt( int i, int val );
    void setEndPts( std::vector<int> _v );
    std::vector<int> getEndPts();
    
};
