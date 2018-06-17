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
  Nan::SetPrototypeMethod( ctor, "getIntMap", getIntMap );
  Nan::SetPrototypeMethod( ctor, "addStructures", addStructures );
  Nan::SetPrototypeMethod( ctor, "getStructures", getStructures );
  Nan::SetPrototypeMethod( ctor, "getStructureGrid", getStructureGrid );
  Nan::SetPrototypeMethod( ctor, "getWays", getWays );
  Nan::SetPrototypeMethod( ctor, "getStructureById", getStructureById );
  Nan::SetPrototypeMethod( ctor, "getCommonTextures", getCommonTextures );
  Nan::SetPrototypeMethod( ctor, "isPtOnStructById", isPtOnStructById );
  Nan::SetPrototypeMethod( ctor, "destroyStructById", destroyStructById );
  Nan::SetPrototypeMethod( ctor, "destroyStructsByZone", destroyStructsByZone );
  Nan::SetPrototypeMethod( ctor, "getStructureUpgradesByTypeName", getStructureUpgradesByTypeName );
  Nan::SetPrototypeMethod( ctor, "upgradeStructure", upgradeStructure );
  Nan::SetPrototypeMethod( ctor, "getStructureIdByPosition", getStructureIdByPosition );
  Nan::SetPrototypeMethod( ctor, "getPathMap", getPathMap );
  Nan::SetPrototypeMethod( ctor, "getPathMapBuffer", getPathMapBuffer );
  Nan::SetPrototypeMethod( ctor, "getTethaPath", getTethaPath );

  Nan::SetPrototypeMethod( ctor, "isTraversable", isTraversable );
  Nan::SetPrototypeMethod( ctor, "getTileSpeed", getTileSpeed );


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
  if( info.Length() == 5 && ! info[ 4 ]->IsArray() ){
    return Nan::ThrowError( Nan::New( "TowerDefense::New - expected arguments 4 to be an array" ).ToLocalChecked() );
  }

  // create a new instance and wrap our javascript instance

  //TowerDefense* towerDef = new TowerDefense( info[0]->IntegerValue(), info[1]->IntegerValue(), converter::jsArrayToVectorInt( v8::Local<v8::Array>::Cast( info[ 2 ] ) ), converter::jsArrayToVectorInt( v8::Local<v8::Array>::Cast( info[ 3 ] ) ) );
  TowerDefense* towerDef = new TowerDefense();
  towerDef->Wrap( info.Holder() );
  if( info.Length() == 4 ){
    towerDef->level = new GameLevel( info[0]->IntegerValue(), info[1]->IntegerValue(), converter::jsArrayToVectorInt( v8::Local<v8::Array>::Cast( info[ 2 ] ) ), converter::jsArrayToVectorInt( v8::Local<v8::Array>::Cast( info[ 3 ] ) ) );
  }else if( info.Length() == 5 ){
    towerDef->level = new GameLevel(  info[0]->IntegerValue(), info[1]->IntegerValue(),//width, height
                                      converter::jsArrayToVectorInt( v8::Local<v8::Array>::Cast( info[ 2 ] ) ),//startPoints
                                      converter::jsArrayToVectorInt( v8::Local<v8::Array>::Cast( info[ 3 ] ) ),//endPoints
                                      converter::jsArrayToVectorInt( v8::Local<v8::Array>::Cast( info[ 4 ] ) ) );//floorIds
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
  int nbFloors = floorsList::floorTypes.size();
  int s = self->level->getSize();
  for( int i = 0; i < s; i++ ){
    int floorId = self->level->getTile( i )->getFloorTypeId();
    Floor* testedFloor;
    bool floorFound = false;
    for( int j = 0; j < nbFloors; j++ ){
      if( floorsList::floorTypes[ j ]->getId() == floorId ){
        testedFloor = floorsList::floorTypes[ j ];
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
  v8::Local<v8::Array> ret = structuresDefList::structuresDefs();
  info.GetReturnValue().Set( ret );
}

NAN_METHOD( TowerDefense::testStructuresPos ){
  if( info.Length() != 3 ) {
    return Nan::ThrowError(Nan::New("TowerDefense::testStructurePos - expected 3 arguments [ positions...], typeName, rotation").ToLocalChecked());
  }
  if( ! info[ 0 ]->IsArray() || ! info[ 1 ]->IsString() || ! info[ 2 ]->IsNumber() ) {
    return Nan::ThrowError(Nan::New("TowerDefense::testStructurePos - expected argument 0 to be an array, argument 1 to be a string, argument 2 to be a number").ToLocalChecked());
  }
  std::vector<int> positions = converter::jsArrayToVectorInt( v8::Local<v8::Array>::Cast( info[ 0 ] ) );
  std::string typeName = std::string( *Nan::Utf8String( info[ 1 ] ) );
  int rotation = info[ 2 ]->IntegerValue();
  TowerDefense* self = Nan::ObjectWrap::Unwrap<TowerDefense>( info.This() );
  std::vector<bool> ret = self->level->testMultipleStructurePos( positions, typeName, rotation );
  info.GetReturnValue().Set( converter::vectorBoolToJsArray( ret ) );
}

NAN_METHOD( TowerDefense::getMoveMap ){
  TowerDefense* self = Nan::ObjectWrap::Unwrap<TowerDefense>( info.This() );
  std::vector<float> moveM = self->level->getMoveMap();
  int size = moveM.size();
  v8::Local<v8::Array> ret = Nan::New<v8::Array>( size );
  for( int i = 0; i < size; i++ ){
    v8::Local<v8::Value> jsElement = Nan::New( moveM[ i ] );
    ret->Set( i, jsElement );
  }
  info.GetReturnValue().Set( ret );
}

NAN_METHOD( TowerDefense::getIntMap ){
  TowerDefense* self = Nan::ObjectWrap::Unwrap<TowerDefense>( info.This() );
  info.GetReturnValue().Set( converter::vectorIntToJsArray( self->level->getIntMap() ) );
}

NAN_METHOD( TowerDefense::addStructures ){
  TowerDefense* self = Nan::ObjectWrap::Unwrap<TowerDefense>( info.This() );
  if( info.Length() != 3 ) {
    return Nan::ThrowError( Nan::New( "TowerDefense::addStructures - expected 3 arguments [ positions...], typeName, rotation" ).ToLocalChecked() );
  }
  if( !info[0]->IsArray() || !info[1]->IsString() || !info[2]->IsNumber() ) {
    return Nan::ThrowError( Nan::New( "TowerDefense::addStructures - expected argument 0 to be an array, argument 1 to be a string, argument 2 to be a number" ).ToLocalChecked() );
  }
  std::vector<int> positions = converter::jsArrayToVectorInt( v8::Local<v8::Array>::Cast( info[ 0 ] ) );
  std::string typeName = std::string( *Nan::Utf8String( info[ 1 ] ) );
  int rotation = info[ 2 ]->IntegerValue();
  bool add = self->level->addStructures( positions, typeName, rotation );

  info.GetReturnValue().Set( Nan::New( add ) );
}

NAN_METHOD( TowerDefense::getStructures ){
  TowerDefense* self = Nan::ObjectWrap::Unwrap<TowerDefense>( info.This() );
  info.GetReturnValue().Set( self->level->getStructures() );
}

NAN_METHOD( TowerDefense::getStructureGrid ){
  TowerDefense* self = Nan::ObjectWrap::Unwrap<TowerDefense>( info.This() );
  if( info.Length() != 2 ) {
    return Nan::ThrowError( Nan::New( "TowerDefense::getStructureGrid - expected 2 arguments: typeName, rotation").ToLocalChecked());
  }
  if( !info[0]->IsString() || !info[1]->IsNumber() ) {
    return Nan::ThrowError( Nan::New( "TowerDefense::getStructureGrid - expected argument 0 to be a string, argument 1 to be a number" ).ToLocalChecked() );
  }
  std::string typeName = std::string( *Nan::Utf8String( info[ 0 ] ) );
  int rotation = info[ 1 ]->IntegerValue();

  info.GetReturnValue().Set( self->level->getStructureGrid( typeName, rotation ) );
}

NAN_METHOD( TowerDefense::getStructureById ){
  if( info.Length() != 1 ) {
    return Nan::ThrowError(Nan::New("TowerDefense::getStructureById - expected 1 arguments id").ToLocalChecked());
  }
  if( ! info[ 0 ]->IsNumber() ) {
    return Nan::ThrowError(Nan::New("TowerDefense::getStructureById - expected argument 0 to be a number").ToLocalChecked());
  }
  int searchedId = info[ 0 ]->IntegerValue();
  if( searchedId == 0 ){
    return Nan::ThrowError(Nan::New("TowerDefense::getStructureById - invalid searched Id").ToLocalChecked());
  }
  TowerDefense* self = Nan::ObjectWrap::Unwrap<TowerDefense>( info.This() );
  info.GetReturnValue().Set( self->level->getStructureById( searchedId )->toObj() );
}

NAN_METHOD( TowerDefense::getWays ){
  TowerDefense* self = Nan::ObjectWrap::Unwrap<TowerDefense>( info.This() );
  info.GetReturnValue().Set( self->level->getWays() );
}

NAN_METHOD( TowerDefense::getCommonTextures ){
  TowerDefense* self = Nan::ObjectWrap::Unwrap<TowerDefense>( info.This() );
  info.GetReturnValue().Set( self->level->getCommonTextures() );
}

NAN_METHOD( TowerDefense::isPtOnStructById ){
  if( info.Length() != 3 ) {
    return Nan::ThrowError(Nan::New("TowerDefense::isPtOnStructById - expected 3 arguments id, x, y").ToLocalChecked());
  }
  if( !info[ 0 ]->IsNumber() || !info[ 1 ]->IsNumber() || !info[ 2 ]->IsNumber() ) {
    return Nan::ThrowError( Nan::New( "TowerDefense::isPtOnStructById - expected all 3 arguments to be numbers" ).ToLocalChecked() );
  }
  TowerDefense* self = Nan::ObjectWrap::Unwrap<TowerDefense>( info.This() );
  int id = info[ 0 ]->IntegerValue();
  int x = info[ 1 ]->IntegerValue();
  int y = info[ 2 ]->IntegerValue();
  info.GetReturnValue().Set( self->level->isPointOnStructureById( id, x, y ) );
}

NAN_METHOD( TowerDefense::destroyStructById ){
  if( info.Length() != 1 ) {
    return Nan::ThrowError(Nan::New("TowerDefense::destroyStructById - expected 1 argument id").ToLocalChecked());
  }
  if( !info[ 0 ]->IsNumber() ) {
    return Nan::ThrowError( Nan::New( "TowerDefense::destroyStructById - expected argument 1 to be a number" ).ToLocalChecked() );
  }
  int id = info[ 0 ]->IntegerValue();
  TowerDefense* self = Nan::ObjectWrap::Unwrap<TowerDefense>( info.This() );
  info.GetReturnValue().Set( self->level->removeStructById( id ) );
}

NAN_METHOD( TowerDefense::destroyStructsByZone ){
  if( info.Length() != 4 ) {
    return Nan::ThrowError(Nan::New("TowerDefense::destroyStructsByZone - expected 4 argument startx, starty, endx, endy").ToLocalChecked());
  }
  if( !info[ 0 ]->IsNumber() || !info[ 0 ]->IsNumber() || !info[ 1 ]->IsNumber() || !info[ 2 ]->IsNumber() || !info[ 3 ]->IsNumber() ) {
    return Nan::ThrowError( Nan::New( "TowerDefense::destroyStructById - expected argument 1 to be a number" ).ToLocalChecked() );
  }
  int startx = info[ 0 ]->IntegerValue();
  int starty = info[ 1 ]->IntegerValue();
  int endx = info[ 2 ]->IntegerValue();
  int endy = info[ 3 ]->IntegerValue();
  TowerDefense* self = Nan::ObjectWrap::Unwrap<TowerDefense>( info.This() );
  info.GetReturnValue().Set( self->level->destroyStructsByZone( startx, starty, endx, endy ) );
}

NAN_METHOD( TowerDefense::getStructureUpgradesByTypeName ){
  if( info.Length() != 1 ) {
    return Nan::ThrowError(Nan::New("TowerDefense::getStructureUpgradesByTypeName - expected 1 argument : typeName").ToLocalChecked());
  }
  if( ! info[ 0 ]->IsString() ) {
    return Nan::ThrowError(Nan::New("TowerDefense::getStructureUpgradesByTypeName - expected argument 0 to be a string").ToLocalChecked());
  }
  TowerDefense* self = Nan::ObjectWrap::Unwrap<TowerDefense>( info.This() );
  std::string typeName = std::string( *Nan::Utf8String( info[ 0 ] ) );
  info.GetReturnValue().Set( self->level->getStructureUpgradesByTypeName( typeName ) );
}

NAN_METHOD( TowerDefense::upgradeStructure ){
  if( info.Length() != 2 ) {
    return Nan::ThrowError(Nan::New("TowerDefense::upgradeStructure - expected 2 argument : id, typeName").ToLocalChecked());
  }
  if( ! info[ 0 ]->IsNumber() || ! info[ 1 ]->IsString() ) {
    return Nan::ThrowError(Nan::New("TowerDefense::upgradeStructure - expected argument 0 to be a number, argument 1 to be a String").ToLocalChecked());
  }
  TowerDefense* self = Nan::ObjectWrap::Unwrap<TowerDefense>( info.This() );
  int id = info[ 0 ]->IntegerValue();
  std::string typeName = std::string( *Nan::Utf8String( info[ 1 ] ) );

  bool upgrade = self->level->upgradeStructure( id, typeName );
  info.GetReturnValue().Set( Nan::New( upgrade ) );
}

NAN_METHOD( TowerDefense::getStructureIdByPosition ){
  if( info.Length() != 2 ) {
    return Nan::ThrowError(Nan::New("TowerDefense::getStructureIdByPosition - expected 2 argument : x, y").ToLocalChecked());
  }
  if( ! info[ 0 ]->IsNumber() || ! info[ 1 ]->IsNumber() ) {
    return Nan::ThrowError(Nan::New("TowerDefense::getStructureIdByPosition - expected argument 0 and 1 to be numbers").ToLocalChecked());
  }
  TowerDefense* self = Nan::ObjectWrap::Unwrap<TowerDefense>( info.This() );
  info.GetReturnValue().Set( self->level->getStructureIdByPosition( info[ 0 ]->IntegerValue(), info[ 1 ]->IntegerValue() ) );
}

NAN_METHOD( TowerDefense::getPathMap ){
  if( info.Length() != 2 ) {
    return Nan::ThrowError(Nan::New("TowerDefense::getPathMap - expected 2 argument : x, y").ToLocalChecked());
  }
  if( ! info[ 0 ]->IsNumber() || ! info[ 1 ]->IsNumber() ) {
    return Nan::ThrowError(Nan::New("TowerDefense::getPathMap - expected argument 0 and 1 to be numbers").ToLocalChecked());
  }
  TowerDefense* self = Nan::ObjectWrap::Unwrap<TowerDefense>( info.This() );
  info.GetReturnValue().Set( converter::vectorFloatToJsArray( self->level->pathMap( info[ 0 ]->IntegerValue(), info[ 1 ]->IntegerValue() ) ) );
}

void buffer_delete_callback(char* data, void* the_vector) {
  delete reinterpret_cast<std::vector<char>*> (the_vector);
}
NAN_METHOD( TowerDefense::getPathMapBuffer ){
  if( info.Length() != 2 ) {
    return Nan::ThrowError(Nan::New("TowerDefense::getPathMap - expected 2 argument : x, y").ToLocalChecked());
  }
  if( ! info[ 0 ]->IsNumber() || ! info[ 1 ]->IsNumber() ) {
    return Nan::ThrowError(Nan::New("TowerDefense::getPathMap - expected argument 0 and 1 to be numbers").ToLocalChecked());
  }
  TowerDefense* self = Nan::ObjectWrap::Unwrap<TowerDefense>( info.This() );
  std::vector<float> paths = self->level->pathMap( info[ 0 ]->IntegerValue(), info[ 1 ]->IntegerValue() );
  std::vector<char>* chars = new std::vector<char> ();
  int l = paths.size();
  for( int i = 0; i < l; i++ ){
    float baseFloat = paths[ i ];
    char a[4];
    memcpy( a, &baseFloat, 4 );
    chars->push_back( a[ 0 ] );
    chars->push_back( a[ 1 ] );
    chars->push_back( a[ 2 ] );
    chars->push_back( a[ 3 ] );
  }
  info.GetReturnValue().Set( Nan::NewBuffer( (char *)chars->data(), chars->size(), buffer_delete_callback, chars).ToLocalChecked() );
}


NAN_METHOD( TowerDefense::isTraversable ){
  TowerDefense* self = Nan::ObjectWrap::Unwrap<TowerDefense>( info.This() );
  if( info.Length() == 1 ) {
    info.GetReturnValue().Set( self->level->isTraversable( info[ 0 ]->IntegerValue(), -1 ) );
  }else{
    info.GetReturnValue().Set( self->level->isTraversable( info[ 0 ]->IntegerValue(), info[ 1 ]->IntegerValue() ) );
  }
}

NAN_METHOD( TowerDefense::getTileSpeed ){
  TowerDefense* self = Nan::ObjectWrap::Unwrap<TowerDefense>( info.This() );
  if( info.Length() == 1 ) {
    info.GetReturnValue().Set( self->level->getTileSpeed( info[ 0 ]->IntegerValue(), -1 ) );
  }else{
    info.GetReturnValue().Set( self->level->getTileSpeed( info[ 0 ]->IntegerValue(), info[ 1 ]->IntegerValue() ) );
  }
}

NAN_METHOD( TowerDefense::getTethaPath ){
  if( info.Length() != 4 ) {
    return Nan::ThrowError( Nan::New( "TowerDefense::getTethaPath - expected 4 argument : startx, starty, destinationx, destinationy" ).ToLocalChecked() );
  }
  TowerDefense* self = Nan::ObjectWrap::Unwrap<TowerDefense>( info.This() );
  info.GetReturnValue().Set( converter::vectorIntToJsArray( self->level->getTethaPath( info[ 0 ]->IntegerValue(), info[ 1 ]->IntegerValue(), info[ 2 ]->IntegerValue(), info[ 3 ]->IntegerValue() ) ) );

}
