#include "Structure.h"

unsigned int Structure::idCounter = 0;

Structure::Structure(){
  Structure::idCounter++;
  id = Structure::idCounter;
}
Structure::Structure( int _x, int _y,  StructureDef* _structureDef ){
  x = _x;
  y = _y;
  structureDef = _structureDef;
  Structure::idCounter++;
  id = Structure::idCounter;
}
void Structure::setPosition( int _x, int _y ){
  x = _x;
  y = _y;
}
void Structure::setX( int _x ){
  x = _x;
}
void Structure::setY( int _y ){
  y = _y;
}
int Structure::getId(){
  int _id = (int) id;
  return _id;
}
int Structure::getX(){
  return x;
}
int Structure::getY(){
  return y;
}
std::string Structure::getTypeName(){
  return structureDef->getTypeName();
}
std::string Structure::getImgUrl(){
  return structureDef->getImgUrl();
}
int Structure::getGridWidth(){
  return structureDef->getGridWidth();
}
int Structure::getGridHeight(){
  return structureDef->getGridHeight();
}
/*
std::vector<int> Structure::getGridPositions(){
  std::vector<int> baseGrid = structureDef->getGrid();
  std::vector<int> ret();
  int bgl = baseGrid.size();
  for( int  i = 0; i < bgl; i++ ){
    if( baseGrid[ i ] != 1 ){ continue; };
    std::vector<int> basePos = structureDef->to2d( i );
    ret.push_back( x + basePos[ 0 ] );
    ret.push_back( y + basePos[ 1 ] );
  }
  return ret;
}
*/
//StructureDef* Structure::structureDef =  new StructureDef( "Structure", std::vector<int>(), "null", 0, 0, 0 );
/*
v8::Local<v8::Object> Structure::getDefinitionObj(){
  v8::Local<v8::Object> ret = Nan::New<v8::Object>();

  const StructureDef* def = structureDef();

  v8::Local<v8::String> typeProp = Nan::New( "typeName" ).ToLocalChecked();
  v8::Local<v8::Value> typeValue = Nan::New( def->typeName ).ToLocalChecked();
  ret->Set( typeProp, typeValue );

  v8::Local<v8::String> widthProp = Nan::New( "gridWidth" ).ToLocalChecked();
  v8::Local<v8::Value> widthValue = Nan::New( gridWidth() );
  ret->Set( widthProp, widthValue );

  v8::Local<v8::String> heightProp = Nan::New( "gridHeight" ).ToLocalChecked();
  v8::Local<v8::Value> heightValue = Nan::New( gridHeight() );
  ret->Set( heightProp, heightValue );

  v8::Local<v8::String> imgProp = Nan::New( "imgUrl" ).ToLocalChecked();
  v8::Local<v8::Value> imgValue = Nan::New( getImgUrl() ).ToLocalChecked();
  ret->Set( imgProp, imgValue );

  v8::Local<v8::String> costProp = Nan::New( "cost" ).ToLocalChecked();
  v8::Local<v8::Value> costValue = Nan::New( getCost() );
  ret->Set( costProp, costValue );

  v8::Local<v8::String> gridProp = Nan::New( "grid" ).ToLocalChecked();
  v8::Local<v8::Array> gridValue = Converter::vectorIntToJsArray( getGrid() );
  ret->Set( gridProp, gridValue );

  return ret;
}
*/
