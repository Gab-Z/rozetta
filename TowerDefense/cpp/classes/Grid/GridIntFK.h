#ifndef DEF_GridIntFK
#define DEF_GridIntFK

#include <iostream>
#include <cmath>
#include <vector>
#include "GridDimensions.h"

class GridIntFK : public GridDimensions {

  int vec[ 250000 ];

  public:
    GridIntFK( int _w, int _h );
    int get( int i );
    int get( int _x, int _y );
    void set( int i, int val );
    void set( int _x, int _y, int val );
};

#endif
