#include "TowerDefense.h"

Nan::Persistent<v8::FunctionTemplate> TowerDefense::constructor;

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

TowerDefense::TowerDefense( int _width, int _height, std::vector<int> _startPts, std::vector<int> _endPts ){
  mapWidth = _width;
  mapHeight = _height;
  startPoints = _startPts;
  endPoints = _endPts;
  size = mapWidth * mapHeight;
  //floorIds = std::vector<int> ( size, 1 );
  tiles = std::vector<Tile*> ( size );
  for( int i = 0; i < size; i++ ){
    std::vector<int> tilePos = to2d( i );
    tiles[ i ] = new Tile( tilePos[ 0 ], tilePos[ 1 ], 1 );
  }
  moveMap = std::vector<double>( size );
  fillMoveMap();
}

NAN_METHOD(TowerDefense::New) {
  // throw an error if constructor is called without new keyword
  if( ! info.IsConstructCall() ) {
    return Nan::ThrowError( Nan::New( "TowerDefense::New - called without new keyword" ).ToLocalChecked() );
  }

  // expect exactly 2 arguments
  if( info.Length() != 4 ) {
    return Nan::ThrowError( Nan::New( "TowerDefense::New - expected arguments width, height, [ startX, startY,...], [ endX, endY, ...]" ).ToLocalChecked() );
  }

  // expect arguments to be numbers
  if( ! info[ 0 ]->IsNumber() || ! info[ 1 ]->IsNumber() || ! info[ 2 ]->IsArray() || ! info[ 3 ]->IsArray() ) {
    return Nan::ThrowError( Nan::New( "TowerDefense::New - expected arguments 0 & 1 to be numbers, 2 & 3 arrays" ).ToLocalChecked() );
  }

  // create a new instance and wrap our javascript instance

  TowerDefense* towerDef = new TowerDefense( info[0]->IntegerValue(), info[1]->IntegerValue(), Converter::jsArrayToVectorInt( v8::Local<v8::Array>::Cast( info[ 2 ] ) ), Converter::jsArrayToVectorInt( v8::Local<v8::Array>::Cast( info[ 3 ] ) ) );
  towerDef->Wrap( info.Holder() );
  //towerDef->grd_terrain = new GridIntVec( info[0]->IntegerValue(), info[1]->IntegerValue(), 1 );

  info.GetReturnValue().Set( info.Holder() );
}

