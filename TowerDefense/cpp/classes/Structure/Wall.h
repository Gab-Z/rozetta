#ifndef DEF_Wall
#define DEF_Wall

#include "Structure.h"

struct dataWall{

  const std::string typeName = "Wall";

  const std::string img_url = "wall.jpg";

  const std::vector<int> grid = { 1 };

  const int cost = 10;

  const int gridWidth = 1;

  const int gridHeight = 1;

}
const StructureData wallData( "Wall", "Wall.jpg", std::vector<int> { 1 }, 10, 1, 1 );

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
