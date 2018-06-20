#include "GameLevel.h"

DestinationPt GameLevel::nullDestinationPt = DestinationPt();


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
      addDestinationPoint( ex, ey );
      if( ex == tilePos[ 0 ] && ey == tilePos[ 1 ] ){
        nTile->setWayInOrOut( "end");
      }
    }
    tiles[ i ] = nTile;
  }
  //moveMap = std::vector<float>( _size );
  //intMap = std::vector<int>( _size, 0 );
  //fillMoveMap();
}

GameLevel::GameLevel( int _width, int _height, std::vector<int> _startPts, std::vector<int> _endPts, std::vector<int> _floorIds ):GameLevelBase( _width, _height, _startPts, _endPts ){
  int _size = getSize();
  tiles = std::vector<Tile*> ( _size );
  int sl = _startPts.size() / 2;
  int el = _endPts.size() / 2;
  for( int i = 0; i < _size; i++ ){
    std::vector<int> tilePos = to2d( i );
    Tile* nTile = new Tile( tilePos[ 0 ], tilePos[ 1 ], _floorIds[ i ] );//_floorIds = [  int _floorId... ]
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
      addDestinationPoint( ex, ey );
      if( ex == tilePos[ 0 ] && ey == tilePos[ 1 ] ){
        nTile->setWayInOrOut( "end");
      }
    }
    tiles[ i ] = nTile;
  }
  //moveMap = std::vector<float>( _size );
  //intMap = std::vector<int>( _size, 0 );
  //fillMoveMap();
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

