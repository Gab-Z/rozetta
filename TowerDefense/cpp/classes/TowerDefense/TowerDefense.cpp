#include "TowerDefense.h"

Nan::Persistent<v8::FunctionTemplate> TowerDefense::constructor;

NAN_MODULE_INIT(TowerDefense::Init) {
  v8::Local<v8::FunctionTemplate> ctor = Nan::New<v8::FunctionTemplate>(TowerDefense::New);
  constructor.Reset(ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(Nan::New("TowerDefense").ToLocalChecked());
  Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("width").ToLocalChecked(), TowerDefense::HandleGetters, TowerDefense::HandleSetters);
  Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("height").ToLocalChecked(), TowerDefense::HandleGetters, TowerDefense::HandleSetters);
//  Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("floors").ToLocalChecked(), TowerDefense::HandleGetters, TowerDefense::HandleSetters);
  /*
  Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("map").ToLocalChecked(), TowerDefense::HandleGetters, TowerDefense::HandleSetters);
  Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("width").ToLocalChecked(), TowerDefense::HandleGetters, TowerDefense::HandleSetters);
  Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("height").ToLocalChecked(), TowerDefense::HandleGetters, TowerDefense::HandleSetters);
  Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("length").ToLocalChecked(), TowerDefense::HandleGetters, TowerDefense::HandleSetters);
  Nan::SetPrototypeMethod( ctor, "addStructure", addStructure );
  */
  Nan::SetPrototypeMethod( ctor, "getFloors", getFloors );
  Nan::SetPrototypeMethod( ctor, "getTiles", getTiles );
  target->Set(Nan::New("TowerDefense").ToLocalChecked(), ctor->GetFunction());

//  instantiateFloors();
}

TowerDefense::TowerDefense( int _width, int _height, std::vector<int> _startPts, std::vector<int> _endPts ){
  mapWidth = _width;
  mapHeight = _height;
  startPoints = _startPts;
  endPoints = _endPts;
  size = mapWidth * mapHeight;
  floorIds = std::vector<int> ( size, 1 );
  tiles = std::vector<Tile*> ( size );
  for( int i = 0; i < size; i++ ){
    std::vector<int> tilePos = to2d( i );
    tiles[ i ] = new Tile( tilePos[ 0 ], tilePos[ 1 ], 1 );
  }
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


int TowerDefense::to1d( int _x, int _y ){
  int pos = _y * mapWidth + _x;
  return pos;
}
std::vector<int> TowerDefense::to2d( int _idx ){
  int _y = std::floor( _idx / mapWidth );
  int _x = _idx - (_y * mapWidth );
  std::vector<int> pos { _x, _y };
  return pos;
}
int TowerDefense::width(){
  return mapWidth;
}
int TowerDefense::height(){
  return mapHeight;
}

v8::Local<v8::Array> TowerDefense::getTilesArray(){
  v8::Local<v8::Array> ret = Nan::New<v8::Array>( size );
  for( int i = 0; i < size; i++ ){
    v8::Local<v8::Object> v8Tile = tiles[ i ]->toObj();
    ret->Set( i, v8Tile );
  }
  return ret;
}

NAN_METHOD( TowerDefense::getTiles ){
  TowerDefense* self = Nan::ObjectWrap::Unwrap<TowerDefense>(info.This());
  info.GetReturnValue().Set( self->getTilesArray() );
}

NAN_METHOD( TowerDefense::getFloors ){
  //v8::Local<v8::Object> TowerDefense::getFloors(){
  TowerDefense* self = Nan::ObjectWrap::Unwrap<TowerDefense>(info.This());
  v8::Local<v8::Object> ret = Nan::New<v8::Object>();
  int nbFloors = floors.size();
  int s = self->size;
  for( int i = 0; i < s; i++ ){
    int floorId = self->floorIds[ i ];
    Floor* testedFloor;
    bool floorFound = false;
    for( int j = 0; j < nbFloors; j++ ){
      if( floors[ j ]->getId() == floorId ){
        testedFloor = floors[ j ];
        floorFound = true;
        break;
      }
    }
    if( floorFound == false ){
      return Nan::ThrowError(Nan::New("Unexpected Error : floor definition not found").ToLocalChecked());
    }
    std::string typeName = testedFloor->getTypeName();
    v8::Local<v8::String> typeProp = Nan::New( typeName ).ToLocalChecked();
    if (! Nan::HasOwnProperty(ret, typeProp).FromJust()) {
      //v8::Local<v8::Value> typeValue = Nan::New( "typeName" ).ToLocalChecked();
      v8::Local<v8::Object> floorOb = Nan::New<v8::Object>();

      v8::Local<v8::String> idProp = Nan::New( "id" ).ToLocalChecked();
      v8::Local<v8::Value> idValue = Nan::New( testedFloor->getId() );
      floorOb->Set( idProp, idValue );

      v8::Local<v8::String> imgUrlProp = Nan::New( "imgUrl" ).ToLocalChecked();
      v8::Local<v8::Value> imgUrlValue = Nan::New( std::string("../img/") + testedFloor->getImg_url() ).ToLocalChecked();
      floorOb->Set( imgUrlProp, imgUrlValue );

      v8::Local<v8::String> floorPositionsProp = Nan::New( "positions" ).ToLocalChecked();
      v8::Local<v8::Array> floorPositionsArr = Nan::New<v8::Array>();
      floorOb->Set( floorPositionsProp, floorPositionsArr );

      ret->Set( typeProp, floorOb );
    }

    v8::Local<v8::Object> floorObject = ret->Get( typeProp )->ToObject();
    v8::Local<v8::String> floorPosProp = Nan::New( "positions" ).ToLocalChecked();
    //v8::Local<v8::Array> floorPos = floorObject->Get( floorPosProp )->ToObject();
    v8::Local<v8::Array> floorPos = v8::Local<v8::Array>::Cast( floorObject->Get( floorPosProp ) );
    int floorPosL = floorPos->Length();
    std::vector<int> tilePos = self->to2d( i );
    v8::Local<v8::Value> xVal = Nan::New( tilePos[ 0 ] );
    v8::Local<v8::Value> yVal = Nan::New( tilePos[ 1 ] );
    floorPos->Set( floorPosL, xVal );
    floorPosL++;
    floorPos->Set( floorPosL, yVal );
  }
  //return ret;
  info.GetReturnValue().Set( ret );
}

std::vector<Floor*> TowerDefense::floors = {
  new Floor( 1, "bareGround", "bareGround.jpg", 1.0 ),
  new Floor( 2, "water", "water.jpg", 2.0 )
};

v8::Local<v8::Array> TowerDefense::structuresDefs(){
  v8::Local<v8::Array> ret = v8::Local<v8::Array>();
  return ret;
}

NAN_METHOD( TowerDefense::getStructuresDefs ){
  TowerDefense* self = Nan::ObjectWrap::Unwrap<TowerDefense>(info.This());
  v8::Local<v8::Array> ret = self->structuresDefs();
  info.GetReturnValue().Set( ret );
}
