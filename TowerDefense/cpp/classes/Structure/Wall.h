#ifndef DEF_Wall
#define DEF_Wall

#include "Structure.h"

const std::string WallTypeName = "Wall";

class Wall : public Structure {

  public:
    Wall();
    Wall( int _x, int _y );
    virtual std::string getTypeName(){ return typeName; };
    virtual std::vector<int> getGrid(){ return std::vector<int> { 1 }; };
    virtual std::string getImgUrl(){ return "wall.jpg"; };
    virtual int getCost(){ return 10; };
    virtual int gridWidth(){ return 1; };
    virtual int gridHeight(){ return 1; };
    static std::string typeName;
};


#endif
