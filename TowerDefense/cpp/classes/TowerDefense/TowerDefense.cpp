#include "TowerDefense.h"

Nan::Persistent<v8::FunctionTemplate> TowerDefense::constructor;

NAN_MODULE_INIT(TowerDefense::Init) {

  v8::Local<v8::FunctionTemplate> ctor = Nan::New<v8::FunctionTemplate>(TowerDefense::New);
  constructor.Reset(ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(Nan::New("TowerDefense").ToLocalChecked());

  Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("map").ToLocalChecked(), TowerDefense::HandleGetters, TowerDefense::HandleSetters);
  /*
  Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("map").ToLocalChecked(), TowerDefense::HandleGetters, TowerDefense::HandleSetters);
  Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("width").ToLocalChecked(), TowerDefense::HandleGetters, TowerDefense::HandleSetters);
  Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("height").ToLocalChecked(), TowerDefense::HandleGetters, TowerDefense::HandleSetters);
  Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("length").ToLocalChecked(), TowerDefense::HandleGetters, TowerDefense::HandleSetters);
  Nan::SetPrototypeMethod( ctor, "addStructure", addStructure );
  */
  Nan::SetPrototypeMethod( ctor, "testClass", testClass );
  Nan::SetPrototypeMethod( ctor, "addStructures", addStructures );

  target->Set(Nan::New("TowerDefense").ToLocalChecked(), ctor->GetFunction());
}
NAN_METHOD(TowerDefense::New) {
  // throw an error if constructor is called without new keyword
  if(!info.IsConstructCall()) {
    return Nan::ThrowError(Nan::New("TowerDefense::New - called without new keyword").ToLocalChecked());
  }

  // expect exactly 2 arguments
  if(info.Length() != 2) {
    return Nan::ThrowError(Nan::New("TowerDefense::New - expected arguments width, height, [ startX, startY,...], [ endX, endY, ...]").ToLocalChecked());
  }

  // expect arguments to be numbers
  if(!info[0]->IsNumber() || !info[1]->IsNumber() ) {
    return Nan::ThrowError(Nan::New("TowerDefense::New - expected arguments 0 & 1 to be numbers, 2 & 3 arrays").ToLocalChecked());
  }

  // create a new instance and wrap our javascript instance
  TowerDefense* towerDef = new TowerDefense();
  towerDef->Wrap(info.Holder());
  //towerDef->grd_terrain = new GridIntVec( info[0]->IntegerValue(), info[1]->IntegerValue(), 1 );
  towerDef->grd_terrain.setDimensions( info[0]->IntegerValue(), info[1]->IntegerValue() );
  towerDef->grd_terrain.set( std::vector<int> ( info[0]->IntegerValue() * info[1]->IntegerValue(), 1 ) );

  /*
  //towerDef->store = Map();

  v8::Local<v8::Array> startArr = v8::Local<v8::Array>::Cast(info[2]);
  std::vector<int> startVec;
  int startL = startArr->Length();
  for (int w = 0; w < startL; w++) {
    v8::Local<v8::Value> jsV = startArr->Get( w );
    int vl = jsV->IntegerValue();
    startVec.push_back( vl );
  }
  v8::Local<v8::Array> endArr = v8::Local<v8::Array>::Cast(info[3]);
  std::vector<int> endVec;
  int endL = endArr->Length();
  for (int q = 0; q < endL; q++) {
    v8::Local<v8::Value> jsVe = endArr->Get( q );
    int vlu = jsVe->IntegerValue();
    endVec.push_back( vlu );
  }

  towerDef->store.init( info[0]->IntegerValue(), info[1]->IntegerValue(), 1, 1, startVec, endVec );
  */
  info.GetReturnValue().Set(info.Holder());
}
NAN_GETTER(TowerDefense::HandleGetters) {
  //TowerDefense* self = Nan::ObjectWrap::Unwrap<TowerDefense>(info.This());
  std::string propertyName = std::string(*Nan::Utf8String(property));

  if( propertyName == "map" ){
    info.GetReturnValue().Set( 1 );
  }else {
    info.GetReturnValue().Set(Nan::Undefined());
  }
}
NAN_SETTER(TowerDefense::HandleSetters) {
  std::string propertyName = std::string(*Nan::Utf8String(property));
  if( propertyName == "map" ){
    info.GetReturnValue().Set( 1 );
  }else {
    info.GetReturnValue().Set(Nan::Undefined());
  }
}
NAN_METHOD( TowerDefense::testClass ){
  TowerDefense* self = Nan::ObjectWrap::Unwrap<TowerDefense>(info.This());

  info.GetReturnValue().Set( self->grd_terrain.getJsArray() );
  /*
  GridIntFK grd( self->grid->getWidth(), self->grid->getHeight() );
  int l = grd.size();
  for( int i = 0 ; i < l; i++ ){
    grd.set( i, self->gridFK->get(i));
  }
  info.GetReturnValue().Set( grd.size() );

  for(int j = 0; j < 4000; j++ ){



    GridIntVec grd( self->gridVec->getWidth(), self->gridVec->getHeight() );
    int l = grd.size();
    for( int i = 0 ; i < l; i++ ){
      grd.set( i, self->gridVec->get(i));
    }



    }

  int ret;
  for( int i = 0; i < 1000; i++ ){
    int tmparr[ self->length ];
    //std::vector<int> tmparr( self->length );
    for( int j = 0; j < self->length; j++ ){
      tmparr[ j ] = self->arr[ j ];
      //tmparr[ j ] = self->vec[ j ];
    }
    ret = tmparr[ 0 ];
  }
  info.GetReturnValue().Set(ret);
  */




}
NAN_METHOD( TowerDefense::addStructures ){

  TowerDefense* self = Nan::ObjectWrap::Unwrap<TowerDefense>(info.This());

  if(info.Length() != 2) {
    return Nan::ThrowError(Nan::New("TowerDefense::addStructures - expected 2 arguments : string:typeName, array:structurePositions").ToLocalChecked());
  }

  Nan::Utf8String utf8_value(info[0]);
  int len = utf8_value.length();
  if (len <= 0) {
     return Nan::ThrowTypeError("TowerDefense::addStructures - expected argument 0 to be a non empty string");
  }
  std::string propertyName(*utf8_value, len);

  // expect arguments to be numbers
  if(!info[1]->IsArray() ) {
    return Nan::ThrowError(Nan::New("TowerDefense::addStructures - expected argument 2 to be an array").ToLocalChecked());
  }


}
