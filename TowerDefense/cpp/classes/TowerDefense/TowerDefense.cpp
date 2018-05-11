#include "TowerDefense.h"

Nan::Persistent<v8::FunctionTemplate> TowerDefense::constructor;

void TowerDefense::setLevel( GameLevelBase* _level ){
  level = _level;
}

NAN_MODULE_INIT(TowerDefense::Init) {
  v8::Local<v8::FunctionTemplate> ctor = Nan::New<v8::FunctionTemplate>(TowerDefense::New);
  constructor.Reset(ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(Nan::New("TowerDefense").ToLocalChecked());
  Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("width").ToLocalChecked(), TowerDefense::HandleGetters, TowerDefense::HandleSetters);
  Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("height").ToLocalChecked(), TowerDefense::HandleGetters, TowerDefense::HandleSetters);
  //Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("floors").ToLocalChecked(), TowerDefense::HandleGetters, TowerDefense::HandleSetters);
  /*
  Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("map").ToLocalChecked(), TowerDefense::HandleGetters, TowerDefense::HandleSetters);
  Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("width").ToLocalChecked(), TowerDefense::HandleGetters, TowerDefense::HandleSetters);
  Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("height").ToLocalChecked(), TowerDefense::HandleGetters, TowerDefense::HandleSetters);
  Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("length").ToLocalChecked(), TowerDefense::HandleGetters, TowerDefense::HandleSetters);
  Nan::SetPrototypeMethod( ctor, "addStructure", addStructure );
  */
  Nan::SetPrototypeMethod( ctor, "getFloors", getFloors );
  Nan::SetPrototypeMethod( ctor, "getTiles", getTiles );
  Nan::SetPrototypeMethod( ctor, "getStructuresDefs", getStructuresDefs );
  Nan::SetPrototypeMethod( ctor, "testStructuresPos", testStructuresPos );
  Nan::SetPrototypeMethod( ctor, "getMoveMap", getMoveMap );
  Nan::SetPrototypeMethod( ctor, "addStructures", addStructures );
  Nan::SetPrototypeMethod( ctor, "getStructures", getStructures );

  target->Set(Nan::New("TowerDefense").ToLocalChecked(), ctor->GetFunction());

}

NAN_METHOD(TowerDefense::New) {
  // throw an error if constructor is called without new keyword
  if( ! info.IsConstructCall() ) {
    return Nan::ThrowError( Nan::New( "TowerDefense::New - called without new keyword" ).ToLocalChecked() );
  }

  // expect exactly 2 arguments
  if( info.Length() < 4 ) {
    return Nan::ThrowError( Nan::New( "TowerDefense::New - expected arguments width, height, [ startX, startY,...], [ endX, endY, ...]" ).ToLocalChecked() );
  }

  // expect arguments to be numbers
  if( ! info[ 0 ]->IsNumber() || ! info[ 1 ]->IsNumber() || ! info[ 2 ]->IsArray() || ! info[ 3 ]->IsArray() ) {
    return Nan::ThrowError( Nan::New( "TowerDefense::New - expected arguments 0 & 1 to be numbers, 2 & 3 arrays" ).ToLocalChecked() );
  }

  // create a new instance and wrap our javascript instance

  //TowerDefense* towerDef = new TowerDefense( info[0]->IntegerValue(), info[1]->IntegerValue(), Converter::jsArrayToVectorInt( v8::Local<v8::Array>::Cast( info[ 2 ] ) ), Converter::jsArrayToVectorInt( v8::Local<v8::Array>::Cast( info[ 3 ] ) ) );
  TowerDefense* towerDef = new TowerDefense();
  towerDef->Wrap( info.Holder() );
  if( info.Length() == 4 ){
    towerDef->level = new GameLevel( info[0]->IntegerValue(), info[1]->IntegerValue(), Converter::jsArrayToVectorInt( v8::Local<v8::Array>::Cast( info[ 2 ] ) ), Converter::jsArrayToVectorInt( v8::Local<v8::Array>::Cast( info[ 3 ] ) ) );
  }

  info.GetReturnValue().Set( info.Holder() );
}

