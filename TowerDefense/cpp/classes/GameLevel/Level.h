#ifndef DEF_Level
#define DEF_Level

#include <nan.h>
#include <cmath>
#include <string>
#include <limits>
#include "../Floor/floorsList.h"
#include "../Structure/Structure.h"
#include "../Structure/structuresDefList.h"
#include "../Tile/Tile.h"
#include "../converter/converter.h"
#include "DestinationPt.h"
#include "TethaSearchTile.h"
#include "../Character/Character.h"
#include "../Character/characterTypeList.h"
#include "../geometry/Vec2.h"
#include "../geometry/GridPos.h"

template <typename T>

class Level{

  int mapWidth;
  int mapHeight;
  int size;
  std::vector<Vec2<int>> startPoints;
  std::vector<Vec2<int>> endPoints;
  std::vector<Structure*> structures;
  std::vector<Character*> ennemies;
  T tiles;
  std::vector<DestinationPt> destinationPoints;
  DestinationPt nullDestinationPt = DestinationPt();

  public:

    //std::vector<DestinationPt> nullDestPt{ DestinationPt() };

    Level();

    Level( int _width, int _height, std::vector<int> _startPts, std::vector<int> _endPts ){
      init( _width, _height, _startPts, _endPts, std::vector<int>( _width * _height, 1 ) );
    }

    Level( int _width, int _height, std::vector<int> _startPts, std::vector<int> _endPts, std::vector<int> _floorIds ){
      init( _width, _height, _startPts, _endPts, _floorIds );
    }

    void initSize( int _size );

    void init( int _width, int _height, std::vector<int> _startPts, std::vector<int> _endPts, std::vector<int> _floorIds ){
      mapWidth = _width;
      mapHeight = _height;
      startPoints = converter::flatVecToVec2s<int>( _startPts );
      endPoints = converter::flatVecToVec2s<int>( _endPts );
      size = mapWidth * mapHeight;
      if( std::is_same<T, std::vector<Tile*>>::value ){
        initSize( size );
      }
      int sl = startPoints.size();
      int el = endPoints.size();
      for( int i = 0; i < size; i++ ){
        Vec2<int> tilePos = to2d( i );
        Tile* nTile = new Tile( tilePos, _floorIds[ i ] );//_floorIds = [  int _floorId... ]
        for( int si = 0; si < sl; si++ ){
          //int sx = _startPts[ si * 2 ];
          //int sy = _startPts[ si * 2 + 1 ];
          Vec2<int> startPt = startPoints[ i ];
          if( startPt.x == tilePos.x && startPt.y == tilePos.y ){
            nTile->setWayInOrOut( "start");
          }
        }
        for( int ei = 0; ei < el; ei++ ){
          //int ex = _endPts[ ei * 2 ];
          //int ey = _endPts[ ei * 2 + 1 ];
          Vec2<int> endPt = endPoints[ ei ];
          addDestinationPoint( endPt.x, endPt.y );
          if( endPt.x == tilePos.x && endPt.y == tilePos.y ){
            nTile->setWayInOrOut( "end");
          }
        }
        tiles[ i ] = nTile;
      }
      updateAllTethaPaths();

    }

