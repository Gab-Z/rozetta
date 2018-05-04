#ifndef DEF_Cross
#define DEF_Cross

#include "Structure.h"

class Cross : public Structure {

  public:

    Cross();
    /*
    Cross( int _x, int _y );

    virtual std::string getTypeName(){ return "Cross"; };
    virtual std::vector<int> getGrid(){ return cross_grid; };
    virtual std::string getImgUrl(){ return cross_img_url; };
    virtual int getCost(){ return 60; };
    virtual int gridWidth(){ return 3; };
    virtual int gridHeight(){ return 3; };
    */
};

#endif
