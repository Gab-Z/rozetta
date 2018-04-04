#include <string>
#include <vector>
#include <cmath>
class Structure{

  public:

    Structure( int _x, int _y, int _gridWidth, int _gridHeight, std::vector<int> _grid );
    int getx();
    int gety();
    virtual std::string getTypeName(){ return "Structure"; };
    std::vector<int> getPositions( int x, int y );
    int getGridWidth();
    int getGridHeight();
    int getGridValue( int i );
    void setGridValue( int i, int val );
    void setRotation( int rot );
    int getRotation();

  private:

    int x;
    int y;
    int gridWidth;
    int gridHeight;
    int gridLength;
    std::vector<int> grid;
    int rotation;
    int to1d( int x, int y );
    std::vector<int> to2d( int p );
};