/*
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
  updateAllTethaPaths();
}
*/

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
      if( tx < 0 || tx >= width() || ty < 0 || ty >= height() || ! tiles[ coord1d ]->isBuildable() ){
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

/*
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
*/

//std::vector<int> GameLevel::getIntMap(){ return intMap; };

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
      //moveMap[ pos1d ] = -2.0;
      //intMap[ pos1d ] = -2;
    }
    pushStructure( newStruct );
  }
  updateAllTethaPaths();
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
      //  intMap[ to1d( px + _strucDefPositions[ ii * 2 ], py + _strucDefPositions[ ii * 2 + 1 ] ) ] = -2;
        tiles[ to1d( px + _strucDefPositions[ ii * 2 ], py + _strucDefPositions[ ii * 2 + 1 ] ) ]->setStructureId( 1 );
    }
  }
  bool test = testMapOpening();
  for( int iii = 0; iii < l; iii++ ){
    int px = _positions[ iii * 2 ];
    int py = _positions[ iii * 2 + 1 ];
    for( int iiii = 0; iiii < nbTiles; iiii++ ){
        //intMap[ to1d( px + _strucDefPositions[ iiii * 2 ], py + _strucDefPositions[ iiii * 2 + 1 ] ) ] = 1;
        tiles[ to1d( px + _strucDefPositions[ iiii * 2 ], py + _strucDefPositions[ iiii * 2 + 1 ] ) ]->setStructureId( 0 );
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
          //int intMapVal = intMap[ leftPos ];
          if( tiles[ leftPos ]->getStructureId() == 0 ){
            testMap[ leftPos ] = 1;
            newList.push_back( px - 1 );
            newList.push_back( py );
          }else{
            testMap[ leftPos ] = -1;
          }
        }
      }

      if( px < mw - 1 ){
        int rightPos = to1d( px + 1, py );
        if( testMap[ rightPos ] == 0 ){
          //int intMapVal = intMap[ rightPos ];
          if( tiles[ rightPos]->getStructureId() == 0 ){
            testMap[ rightPos ] = 1;
            newList.push_back( px + 1 );
            newList.push_back( py );
          }else{
            testMap[ rightPos ] = -1;
          }
        }
      }

      if( py > 0 ){
        int topPos = to1d( px, py - 1 );
        if( testMap[ topPos ] == 0 ){
          //int intMapVal = intMap[ topPos ];
          if( tiles[ topPos]->getStructureId() == 0 ){
            testMap[ topPos ] = 1;
            newList.push_back( px );
            newList.push_back( py - 1 );
          }else{
            testMap[ topPos ] = -1;
          }
        }
      }

      if( py < mh - 1){
        int bottomPos = to1d( px, py + 1 );
        if( testMap[ bottomPos ] == 0 ){
          //int intMapVal = intMap[ bottomPos ];
          if( tiles[ bottomPos ]->getStructureId() == 0 ){
            testMap[ bottomPos ] = 1;
            newList.push_back( px );
            newList.push_back( py + 1 );
          }else{
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
    if( testMap[ n ] == 0 && tiles[ n ]->getStructureId() == 0 ){
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
    //moveMap[ pos1d ] = floorsList::getFloorTypeById( tile->getFloorTypeId() )->getSpeed();
    //intMap[ pos1d ] = 1;
  }
  if( testMapOpening() == false ){
    for( int ii = 0; ii < l; ii++ ){
      if( strucGrid[ ii ] == 0 ){ continue; }
      std::vector<int> gridTilePos2d = strucDef->to2d( ii, strucRot );
      int pos1d = to1d( sx + gridTilePos2d[ 0 ], sy + gridTilePos2d[ 1 ] );
      Tile* tile = tiles[ pos1d ];
      tile->setStructureId( _id );
      //moveMap[ pos1d ] = -2.0;
      //intMap[ pos1d ] = -2;
    }
    return 0;
  }
  if( _id == 0 ){
    return 0;
  }else{
    int destroyedStructId = destroyStructById( _id );
    updateAllTethaPaths();
    return destroyedStructId;
  }

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
    Tile* tile = tiles[ pos1d ];
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
      //moveMap[ pos1dB ] = floorsList::getFloorTypeById( tileB->getFloorTypeId() )->getSpeed();
      //intMap[ pos1dB ] = 1;
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
        //moveMap[ pos1d ] = -2.0;
        //intMap[ pos1d ] = -2;
      }
    }
    return 0;
  }
  for( int jj = 0; jj < nbStructs; jj++ ){
    //int destroRet = destroyStructById( structsIds[ jj ] );
    destroyStructById( structsIds[ jj ] );
  }
  updateAllTethaPaths();
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
        Tile* leftTile = tiles[ left ];
        float testVal = val + leftTile->getSpeed();
        if( leftTile->isTraversable() && ( leftVal == -1.0 || testVal < leftVal ) ){
          ret[ left ] = testVal;
          newList.push_back( x - 1 );
          newList.push_back( y );
        }
      }
      if( x < w ){
        int right = to1d( x + 1, y );
        float& rightVal = ret[ right ];
        Tile* rightTile = tiles[ right ];
        float testVal = val + rightTile->getSpeed();
        if( rightTile->isTraversable() && ( rightVal == -1.0 || testVal < rightVal ) ){
          ret[ right ] = testVal;
          newList.push_back( x + 1 );
          newList.push_back( y );
        }
      }
      if( y > 0 ){
        int top = to1d( x, y - 1 );
        float& topVal = ret[ top ];
        Tile* topTile = tiles[ top ];
        float testVal = val + topTile->getSpeed();
        if( topTile->isTraversable() && ( topVal == -1.0 || testVal < topVal ) ){
          ret[ top ] = testVal;
          newList.push_back( x );
          newList.push_back( y - 1 );
        }
      }
      if( y < h ){
        int bottom = to1d( x, y + 1 );
        float& bottomVal = ret[ bottom ];
        Tile* bottomTile = tiles[ bottom ];
        float testVal = val + bottomTile->getSpeed();
        if( bottomTile->isTraversable() && ( bottomVal == -1.0 || testVal < bottomVal ) ){
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
        Tile* leftTile = tiles[ left ];
        float testVal = val + leftTile->getSpeed();
        if( leftTile->isTraversable() && ( leftVal == -1.0 || testVal < leftVal ) ){
          ret[ left ] = (char) testVal;
          newList.push_back( x - 1 );
          newList.push_back( y );
        }
      }
      if( x < w ){
        int right = to1d( x + 1, y );
        float rightVal = (float) ret[ right ];
        Tile* rightTile = tiles[ right ];
        float testVal = val + rightTile->getSpeed();
        if( rightTile->isTraversable() && ( rightVal == -1.0 || testVal < rightVal ) ){
          ret[ right ] = (char) testVal;
          newList.push_back( x + 1 );
          newList.push_back( y );
        }
      }
      if( y > 0 ){
        int top = to1d( x, y - 1 );
        float topVal = (float) ret[ top ];
        Tile* topTile = tiles[ top ];
        float testVal = val + topTile->getSpeed();
        if( topTile->isTraversable() && ( topVal == -1.0 || testVal < topVal ) ){
          ret[ top ] = (char) testVal;
          newList.push_back( x );
          newList.push_back( y - 1 );
        }
      }
      if( y < h ){
        int bottom = to1d( x, y + 1 );
        float bottomVal = (float) ret[ bottom ];
        Tile* bottomTile = tiles[ bottom ];
        float testVal = val + bottomTile->getSpeed();
        if( bottomTile->isTraversable() && ( bottomVal == -1.0 || testVal < bottomVal ) ){
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

/*
bool GameLevel::isTraversable( int _x, int _y ){
  int pos1d;
  if( _y < 0 ){
    pos1d = _x;
  }else{
    pos1d = to1d( _x, _y );
  }
  if( tiles[ pos1d ]->getWayType() != 0 ){
    return true;
  }
  if( intMap[ pos1d ] < 0 ){
    return false;
  }
  return true;
};
*/

float GameLevel::getTileSpeed( int _x, int _y ){
  if( _y < 0 ){
    return moveMap[ _x ];
  }else{
    return moveMap[ to1d( _x, _y ) ];
  }
}

void GameLevel::addDestinationPoint( int _x, int _y ){
  int l = destinationPoints.size();
  int pos1d;
  if( _y < 0 ){
    pos1d = _x;
  }else{
    pos1d = to1d( _x, _y );
  }
  for( int i = 0; i < l; i++ ){
    if( destinationPoints[ i ].pos1d == pos1d ){ return void(); };
  }
  destinationPoints.push_back( DestinationPt( pos1d, getSize() ) );
};

void GameLevel::removeDestinationPoint( int _x, int _y ){
  int pos1d;
  if( _y < 0 ){
    pos1d = _x;
  }else{
    pos1d = to1d( _x, _y );
  }
  std::vector<DestinationPt>::iterator searchedIterator;
  for( std::vector<DestinationPt>::iterator i = destinationPoints.begin(); i < destinationPoints.end(); ++i ){
      if( (*i).pos1d == pos1d ){
        destinationPoints.erase( i );
      }
  }
};

DestinationPt& GameLevel::getOrAddDestinationPt( int _x, int _y ){
  int pos1d = to1d( _x, _y );
  int l = destinationPoints.size();
  for( int i = 0; i < l; i++ ){
    if( destinationPoints[ i ].pos1d == pos1d ){
      return destinationPoints[ i ];
    }
  }
  addDestinationPoint( pos1d, -1 );
  int ll = destinationPoints.size();
  for( int ii = 0; ii < ll; ii++ ){
    if( destinationPoints[ ii ].pos1d == pos1d ){
      return destinationPoints[ ii ];
    }
  }
  DestinationPt& refToNullDestinationPt = nullDestinationPt;
  return refToNullDestinationPt;
}

DestinationPt& GameLevel::getDestinationPt( int _x, int _y ){
  int pos1d = to1d( _x, _y );
  int ll = destinationPoints.size();
  for( int ii = 0; ii < ll; ii++ ){
    if( destinationPoints[ ii ].pos1d == pos1d ){
      return destinationPoints[ ii ];
    }
  }
  DestinationPt& refToNullDestinationPt = nullDestinationPt;
  return refToNullDestinationPt;
}

float GameLevel::lineSight( int x0, int y0, int x1, int y1 ){
  int nx = x1 - x0;
  int ny = y1 - y0;
  int sign_x = 1;
  int sign_y = 1;
  if( nx < 0 ){
    nx = -nx;
    sign_x = -1;
  }
  if( ny < 0 ){
    ny = -ny;
    sign_y = -1;
  }
  int px = x0;
  int py = y0;
  float speedsSum = 0;
  int nbTiles = 0;
  for (int ix = 0, iy = 0; ix < nx || iy < ny;) {
      if ((0.5+(float)ix) / (float)nx == (0.5+(float)iy) / (float)ny) {
          // next step is diagonal

          if( ! tiles[ to1d( px, py + sign_y ) ]->isTraversable() || ! tiles[ to1d( px + sign_x, py ) ]->isTraversable() ) return 0.0;
          px += sign_x;
          py += sign_y;
          ix++;
          iy++;
      } else if ((0.5+(float)ix) / (float)nx < (0.5+(float)iy) / (float)ny) {
          // next step is horizontal
          px += sign_x;
          ix++;
      } else {
          // next step is vertical
          py += sign_y;
          iy++;
      }
      Tile* nextTile = tiles[ to1d( px, py ) ];
      if( nextTile->isTraversable() ){
        nbTiles++;
        speedsSum += nextTile->getSpeed();
      }else{
        return 0.0;
      }
  }
  return speedsSum / (float)nbTiles;
}

float dist( int x0, int y0, int x1, int y1 ){
  int dx = x1 - x0;
  int dy = y1 - y0;
  return (float) sqrt( dx * dx + dy * dy );
}
/*
void GameLevel::tethaCheck( int tx, int ty, std::vector<TethaSearchTile> &retMap, int &neighbx, int &neighby, int &parentx, int &parenty, float &parentVal, float &nv, float hDist, std::vector<int> &newList ){
  TethaSearchTile& t = retMap[ to1d( tx, ty ) ];
  float distByNeighbour = hDist * getTileSpeed( neighbx, neighby ) + nv;
  float sightDistToParent = lineSight( tx , ty, parentx, parenty );
  float distByParent = sightDistToParent * dist( tx, ty, parentx, parenty ) + parentVal;
  bool testToNeighbour = t.hVal > distByNeighbour ? true : false;
  bool testToParent = t.hVal > distByParent ? true : false;
  if( testToParent == true && sightDistToParent > 0.0 ){
    //t.parentPos = to1d( parentx, parenty );
    t.setParentPos( to1d( parentx, parenty ) );
    //t.hVal = distByParent;
    t.setHVal( distByParent );
    newList.push_back( tx );
    newList.push_back( ty );
  }else if( testToNeighbour == true ){
    //t.parentPos = to1d( neighbx, neighby );
    t.setParentPos( to1d( neighbx, neighby ) );
    //t.hVal = distByNeighbour;
    t.setHVal( distByNeighbour );
    newList.push_back( tx );
    newList.push_back( ty );
  }
}
*/
void GameLevel::tethaCheck( int tx, int ty, TethaSearchTile& t, int &neighbx, int &neighby, int &parentx, int &parenty, float &parentVal, float &nv, float hDist, std::vector<int> &newList ){
  //TethaSearchTile& t = retMap[ to1d( tx, ty ) ];
  float distByNeighbour = hDist * tiles[ to1d( neighbx, neighby ) ]->getSpeed() + nv;
  //float distByNeighbour = hDist * t.getSpeed() + nv;
  float sightDistToParent = lineSight( tx , ty, parentx, parenty );
  float distByParent = sightDistToParent * dist( tx, ty, parentx, parenty ) + parentVal;
  bool testToNeighbour = t.hVal > distByNeighbour ? true : false;
  bool testToParent = t.hVal > distByParent ? true : false;
  if( testToParent == true && sightDistToParent > 0.0 ){
    //t.parentPos = to1d( parentx, parenty );
    t.setParentPos( to1d( parentx, parenty ) );
    //t.hVal = distByParent;
    t.setHVal( distByParent );
    newList.push_back( tx );
    newList.push_back( ty );
  }else if( testToNeighbour == true ){
    //t.parentPos = to1d( neighbx, neighby );
    t.setParentPos( to1d( neighbx, neighby ) );
    //t.hVal = distByNeighbour;
    t.setHVal( distByNeighbour );
    newList.push_back( tx );
    newList.push_back( ty );
  }
}
void GameLevel::tethaSearch( int _startx, int _starty ){
  int w = width();
  int h = height();
  int mapL = getSize();
  //std::vector<TethaSearchTile> ret ( mapL, TethaSearchTile( -1, std::numeric_limits<float>::max() ) );
  TethaSearchTile ret[ mapL ];// = { TethaSearchTile( -1, std::numeric_limits<float>::max() ) };
  for( int u = 0; u < mapL; u++ ){ ret[ u ] = TethaSearchTile( -1, std::numeric_limits<float>::max() ); };
  std::vector<int> openList{ _startx, _starty };
  int basePos = to1d( _startx, _starty );
  TethaSearchTile  &baseTile = ret[ basePos ];
  //baseTile.parentPos = to1d( _startx, _starty );
  baseTile.setParentPos( basePos );
  //baseTile.hVal = 0.0;
  baseTile.setHVal( -1.0 );
  for( int q = 0; q!= -1; q+=0 ){
    std::vector<int> newList;
    int l = openList.size() / 2;
    for( int i = 0; i < l; i++ ){
      int& neighbx = openList[ i * 2 ];
      int& neighby = openList[ i * 2 + 1 ];
      TethaSearchTile& neighbour = ret[ to1d( neighbx, neighby ) ];
      float& nv = neighbour.hVal;
      std::vector<int> parentPos = to2d( neighbour.parentPos );
      int& parentx = parentPos[ 0 ];
      int& parenty = parentPos[ 1 ];
      TethaSearchTile& parentTile = ret[ to1d( parentx, parenty ) ];
      float& parentVal = parentTile.hVal;

      if( neighbx > 0 ){
        //if( isTraversable( neighbx - 1, neighby ) ){
        if( tiles[ to1d( neighbx - 1, neighby ) ]->isTraversable() ){
          tethaCheck( neighbx - 1, neighby, ret[ to1d( neighbx - 1, neighby ) ], neighbx, neighby, parentx, parenty, parentVal, nv, 1.0, newList );
        }
        //if( neighby > 0 && isTraversable( neighbx - 1, neighby - 1 ) && ( isTraversable( neighbx - 1, neighby ) && isTraversable( neighbx, neighby - 1 ) ) ){
        if( neighby > 0 && tiles[ to1d( neighbx - 1, neighby - 1 ) ]->isTraversable() && ( tiles[ to1d( neighbx - 1, neighby ) ]->isTraversable() && tiles[ to1d( neighbx, neighby - 1 ) ]->isTraversable() ) ){
          tethaCheck( neighbx - 1, neighby - 1, ret[ to1d( neighbx - 1, neighby - 1 ) ], neighbx, neighby, parentx, parenty, parentVal, nv, 1.414, newList );
        }
        if( neighby < h - 1 && tiles[ to1d( neighbx - 1, neighby + 1 ) ]->isTraversable() && ( tiles[ to1d( neighbx - 1, neighby ) ]->isTraversable() && tiles[ to1d( neighbx, neighby + 1 ) ]->isTraversable() ) ){
          tethaCheck( neighbx - 1, neighby + 1, ret[ to1d( neighbx - 1, neighby + 1 ) ], neighbx, neighby, parentx, parenty, parentVal, nv, 1.414, newList );
        }
      }
      if( neighbx < w - 1 ){
        if( tiles[ to1d( neighbx + 1, neighby ) ]->isTraversable() ){
          tethaCheck( neighbx + 1, neighby, ret[ to1d( neighbx + 1, neighby ) ], neighbx, neighby, parentx, parenty, parentVal, nv, 1.0, newList );
        }
        if( neighby > 0 && tiles[ to1d( neighbx + 1, neighby - 1 ) ]->isTraversable() && ( tiles[ to1d( neighbx + 1, neighby ) ]->isTraversable() && tiles[ to1d( neighbx, neighby - 1 ) ]->isTraversable() ) ){
          tethaCheck( neighbx + 1, neighby - 1, ret[ to1d( neighbx + 1, neighby - 1 ) ], neighbx, neighby, parentx, parenty, parentVal, nv, 1.414, newList );
        }
        if( neighby < h - 1 && tiles[ to1d( neighbx + 1, neighby + 1 ) ]->isTraversable() && ( tiles[ to1d( neighbx + 1, neighby ) ]->isTraversable() && tiles[ to1d( neighbx, neighby + 1 ) ]->isTraversable() ) ){
          tethaCheck( neighbx + 1, neighby + 1, ret[ to1d( neighbx + 1, neighby + 1 ) ], neighbx, neighby, parentx, parenty, parentVal, nv, 1.414, newList );
        }
      }
      if( neighby > 0 && tiles[ to1d( neighbx, neighby - 1 ) ]->isTraversable() ){
        tethaCheck( neighbx, neighby - 1, ret[ to1d( neighbx, neighby - 1 ) ], neighbx, neighby, parentx, parenty, parentVal, nv, 1.0, newList );
      }
      if( neighby < h - 1 && tiles[ to1d( neighbx, neighby + 1 ) ]->isTraversable() ){
        tethaCheck( neighbx, neighby + 1, ret[ to1d( neighbx, neighby + 1 ) ], neighbx, neighby, parentx, parenty, parentVal, nv, 1.0, newList );
      }
    }
    if( newList.size() > 0 ){
      openList = newList;
    }else{
      break;
    }

  }
  //return ret;
  DestinationPt& destPt = getOrAddDestinationPt( _startx, _starty );
  destPt.init( mapL );
  for( int j = 0; j < mapL; j++ ){
    //destPt.pathPoints[ j ] = ret[ j ].parentPos;
    destPt.setPointTarget( j, ret[ j ].parentPos );
  }
}
/*
void GameLevel::tethaSearch( int _startx, int _starty ){
  int w = width();
  int h = height();
  int mapL = getSize();
  std::vector<TethaSearchTile> ret ( mapL, TethaSearchTile( -1, std::numeric_limits<float>::max() ) );
  std::vector<int> openList{ _startx, _starty };
  int basePos = to1d( _startx, _starty );
  TethaSearchTile  &baseTile = ret[ basePos ];
  //baseTile.parentPos = to1d( _startx, _starty );
  baseTile.setParentPos( basePos );
  //baseTile.hVal = 0.0;
  baseTile.setHVal( 0.0 );
  for( int q = 0; q!= -1; q+=0 ){
    std::vector<int> newList;
    int l = openList.size() / 2;
    for( int i = 0; i < l; i++ ){
      int& neighbx = openList[ i * 2 ];
      int& neighby = openList[ i * 2 + 1 ];
      TethaSearchTile& neighbour = ret[ to1d( neighbx, neighby ) ];
      float& nv = neighbour.hVal;
      std::vector<int> parentPos = to2d( neighbour.parentPos );
      int& parentx = parentPos[ 0 ];
      int& parenty = parentPos[ 1 ];
      TethaSearchTile& parentTile = ret[ to1d( parentx, parenty ) ];
      float& parentVal = parentTile.hVal;

      if( neighbx > 0 ){
        if( isTraversable( neighbx - 1, neighby ) ){
          tethaCheck( neighbx - 1, neighby, ret, neighbx, neighby, parentx, parenty, parentVal, nv, 1.0, newList );
        }
        if( neighby > 0 && isTraversable( neighbx - 1, neighby - 1 ) && ( isTraversable( neighbx - 1, neighby ) && isTraversable( neighbx, neighby - 1 ) ) ){
          tethaCheck( neighbx - 1, neighby - 1, ret, neighbx, neighby, parentx, parenty, parentVal, nv, 1.414, newList );
        }
        if( neighby < h - 1 && isTraversable( neighbx - 1, neighby + 1 ) && ( isTraversable( neighbx - 1, neighby ) && isTraversable( neighbx, neighby + 1 ) ) ){
          tethaCheck( neighbx - 1, neighby + 1, ret, neighbx, neighby, parentx, parenty, parentVal, nv, 1.414, newList );
        }
      }
      if( neighbx < w - 1 ){
        if( isTraversable( neighbx + 1, neighby ) ){
          tethaCheck( neighbx + 1, neighby, ret, neighbx, neighby, parentx, parenty, parentVal, nv, 1.0, newList );
        }
        if( neighby > 0 && isTraversable( neighbx + 1, neighby - 1 ) && ( isTraversable( neighbx + 1, neighby ) && isTraversable( neighbx, neighby - 1 ) ) ){
          tethaCheck( neighbx + 1, neighby - 1, ret, neighbx, neighby, parentx, parenty, parentVal, nv, 1.414, newList );
        }
        if( neighby < h - 1 && isTraversable( neighbx + 1, neighby + 1 ) && ( isTraversable( neighbx + 1, neighby ) && isTraversable( neighbx, neighby + 1 ) ) ){
          tethaCheck( neighbx + 1, neighby + 1, ret, neighbx, neighby, parentx, parenty, parentVal, nv, 1.414, newList );
        }
      }
      if( neighby > 0 && isTraversable( neighbx, neighby - 1 ) ){
        tethaCheck( neighbx, neighby - 1, ret, neighbx, neighby, parentx, parenty, parentVal, nv, 1.0, newList );
      }
      if( neighby < h - 1 && isTraversable( neighbx, neighby + 1 ) ){
        tethaCheck( neighbx, neighby + 1, ret, neighbx, neighby, parentx, parenty, parentVal, nv, 1.0, newList );
      }
    }
    if( newList.size() > 0 ){
      openList = newList;
    }else{
      break;
    }

  }
  //return ret;
  DestinationPt& destPt = getOrAddDestinationPt( _startx, _starty );
  destPt.init( mapL );
  for( int j = 0; j < mapL; j++ ){
    //destPt.pathPoints[ j ] = ret[ j ].parentPos;
    destPt.setPointTarget( j, ret[ j ].parentPos );
  }
}
*/
void GameLevel::updateAllTethaPaths(){
  int nbDestPts = destinationPoints.size();
  if( nbDestPts == 0 ){ return void(); };
  for( int i = 0; i < nbDestPts; i++ ){
    std::vector<int> DestPtPos = to2d( destinationPoints[ i ].pos1d );
    tethaSearch( DestPtPos[ 0 ], DestPtPos[ 1 ] );
  }
}

std::vector<int> GameLevel::getTethaPath( int _startx, int _starty, int _destinationx, int _destinationy ){
  DestinationPt& destPt = getDestinationPt( _destinationx, _destinationy );
  std::vector<int> ret;
  int lastPos1d = to1d( _startx, _starty );
  ret.push_back( _startx );
  ret.push_back( _starty );
  std::vector<int>& pathPoints = destPt.pathPoints;
  for( int i = 0; i != 1; i+=0 ){
    int parentPos1d = pathPoints[ lastPos1d ];
    if( parentPos1d == lastPos1d ){
      return ret;
    }
    std::vector<int> parentPos2d = to2d( parentPos1d );
    ret.push_back( parentPos2d[ 0 ] );
    ret.push_back( parentPos2d[ 1 ] );
    lastPos1d = parentPos1d;
  }
  return ret;
}
