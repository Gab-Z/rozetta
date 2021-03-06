#ifndef DEF_PointList
#define DEF_PointList

#include <vector>
#include "../Converter/Converter.h"

class PointList {

  std::vector<int> vec;

  public:
    PointList();
    PointList( std::vector<int> v );
    PointList( v8::Local<v8::Array> _arr );
    void set( std::vector<int> v );
    void set( int i, int val );
    std::vector<int> get();
    int get( int i );
    int get( int _x, int _y );
    void push( int val );
    int size();
    void add( int i, int val );

};

#endif
