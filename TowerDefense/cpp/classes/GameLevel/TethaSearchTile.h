#ifndef DEF_TethaSearchTile
#define DEF_TethaSearchTile

class TethaSearchTile {
  public:
    int parentPos;
    float hVal;
    TethaSearchTile();
    TethaSearchTile( int _parentPos, float _hVal );
    void setParentPos( int _parentPos );
    void setHVal( float _hVal );
    void set( int _parentPos, float _hVal );
};

#endif
