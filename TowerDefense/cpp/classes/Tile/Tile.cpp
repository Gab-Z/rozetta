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
  if( structure ){
    return structure->getId();
  }
  return 0;
}
int Tile::getFloorTypeId(){
  return floorTypeId;
}
void Tile::setPosition( int _x, int _y ){
  x = _x;
  y = _y;
}
/*
void Tile::setStructureId( int _id ){
  structureId = _id;
}
*/
void Tile::setStructure( Structure* _structure ){
  structure = _structure;
}
Structure* Tile::getStructure(){
  return structure;
}
Structure*& Tile::getStructureRef(){
  return structure;
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
  v8::Local<v8::Value> floorValue = Nan::New( getFloorTypeId() );
  ret->Set( floorProp, floorValue );

  int wayType = getWayType();
  if( wayType == 1 ){
    v8::Local<v8::String> startProp = Nan::New( "isStart" ).ToLocalChecked();
    v8::Local<v8::Value> startValue = Nan::New( true );
    ret->Set( startProp, startValue );
  }else if( wayType == 2 ){
    v8::Local<v8::String> endProp = Nan::New( "isEnd" ).ToLocalChecked();
    v8::Local<v8::Value> endValue = Nan::New( true );
    ret->Set( endProp, endValue );
  }
  return ret;
};
void Tile::setWayInOrOut( std::string _wayType ){
  if( _wayType == "start" ){
    isWayInOrOut = 1;
  }else if( _wayType == "end" ){
    isWayInOrOut = 2;
  }else{
    isWayInOrOut = 0;
  }
}
int Tile::getWayType(){
  return isWayInOrOut;
}
bool Tile::isBuildable(){
  if(   ! floorsList::getFloorTypeById( floorTypeId )->isBuildable()
    ||   getWayType() != 0
    ||   structure
  ){
    return false;
  }
  return true;
}
bool Tile::isTraversable(){
  if( structure ){
    return false;
  };
  return true;
}
float Tile::getSpeed(){
  if( floorTypeId != 0 ){
    return floorsList::getFloorTypeById( floorTypeId )->getSpeed();
  }else{
    return 0.0;
  }
}
