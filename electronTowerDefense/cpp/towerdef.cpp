#include "towerdef.h"

Nan::Persistent<v8::FunctionTemplate> TowerDefense::constructor;

NAN_MODULE_INIT(TowerDefense::Init) {

  v8::Local<v8::FunctionTemplate> ctor = Nan::New<v8::FunctionTemplate>(TowerDefense::New);
  constructor.Reset(ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(Nan::New("TowerDefense").ToLocalChecked());

  // link our getters and setter to the object property
  //Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("x").ToLocalChecked(), Vector::HandleGetters, Vector::HandleSetters);
  //Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("y").ToLocalChecked(), Vector::HandleGetters, Vector::HandleSetters);
  //Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("z").ToLocalChecked(), Vector::HandleGetters, Vector::HandleSetters);

  Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("map").ToLocalChecked(), TowerDefense::HandleGetters, TowerDefense::HandleSetters);
  Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("width").ToLocalChecked(), TowerDefense::HandleGetters, TowerDefense::HandleSetters);
  Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("height").ToLocalChecked(), TowerDefense::HandleGetters, TowerDefense::HandleSetters);
  Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("length").ToLocalChecked(), TowerDefense::HandleGetters, TowerDefense::HandleSetters);
  Nan::SetPrototypeMethod( ctor, "addStructure", addStructure );
  Nan::SetPrototypeMethod( ctor, "testClass", testClass );

  target->Set(Nan::New("TowerDefense").ToLocalChecked(), ctor->GetFunction());
}

NAN_METHOD(TowerDefense::New) {
  // throw an error if constructor is called without new keyword
  if(!info.IsConstructCall()) {
    return Nan::ThrowError(Nan::New("TowerDefense::New - called without new keyword").ToLocalChecked());
  }

  // expect exactly 4 arguments
  if(info.Length() != 4) {
    return Nan::ThrowError(Nan::New("TowerDefense::New - expected arguments width, height, [ startX, startY,...], [ endX, endY, ...]").ToLocalChecked());
  }

  // expect arguments to be numbers
  if(!info[0]->IsNumber() || !info[1]->IsNumber() || !info[2]->IsArray() || !info[3]->IsArray() ) {
    return Nan::ThrowError(Nan::New("TowerDefense::New - expected arguments 0 & 1 to be numbers, 2 & 3 arrays").ToLocalChecked());
  }

  // create a new instance and wrap our javascript instance
  TowerDefense* towerDef = new TowerDefense();
  towerDef->Wrap(info.Holder());

  towerDef->store = Map();

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

  info.GetReturnValue().Set(info.Holder());
}

NAN_GETTER(TowerDefense::HandleGetters) {
  TowerDefense* self = Nan::ObjectWrap::Unwrap<TowerDefense>(info.This());

  std::string propertyName = std::string(*Nan::Utf8String(property));
  if ( propertyName == "map" ) {
    v8::Local<v8::Array> a = Nan::New<v8::Array>();
    int length = self->store.getLength();
    std::vector<int> map = self->store.getArray();
    //Local<Array> a = New<v8::Array>(arrLength);
    for (int i = 0; i < length; i++ ) {
      //a[i] = arr[i];
      Nan::Set(a, i, Nan::New(map[i]));
    }
    info.GetReturnValue().Set(a);
  } else if( propertyName == "width" ){
    info.GetReturnValue().Set(self->store.getWidth());
  } else if( propertyName == "height" ){
    info.GetReturnValue().Set(self->store.getHeight());
  } else if( propertyName == "length" ){
    info.GetReturnValue().Set(self->store.getLength());
  }else {
    info.GetReturnValue().Set(Nan::Undefined());
  }
}

NAN_SETTER(TowerDefense::HandleSetters) {
  //TowerDefense* self = Nan::ObjectWrap::Unwrap<TowerDefense>(info.This());

  //  if(!value->IsNumber()) {
  //    return Nan::ThrowError(Nan::New("expected value to be a number").ToLocalChecked());
  //  }

  std::string propertyName = std::string(*Nan::Utf8String(property));
  if (propertyName == "map") {
    return Nan::ThrowError(Nan::New("map property of TowerDefense Class is read only").ToLocalChecked());
  }else if( propertyName == "width" ){
    return Nan::ThrowError(Nan::New("width property of TowerDefense Class is read only").ToLocalChecked());
  }else if( propertyName == "height" ){
    return Nan::ThrowError(Nan::New("height property of TowerDefense Class is read only").ToLocalChecked());
  }else if( propertyName == "length" ){
    return Nan::ThrowError(Nan::New("length property of TowerDefense Class is read only").ToLocalChecked());
  }

  //  else if (propertyName == "y") {
  //    self->y = value->NumberValue();
  //  } else if (propertyName == "z") {
  //    self->z = value->NumberValue();
  //  }

}

NAN_METHOD( TowerDefense::addStructure ){
  TowerDefense* self = Nan::ObjectWrap::Unwrap<TowerDefense>(info.This());
  if(info.Length() != 2) {
    return Nan::ThrowError(Nan::New("TowerDefense::addStructure - expected arguments typeName, [ positions, ... ]").ToLocalChecked());
  }
  if( !info[1]->IsArray() ) {
    return Nan::ThrowError(Nan::New("TowerDefense::New - expected argument 2 to be an array").ToLocalChecked());
  }
  v8::Local<v8::Array> posArr = v8::Local<v8::Array>::Cast(info[1]);
  std::vector<int> posVec;

  int vl = posArr->Length();
  for (int i = 0; i < vl; i++) {
    v8::Local<v8::Value> jsElement = posArr->Get(i);
    int number = jsElement->IntegerValue();
    posVec.push_back(number);
  }
  Nan::Utf8String utf8_value(info[0]);
  int len = utf8_value.length();
  if (len <= 0) {
     return Nan::ThrowTypeError("arg must be a non-empty string");
  }
  std::string propertyName(*utf8_value, len);
  bool addRez = self->store.addStructure( propertyName, posVec );

  if(addRez == true ){
   v8::Local<v8::Array> jsArr = self->store.getStructures();
  //  v8::Local<v8::Array> jsArr = self->store.getFilledAccess();

    info.GetReturnValue().Set(jsArr);
  }else{
    info.GetReturnValue().Set(Nan::False());
  }
}

NAN_METHOD( TowerDefense::testClass ){
  std::vector<Structure*> vec;
  for(int i = 0; i < 20; i++ ){
    Wall * walli = new Wall( i , i*2 );
    vec.push_back( walli );
  }
  std::vector<int> ret;
//  int l = vec.size();
  for( std::vector<Structure*>::size_type j = 0; j < vec.size(); j++ ){
    Structure* walli = vec[ j ];
    ret.push_back( walli->getx() );
    ret.push_back( walli->gety() );
  }
  int vl = ret.size();
  v8::Local<v8::Array> posesArr = Nan::New<v8::Array>(vl);
  for (int p = 0; p < vl; p++) {
    int number = ret[ p ];
    v8::Local<v8::Value> jsElement = Nan::New(number);
    posesArr->Set(p, jsElement);
  }
    info.GetReturnValue().Set(posesArr);
}
