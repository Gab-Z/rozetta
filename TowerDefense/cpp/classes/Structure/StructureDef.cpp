#include "StructureDef.h"

StructureDef::StructureDef(){}

StructureDef::StructureDef( std::string _typeName, std::vector<int> _grid, std::string _imgUrl, int _cost, int _gridWidth, int _gridHeight ){
  typeName = _typeName;
  grid = _grid;
  imgUrl = _imgUrl;
  cost = _cost;
  gridWidth = _gridWidth;
  gridHeight = _gridHeight;
}
v8::Local<v8::Object> StructureDef::toObj(){

  v8::Local<v8::Object> ret = Nan::New<v8::Object>();

  v8::Local<v8::String> typeProp = Nan::New( "typeName" ).ToLocalChecked();
  v8::Local<v8::Value> typeValue = Nan::New( typeName ).ToLocalChecked();
  ret->Set( typeProp, typeValue );

  v8::Local<v8::String> widthProp = Nan::New( "gridWidth" ).ToLocalChecked();
  v8::Local<v8::Value> widthValue = Nan::New( gridWidth );
  ret->Set( widthProp, widthValue );

  v8::Local<v8::String> heightProp = Nan::New( "gridHeight" ).ToLocalChecked();
  v8::Local<v8::Value> heightValue = Nan::New( gridHeight );
  ret->Set( heightProp, heightValue );

  v8::Local<v8::String> imgProp = Nan::New( "imgUrl" ).ToLocalChecked();
  v8::Local<v8::Value> imgValue = Nan::New( std::string("../img/") + imgUrl ).ToLocalChecked();
  ret->Set( imgProp, imgValue );

  v8::Local<v8::String> costProp = Nan::New( "cost" ).ToLocalChecked();
  v8::Local<v8::Value> costValue = Nan::New( cost );
  ret->Set( costProp, costValue );

  v8::Local<v8::String> gridProp = Nan::New( "grid" ).ToLocalChecked();
  v8::Local<v8::Array> gridValue = Converter::vectorIntToJsArray( grid );
  ret->Set( gridProp, gridValue );

  return ret;
}
