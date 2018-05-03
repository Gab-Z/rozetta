#include "Structure.h"

Structure::Structure(){

}
Structure::Structure( int _x, int _y ){
  x = _x;
  y = _y;
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
//StructureDef* Structure::structureDef =  new StructureDef( "Structure", std::vector<int>(), "null", 0, 0, 0 );

v8::Local<v8::Object> Structure::getDefinitionObj(){
  v8::Local<v8::Object> ret = Nan::New<v8::Object>();

  const StructureDef* def = structureDef();

  v8::Local<v8::String> typeProp = Nan::New( "typeName" ).ToLocalChecked();
  v8::Local<v8::Value> typeValue = Nan::New( def->typeName ).ToLocalChecked();
  ret->Set( typeProp, typeValue );
/*
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
*/
  return ret;
}
/*
std::string Structure::typeName = "Structure";
std::vector<int> Structure::grid = std::vector<int>();
std::string Structure::imgUrl = "null";
int Structure::cost = 0;
int Structure::gridWidth = 0;
int Structure::gridHeight = 0;
*/
