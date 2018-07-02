#ifndef DEF_GridPos
#define DEF_GridPos

struct GridPos {

  int x;
  int y;
  int pos1d;
  //void(*)(update2d);
  //void(*)(update1d);

  GridPos();
  GridPos( int _x, int _y, int _pos1d );

};

#endif
