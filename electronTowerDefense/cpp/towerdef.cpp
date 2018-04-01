

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

  target->Set(Nan::New("TowerDefense").ToLocalChecked(), ctor->GetFunction());
}

NAN_METHOD(TowerDefense::New) {
  // throw an error if constructor is called without new keyword
  if(!info.IsConstructCall()) {
    return Nan::ThrowError(Nan::New("TowerDefense::New - called without new keyword").ToLocalChecked());
  }

  // expect exactly 3 arguments
  if(info.Length() != 6) {
    return Nan::ThrowError(Nan::New("TowerDefense::New - expected arguments width, height, startX, startY, endX, endY").ToLocalChecked());
  }

  // expect arguments to be numbers
  if(!info[0]->IsNumber() || !info[1]->IsNumber() || !info[2]->IsNumber() || !info[3]->IsNumber() || !info[4]->IsNumber() || !info[5]->IsNumber() ) {
    return Nan::ThrowError(Nan::New("TowerDefense::New - expected arguments to be numbers").ToLocalChecked());
  }

  // create a new instance and wrap our javascript instance
  TowerDefense* towerDef = new TowerDefense();
  towerDef->Wrap(info.Holder());

  // initialize it's values
  towerDef->store = Map();

  towerDef->store.init( info[0]->NumberValue(), info[1]->NumberValue(), 1, 1, info[2]->NumberValue(), info[3]->NumberValue(), info[4]->NumberValue(), info[5]->NumberValue()  );

  //vec->x = info[0]->NumberValue();
  //vec->y = info[1]->NumberValue();
  //vec->z = info[2]->NumberValue();

  // return the wrapped javascript instance
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
  if(info.Length() != 3) {
    return Nan::ThrowError(Nan::New("TowerDefense::addStructure - expected arguments x, y, typeName").ToLocalChecked());
  }
  if(!info[0]->IsNumber() || !info[1]->IsNumber()  ) {
    return Nan::ThrowError(Nan::New("TowerDefense::New - expected arguments to be numbers").ToLocalChecked());
  }
  Nan::Utf8String utf8_value(info[2]);
  //  std::string utf8_value = std::string(*Nan::Utf8String(info[2]));
  int len = utf8_value.length();
  if (len <= 0) {
     return Nan::ThrowTypeError("arg must be a non-empty string");
  }
  /// work with string data here....
  // e.g. convert to a std::string
  std::string propertyName(*utf8_value, len);
  //  std::string propertyName = std::string( *Nan::Utf8String( utf8_value ) );
  bool addRez = self->store.addStructure( info[0]->NumberValue(), info[1]->NumberValue(), propertyName );

  if( addRez == true ){
    v8::Local<v8::Array> jsArr = self->store.getStructures();
    info.GetReturnValue().Set(jsArr);
  }else{

    std::vector<int> fmap =  self->store.getFilledMap( self->store.getStartX(), self->store.getStartY() );

    v8::Local<v8::Array> a = Nan::New<v8::Array>();
  //Local<Array> a = New<v8::Array>(arrLength);
    for (int i = 0; i < fmap.size(); i++ ) {
      //a[i] = arr[i];
      Nan::Set(a, i, Nan::New(fmap[i]));
    }
    info.GetReturnValue().Set(a);
  }

  //  if( propertyName == "Wall" ){
  //    self->store.addWall( info[0]->NumberValue(), info[1]->NumberValue(), propertyName )
  //  }else{
  //    return Nan::ThrowError(Nan::New("unknown structure type").ToLocalChecked());
  //  }
}
