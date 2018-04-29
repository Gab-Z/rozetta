#include "Tile.h"

Tile::Tile(){

}
Tile::Tile( int _x, int _y ){
  x = _x;
  y = _y;
}
Tile::Tile( int _x, int _y, int _floorTypeId ){
  x = _x;
  y = _y;
  floorTypeId = _floorTypeId;
}
std::vector<int> Tile::getPosition(){
  return std::vector<int> { x, y };
}
int Tile::getStructureId(){
  return structureId;
}
int Tile::getFloorId(){
  return floorTypeId;
}
void Tile::setPosition( int _x, int _y ){
  x = _x;
  y = _y;
}
void Tile::setStructureId( int _id ){
  structureId = _id;
}
void Tile::setFloorTypeId( int _id ){
  floorTypeId = _id;
}
int Tile::getx(){
  return x;
}
int Tile::gety(){
  return y;
}
v8::Local<v8::Object> Tile::toObj(){
  v8::Local<v8::Object> ret = Nan::New<v8::Object>();

  v8::Local<v8::String> xProp = Nan::New( "x" ).ToLocalChecked();
  v8::Local<v8::Value> xValue = Nan::New( getx() );
  ret->Set( xProp, xValue );

  v8::Local<v8::String> yProp = Nan::New( "y" ).ToLocalChecked();
  v8::Local<v8::Value> yValue = Nan::New( gety() );
  ret->Set( yProp, yValue );

  v8::Local<v8::String> strucProp = Nan::New( "structureId" ).ToLocalChecked();
  v8::Local<v8::Value> strucValue = Nan::New( getStructureId() );
  ret->Set( strucProp, strucValue );

  v8::Local<v8::String> floorProp = Nan::New( "floorTypeId" ).ToLocalChecked();
  v8::Local<v8::Value> floorValue = Nan::New( getFloorId() );
  ret->Set( floorProp, floorValue );

  return ret;
};
