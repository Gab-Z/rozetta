#ifndef DEF_GridIndicesList
#define DEF_GridIndicesList

#include "GridDimensions.h"

class GridIndicesList : public GridDimensions {

  std::vector<int> vec;

  public:

    GridIndicesList();
    GridIndicesList( std::vector<int> _v );
    GridIndicesList( std::vector<int> _v, int _w, int _h );
    void convertTwoPointsList( std::vector<int> _v );
    int getIndice( int i );
    std::vector<int> getPos( int i );

};

#endif
