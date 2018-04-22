#include "TowerDefense.h"

Nan::Persistent<v8::FunctionTemplate> TowerDefense::constructor;

NAN_MODULE_INIT(TowerDefense::Init) {
  v8::Local<v8::FunctionTemplate> ctor = Nan::New<v8::FunctionTemplate>(TowerDefense::New);
  constructor.Reset(ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(Nan::New("TowerDefense").ToLocalChecked());
  Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("width").ToLocalChecked(), TowerDefense::HandleGetters, TowerDefense::HandleSetters);
  Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("height").ToLocalChecked(), TowerDefense::HandleGetters, TowerDefense::HandleSetters);
  /*
  Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("map").ToLocalChecked(), TowerDefense::HandleGetters, TowerDefense::HandleSetters);
  Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("width").ToLocalChecked(), TowerDefense::HandleGetters, TowerDefense::HandleSetters);
  Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("height").ToLocalChecked(), TowerDefense::HandleGetters, TowerDefense::HandleSetters);
  Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("length").ToLocalChecked(), TowerDefense::HandleGetters, TowerDefense::HandleSetters);
  Nan::SetPrototypeMethod( ctor, "addStructure", addStructure );
  */

  target->Set(Nan::New("TowerDefense").ToLocalChecked(), ctor->GetFunction());
}

TowerDefense::TowerDefense( int _width, int _height, std::vector<int> _startPts, std::vector<int> _endPts ){
  mapWidth = _width;
  mapHeight = _height;
  startPoints = _startPts;
  endPoints = _endPts;
}

NAN_METHOD(TowerDefense::New) {
  // throw an error if constructor is called without new keyword
  if(!info.IsConstructCall()) {
    return Nan::ThrowError(Nan::New("TowerDefense::New - called without new keyword").ToLocalChecked());
  }

  // expect exactly 2 arguments
  if(info.Length() != 4) {
    return Nan::ThrowError(Nan::New("TowerDefense::New - expected arguments width, height, [ startX, startY,...], [ endX, endY, ...]").ToLocalChecked());
  }

  // expect arguments to be numbers
  if( !info[0]->IsNumber() || !info[1]->IsNumber() || !info[2]->IsArray() || !info[3]->IsArray() ) {
    return Nan::ThrowError(Nan::New("TowerDefense::New - expected arguments 0 & 1 to be numbers, 2 & 3 arrays").ToLocalChecked());
  }

  // create a new instance and wrap our javascript instance

  TowerDefense* towerDef = new TowerDefense( info[0]->IntegerValue(), info[1]->IntegerValue(), Converter::jsArrayToVectorInt( v8::Local<v8::Array>::Cast( info[ 2 ] ) ), Converter::jsArrayToVectorInt( v8::Local<v8::Array>::Cast( info[ 3 ] ) ) );
  towerDef->Wrap(info.Holder());
  //towerDef->grd_terrain = new GridIntVec( info[0]->IntegerValue(), info[1]->IntegerValue(), 1 );

  info.GetReturnValue().Set(info.Holder());
}

NAN_GETTER(TowerDefense::HandleGetters) {
  TowerDefense* self = Nan::ObjectWrap::Unwrap<TowerDefense>(info.This());
  std::string propertyName = std::string(*Nan::Utf8String(property));

  if( propertyName == "width" ){
    info.GetReturnValue().Set( self->width() );
  }else if( propertyName == "height" ){
    info.GetReturnValue().Set( self->height() );
  }else{
    info.GetReturnValue().Set(Nan::Undefined());
  }
}

NAN_SETTER( TowerDefense::HandleSetters ) {
  std::string propertyName = std::string(*Nan::Utf8String(property));
  if( propertyName == "width" ){
    return Nan::ThrowError(Nan::New("width property of TowerDefense Class is read only").ToLocalChecked());
  }else if( propertyName == "height" ){
    return Nan::ThrowError(Nan::New("height property of TowerDefense Class is read only").ToLocalChecked());
  }else{
    info.GetReturnValue().Set(Nan::Undefined());
  }
}

int TowerDefense::width(){
  return mapWidth;
}
int TowerDefense::height(){
  return mapHeight;
}
