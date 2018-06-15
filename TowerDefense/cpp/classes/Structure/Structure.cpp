#include "Structure.h"

int Structure::idCounter = 0;

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
Structure::Structure( int _x, int _y,  StructureDef* _structureDef, int _rotation ){
  x = _x;
  y = _y;
  structureDef = _structureDef;
  Structure::idCounter++;
  id = Structure::idCounter;
  if( structureDef->isRotating() == true ){
    rotation = _rotation;
  }
}
Structure::Structure( int _id, int _x, int _y,  StructureDef* _structureDef, int _rotation ){
  x = _x;
  y = _y;
  structureDef = _structureDef;
  id = _id;
  if( structureDef->isRotating() == true ){
    rotation = _rotation;
  }
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
  return id;
}
int Structure::getX(){
  return x;
}
int Structure::getY(){
  return y;
}
int Structure::getRotation(){
  return rotation;
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
v8::Local<v8::Object> Structure::toObj(){
  v8::Local<v8::Object> ret = Nan::New<v8::Object>();

  v8::Local<v8::String> xProp = Nan::New("x").ToLocalChecked();
  v8::Local<v8::Value> xVal = Nan::New( x );
  ret->Set( xProp, xVal );

  v8::Local<v8::String> yProp = Nan::New("y").ToLocalChecked();
  v8::Local<v8::Value> yVal = Nan::New( y );
  ret->Set( yProp, yVal );

  v8::Local<v8::String> idProp = Nan::New("id").ToLocalChecked();
  v8::Local<v8::Value> idVal = Nan::New( id );
  ret->Set( idProp, idVal );

  v8::Local<v8::String> rotProp = Nan::New("rotation").ToLocalChecked();
  v8::Local<v8::Value> rotVal = Nan::New( rotation );
  ret->Set( rotProp, rotVal );

  v8::Local<v8::String> typeProp = Nan::New("typeName").ToLocalChecked();
  v8::Local<v8::Value> typeVal = Nan::New( structureDef->getTypeName() ).ToLocalChecked();
  ret->Set( typeProp, typeVal );

  int gWidth;
  int gHeight;
  if( rotation == 0 || rotation == 2 ){
    gWidth = structureDef->getGridWidth();
    gHeight = structureDef->getGridHeight();
  }else{
    gWidth = structureDef->getGridHeight();
    gHeight = structureDef->getGridWidth();
  }
  v8::Local<v8::String> wProp = Nan::New("gridWidth").ToLocalChecked();
  v8::Local<v8::Value> wVal = Nan::New( gWidth );
  ret->Set( wProp, wVal );

  v8::Local<v8::String> hProp = Nan::New("gridHeight").ToLocalChecked();
  v8::Local<v8::Value> hVal = Nan::New( gHeight );
  ret->Set( hProp, hVal );

  return ret;

}
bool Structure::testPoint( int _x, int _y ){
  int tx = _x - x;
  int ty = _y - y;
  return structureDef->testPoint( tx, ty, rotation );
}
void Structure::setStructureDef( StructureDef* _structureDef ){
  structureDef = _structureDef;
}
