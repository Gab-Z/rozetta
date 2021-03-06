#ifndef DEF_PATHMAP
#define DEF_PATHMAP
#include <nan.h>
class PathMap{
  public:

    void setStart( int x, int y );
    void clearMap();
    void setValue( int index, int value );
    void push( int value );
    std::vector<int> getStart();
    int getValue( int index );
    std::vector<int> getMap();
    int getLength();
    bool isOpen();

  private:
    int startx;
    int starty;
    std::vector<int> map;
};

#endif
