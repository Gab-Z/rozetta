#ifndef DEF_Wall
#define DEF_Wall

#include "Structure.h"

class Wall : public Structure {

  public:
    Wall();
    Wall( int _x, int _y );
    static std::vector<int> grid;
    virtual std::string typeName(){ return "Wall"; };

};


#endif
