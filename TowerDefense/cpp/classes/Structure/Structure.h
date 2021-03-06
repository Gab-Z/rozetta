#ifndef DEF_Structure
#define DEF_Structure


#include <vector>
#include <string>
#include "./StructureDef.h"

const int idCounter = 0;

class Structure {

  static int idCounter;
  int id;
  int x;
  int y;
  int rotation = 0;
  StructureDef* structureDef;

  public:

    Structure();
    Structure( int _x, int _y,  StructureDef* _structureDef );
    Structure( int _x, int _y,  StructureDef* _structureDef, int _rotation );
    Structure( int _id, int _x, int _y,  StructureDef* _structureDef, int _rotation );
    //std::vector<int> getPosition();
    void setPosition( int _x, int _y );
    void setX( int _x );
    void setY( int _y );
    int getId();
    int getX();
    int getY();
    int getRotation();
    //std::vector<int> getGridPositions();
    std::string getTypeName();
    std::string getImgUrl();
    int getGridWidth();
    int getGridHeight();
    v8::Local<v8::Object> toObj();
    bool testPoint( int _x, int _y );
    void setStructureDef( StructureDef* _structureDef );
    StructureDef* getStructureDef();
    std::vector<int> getPositions();
};

#endif