    static v8::Local<v8::Array> getCommonTextures(){
      std::vector<std::string> strings {
        "destroyStructure", "Hammer.png",
        "vue",              "eye.png",
        "lab",              "lab.png",
        "start",            "play-button.png"
      };

      v8::Local<v8::Array> ret = Nan::New<v8::Array>();

      v8::Local<v8::String> nameProp = Nan::New("name").ToLocalChecked();
      v8::Local<v8::String> urlProp = Nan::New("url").ToLocalChecked();

      int l = strings.size() / 2;

      for( int i = 0; i < l; i++ ){
        v8::Local<v8::Object> obj = Nan::New<v8::Object>();
        v8::Local<v8::Value> nameVal = Nan::New( strings[ i * 2 ] ).ToLocalChecked();
        obj->Set( nameProp, nameVal );
        v8::Local<v8::Value> urlVal = Nan::New( std::string("../img/") + strings[ i * 2 + 1 ] ).ToLocalChecked();
        obj->Set( urlProp, urlVal );
        ret->Set( i, obj );
      }

      return ret;

    }

/*



    int width();
    int height();
    int to1d( int _x, int _y );
    std::vector<int> to2d( int _idx );
    int getSize();
    void pushStructure( Structure* _structure );
    int structuresSize();
    Structure* getStructure( int _i );
    Structure* getStructureById( int _id );
    Structure*& getStructureRefById( int _id );
    std::vector<int> getStartByIndex( int _i );
    v8::Local<v8::Object> getWays();
    bool isPointOnStructureById( int _id, int _x, int _y );
    int destroyStructById( int _id );
    v8::Local<v8::Array> getStructureUpgradesByTypeName( std::string _typeName );
    bool upgradeStructure( int _id, std::string _typeName );

    v8::Local<v8::Array> getTilesArray();
    bool testStructurePos( int _x, int _y, std::string _typeName );
    std::vector<bool> testMultipleStructurePos( std::vector<int> _positions, std::string _typeName, int _rotation );
    bool addStructures( std::vector<int> _positions, std::string _typeName, int _rotation );
    v8::Local<v8::Array> getStructures();
    Tile* getTile( int _i );
    Tile* getTileByPosition( int _x, int _y );
    v8::Local<v8::Array> getStructureGrid( std::string _typeName, int _rotation );
    bool newStructuresBlockingTest(  std::vector<int> _positions, std::vector<int> _strucDefPositions );
    bool newStructuresBlockingTest( std::vector<std::vector<int>> _positions, std::vector<std::vector<int>> _strucDefPositions );
    bool removeStructuresBlockingTest( std::vector<int> _positions );
    bool testCharMapOpening( std::vector<char> testMap );
    int removeStructById( int _id );
    int destroyStructsByZone( int _startx, int _starty, int _endx, int _endy );
    int getStructureIdByPosition( int _x, int _y );
    std::vector<float> pathMap( int _startx, int _starty);
    std::vector<char> pathMapChar( int _startx, int _starty);
    std::vector<char> getCharMap();
    float getTileSpeed( int _x, int _y );
    void tethaCheck( int tx, int ty, TethaSearchTile& t, int &neighbx, int &neighby, int &parentx, int &parenty, float &parentVal, float &nv, float hDist, std::vector<int> &newList );
    void tethaSearch( int _startx, int _starty );
    float lineSight( int x0, int y0, int x1, int y1 );
    void updateAllTethaPaths();
    std::vector<int> getTethaPath( int _startx, int _starty, int _destinationx, int _destinationy );
    void addDestinationPoint( int _x, int _y );
    void removeDestinationPoint( int _x, int _y );
    DestinationPt& getOrAddDestinationPt( int _x, int _y );
    DestinationPt& getDestinationPt( int _x, int _y );
*/




int width(){
  return mapWidth;
}

int height(){
  return mapHeight;
}

int to1d( int _x, int _y ){
  return _y * mapWidth + _x;
}

int to1d( Vec2<int> _pos ){
  return _pos.y * mapWidth + _pos.x;
}

Vec2<int> to2d( int _idx ){
  /*
  int _y =  (int) _idx / mapWidth ;
  int _x = _idx % mapWidth;
  std::vector<int> pos { _x, _y };
  return pos;
  */
  return Vec2<int>( _idx % mapWidth, (int) _idx / mapWidth );
}

GridPos gridPos( int _x, int _y ){
  return GridPos( _x, _y, to1d( _x, _y ) );
}

GridPos gridPos( Vec2<int> _vec2 ){
  return GridPos( _vec2.x, _vec2.y, to1d( _vec2 ) );
}

GridPos gridPos( int _idx ){
  return GridPos( _idx % mapWidth, (int) _idx / mapWidth, _idx );
}

int getSize(){
  return size;
}

void pushStructure( Structure* _structure ){
  structures.push_back( _structure );
}

int structuresSize(){
  return structures.size();
}

Structure* getStructure( int _i ){
  return structures[ _i ];
}

Structure* getStructureById( int _id ){
  int l = structures.size();
  for( int i = 0; i < l; i++ ){
    if( structures[ i ]->getId() == _id ){
      return structures[ i ];
    }
  }
  return structures[ 0 ];
}

Structure*& getStructureRefById( int _id ){
  int l = structures.size();
  for( int i = 0; i < l; i++ ){
    if( structures[ i ]->getId() == _id ){
      return structures[ i ];
    }
  }
  return structures[ 0 ];
}

Vec2<int> getStartByIndex( int _i ){
  int l = startPoints.size() / 2;
  if( _i < 0 || _i >= l ){
    return startPoints[ 0 ];
  }else{
    return startPoints[ _i ];
  }
}

v8::Local<v8::Object> getWays(){
  v8::Local<v8::Object> ret = Nan::New<v8::Object>();

  v8::Local<v8::String> startProp = Nan::New("startPoints").ToLocalChecked();
  v8::Local<v8::Array> startArr = converter::vectorVec2ToJsArray<int>( startPoints );
  ret->Set( startProp, startArr );

  v8::Local<v8::String> endProp = Nan::New("endPoints").ToLocalChecked();
  v8::Local<v8::Array> endArr = converter::vectorVec2ToJsArray<int>( endPoints );
  ret->Set( endProp, endArr );

  return ret;
}

bool isPointOnStructureById( int _id, int _x, int _y ){
  return getStructureById( _id )->testPoint( _x, _y );
}

int destroyStructById( int _id ){
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

v8::Local<v8::Array> getStructureUpgradesByTypeName( std::string _typeName ){
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

bool upgradeStructure( int _id, std::string _typeName ){
  getStructureById( _id )->setStructureDef( structuresDefList::getStructureTypeByName( _typeName ) );
  return true;
}

/////////////////////////////////

v8::Local<v8::Array> getTilesArray(){
  int _size = getSize();
  v8::Local<v8::Array> ret = Nan::New<v8::Array>( _size );
  for( int i = 0; i < _size; i++ ){
    v8::Local<v8::Object> v8Tile = tiles[ i ]->toObj();
    ret->Set( i, v8Tile );
  }
  return ret;
}

bool testStructurePos( int _x, int _y, std::string _typeName ){
  StructureDef* structureType = structuresDefList::getStructureTypeByName( _typeName );
  std::vector<int> grid = structureType->getGrid();
  int gl = grid.size();
  for( int i = 0; i < gl; i++ ){
    if( grid[ i ] == 0 ){
      continue;
    }
    Vec2<int> gridPt2dCoords = structureType->to2d( i );
    int tx = _x + gridPt2dCoords.x;
    int ty = _y + gridPt2dCoords.y;
    int coord1d = to1d( tx, ty );
    if(  tx < 0 || tx >= mapWidth || ty < 0 || ty >= mapHeight || ! tiles[ coord1d ]->isBuildable() ){
      return false;
    }
  }
  return true;
}

std::vector<bool> testMultipleStructurePos( std::vector<int> _positions, std::string _typeName, int _rotation ){
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
      Vec2<int> gridPt2dCoords = structureType->to2d( i, _rotation );
      int tx = sx + gridPt2dCoords.x;
      int ty = sy + gridPt2dCoords.y;
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

bool addStructures( std::vector<int> _positions, std::string _typeName, int _rotation ){
  std::vector<bool> testPositions = testMultipleStructurePos( _positions, _typeName, _rotation );
  int nbS = testPositions.size();
  StructureDef* strucDef = structuresDefList::getStructureTypeByName( _typeName );
  std::vector<int> strucDefGrid = strucDef->getGrid( _rotation );
  int bgl = strucDefGrid.size();
  std::vector<int> strucDefPos;
  for( int c = 0; c < bgl; c++ ){
    if( strucDefGrid[ c ] != 1 ){ continue; };
    Vec2<int> basePos = strucDef->to2d( c, _rotation );
    strucDefPos.push_back( basePos.x );
    strucDefPos.push_back( basePos.y );
  }

  std::vector<int> okPositions;
  for( int t = 0; t < nbS; t++ ){
    if( testPositions[ t ] == true ){
      okPositions.push_back( _positions[ t * 2 ] );
      okPositions.push_back( _positions[ t * 2 + 1 ] );
    };
  }
  bool testBlocking = newStructuresBlockingTest(  okPositions, strucDefPos );
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

v8::Local<v8::Array> getStructures(){
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

Tile* getTile( int _i ){
  return tiles[ _i ];
}

Tile* getTileByPosition( int _x, int _y ){
  if( _x >= 0 && _y >= 0 && _x <  width() && _y < height() ){
    return tiles[ to1d( _x, _y ) ];
  }
  return new Tile();
}

v8::Local<v8::Array> getStructureGrid( std::string _typeName, int _rotation ){
  StructureDef* structDef = structuresDefList::getStructureTypeByName( _typeName );
  std::vector<int> grid = structDef->getGrid( _rotation );
  v8::Local<v8::Array> ret = converter::vectorIntToJsArray( grid );
  return ret;
}

bool newStructuresBlockingTest( std::vector<int> _positions, std::vector<int> _strucDefPositions ){
  std::vector<std::vector<int>> poses = { _positions };
  std::vector<std::vector<int>> structuresPos = { _strucDefPositions };
  return newStructuresBlockingTest( poses, structuresPos );
}

bool newStructuresBlockingTest( std::vector<std::vector<int>> _positions, std::vector<std::vector<int>> _strucDefPositions ){
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

bool testCharMapOpening( std::vector<char> testMap ){
  int l = getSize();
  GridPos startPos = gridPos( getStartByIndex( 0 ) );
  std::vector<GridPos> openList { startPos };
  testMap[ startPos.pos1d ] = 2;
  bool endFound = false;
  int mw = width();
  int mh = height();
  for( int s = 0; endFound == false; s++ ){
    std::vector<GridPos> newList;
    int l = openList.size();
    for( int i = 0; i < l; i++ ){
      GridPos testedPos = openList[ i ];
      int px = testedPos.x;
      int py = testedPos.y;
      if( px > 0 ){
        int leftPos = to1d( px - 1, py );
        if( testMap[ leftPos ] == 0 ){
            testMap[ leftPos ] = 1;
            //newList.push_back( px - 1 );
            //newList.push_back( py );
            newList.push_back( gridPos( px - 1, py ) );
        }
      }
      if( px < mw - 1 ){
        int rightPos = to1d( px + 1, py );
        if( testMap[ rightPos ] == 0 ){
            testMap[ rightPos ] = 1;
            //newList.push_back( px + 1 );
            //newList.push_back( py );
            newList.push_back( gridPos( px + 1, py ) );
        }
      }
      if( py > 0 ){
        int topPos = to1d( px, py - 1 );
        if( testMap[ topPos ] == 0 ){
          //int intMapVal = intMap[ topPos ];
            testMap[ topPos ] = 1;
            //newList.push_back( px );
            //newList.push_back( py - 1 );
            newList.push_back( gridPos( px, py - 1 ) );
        }
      }

      if( py < mh - 1){
        int bottomPos = to1d( px, py + 1 );
        if( testMap[ bottomPos ] == 0 ){
          //int intMapVal = intMap[ bottomPos ];
            testMap[ bottomPos ] = 1;
            //newList.push_back( px );
            //newList.push_back( py + 1 );
            newList.push_back( gridPos( px, py + 1 ) );
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

bool removeStructuresBlockingTest( std::vector<int> _positions ){
  std::vector<char> charMap = getCharMap();
  int pl = _positions.size() / 2;
  for( int i = 0; i < pl; i++ ){
    charMap[ to1d( _positions[ i * 2 ], _positions[ i * 2 + 1 ] ) ] = 0;
  }
  return testCharMapOpening( charMap );
}

int removeStructById( int _id ){
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

std::vector<char> getCharMap(){
  int l = getSize();
  std::vector<char> ret( l, 0 );
  for( int i = 0; i < l; i ++ ){
    Tile* tile = tiles[ i ];
    if( ! tile->isTraversable() ){
      ret[ i ] = -1;
    }
  }
  return ret;
}

int destroyStructsByZone( int _startx, int _starty, int _endx, int _endy ){
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

int getStructureIdByPosition( int _x, int _y ){
  if( _x >= 0 && _y >= 0 && _x <  width() && _y < height() ){
    return getTileByPosition( _x, _y )->getStructureId();
  }
  return 0;
}

std::vector<float> pathMap( int _startx, int _starty){
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

std::vector<char> pathMapChar( int _startx, int _starty){
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

float getTileSpeed( int _x, int _y ){
  if( _y < 0 ){
    return tiles[ _x ]->getSpeed();
  }else{
    return tiles[ to1d( _x, _y ) ]->getSpeed();
  }
}

void addDestinationPoint( int _x, int _y ){
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

void removeDestinationPoint( int _x, int _y ){
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

DestinationPt& getOrAddDestinationPt( int _x, int _y ){
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

DestinationPt& getDestinationPt( int _x, int _y ){
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

float lineSight( int x0, int y0, int x1, int y1 ){
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

void tethaCheck( int tx, int ty, TethaSearchTile& t, int &neighbx, int &neighby, int &parentx, int &parenty, float &parentVal, float &nv, float hDist, std::vector<int> &newList ){
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

void tethaSearch( int _startx, int _starty ){
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
      Vec2<int> parentPos = to2d( neighbour.parentPos );
      int& parentx = parentPos.x;
      int& parenty = parentPos.y;
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

void updateAllTethaPaths(){
  int nbDestPts = destinationPoints.size();
  if( nbDestPts == 0 ){ return void(); };
  for( int i = 0; i < nbDestPts; i++ ){
    Vec2<int> DestPtPos = to2d( destinationPoints[ i ].pos1d );
    tethaSearch( DestPtPos.x, DestPtPos.y );
  }
}


std::vector<int> getTethaPath( int _startx, int _starty, int _destinationx, int _destinationy ){
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
    Vec2<int> parentPos2d = to2d( parentPos1d );
    ret.push_back( parentPos2d.x );
    ret.push_back( parentPos2d.y );
    lastPos1d = parentPos1d;
  }
  return ret;
}


};

#endif