NAN_GETTER( TowerDefense::HandleGetters ) {
  TowerDefense* self = Nan::ObjectWrap::Unwrap<TowerDefense>( info.This() );
  std::string propertyName = std::string( *Nan::Utf8String( property ) );
  if( propertyName == "width" ){
    info.GetReturnValue().Set( self->level->width() );
  }else if( propertyName == "height" ){
    info.GetReturnValue().Set( self->level->height() );
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

NAN_METHOD( TowerDefense::getTiles ){
  TowerDefense* self = Nan::ObjectWrap::Unwrap<TowerDefense>(info.This());
  info.GetReturnValue().Set( self->level->getTilesArray() );
}

NAN_METHOD( TowerDefense::getFloors ){
  //v8::Local<v8::Object> TowerDefense::getFloors(){
  TowerDefense* self = Nan::ObjectWrap::Unwrap<TowerDefense>(info.This());
  v8::Local<v8::Object> ret = Nan::New<v8::Object>();
  int nbFloors = FloorsList::floorTypes.size();
  int s = self->level->getSize();
  for( int i = 0; i < s; i++ ){
    int floorId = self->level->getTile( i )->getFloorTypeId();
    Floor* testedFloor;
    bool floorFound = false;
    for( int j = 0; j < nbFloors; j++ ){
      if( FloorsList::floorTypes[ j ]->getId() == floorId ){
        testedFloor = FloorsList::floorTypes[ j ];
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
    std::vector<int> tilePos = self->level->to2d( i );
    v8::Local<v8::Value> xVal = Nan::New( tilePos[ 0 ] );
    v8::Local<v8::Value> yVal = Nan::New( tilePos[ 1 ] );
    floorPos->Set( floorPosL, xVal );
    floorPosL++;
    floorPos->Set( floorPosL, yVal );
  }
  //return ret;
  info.GetReturnValue().Set( ret );
}

NAN_METHOD( TowerDefense::getStructuresDefs ){
  //TowerDefense* self = Nan::ObjectWrap::Unwrap<TowerDefense>(info.This());
  //v8::Local<v8::Array> ret = self->structuresDefs();
  v8::Local<v8::Array> ret = StructuresDefList::structuresDefs();
  info.GetReturnValue().Set( ret );
}

NAN_METHOD( TowerDefense::testStructuresPos ){
  if( info.Length() != 2 ) {
    return Nan::ThrowError(Nan::New("TowerDefense::testStructurePos - expected 2 arguments [ positions...], typeName").ToLocalChecked());
  }
  if( !info[0]->IsArray() || !info[1]->IsString() ) {
    return Nan::ThrowError(Nan::New("TowerDefense::testStructurePos - expected argument 0 to be an array, argument 1 to be a string").ToLocalChecked());
  }
  std::vector<int> positions = Converter::jsArrayToVectorInt( v8::Local<v8::Array>::Cast( info[ 0 ] ) );
  std::string typeName = std::string( *Nan::Utf8String( info[ 1 ] ) );
  TowerDefense* self = Nan::ObjectWrap::Unwrap<TowerDefense>( info.This() );
  std::vector<bool> ret = self->level->testMultipleStructurePos( positions, typeName );
  info.GetReturnValue().Set( Converter::vectorBoolToJsArray( ret ) );
}

NAN_METHOD( TowerDefense::getMoveMap ){
  TowerDefense* self = Nan::ObjectWrap::Unwrap<TowerDefense>( info.This() );
  std::vector<double> moveM = self->level->getMoveMap();
  int size = moveM.size();
  v8::Local<v8::Array> ret = Nan::New<v8::Array>( size );
  for( int i = 0; i < size; i++ ){
    v8::Local<v8::Value> jsElement = Nan::New( moveM[ i ] );
    ret->Set( i, jsElement );
  }
  info.GetReturnValue().Set( ret );
}

NAN_METHOD( TowerDefense::addStructures ){
  TowerDefense* self = Nan::ObjectWrap::Unwrap<TowerDefense>( info.This() );
  if( info.Length() != 2 ) {
    return Nan::ThrowError(Nan::New("TowerDefense::addStructures - expected 2 arguments [ positions...], typeName").ToLocalChecked());
  }
  if( !info[0]->IsArray() || !info[1]->IsString() ) {
    return Nan::ThrowError(Nan::New("TowerDefense::addStructures - expected argument 0 to be an array, argument 1 to be a string").ToLocalChecked());
  }
  std::vector<int> positions = Converter::jsArrayToVectorInt( v8::Local<v8::Array>::Cast( info[ 0 ] ) );
  std::string typeName = std::string( *Nan::Utf8String( info[ 1 ] ) );
  self->level->addStructures( positions, typeName );
}

NAN_METHOD( TowerDefense::getStructures ){
  TowerDefense* self = Nan::ObjectWrap::Unwrap<TowerDefense>( info.This() );
  info.GetReturnValue().Set( self->level->getStructures() );
}
