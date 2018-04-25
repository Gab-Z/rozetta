#ifndef DEF_Cross
#define DEF_Cross

#include "Structure.h"

class Cross : public Structure {

  public:

    Cross();
    Cross( int _x, int _y );
    static std::vector<int> grid;
    virtual std::string typeName(){ return "Cross"; };


};

#endif