NAN_GETTER( TowerDefense::HandleGetters ) {
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
  int nbFloors = floorTypes.size();
  int s = self->size;
  for( int i = 0; i < s; i++ ){
    int floorId = self->tiles[ i ]->getFloorTypeId();
    Floor* testedFloor;
    bool floorFound = false;
    for( int j = 0; j < nbFloors; j++ ){
      if( floorTypes[ j ]->getId() == floorId ){
        testedFloor = floorTypes[ j ];
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

std::vector<Floor*> TowerDefense::floorTypes = {
  new Floor( 0, "null", "null", 0.0 ),
  new Floor( 1, "bareGround", "bareGround.jpg", 1.0 ),
  new Floor( 2, "water", "water.jpg", 2.0 )
};

std::vector<StructureDef*> TowerDefense::structureTypes = {

  new StructureDef( "null", std::vector<int>(), "null", 0, 0, 0 ),

  new StructureDef( "Wall", std::vector<int>{ 1 }, "wall.jpg", 10, 1, 1 ),

  new StructureDef( "Cross", std::vector<int>{
    0,  1,  0,
    1,  1,  1,
    0,  1,  0
  }, "cross.png", 60, 3, 3 )

};

v8::Local<v8::Array> TowerDefense::structuresDefs(){
  int l =  TowerDefense::structureTypes.size();
  v8::Local<v8::Array> ret = Nan::New<v8::Array>( l - 1 );
  for( int i = 1; i < l; i++ ){
    int ii = i - 1;
    StructureDef* strucDef = TowerDefense::structureTypes[ i ];
    v8::Local<v8::Object> strucDefObj = strucDef->toObj();
    ret->Set( ii, strucDefObj );
  }
  return ret;
}

NAN_METHOD( TowerDefense::getStructuresDefs ){
  //TowerDefense* self = Nan::ObjectWrap::Unwrap<TowerDefense>(info.This());
  //v8::Local<v8::Array> ret = self->structuresDefs();
  v8::Local<v8::Array> ret = TowerDefense::structuresDefs();
  info.GetReturnValue().Set( ret );
}

void TowerDefense::fillMoveMap(){
  for( int i = 0; i < size; i++ ){
    Tile* _tile = tiles[ i ];
    if( _tile->getStructureId() != 0 ){
      moveMap[ i ] = -2.0;
    }else{
      moveMap[ i ] = TowerDefense::getFloorTypeById( _tile->getFloorTypeId() )->getSpeed();
    }
  }
  moveMap[ 0 ] = -2.0;
}

Floor* TowerDefense::getFloorTypeById( int _id ){
  int l = TowerDefense::floorTypes.size();
  for( int i = 0; i < l; i ++ ){
    if( TowerDefense::floorTypes[ i ]->getId() == _id ){
      return TowerDefense::floorTypes[ i ];
    }
  }
  return TowerDefense::floorTypes[ 0 ];
}

StructureDef* TowerDefense::getStructureTypeByName( std::string _typeName ){
  int l = TowerDefense::structureTypes.size();
  for( int i = 0; i < l; i++ ){
    StructureDef* strucDef = TowerDefense::structureTypes[ i ];
    if( strucDef->getTypeName() == _typeName ){
      return strucDef;
    }
  }
  return TowerDefense::structureTypes[ 0 ];
}

bool TowerDefense::testStructurePos( int _x, int _y, std::string _typeName ){
  StructureDef* structureType = TowerDefense::getStructureTypeByName( _typeName );
  std::vector<int> grid = structureType->getGrid();
  int gl = grid.size();
  for( int i = 0; i < gl; i++ ){
    if( grid[ i ] == 0 ){
      continue;
    }
    std::vector<int> gridPt2dCoords = structureType->to2d( i );
    int tx = _x + gridPt2dCoords[ 0 ];
    int ty = _y + gridPt2dCoords[ 1 ];
    int coord1d = to1d( tx, ty );
    if( moveMap[ coord1d ] < 0.0 ){
      return false;
    }
  }
  return true;
}

std::vector<bool> TowerDefense::testMultipleStructurePos( std::vector<int> _positions, std::string _typeName ){
  StructureDef* structureType = TowerDefense::getStructureTypeByName( _typeName );
  std::vector<int> grid = structureType->getGrid();
  int gl = grid.size();
  int nbStructures =  _positions.size() / 2;
  std::vector<bool> ret( nbStructures );
  for( int si = 0; si < nbStructures; si++ ){
    int sx = _positions[ si * 2 ];
    int sy = _positions[ si * 2 + 1 ];
    bool breakFound = false;
    for( int i = 0; i < gl; i++ ){
      if( grid[ i ] == 0 ){
        continue;
      }
      std::vector<int> gridPt2dCoords = structureType->to2d( i );
      int tx = sx + gridPt2dCoords[ 0 ];
      int ty = sy + gridPt2dCoords[ 1 ];
      int coord1d = to1d( tx, ty );
      if( moveMap[ coord1d ] < 0.0 ){
        ret[ si ] = false;
        breakFound = true;
        break;
      }
    }
    if( breakFound == false ){
      ret[ si ] = true;
    }
  }
  return ret;
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
  std::vector<bool> ret = self->testMultipleStructurePos( positions, typeName );
  info.GetReturnValue().Set( Converter::vectorBoolToJsArray( ret ) );
}

/*
std::vector<int> TowerDefense::testMultipleStructurePos( std::vector<int> _positions, std::string _typeName ){
  StructureDef* structureType = TowerDefense::getStructureTypeByName( _typeName );
  std::vector<int> grid = structureType->getGrid();
  int gl = grid.size();
  int nbStructures =  _positions.size() / 2;
  std::vector<int> ret( nbStructures );
  for( int si = 0; si < nbStructures; si++ ){
    int sx = _positions[ si * 2 ];
    int sy = _positions[ si * 2 + 1 ];
    for( int i = 0; i < gl; i++ ){
      if( grid[ i ] == 0 ){
        continue;
      }
      std::vector<int> gridPt2dCoords = structureType->to2d( i );
      int tx = sx + gridPt2dCoords[ 0 ];
      int ty = sy + gridPt2dCoords[ 1 ];
      int coord1d = to1d( tx, ty );
    //  if( moveMap[ coord1d ] < 0.0 ){
      int vel =(int)  moveMap[ coord1d ];
      ret[ si ] = vel;
    //  }
    }
    //ret[ si ] = true;
  }
  return ret;
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
  std::vector<int> ret = self->testMultipleStructurePos( positions, typeName );
  info.GetReturnValue().Set( Converter::vectorIntToJsArray( ret ) );
}
*/

NAN_METHOD( TowerDefense::getMoveMap ){
  TowerDefense* self = Nan::ObjectWrap::Unwrap<TowerDefense>( info.This() );
  std::vector<double> moveM = self->getMoveMap();
  int size = moveM.size();
  v8::Local<v8::Array> ret = Nan::New<v8::Array>( size );
  for( int i = 0; i < size; i++ ){
    v8::Local<v8::Value> jsElement = Nan::New( moveM[ i ] );
    ret->Set( i, jsElement );
  }
  info.GetReturnValue().Set( ret );
}

std::vector<double> TowerDefense::getMoveMap(){
  return moveMap;
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
  self->addStructures( positions, typeName );
}

void TowerDefense::addStructures( std::vector<int> _positions, std::string _typeName ){
  std::vector<bool> testPositions = testMultipleStructurePos( _positions, _typeName );
  int nbS = testPositions.size();
  StructureDef* strucDef = getStructureTypeByName( _typeName );
  std::vector<int> strucDefGrid = strucDef->getGrid();
  int bgl = strucDefGrid.size();
  std::vector<int> strucDefPos;
  for( int c = 0; c < bgl; c++ ){
    if( strucDefGrid[ c ] != 1 ){ continue; };
    std::vector<int> basePos = strucDef->to2d( c );
    strucDefPos.push_back( basePos[ 0 ] );
    strucDefPos.push_back( basePos[ 1 ] );
  }
  int nbTiles = strucDefPos.size() / 2;
  for( int i = 0; i < nbS; i++ ){
    if( testPositions[ i ] != true ){ continue; };
    int px = _positions[ i * 2 ];
    int py = _positions[ i * 2 + 1 ];
    Structure* newStruct = new Structure( px, py, strucDef );
    int structId = newStruct->getId();
    for( int ii = 0; ii < nbTiles; ii++ ){
      int tileX = px + strucDefPos[ ii * 2 ];
      int tileY = py + strucDefPos[ ii * 2 + 1 ];
      int pos1d = to1d( tileX, tileY );
      tiles[ pos1d ]->setStructureId( structId );
      moveMap[ pos1d ] = -2.0;
    }
    structures.push_back( newStruct );
  }
}

NAN_METHOD( TowerDefense::getStructures ){
  TowerDefense* self = Nan::ObjectWrap::Unwrap<TowerDefense>( info.This() );
  info.GetReturnValue().Set( self->getStructures() );
}

v8::Local<v8::Array> TowerDefense::getStructures(){
  v8::Local<v8::Array> ret = Nan::New<v8::Array>();
  int structL = structures.size();
  for( int i = 0; i < structL; i++ ){
    Structure* struc = structures[ i ];
    std::string typeName = struc->getTypeName();
    //std::string imgUrl =  struc->getImgUrl();
    int retL = ret->Length();
    v8::Local<v8::Object> targetCell;
    bool matchFound = false;
    for( int ii = 0; ii <retL; ii++ ){
      v8::Local<v8::Object> retCell = ret->Get( ii )->ToObject();

      v8::Local<v8::String> typeProp = Nan::New("typeName").ToLocalChecked();
      v8::Local<v8::Value> v8cellType = retCell->Get( typeProp );
      std::string cellType = std::string( *Nan::Utf8String( v8cellType->ToString() ) );

      if( cellType == typeName ){
        targetCell = retCell;
        matchFound = true;
        break;
      }
    }
    if( matchFound == false ){
      targetCell = Nan::New<v8::Object>();

      v8::Local<v8::String> typeProp = Nan::New( "typeName" ).ToLocalChecked();
      v8::Local<v8::Value> typeValue = Nan::New( typeName ).ToLocalChecked();
      targetCell->Set( typeProp, typeValue );

      v8::Local<v8::String> imgUrlProp = Nan::New( "imgUrl" ).ToLocalChecked();
      v8::Local<v8::Value> imgUrlValue = Nan::New( std::string("../img/") + struc->getImgUrl() ).ToLocalChecked();
      targetCell->Set( imgUrlProp, imgUrlValue );

      v8::Local<v8::String> gridWidthProp = Nan::New( "gridWidth" ).ToLocalChecked();
      v8::Local<v8::Value> gridWidthValue = Nan::New( struc->getGridWidth() );
      targetCell->Set( gridWidthProp, gridWidthValue );

      v8::Local<v8::String> gridHeightProp = Nan::New( "gridHeight" ).ToLocalChecked();
      v8::Local<v8::Value> gridHeightValue = Nan::New( struc->getGridHeight() );
      targetCell->Set( gridHeightProp, gridHeightValue );

      v8::Local<v8::String> positionsProp = Nan::New( "positions" ).ToLocalChecked();
      v8::Local<v8::Array> positionsArr = Nan::New<v8::Array>();
      targetCell->Set( positionsProp, positionsArr );

      ret->Set( retL, targetCell );
    }

    v8::Local<v8::String> cellArrayProp = Nan::New( "positions" ).ToLocalChecked();
    v8::Local<v8::Array> targetCellArray = v8::Local<v8::Array>::Cast( targetCell->Get( cellArrayProp ) );
    int cellArrL = targetCellArray->Length();
    v8::Local<v8::Value> idValue = Nan::New( struc->getId() );
    targetCellArray->Set( cellArrL, idValue );
    v8::Local<v8::Value> xValue = Nan::New( struc->getX() );
    targetCellArray->Set( cellArrL + 1, xValue );
    v8::Local<v8::Value> yValue = Nan::New( struc->getY() );
    targetCellArray->Set( cellArrL + 2, yValue );
  }
  return ret;
}
