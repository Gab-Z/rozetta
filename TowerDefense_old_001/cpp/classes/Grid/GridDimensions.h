#ifndef DEF_GridDimensions
#define DEF_GridDimensions

#include <cmath>
#include <vector>

class GridDimensions {

  int width;
  int height;
  int length;

  public:
    GridDimensions();
    GridDimensions( int _w, int _h );
    int to1d( int _x, int _y );
    std::vector<int> to2d( int i );
    int size();
    int getWidth();
    int getHeight();
    void setDimensions( int _w, int _h );
};

#endif
