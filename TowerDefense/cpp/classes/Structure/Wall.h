#ifndef DEF_Wall
#define DEF_Wall

#include "Structure.h"

const std::string wall_img_url = "wall.jpg";

const std::vector<int> wall_grid = { 1 };

class Wall : public Structure {

  public:
    Wall();
    Wall( int _x, int _y );
    virtual std::string getTypeName(){ return "Wall"; };
    virtual std::vector<int> getGrid(){ return wall_grid; };
    virtual std::string getImgUrl(){ return wall_img_url; };
    virtual int getCost(){ return 10; };
    virtual int gridWidth(){ return 1; };
    virtual int gridHeight(){ return 1; };
};


#endif
