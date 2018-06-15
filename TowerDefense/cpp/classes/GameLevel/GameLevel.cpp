#include "GameLevel.h"

GameLevel::GameLevel(){}

GameLevel::GameLevel( int _width, int _height, std::vector<int> _startPts, std::vector<int> _endPts ):GameLevelBase( _width, _height, _startPts, _endPts ){
  int _size = getSize();
  tiles = std::vector<Tile*> ( _size );
  int sl = _startPts.size() / 2;
  int el = _endPts.size() / 2;
  for( int i = 0; i < _size; i++ ){
    std::vector<int> tilePos = to2d( i );
    Tile* nTile = new Tile( tilePos[ 0 ], tilePos[ 1 ], 1 );
    for( int si = 0; si < sl; si++ ){
      int sx = _startPts[ si * 2 ];
      int sy = _startPts[ si * 2 + 1 ];
      if( sx == tilePos[ 0 ] && sy == tilePos[ 1 ] ){
        nTile->setWayInOrOut( "start");
      }
    }
    for( int ei = 0; ei < el; ei++ ){
      int ex = _endPts[ ei * 2 ];
      int ey = _endPts[ ei * 2 + 1 ];
      if( ex == tilePos[ 0 ] && ey == tilePos[ 1 ] ){
        nTile->setWayInOrOut( "end");
      }
    }
    tiles[ i ] = nTile;
  }
  moveMap = std::vector<float>( _size );
  intMap = std::vector<int>( _size, 0 );
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
      intMap[ i ] = -2;
    }else if( _tile->getWayType() > 0 ){
      moveMap[ i ] = -1.0;
      intMap[ i ] = -1;
    }else{
      moveMap[ i ] = floorsList::getFloorTypeById( _tile->getFloorTypeId() )->getSpeed();
      intMap[ i ] = 1;
    }
  }
}

