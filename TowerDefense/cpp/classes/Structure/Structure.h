#ifndef DEF_Structure
#define DEF_Structure

#include <vector>
#include <string>

class Structure {

  int id;
  int gridWidth;
  int gridHeight;
  int x;
  int y;

  public:

    Structure();
    Structure( int _gridWidth, int _gridHeight );
    Structure( int _x, int _y, int _gridWidth, int _gridHeight );
    std::vector<int> getDimensions();
    void setDimensions( int _gridWidth, int _gridHeight );
    void setGridWidth( int _gridWidth );
    void setGridHeight( int _gridHeight );
    std::vector<int> getPosition();
    void setPosition( int _x, int _y );
    void setX( int _x );
    void setY( int _y );
    virtual std::string typeName(){ return "Structure"; };

};

#endif
