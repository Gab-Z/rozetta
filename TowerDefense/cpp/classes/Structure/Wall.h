#ifndef DEF_Wall
#define DEF_Wall

#include "Structure.h"

const StructureDef* wallDef = new StructureDef( "Wall", std::vector<int>{ 1 }, "wall.jpg", 10, 1, 1 );

class Wall : public Structure {

  public:
    Wall();
    Wall( int _x, int _y );

    virtual const StructureDef* structureDef(){ return wallDef; };

    /*
    static const std::string typeName;
    static const std::vector<int> grid;
    static const std::string imgUrl;
    static const int cost;
    static const int gridWidth;
    static const int gridHeight;




    virtual std::string getTypeName(){ return typeName; };
    virtual std::vector<int> getGrid(){ return std::vector<int> { 1 }; };
    virtual std::string getImgUrl(){ return "wall.jpg"; };
    virtual int getCost(){ return 10; };
    virtual int gridWidth(){ return 1; };
    virtual int gridHeight(){ return 1; };
    static std::string typeName;
    */
};


#endif
