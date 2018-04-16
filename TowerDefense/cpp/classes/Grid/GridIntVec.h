#ifndef DEF_GridIntVec
#define DEF_GridIntVec

#include <iostream>
#include <nan.h>
#include <cmath>
#include <vector>
#include "GridDimensions.h"
#include "GridIndicesList.h"

class GridIntVec : public GridDimensions {

  std::vector<int> vec;

  public:
    GridIntVec();
    GridIntVec( int _w, int _h );
    GridIntVec( int _w, int _h, int val );
    void push( int val );
    std::vector<int> get();
    int get( int i );
    int get( int _x, int _y );
    void set( std::vector<int> v );
    void set( int i, int val );
    void set( int _x, int _y, int val );
    void reset( int val );
    v8::Local<v8::Array> getJsArray();
    void paste( GridIndicesList _gridpoints, int _x, int _y, int _val );
};

#endif
