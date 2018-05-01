#ifndef DEF_Structure
#define DEF_Structure

#include <vector>
#include <string>
#include "../Converter/Converter.h"

class Structure {

  int id;
  int x;
  int y;

  public:

    Structure();
    Structure( int _x, int _y );
    std::vector<int> getPosition();
    void setPosition( int _x, int _y );
    void setX( int _x );
    void setY( int _y );
    virtual std::string getTypeName(){ return "Structure"; };
    virtual std::vector<int> getGrid(){ return std::vector<int> ( 0 ); };
    virtual std::string getImgUrl(){ return "null"; };
    virtual int getCost(){ return 0; };
    virtual int gridWidth(){ return 0; };
    virtual int gridHeight(){ return 0; };
    v8::Local<v8::Object> getDefinitionObj();

};

#endif
