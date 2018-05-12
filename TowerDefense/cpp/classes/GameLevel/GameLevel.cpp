#include "GameLevel.h"

GameLevel::GameLevel(){}

GameLevel::GameLevel( int _width, int _height, std::vector<int> _startPts, std::vector<int> _endPts ):GameLevelBase( _width, _height, _startPts, _endPts ){
  int _size = getSize();
  tiles = std::vector<Tile*> ( _size );
  for( int i = 0; i < _size; i++ ){
    std::vector<int> tilePos = to2d( i );
    tiles[ i ] = new Tile( tilePos[ 0 ], tilePos[ 1 ], 1 );
  }
  moveMap = std::vector<double>( _size );
  fillMoveMap();
}

v8::Local<v8::Array> GameLevel::getTilesArray(){
  int _size = getSize();
  v8::Local<v8::Array> ret = Nan::New<v8::Array>( _size );
  for( int i = 0; i < _size; i++ ){
    v8::Local<v8::Object> v8Tile = tiles[ i ]->toObj();
    ret->Set( i, v8Tile );
  }
  return ret;
}

void GameLevel::fillMoveMap(){
  int _size = getSize();
  for( int i = 0; i < _size; i++ ){
    Tile* _tile = tiles[ i ];
    if( _tile->getStructureId() != 0 ){
      moveMap[ i ] = -2.0;
    }else{
      moveMap[ i ] = FloorsList::getFloorTypeById( _tile->getFloorTypeId() )->getSpeed();
    }
  }
}

bool GameLevel::testStructurePos( int _x, int _y, std::string _typeName ){
  StructureDef* structureType = StructuresDefList::getStructureTypeByName( _typeName );
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
    if( moveMap[ coord1d ] < 0.0 || tx < 0 || tx >= width() || ty < 0 || ty >= height() ){
      return false;
    }
  }
  return true;
}

std::vector<bool> GameLevel::testMultipleStructurePos( std::vector<int> _positions, std::string _typeName, int _rotation ){
  StructureDef* structureType = StructuresDefList::getStructureTypeByName( _typeName );
  std::vector<int> grid = structureType->getGrid( _rotation );
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
      std::vector<int> gridPt2dCoords = structureType->to2d( i, _rotation );
      int tx = sx + gridPt2dCoords[ 0 ];
      int ty = sy + gridPt2dCoords[ 1 ];
      int coord1d = to1d( tx, ty );
      if( moveMap[ coord1d ] < 0.0 || tx < 0 || tx >= width() || ty < 0 || ty >= height() ){
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

std::vector<double> GameLevel::getMoveMap(){
  return moveMap;
}

void GameLevel::addStructures( std::vector<int> _positions, std::string _typeName, int _rotation ){
  std::vector<bool> testPositions = testMultipleStructurePos( _positions, _typeName, _rotation );
  int nbS = testPositions.size();
  StructureDef* strucDef = StructuresDefList::getStructureTypeByName( _typeName );
  std::vector<int> strucDefGrid = strucDef->getGrid( _rotation );
  int bgl = strucDefGrid.size();
  std::vector<int> strucDefPos;
  for( int c = 0; c < bgl; c++ ){
    if( strucDefGrid[ c ] != 1 ){ continue; };
    std::vector<int> basePos = strucDef->to2d( c, _rotation );
    strucDefPos.push_back( basePos[ 0 ] );
    strucDefPos.push_back( basePos[ 1 ] );
  }
  int nbTiles = strucDefPos.size() / 2;
  for( int i = 0; i < nbS; i++ ){
    if( testPositions[ i ] != true ){ continue; };
    int px = _positions[ i * 2 ];
    int py = _positions[ i * 2 + 1 ];
    Structure* newStruct = new Structure( px, py, strucDef, _rotation );
    int structId = newStruct->getId();
    for( int ii = 0; ii < nbTiles; ii++ ){
      int tileX = px + strucDefPos[ ii * 2 ];
      int tileY = py + strucDefPos[ ii * 2 + 1 ];
      int pos1d = to1d( tileX, tileY );
      tiles[ pos1d ]->setStructureId( structId );
      moveMap[ pos1d ] = -2.0;
    }
    pushStructure( newStruct );
  }
}

v8::Local<v8::Array> GameLevel::getStructures(){
  v8::Local<v8::Array> ret = Nan::New<v8::Array>();
  int structL = structuresSize();
  for( int i = 0; i < structL; i++ ){
    Structure* struc = getStructure( i );
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

    cellArrL++;
    v8::Local<v8::Value> rotValue = Nan::New( struc->getRotation() );
    targetCellArray->Set( cellArrL, rotValue );

    cellArrL++;
    v8::Local<v8::Value> xValue = Nan::New( struc->getX() );
    targetCellArray->Set( cellArrL, xValue );

    cellArrL++;
    v8::Local<v8::Value> yValue = Nan::New( struc->getY() );
    targetCellArray->Set( cellArrL, yValue );
  }
  return ret;
}

Tile* GameLevel::getTile( int _i ){
  return tiles[ _i ];
}
