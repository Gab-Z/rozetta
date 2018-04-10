#include <iostream>
#include <cmath>
#include <vector>

class Grid {

  int width;
  int height;
  int length;
  std::vector<int> vec;

  public:
    Grid();
    Grid( int _w, int _h );
    int to1d( int _x, int _y );
    std::vector<int> to2d( int i );
    void push( int val );
    int get( int i );
    int get( int _x, int _y );
    void set( int i, int val );
    void set( int _x, int _y, int val );
    int size();
};