bool GameLevel::testStructurePos( int _x, int _y, std::string _typeName ){
  StructureDef* structureType = structuresDefList::getStructureTypeByName( _typeName );
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
  StructureDef* structureType = structuresDefList::getStructureTypeByName( _typeName );
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

std::vector<float> GameLevel::getMoveMap(){
  //return moveMap;
  int l = moveMap.size();
  std::vector<float> ret( l * 3 );
  for( int i = 0; i < l; i++ ){
    std::vector<int> pos = to2d( i );
    ret[ i * 3 ] = moveMap[ i ];
    ret[ i * 3 + 1 ] = (float) pos[ 0 ];
    ret[ i * 3 + 2 ] = (float) pos[ 1 ];
  }
  return ret;
}

std::vector<int> GameLevel::getIntMap(){ return intMap; };

bool GameLevel::addStructures( std::vector<int> _positions, std::string _typeName, int _rotation ){
  std::vector<bool> testPositions = testMultipleStructurePos( _positions, _typeName, _rotation );
  int nbS = testPositions.size();
  StructureDef* strucDef = structuresDefList::getStructureTypeByName( _typeName );
  std::vector<int> strucDefGrid = strucDef->getGrid( _rotation );
  int bgl = strucDefGrid.size();
  std::vector<int> strucDefPos;
  for( int c = 0; c < bgl; c++ ){
    if( strucDefGrid[ c ] != 1 ){ continue; };
    std::vector<int> basePos = strucDef->to2d( c, _rotation );
    strucDefPos.push_back( basePos[ 0 ] );
    strucDefPos.push_back( basePos[ 1 ] );
  }

  std::vector<int> okPositions;
  for( int t = 0; t < nbS; t++ ){
    if( testPositions[ t ] == true ){
      okPositions.push_back( _positions[ t * 2 ] );
      okPositions.push_back( _positions[ t * 2 + 1 ] );
    };
  }
  bool testBlocking = newStructuresBlockingTest(  okPositions, strucDefPos );
  //bool testBlocking = true;
  if( testBlocking == false ){
    return false;
  }
  int nbTestedStructs = okPositions.size() / 2;
  int nbTiles = strucDefPos.size() / 2;
  for( int i = 0; i < nbTestedStructs; i++ ){
    int px = okPositions[ i * 2 ];
    int py = okPositions[ i * 2 + 1 ];
    Structure* newStruct = new Structure( px, py, strucDef, _rotation );
    int structId = newStruct->getId();
    for( int ii = 0; ii < nbTiles; ii++ ){
      int tileX = px + strucDefPos[ ii * 2 ];
      int tileY = py + strucDefPos[ ii * 2 + 1 ];
      int pos1d = to1d( tileX, tileY );
      tiles[ pos1d ]->setStructureId( structId );
      moveMap[ pos1d ] = -2.0;
      intMap[ pos1d ] = -2;
    }
    pushStructure( newStruct );
  }
  return true;
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

Tile* GameLevel::getTileByPosition( int _x, int _y ){
  if( _x >= 0 && _y >= 0 && _x <  width() && _y < height() ){
    return tiles[ to1d( _x, _y ) ];
  }
  return new Tile();
}

v8::Local<v8::Array> GameLevel::getStructureGrid( std::string _typeName, int _rotation ){
  StructureDef* structDef = structuresDefList::getStructureTypeByName( _typeName );
  std::vector<int> grid = structDef->getGrid( _rotation );
  v8::Local<v8::Array> ret = converter::vectorIntToJsArray( grid );
  return ret;
}

bool GameLevel::newStructuresBlockingTest(  std::vector<int> _positions, std::vector<int> _strucDefPositions ){
  int l = _positions.size() / 2;
  int nbTiles = _strucDefPositions.size() / 2;
  for( int i = 0; i < l; i++ ){
    int px = _positions[ i * 2 ];
    int py = _positions[ i * 2 + 1 ];
    for( int ii = 0; ii < nbTiles; ii++ ){
        intMap[ to1d( px + _strucDefPositions[ ii * 2 ], py + _strucDefPositions[ ii * 2 + 1 ] ) ] = -2;
    }
  }
  bool test = testMapOpening();
  for( int iii = 0; iii < l; iii++ ){
    int px = _positions[ iii * 2 ];
    int py = _positions[ iii * 2 + 1 ];
    for( int iiii = 0; iiii < nbTiles; iiii++ ){
        intMap[ to1d( px + _strucDefPositions[ iiii * 2 ], py + _strucDefPositions[ iiii * 2 + 1 ] ) ] = 1;
    }
  }
  return test;
}

bool GameLevel::testMapOpening(){
  int mapl =  getSize();
  int testMap[ mapl ] = { 0 };
  std::vector<int> openList = getStartByIndex( 0 );
  testMap[ to1d( openList[ 0 ], openList[ 1 ] ) ] = 2;
  bool endFound = false;
  int mw = width();
  int mh = height();
  for( int s = 0; endFound == false; s++ ){
    std::vector<int> newList;
    int l = openList.size() / 2;
    for( int i = 0; i < l; i++ ){
      int px = openList[ i * 2 ];
      int py = openList[ i * 2 + 1 ];

      if( px > 0 ){
        int leftPos = to1d( px - 1, py );
        if( testMap[ leftPos ] == 0 ){
          int intMapVal = intMap[ leftPos ];
          if( intMapVal > 0 ){
            testMap[ leftPos ] = 1;
            newList.push_back( px - 1 );
            newList.push_back( py );
          }else if( intMapVal < 0 ){
            testMap[ leftPos ] = -1;
          }
        }
      }

      if( px < mw - 1 ){
        int rightPos = to1d( px + 1, py );
        if( testMap[ rightPos ] == 0 ){
          int intMapVal = intMap[ rightPos ];
          if( intMapVal > 0 ){
            testMap[ rightPos ] = 1;
            newList.push_back( px + 1 );
            newList.push_back( py );
          }else if( intMapVal < 0 ){
            testMap[ rightPos ] = -1;
          }
        }
      }

      if( py > 0 ){
        int topPos = to1d( px, py - 1 );
        if( testMap[ topPos ] == 0 ){
          int intMapVal = intMap[ topPos ];
          if( intMapVal > 0 ){
            testMap[ topPos ] = 1;
            newList.push_back( px );
            newList.push_back( py - 1 );
          }else if( intMapVal < 0 ){
            testMap[ topPos ] = -1;
          }
        }
      }

      if( py < mh - 1){
        int bottomPos = to1d( px, py + 1 );
        if( testMap[ bottomPos ] == 0 ){
          int intMapVal = intMap[ bottomPos ];
          if( intMapVal > 0 ){
            testMap[ bottomPos ] = 1;
            newList.push_back( px );
            newList.push_back( py + 1 );
          }else if( intMapVal < 0 ){
            testMap[ bottomPos ] = -1;
          }
        }
      }

    }
    if( newList.size() > 0 ){
      openList = newList;
    }else{
      endFound = true;
      break;
    }
  }
  for ( int n = 0; n < mapl; n++ ){
    if( testMap[ n ] == 0 && intMap[ n ] > 0 ){
      return false;
    }
  }
  return true;
}

int GameLevel::removeStructById( int _id ){
  Structure* strucToRemove = getStructureById( _id );
  StructureDef* strucDef = structuresDefList::getStructureTypeByName( strucToRemove->getTypeName() );
  int strucRot = strucToRemove->getRotation();
  std::vector<int> strucGrid = strucDef->getGrid( strucRot );
  int sx = strucToRemove->getX();
  int sy = strucToRemove->getY();
  int l = strucGrid.size();
  for( int i = 0; i < l; i++ ){
    if( strucGrid[ i ] == 0 ){ continue; }
    std::vector<int> gridTilePos2d = strucDef->to2d( i, strucRot );
    int pos1d = to1d( sx + gridTilePos2d[ 0 ], sy + gridTilePos2d[ 1 ] );
    Tile* tile = tiles[ pos1d ];
    tile->setStructureId( 0 );
    moveMap[ pos1d ] = floorsList::getFloorTypeById( tile->getFloorTypeId() )->getSpeed();
    intMap[ pos1d ] = 1;
  }
  if( testMapOpening() == false ){
    for( int ii = 0; ii < l; ii++ ){
      if( strucGrid[ ii ] == 0 ){ continue; }
      std::vector<int> gridTilePos2d = strucDef->to2d( ii, strucRot );
      int pos1d = to1d( sx + gridTilePos2d[ 0 ], sy + gridTilePos2d[ 1 ] );
      Tile* tile = tiles[ pos1d ];
      tile->setStructureId( _id );
      moveMap[ pos1d ] = -2.0;
      intMap[ pos1d ] = -2;
    }
    return 0;
  }
  return destroyStructById( _id );
}

int GameLevel::destroyStructsByZone( int _startx, int _starty, int _endx, int _endy ){
  int w = width();
  int h = height();
  if( _startx < 0 || _starty < 0 || _endx < 0 || _endy < 0 || _startx > w || _endx > w || _starty > h || _endy > h ){
    return 0;
  }
  int zoneW = _endx - _startx;
  int zoneH = _endy - _starty;
  int nbZoneTiles = zoneW * zoneH;
  std::vector<int> structsIds;
  for( int i = 0; i < nbZoneTiles; i++ ){
    int zoney = std::floor( i / zoneW );
    int zonex = i - zoney * zoneW;
    int sx = _startx + zonex;
    int sy = _starty + zoney;
    int pos1d = to1d( sx, sy );
    Tile* tile = getTile( pos1d );
    int structId = tile->getStructureId();
    if( structId == 0 ){ continue; }
    int sIdsl = structsIds.size();
    bool isPresent = false;
    for( int y = 0; y < sIdsl; y++ ){
      if( structsIds[ y ] == structId ){
        isPresent = true;
        break;
      }
    }
    if( isPresent == true ){ continue; }
    structsIds.push_back( structId );
  }
  int sIl = structsIds.size();
  for( int si = 0; si < sIl; si++ ){
    Structure* strucToRemove = getStructureById( structsIds[ si ] );
    StructureDef* strucDef = structuresDefList::getStructureTypeByName( strucToRemove->getTypeName() );
    int strucRot = strucToRemove->getRotation();
    std::vector<int> strucGrid = strucDef->getGrid( strucRot );
    int l = strucGrid.size();
    int sX = strucToRemove->getX();
    int sY = strucToRemove->getY();
    for( int ii = 0; ii < l; ii++ ){
      if( strucGrid[ ii ] == 0 ){ continue; }
      std::vector<int> gridTilePos2d = strucDef->to2d( ii, strucRot );
      int pos1dB = to1d( sX + gridTilePos2d[ 0 ], sY + gridTilePos2d[ 1 ] );
      Tile* tileB = tiles[ pos1dB ];
      tileB->setStructureId( 0 );
      moveMap[ pos1dB ] = floorsList::getFloorTypeById( tileB->getFloorTypeId() )->getSpeed();
      intMap[ pos1dB ] = 1;
    }
  }
  int nbStructs = structsIds.size();
  if( testMapOpening() == false ){
    for( int iii = 0; iii < nbStructs; iii++ ){
      int structureId = structsIds[ iii ];
      Structure* strucToRestore = getStructureById( structureId );
      int sx = strucToRestore->getX();
      int sy = strucToRestore->getY();
      StructureDef* strucDef = structuresDefList::getStructureTypeByName( strucToRestore->getTypeName() );
      int strucRot = strucToRestore->getRotation();
      std::vector<int> strucGrid = strucDef->getGrid( strucRot );
      int l = strucGrid.size();
      for( int j = 0; j < l; j++ ){
        if( strucGrid[ j ] == 0 ){ continue; }
        std::vector<int> gridTilePos2d = strucDef->to2d( j, strucRot );
        int pos1d = to1d( sx + gridTilePos2d[ 0 ], sy + gridTilePos2d[ 1 ] );
        Tile* tile = tiles[ pos1d ];
        tile->setStructureId( structureId );
        moveMap[ pos1d ] = -2.0;
        intMap[ pos1d ] = -2;
      }
    }
    return 0;
  }
  for( int jj = 0; jj < nbStructs; jj++ ){
    //int destroRet = destroyStructById( structsIds[ jj ] );
    destroyStructById( structsIds[ jj ] );
  }
  return 1;
}

int GameLevel::getStructureIdByPosition( int _x, int _y ){
  if( _x >= 0 && _y >= 0 && _x <  width() && _y < height() ){
    return getTileByPosition( _x, _y )->getStructureId();
  }
  return 0;
}

std::vector<float> GameLevel::pathMap( int _startx, int _starty){
  std::vector<float> ret( getSize(), -1.0 );
  std::vector<int> posList{ _startx, _starty };
  int basePos = to1d( _startx, _starty );
  ret[ basePos ] = 0;
  int w = width() - 1;
  int h = height() - 1;
  for( int n = 0; n > -1; n++ ){
    std::vector<int> newList;
    int l = posList.size() / 2;
    for( int i = 0; i < l; i++ ){
      int& x = posList[ i * 2 ];
      int& y = posList[ i * 2 + 1 ];
      float& val = ret[ to1d( x, y ) ];
      if( x > 0 ){
        int left = to1d( x - 1, y );
        float& leftVal = ret[ left ];
        float testVal = val + moveMap[ left ];
        if( intMap[ left ] > 0 && ( leftVal == -1.0 || testVal < leftVal ) ){
          ret[ left ] = testVal;
          newList.push_back( x - 1 );
          newList.push_back( y );
        }
      }
      if( x < w ){
        int right = to1d( x + 1, y );
        float& rightVal = ret[ right ];
        float testVal = val + moveMap[ right ];
        if( intMap[ right ] > 0 && ( rightVal == -1.0 || testVal < rightVal ) ){
          ret[ right ] = testVal;
          newList.push_back( x + 1 );
          newList.push_back( y );
        }
      }
      if( y > 0 ){
        int top = to1d( x, y - 1 );
        float& topVal = ret[ top ];
        float testVal = val + moveMap[ top ];
        if( intMap[ top ] > 0 && ( topVal == -1.0 || testVal < topVal ) ){
          ret[ top ] = testVal;
          newList.push_back( x );
          newList.push_back( y - 1 );
        }
      }
      if( y < h ){
        int bottom = to1d( x, y + 1 );
        float& bottomVal = ret[ bottom ];
        float testVal = val + moveMap[ bottom ];
        if( intMap[ bottom ] > 0 && ( bottomVal == -1.0 || testVal < bottomVal ) ){
          ret[ bottom ] = testVal;
          newList.push_back( x );
          newList.push_back( y + 1 );
        }
      }
    }
    if( newList.size() == 0 ){
      break;
    }else{
      posList = newList;
    }
  }
  return ret;
}

std::vector<char> GameLevel::pathMapChar( int _startx, int _starty){
  std::vector<char> ret ( getSize(), -1.0 );
  std::vector<int> posList{ _startx, _starty };
  int basePos = to1d( _startx, _starty );
  ret[ basePos ] = (char) 0;
  int w = width() - 1;
  int h = height() - 1;
  for( int n = 0; n > -1; n++ ){
    std::vector<int> newList;
    int l = posList.size() / 2;
    for( int i = 0; i < l; i++ ){
      int& x = posList[ i * 2 ];
      int& y = posList[ i * 2 + 1 ];
      float val = (float) ret[ to1d( x, y ) ];
      if( x > 0 ){
        int left = to1d( x - 1, y );
        float leftVal = (float) ret[ left ];
        float testVal = val + moveMap[ left ];
        if( intMap[ left ] > 0 && ( leftVal == -1.0 || testVal < leftVal ) ){
          ret[ left ] = (char) testVal;
          newList.push_back( x - 1 );
          newList.push_back( y );
        }
      }
      if( x < w ){
        int right = to1d( x + 1, y );
        float rightVal = (float) ret[ right ];
        float testVal = val + moveMap[ right ];
        if( intMap[ right ] > 0 && ( rightVal == -1.0 || testVal < rightVal ) ){
          ret[ right ] = (char) testVal;
          newList.push_back( x + 1 );
          newList.push_back( y );
        }
      }
      if( y > 0 ){
        int top = to1d( x, y - 1 );
        float topVal = (float) ret[ top ];
        float testVal = val + moveMap[ top ];
        if( intMap[ top ] > 0 && ( topVal == -1.0 || testVal < topVal ) ){
          ret[ top ] = (char) testVal;
          newList.push_back( x );
          newList.push_back( y - 1 );
        }
      }
      if( y < h ){
        int bottom = to1d( x, y + 1 );
        float bottomVal = (float) ret[ bottom ];
        float testVal = val + moveMap[ bottom ];
        if( intMap[ bottom ] > 0 && ( bottomVal == -1.0 || testVal < bottomVal ) ){
          ret[ bottom ] = (char) testVal;
          newList.push_back( x );
          newList.push_back( y + 1 );
        }
      }
    }
    if( newList.size() == 0 ){
      break;
    }else{
      posList = newList;
    }
  }
  return ret;
}

std::vector<int> GameLevel::thetaStar( int _startx, int _starty){
  std::vector<int> ret( getSize(), -1 );
  /*
  int basePos = to1d( _startx, _starty );
  ret[ basePos ] = -1;
  int w = width() - 1;
  int h = height() - 1;
  std::vector<int> posList;
  if( _startx > 0 ){
    if( intMap[ to1d( _startx - 1, _starty ) ] > 0 ){ posList.push_back( basePos );
                                                      posList.push_back( _startx - 1 );
                                                      posList.push_back( _starty ); }
    if( _starty > 0 && intMap[ to1d( _startx - 1, _starty - 1 ) ] > 0 ){  posList.push_back( basePos );
                                                                          posList.push_back( _startx - 1 );
                                                                          posList.push_back( _starty - 1 ); }
    if( _starty < h - 1 && intMap[ to1d( _startx - 1, _starty + 1 ) ] > 0  ){ posList.push_back( basePos );
                                                                              posList.push_back( _startx - 1 );
                                                                              posList.push_back( _starty + 1 ); }
  }
  if( _startx < w - 1 ){
    if( intMap[ to1d( _startx + 1, _starty ) ] > 0 ){ posList.push_back( basePos );
                                                      posList.push_back( _startx + 1 );
                                                      posList.push_back( _starty ); }
    if( _starty > 0 && intMap[ to1d( _startx + 1, _starty - 1 ) ] > 0 ){  posList.push_back( basePos );
                                                                          posList.push_back( _startx + 1 );
                                                                          posList.push_back( _starty - 1 ); }
    if( _starty < h - 1 && intMap[ to1d( _startx + 1, _starty + 1 ) ] > 0  ){ posList.push_back( basePos );
                                                                              posList.push_back( _startx + 1 );
                                                                              posList.push_back( _starty + 1 ); }
  }
  if( _starty > 0 && intMap[ to1d( _startx, _starty - 1 ) ] > 0  ){ posList.push_back( basePos );
                                                                    posList.push_back( _startx );
                                                                    posList.push_back( _starty - 1 ); }
  if( _starty < h - 1 && intMap[ to1d( _startx, _starty + 1 ) ] > 0  ){ posList.push_back( basePos );
                                                                        posList.push_back( _startx );
                                                                        posList.push_back( _starty + 1 ); }

  for( int n = 0; n != -1 ){
    std::vector<int> newList;
    int l = posList.size() / 3;
    for( int i = 0; i < l; i++ ){

    }
  }
  */
  return ret;
}

bool GameLevel::lineOfSight( int x0, int y0, int x1, int y1 ){
  int diffx = x1 - x0;
  int diffy = y1 - y0;
  int f = 0;
  int dirx;
  int diry;
  int offsetx;
  int offsety;
  if(diffy < 0){
    diffy = -diffy;
    diry = -1;
    offsety = 0; // Cell is to the North
  }else{
    diry = 1;
    offsety = 1; // Cell is to the South
  }

  if(diffx < 0){
    diffx = -diffx;
    dirx = -1;
    offsetx = 0; // Cell is to the West
  }else{
    dirx = 1;
    offsetx = 1; // Cell is to the East
  }

  if(diffx >= diffy){ // Move along the x axis and increment/decrement y when f >= diff.x.
    while(x0 != x1){
      f += diffy;
      if(f >= diffx){  // We are changing rows, we might need to check two cells this iteration.
        //if (!mIsTraversable(l1 + offset)){
        //if ( intMap[ pos1d(  ) ] < 0 ){
        if ( intMap[ to1d( x0 + offsetx, y0 + offsety ) ] < 0 ){
          return false;
        }
        y0 += diry;
        f -= diffx;
      }
      //if(f != 0 && !mIsTraversable(l1 + offset)){
      if( f == 0 && (
            ( intMap[ to1d( x0 , y0 ) ] < 0 && intMap[ to1d( x0 - 1, y0 - 1 ) ] < 0 )
        ||  ( intMap[ to1d( x0 - 1, y0 ) ] < 0 && intMap[ to1d( x0 , y0 - 1 ) ] < 0 )
      ) ){
        return false;
      }



      if(f != 0 && intMap[ to1d( x0 + offsetx, y0 + offsety ) ] < 0 ){
        return false;
      }
      // If we are moving along a horizontal line, either the north or the south cell should be unblocked.
      //if (diff.y == 0 && !mIsTraversable({x0 + offsetx, l1.y}) && !mIsTraversable({l1.x + offsetx, l1.y + 1})){
      if (diffy == 0 && intMap[ to1d( x0 + offsetx, y0 ) ] < 0 && intMap[ to1d( x0 + offsetx, y0 + 1 ) ] < 0){
        return false;
      }
      x0 += dirx;
    }
  }else{  //if (diff.x < diff.y). Move along the y axis and increment/decrement x when f >= diff.y.
    while (y0 != y1){
      f += diffx;
      if(f >= diffy){
        if(intMap[ to1d( x0 + offsetx, y0 + offsety ) ] < 0){
          return false;
        }
        x0 += dirx;
        f -= diffy;
      }

      if( f == 0 && (
            ( intMap[ to1d( x0 , y0 ) ] < 0 && intMap[ to1d( x0 - 1, y0 - 1 ) ] < 0 )
        ||  ( intMap[ to1d( x0 - 1, y0 ) ] < 0 && intMap[ to1d( x0 , y0 - 1 ) ] < 0 )
      ) ){
        return false;
      }

      if(f != 0 && intMap[ to1d( x0 + offsetx, y0 + offsety ) ] < 0){
        return false;
      }
      //if (diffx == 0 && !mIsTraversable({l1.x, l1.y + offsety}) && !mIsTraversable({l1.x + 1, l1.y + offsety})){
      if (diffx == 0 && intMap[ to1d( x0, y0 + offsety ) ] < 0 && intMap[ to1d( x0 + 1, y0 + offsety ) ] < 0 ){
        return false;
      }
      y0 += diry;
    }
  }
  return true;
}

std::vector<int> GameLevel::lineOfSight4View( int x0, int y0, int x1, int y1 ){

  std::vector<int> ret;

  int diffx = x1 - x0;
  int diffy = y1 - y0;
  int f = 0;
  int dirx;
  int diry;
  int offsetx;
  int offsety;
  if(diffy < 0){
    diffy = -diffy;
    diry = -1;
    offsety = -1; // Cell is to the North
  }else{
    diry = 1;
    offsety = 1; // Cell is to the South
  }

  if(diffx < 0){
    diffx = -diffx;
    dirx = -1;
    offsetx = -1; // Cell is to the West
  }else{
    dirx = 1;
    offsetx = 1; // Cell is to the East
  }

  if(diffx >= diffy){ // Move along the x axis and increment/decrement y when f >= diff.x.
    while(x0 != x1){
      f += diffy;
      if(f >= diffx){  // We are changing rows, we might need to check two cells this iteration.
        //if (!mIsTraversable(l1 + offset)){
        //if ( intMap[ pos1d(  ) ] < 0 ){
        ret.push_back( x0 + offsetx );
        ret.push_back( y0 + offsety );
        if ( intMap[ to1d( x0 + offsetx, y0 + offsety ) ] < 0 ){
          return ret;
        }
        y0 += diry;
        f -= diffx;
      }
      //if(f != 0 && !mIsTraversable(l1 + offset)){

      if( f == 0 && (
            ( intMap[ to1d( x0 , y0 ) ] < 0 && intMap[ to1d( x0 - 1, y0 - 1 ) ] < 0 )
        ||  ( intMap[ to1d( x0 - 1, y0 ) ] < 0 && intMap[ to1d( x0 , y0 - 1 ) ] < 0 )
      ) ){
        ret.push_back( x0 );
        ret.push_back( y0 );
        ret.push_back( x0 - 1 );
        ret.push_back( y0 - 1 );

        ret.push_back( x0 - 1 );
        ret.push_back( y0 );
        ret.push_back( x0 );
        ret.push_back( y0 - 1 );
        return ret;
      }else if( f == 0 ){
        ret.push_back( x0 );
        ret.push_back( y0 );
        ret.push_back( x0 - 1 );
        ret.push_back( y0 - 1 );

        ret.push_back( x0 - 1 );
        ret.push_back( y0 );
        ret.push_back( x0 );
        ret.push_back( y0 - 1 );
      }


      if(f != 0 && intMap[ to1d( x0 + offsetx, y0 + offsety ) ] < 0 ){
        ret.push_back( x0 + offsetx );
        ret.push_back( y0 + offsety );
        return ret;
      }else if( f != 0 ){
        ret.push_back( x0 + offsetx );
        ret.push_back( y0 + offsety );
      }
      // If we are moving along a horizontal line, either the north or the south cell should be unblocked.
      //if (diff.y == 0 && !mIsTraversable({x0 + offsetx, l1.y}) && !mIsTraversable({l1.x + offsetx, l1.y + 1})){
      if (diffy == 0 && intMap[ to1d( x0 + offsetx, y0 ) ] < 0 && intMap[ to1d( x0 + offsetx, y0 + 1 ) ] < 0){
        ret.push_back(  x0 + offsetx );
        ret.push_back( y0 );
        ret.push_back( x0 + offsetx );
        ret.push_back( y0 + 1 );
        return ret;
      }else if( diffy == 0 ){
        ret.push_back(  x0 + offsetx );
        ret.push_back( y0 );
        ret.push_back( x0 + offsetx );
        ret.push_back( y0 + 1 );
      }
      x0 += dirx;
    }
  }else{  //if (diff.x < diff.y). Move along the y axis and increment/decrement x when f >= diff.y.
    while (y0 != y1){
      f += diffx;
      if(f >= diffy){
        ret.push_back( x0 + offsetx );
        ret.push_back( y0 + offsety );
        if(intMap[ to1d( x0 + offsetx, y0 + offsety ) ] < 0){
          return ret;
        }
        x0 += dirx;
        f -= diffy;
      }

      if( f == 0 && (
            ( intMap[ to1d( x0 , y0 ) ] < 0 && intMap[ to1d( x0 - 1, y0 - 1 ) ] < 0 )
        ||  ( intMap[ to1d( x0 - 1, y0 ) ] < 0 && intMap[ to1d( x0 , y0 - 1 ) ] < 0 )
      ) ){
        ret.push_back( x0 );
        ret.push_back( y0 );
        ret.push_back( x0 - 1 );
        ret.push_back( y0 - 1 );

        ret.push_back( x0 - 1 );
        ret.push_back( y0 );
        ret.push_back( x0 );
        ret.push_back( y0 - 1 );
        return ret;
      }else if ( f== 0 ){
        ret.push_back( x0 );
        ret.push_back( y0 );
        ret.push_back( x0 - 1 );
        ret.push_back( y0 - 1 );

        ret.push_back( x0 - 1 );
        ret.push_back( y0 );
        ret.push_back( x0 );
        ret.push_back( y0 - 1 );
      }

      if(f != 0 && intMap[ to1d( x0 + offsetx, y0 + offsety ) ] < 0){
        ret.push_back( x0 + offsetx );
        ret.push_back( y0 + offsety );
        return ret;
      }else if ( f != 0 ){
        ret.push_back( x0 + offsetx );
        ret.push_back( y0 + offsety );
      }
      //if (diffx == 0 && !mIsTraversable({l1.x, l1.y + offsety}) && !mIsTraversable({l1.x + 1, l1.y + offsety})){
      if (diffx == 0 && intMap[ to1d( x0, y0 + offsety ) ] < 0 && intMap[ to1d( x0 + 1, y0 + offsety ) ] < 0 ){
        ret.push_back( x0 );
        ret.push_back( y0 + offsety );
        ret.push_back( x0 + 1 );
        ret.push_back( y0 + offsety );
        return ret;
      }else if( diffx == 0 ){
        ret.push_back( x0 );
        ret.push_back( y0 + offsety );
        ret.push_back( x0 + 1 );
        ret.push_back( y0 + offsety );
      }
      y0 += diry;
    }
  }
  return ret;
}

bool GameLevel::isTraversable( int _x, int _y ){
  if( _y < 0 ){
    if( intMap[ _x ] < 0 ){
      return false;
    }
    return true;
  }
  if( intMap[ to1d( _x, _y ) ] < 0 ){
    return false;
  }
  return true;
};
