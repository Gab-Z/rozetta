#include "Level.h"

template <>
void Level<std::vector<Tile*>>::initSize( int _size ){
  tiles.reserve( _size );
}

/*
template<typename T>
Level<T>::GridPos::GridPos(){};
template<typename T>
Level<T>::GridPos::GridPos( int _x, int _y, int _pos1d ):pos( Vec2<int>( _x, _y ) ), idx( _pos1d ){};
template<typename T>
Level<T>::GridPos::GridPos( int _x, int _y ):pos( Vec2<int>( _x, _y ) ), idx( to1d( _x, _y ) ){};
template<typename T>
Level<T>::GridPos::GridPos( Vec2<int> _pos ):pos( _pos ), idx( to1d( _pos ) ){};
template<typename T>
Level<T>::GridPos::GridPos( int _idx ):idx( _idx ),pos( to2d( _idx ) ){};
template<typename T>
int Level<T>::GridPos::x(){ return pos.x; };
template<typename T>
int Level<T>::GridPos::y(){ return pos.y; };
*/

/*
template<typename T>
int Level<T>::width(){
  return mapWidth;
}

template<typename T>
int Level<T>::height(){
  return mapHeight;
}

template<typename T>
int Level<T>::to1d( int _x, int _y ){
  int pos = _y * mapWidth + _x;
  return pos;
}

template<typename T>
std::vector<int> Level<T>::to2d( int _idx ){
  int _y = std::floor( _idx / mapWidth );
  int _x = _idx - (_y * mapWidth );
  std::vector<int> pos { _x, _y };
  return pos;
}

template<typename T>
int Level<T>::getSize(){
  return size;
}

template<typename T>
void Level<T>::pushStructure( Structure* _structure ){
  structures.push_back( _structure );
}

template<typename T>
int Level<T>::structuresSize(){
  return structures.size();
}

template<typename T>
Structure* Level<T>::getStructure( int _i ){
  return structures[ _i ];
}

template<typename T>
Structure* Level<T>::getStructureById( int _id ){
  int l = structures.size();
  for( int i = 0; i < l; i++ ){
    if( structures[ i ]->getId() == _id ){
      return structures[ i ];
    }
  }
  return structures[ 0 ];
}

template<typename T>
Structure*& Level<T>::getStructureRefById( int _id ){
  int l = structures.size();
  for( int i = 0; i < l; i++ ){
    if( structures[ i ]->getId() == _id ){
      return structures[ i ];
    }
  }
  return structures[ 0 ];
}

template<typename T>
std::vector<int> Level<T>::getStartByIndex( int _i ){
  int l = startPoints.size() / 2;
  if( _i < 0 || _i >= l ){
    return std::vector<int> { startPoints[ 0 ], startPoints[ 1 ] };
  }else{
    return std::vector<int> { startPoints[ _i * 2 ], startPoints[ _i * 2 + 1 ] };
  }
}

template<typename T>
v8::Local<v8::Object> Level<T>::getWays(){
  v8::Local<v8::Object> ret = Nan::New<v8::Object>();

  v8::Local<v8::String> startProp = Nan::New("startPoints").ToLocalChecked();
  v8::Local<v8::Array> startArr = converter::vectorIntToJsArray( startPoints );
  ret->Set( startProp, startArr );

  v8::Local<v8::String> endProp = Nan::New("endPoints").ToLocalChecked();
  v8::Local<v8::Array> endArr = converter::vectorIntToJsArray( endPoints );
  ret->Set( endProp, endArr );

  return ret;
}

template<typename T>
bool Level<T>::isPointOnStructureById( int _id, int _x, int _y ){
  return getStructureById( _id )->testPoint( _x, _y );
}

template<typename T>
int Level<T>::destroyStructById( int _id ){
  std::vector<Structure*>::iterator searchedIterator;
  //Structure* searchedStruct;
  bool found = false;
  for( std::vector<Structure*>::iterator i = structures.begin(); i < structures.end(); ++i ){
    if( (*i)->getId() == _id ){
      //searchedStruct = (*i);
      searchedIterator = i;
      found = true;
      break;
    }
  }
  if( found == true ){
    //delete searchedStruct;
    Structure*& dl = getStructureRefById( _id );
    delete dl;
    structures.erase( searchedIterator );
    return 1;
  }
  return 0;
}

template<typename T>
v8::Local<v8::Array> Level<T>::getStructureUpgradesByTypeName( std::string _typeName ){
  //return structuresDefList::getStructureTypeByName( _typeName )->getUpgrades();
  v8::Local<v8::Array> ret = Nan::New<v8::Array>();
  StructureDef* baseDef =  structuresDefList::getStructureTypeByName( _typeName );
  std::vector<std::string> upgradeList = baseDef->getUpgradeList();
  int l = upgradeList.size();
  for( int i = 0; i < l; i++ ){
    std::string upgradeTypeName = upgradeList[ i ];
    v8::Local<v8::Object> upgradeData = structuresDefList::getStructureTypeByName( upgradeTypeName )->getUpgradeData();
    ret->Set( i, upgradeData );
  }
  return ret;
}

template<typename T>
bool Level<T>::upgradeStructure( int _id, std::string _typeName ){
  getStructureById( _id )->setStructureDef( structuresDefList::getStructureTypeByName( _typeName ) );
  return true;
}

/////////////////////////////////

template<typename T>
v8::Local<v8::Array> Level<T>::getTilesArray(){
  int _size = getSize();
  v8::Local<v8::Array> ret = Nan::New<v8::Array>( _size );
  for( int i = 0; i < _size; i++ ){
    v8::Local<v8::Object> v8Tile = tiles[ i ]->toObj();
    ret->Set( i, v8Tile );
  }
  return ret;
}

template<typename T>
bool Level<T>::testStructurePos( int _x, int _y, std::string _typeName ){
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
    if(  tx < 0 || tx >= mapWidth || ty < 0 || ty >= mapHeight || ! tiles[ coord1d ]->isTraversable() ){
      return false;
    }
  }
  return true;
}

template<typename T>
std::vector<bool> Level<T>::testMultipleStructurePos( std::vector<int> _positions, std::string _typeName, int _rotation ){
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

template<typename T>
bool Level<T>::addStructures( std::vector<int> _positions, std::string _typeName, int _rotation ){
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
    //int structId = newStruct->getId();
    for( int ii = 0; ii < nbTiles; ii++ ){
      int tileX = px + strucDefPos[ ii * 2 ];
      int tileY = py + strucDefPos[ ii * 2 + 1 ];
      int pos1d = to1d( tileX, tileY );
      //tiles[ pos1d ]->setStructureId( structId );
      tiles[ pos1d ]->setStructure( newStruct );
    }
    pushStructure( newStruct );
  }
  updateAllTethaPaths();
  return true;
}

template<typename T>
v8::Local<v8::Array> Level<T>::getStructures(){
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

template<typename T>
Tile* Level<T>::getTile( int _i ){
  return tiles[ _i ];
}

template<typename T>
Tile* Level<T>::getTileByPosition( int _x, int _y ){
  if( _x >= 0 && _y >= 0 && _x <  width() && _y < height() ){
    return tiles[ to1d( _x, _y ) ];
  }
  return new Tile();
}

template<typename T>
v8::Local<v8::Array> Level<T>::getStructureGrid( std::string _typeName, int _rotation ){
  StructureDef* structDef = structuresDefList::getStructureTypeByName( _typeName );
  std::vector<int> grid = structDef->getGrid( _rotation );
  v8::Local<v8::Array> ret = converter::vectorIntToJsArray( grid );
  return ret;
}

template<typename T>
bool Level<T>::newStructuresBlockingTest( std::vector<int> _positions, std::vector<int> _strucDefPositions ){
  std::vector<std::vector<int>> poses = { _positions };
  std::vector<std::vector<int>> structuresPos = { _strucDefPositions };
  return newStructuresBlockingTest( poses, structuresPos );
}

template<typename T>
bool Level<T>::newStructuresBlockingTest( std::vector<std::vector<int>> _positions, std::vector<std::vector<int>> _strucDefPositions ){
  std::vector<char> testMap = getCharMap();

  int nbStructDefs = _positions.size();

  for( int n = 0; n < nbStructDefs; n++ ){
    std::vector<int>& posits = _positions[ n ];
    std::vector<int>& strDef = _strucDefPositions[ n ];
    int l = posits.size() / 2;
    int nbTiles = strDef.size() / 2;
    for( int i = 0; i < l; i++ ){
      int px = posits[ i * 2 ];
      int py = posits[ i * 2 + 1 ];
      for( int ii = 0; ii < nbTiles; ii++ ){
          testMap[ to1d( px + strDef[ ii * 2 ], py + strDef[ ii * 2 + 1 ] ) ] = -1;
      }
    }
  }
  return testCharMapOpening( testMap );

}

template<typename T>
bool Level<T>::testCharMapOpening( std::vector<char> testMap ){
  int l = getSize();
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
            testMap[ leftPos ] = 1;
            newList.push_back( px - 1 );
            newList.push_back( py );
        }
      }

      if( px < mw - 1 ){
        int rightPos = to1d( px + 1, py );
        if( testMap[ rightPos ] == 0 ){
            testMap[ rightPos ] = 1;
            newList.push_back( px + 1 );
            newList.push_back( py );
        }
      }

      if( py > 0 ){
        int topPos = to1d( px, py - 1 );
        if( testMap[ topPos ] == 0 ){
          //int intMapVal = intMap[ topPos ];
            testMap[ topPos ] = 1;
            newList.push_back( px );
            newList.push_back( py - 1 );
        }
      }

      if( py < mh - 1){
        int bottomPos = to1d( px, py + 1 );
        if( testMap[ bottomPos ] == 0 ){
          //int intMapVal = intMap[ bottomPos ];
            testMap[ bottomPos ] = 1;
            newList.push_back( px );
            newList.push_back( py + 1 );
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
  for ( int n = 0; n < l; n++ ){
    if( testMap[ n ] == 0 ){
      return false;
    }
  }
  return true;
}

template<typename T>
bool Level<T>::removeStructuresBlockingTest( std::vector<int> _positions ){
  std::vector<char> charMap = getCharMap();
  int pl = _positions.size() / 2;
  for( int i = 0; i < pl; i++ ){
    charMap[ to1d( _positions[ i * 2 ], _positions[ i * 2 + 1 ] ) ] = 0;
  }
  return testCharMapOpening( charMap );
}

template<typename T>
int Level<T>::removeStructById( int _id ){
  Structure* strucToRemove = getStructureById( _id );
  std::vector<int> gridPositions = strucToRemove->getPositions();
  if( removeStructuresBlockingTest( gridPositions ) == false ){
    return 0;
  }

  int nbTiles = gridPositions.size() / 2;
  for( int s = 0; s < nbTiles; s++ ){
    tiles[ to1d( gridPositions[ s * 2 ], gridPositions[ s * 2 + 1 ] ) ]->setStructure( nullptr );
  }
  int destroyedStructId = destroyStructById( _id );
  updateAllTethaPaths();
  return destroyedStructId;

}

template<typename T>
std::vector<char> Level<T>::getCharMap(){
  int l = getSize();
  std::vector<char> ret( l, 0 );
  for( int i = 0; i < l; i ++ ){
    Tile* tile = tiles[ i ];
    if( tile->getStructure() ){
      ret[ i ] = -1;
    }
  }
  return ret;
}

template<typename T>
int Level<T>::destroyStructsByZone( int _startx, int _starty, int _endx, int _endy ){
  int w = width();
  int h = height();
  if( _startx < 0 || _starty < 0 || _endx < 0 || _endy < 0 || _startx > w || _endx > w || _starty > h || _endy > h ){
    return 0;
  }
  int zoneW = _endx - _startx;
  int zoneH = _endy - _starty;
  int nbZoneTiles = zoneW * zoneH;
  std::vector<int> structsIds;
  std::vector<int> structuresPos;
  for( int i = 0; i < nbZoneTiles; i++ ){
    int zoney = std::floor( i / zoneW );
    int zonex = i - zoney * zoneW;
    int sx = _startx + zonex;
    int sy = _starty + zoney;
    int pos1d = to1d( sx, sy );
    Tile* tile = tiles[ pos1d ];
    Structure* tileStruct = tile->getStructure();
    int structId;
    if( ! tileStruct ){ continue; }
    structId = tileStruct->getId();
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
    std::vector<int> tileStructPoses = tileStruct->getPositions();
    structuresPos.insert( structuresPos.end(), tileStructPoses.begin(), tileStructPoses.end() );
  }
  if( ! removeStructuresBlockingTest( structuresPos ) ){ return 0; }
  int nbT = structuresPos.size() / 2;
  for( int s = 0; s < nbT; s++ ){
    tiles[ to1d( structuresPos[ s * 2 ], structuresPos[ s * 2 + 1 ] ) ]->setStructure( nullptr );
  }

  int sIl = structsIds.size();
  for( int jj = 0; jj < sIl; jj++ ){
    //int destroRet = destroyStructById( structsIds[ jj ] );
    destroyStructById( structsIds[ jj ] );
  }
  updateAllTethaPaths();
  return 1;
}

template<typename T>
int Level<T>::getStructureIdByPosition( int _x, int _y ){
  if( _x >= 0 && _y >= 0 && _x <  width() && _y < height() ){
    return getTileByPosition( _x, _y )->getStructureId();
  }
  return 0;
}

template<typename T>
std::vector<float> Level<T>::pathMap( int _startx, int _starty){
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

template<typename T>
std::vector<char> Level<T>::pathMapChar( int _startx, int _starty){
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

template<typename T>
float Level<T>::getTileSpeed( int _x, int _y ){
  if( _y < 0 ){
    return tiles[ _x ]->getSpeed();
  }else{
    return tiles[ to1d( _x, _y ) ]->getSpeed();
  }
}

template<typename T>
void Level<T>::addDestinationPoint( int _x, int _y ){
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

template<typename T>
void Level<T>::removeDestinationPoint( int _x, int _y ){
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

template<typename T>
DestinationPt& Level<T>::getOrAddDestinationPt( int _x, int _y ){
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

template<typename T>
DestinationPt& Level<T>::getDestinationPt( int _x, int _y ){
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

template<typename T>
float Level<T>::lineSight( int x0, int y0, int x1, int y1 ){
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

float distance( int x0, int y0, int x1, int y1 ){
  int dx = x1 - x0;
  int dy = y1 - y0;
  return (float) sqrt( dx * dx + dy * dy );
}

template<typename T>
void Level<T>::tethaCheck( int tx, int ty, TethaSearchTile& t, int &neighbx, int &neighby, int &parentx, int &parenty, float &parentVal, float &nv, float hDist, std::vector<int> &newList ){
  //TethaSearchTile& t = retMap[ to1d( tx, ty ) ];
  float distByNeighbour = hDist * tiles[ to1d( neighbx, neighby ) ]->getSpeed() + nv;
  //float distByNeighbour = hDist * t.getSpeed() + nv;
  float sightDistToParent = lineSight( tx , ty, parentx, parenty );
  float distByParent = sightDistToParent * distance( tx, ty, parentx, parenty ) + parentVal;
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

template<typename T>
void Level<T>::tethaSearch( int _startx, int _starty ){
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

template<typename T>
void Level<T>::updateAllTethaPaths(){
  int nbDestPts = destinationPoints.size();
  if( nbDestPts == 0 ){ return void(); };
  for( int i = 0; i < nbDestPts; i++ ){
    std::vector<int> DestPtPos = to2d( destinationPoints[ i ].pos1d );
    tethaSearch( DestPtPos[ 0 ], DestPtPos[ 1 ] );
  }
}

template<typename T>
std::vector<int> Level<T>::getTethaPath( int _startx, int _starty, int _destinationx, int _destinationy ){
  //if( _startx < 0 || _starty < 0 || _startx >=  width() || _starty >= height() ){ return std::vector<int>(); }
  int lastPos1d = to1d( _startx, _starty );
  if( getTile( lastPos1d )->getStructure() ){ return std::vector<int>(); }
  DestinationPt& destPt = getDestinationPt( _destinationx, _destinationy );
  std::vector<int> ret;

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
*/
