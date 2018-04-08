#include <string>
#include <vector>
#include <cmath>
class Structure{

  public:

    Structure( int _x, int _y, int _gridWidth, int _gridHeight);
    int getx();
    int gety();
    virtual std::string getTypeName(){ return "Structure"; };
    std::vector<int> getPositions( int _x, int _y );
    int getGridWidth();
    int getGridHeight();
    int getGridValue( int i );
    void setGridValue( int i, int val );
    void setRotation( int rot );
    int getRotation();
    void setx( int _x );
    void sety( int _y );
    void setGridW( int _w );
    void setGridHeight( int _h );
    void setGrid( std::vector<int> _grid );
    void setGridLength();

  private:

    int x;
    int y;
    int gridWidth;
    int gridHeight;
    int gridLength;
    std::vector<int> grid;
    int rotation;
    int to1d( int _x, int _y );
    std::vector<int> to2d( int p );
};
