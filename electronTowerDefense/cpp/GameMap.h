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
    void setEndPt( int i );
    void setEndPt( int _x)
    void setEndPts( std::vector<int> _v );
    std::vector<int> getEndPts();
};
