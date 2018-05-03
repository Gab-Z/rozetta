#ifndef DEF_Structure
#define DEF_Structure

#include <nan.h>
#include <vector>
#include <string>
#include "./StructureDef.h"

const StructureDef* strucDef = new StructureDef( "Structure", std::vector<int>(), "null", 0, 0, 0 );

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
    static v8::Local<v8::Object> getDefinitionObj();
    virtual const StructureDef* structureDef(){ return strucDef; };



    /*
    static const std::string typeName;
    static const std::vector<int> grid;
    static const std::string imgUrl;
    static const int cost;
    static const int gridWidth;
    static const int gridHeight;

    virtual std::string getTypeName(){ return "Structure"; };
    virtual std::vector<int> getGrid(){ return std::vector<int> ( 0 ); };
    virtual std::string getImgUrl(){ return "null"; };
    virtual int getCost(){ return 0; };
    virtual int gridWidth(){ return 0; };
    virtual int gridHeight(){ return 0; };
    static v8::Local<v8::Object> getDefinitionObj();
    */
};

#endif
