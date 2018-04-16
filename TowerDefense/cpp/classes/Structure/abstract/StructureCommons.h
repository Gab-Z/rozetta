#ifndef DEF_StructureCommons
#define DEF_StructureCommons

#include "../../Grid/GridIndicesList.h"
#include <string>

class StructureCommons{

  int x;
  int y;
  int rotation;
  GridIndicesList* positions;

  public:

    StructureCommons();
    StructureCommons( int _x, int _y );
    StructureCommons( int _x, int _y, GridIndicesList* _points);
    StructureCommons( int _x, int _y, std::vector<int> _points );
    virtual std::string getTypeName(){ return "Structure"; };
    void setGridPositions( GridIndicesList* _points );
    void setGridPositions( std::vector<int> v );
    void setPosition( int _x, int _y );

};

#endif
