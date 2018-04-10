#include "towerdef_Map.h"

const std::vector<int> turnCoords { -1,0,   -1,-1,   0,-1,    1,-1,    1,0,   1,1,   0,1,   -1,1 };

void Map::init( int w, int h, int depMin, int depMax, std::vector<int> _startPts, std::vector<int> _endPts ){
  arrW = w;
  arrH = h;
  arrLength = arrW * arrH;
  rangeMin = depMin;
  rangeMax = depMax;
  startPts = _startPts;
  endPts = _endPts;
  for (int i = 0; i < arrLength; i++){
    //int b = rand() % rangeMax + rangeMin;
    arr.push_back(1);
  }
}

int Map::getPathMapIndex( int x, int y ){
  int size = paths.size();
  for( int i = 0; i < size; i++ ){
    PathMap& pm = paths[ i ];
    std::vector<int> pos = pm.getStart();
    if( pos[ 0 ] == x && pos[ 1 ] == y ){
      return i;
    }
  }
  return -1;
}

std::vector<int> Map::getArray(){
  return arr;
}

int Map::getLength(){
  return arrLength;
}

std::vector<int> Map::getFilledMap( int x, int y ){
  int index = getPathMapIndex( x, y );
  return paths[ index ].getMap();
}

void Map::fillMap( int x, int y ){
  int p1d = to1d( x, y);
  int pathId = getPathMapIndex( x, y );
  if( pathId < 0 ){
    PathMap newPathMap;
    newPathMap.setStart( x, y );
    paths.push_back( newPathMap );
    pathId = getPathMapIndex( x, y );
  }
  PathMap& pathMap = paths[ pathId ];
  pathMap.clearMap();
  for ( int i = 0; i < arrLength; i++ ) {
    pathMap.push( -1 );
  };
  pathMap.setValue( p1d, arr[ p1d ] );
  std::vector<int> searchList;
  searchList.push_back( x );
  searchList.push_back( y );
  for( std::vector<int>::size_type n = 0; searchList.size() > 0; n++ ){
    searchList = fillMapStep( pathMap, searchList );
  }
}

std::vector<int> Map::fillMapStep( PathMap& pathMap,  std::vector<int> searchList ){
  std::vector<int> newList;
  for( std::vector<int>::size_type i = 0; i < searchList.size() / 2; i++ ){
    int x  = searchList[ i * 2 ];
    int y = searchList[ i * 2 + 1 ];
    int pos = to1d( x, y );
    int value = pathMap.getValue( pos );
    if( x > 0 ){
      if( y > 0){
        newList =  testTile( x, y, -1, -1, value, pathMap, newList );
      }
      newList = testTile( x, y, -1, 0, value, pathMap, newList );
      if( y + 1 < arrH ){
        newList =  testTile( x, y, -1, 1, value, pathMap, newList );
      }
    }
    //MIDDLE
    if( y > 0 ){
      newList =  testTile( x, y, 0, -1, value, pathMap, newList );
    }
    if( y + 1 < arrH ){
      newList =  testTile( x, y, 0, 1, value, pathMap, newList );
    }
    //RIGHT
    if( x + 1 < arrW ){
      if( y > 0){
        newList =  testTile( x, y, 1, -1, value, pathMap, newList );
      }
      newList =  testTile( x, y, 1, 0, value, pathMap, newList );
      if( y + 1 < arrH ){
        newList =  testTile( x, y, 1, 1, value, pathMap, newList );
      }
    }
  }
  return newList;
}

std::vector<int>  Map::testTile( int srcX, int srcY, int offsetX, int offsetY, int value, PathMap& pathMap, std::vector<int> _list ){
  int tX = srcX + offsetX;
  int tY = srcY + offsetY;
  int pos1d = to1d( tX, tY );
  std::vector<int> newList;
  newList = _list;
  int listl = _list.size() / 2;
  int retTarget = pathMap.getValue( pos1d );
  int srcTarget = arr[ pos1d ];
  if(srcTarget < 0-1 ){
    pathMap.setValue( pos1d, srcTarget );
    return newList;
  }
  if( retTarget == 0-1 || value + srcTarget < retTarget ){
  //  pathMap[ pos1d ] = value + srcTarget;
    int newVal = value + srcTarget;
    pathMap.setValue( pos1d, newVal );
    bool isNew = true;
    for( int i = 0; i < listl; i++ ){
      int tx = _list[ i * 2 ];
      int ty = _list[ i * 2 + 1 ];
      if( tx == tX && ty == tY ){
        isNew = false;
        break;
      }
    }
    if( isNew == true ){
      newList.push_back( tX );
      newList.push_back( tY );
    }
  }
  return newList;
}

std::vector<int> Map::getPath( int startx, int starty, int destinationx, int destinationy ){
  int pathMapIndex = getPathMapIndex( destinationx, destinationy );
  if( pathMapIndex < 0 ){
    fillMap( destinationx, destinationy );
    pathMapIndex = getPathMapIndex( destinationx, destinationy );
  }
  PathMap& pathMap = paths[ pathMapIndex ];
  std::vector<int> ret;
  ret.push_back( startx );
  ret.push_back( starty );
  for( std::vector<int>::size_type i = 0; i < ret.size(); i+=2 ){
    int x = ret[ i ];
    int y = ret[ i + 1 ];
    int p1d = to1d( x, y );
    int val = pathMap.getValue( p1d );
    int rx = -1;
    int ry = -1;
    for( int j = 0; j < 8; j++ ){
      int tx = x + turnCoords[ j * 2 ];
      int ty = y + turnCoords[ j * 2 + 1 ];
      int t1d = to1d( tx, ty );
      int testVal = pathMap.getValue( t1d );
      if( testVal < val ){
        rx = tx;
        ry = ty;
        val = testVal;
      }
    }
    if( rx > -1 ){
      ret.push_back( rx );
      ret.push_back( ry );
    }
  }
  return ret;
}

int Map::to1d( int x, int y ){
  int pos = y * arrW + x;
  return pos;
}

std::vector<int> Map::to2d( int p ){
  std::vector<int> pos;
  int y = std::floor( p / arrW );
  int nbCellRows = y * arrW;
  int x = p - nbCellRows;
  pos.push_back(x);
  pos.push_back(y);
  return pos;
}
int Map::getWidth(){
  return arrW;
}
int Map::getHeight(){
  return arrH;
}
std::vector<int> Map::getStartPts(){
  return startPts;
}
std::vector<int> Map::getEndPts(){
  return endPts;
}

std::vector<int> Map::getAccessMap( std::vector<int> positions ){
  std::vector<int> copyMap;
  for( int p = 0; p < arrLength; p++ ){
    int nw = arr[ p ];
    copyMap.push_back( nw );
  }
  int pl = positions.size() / 2;
  int sl = startPts.size() / 2;
  for( int j = 0; j < pl; j++ ){
    int _x = positions[ j * 2 ];
    int _y = positions[ j * 2 + 1 ];
    for( int m = 0; m < sl; m++ ){
      if( ( _x == startPts[ m * 2 ] && _y == startPts[ m * 2 + 1 ] ) || ( _x == endPts[ m * 2 ] && _y == endPts[ m * 2 + 1 ] ) ){
        std::vector<int> retf;
        return retf;
      }
    }
    int t1d = to1d( _x, _y );
    if(  copyMap[ t1d ] > -2 ){
      copyMap[ t1d ] = -2;
    }
  }
  for( int c = 0; c < arrLength; c++ ){
    if( copyMap[ c ] > -2 ){
      copyMap[ c ] = 1;
    }
  }
  return copyMap;
}

bool Map::addStructure( std::string _className, std::vector<int> positions ){
  std::vector<int> copyMap = arr;
  std::vector<int> newPositions;
  int pl = positions.size() / 2;
  int sl = startPts.size() / 2;
  std::vector<Structure*> filteredStructs;

  for( int n = 0; n < pl; n++ ){
    int _x = positions[ n * 2 ];
    int _y = positions[ n * 2 + 1 ];
    Structure *struc;
    if( _className == "Wall" ){
      struc = new Wall( _x, _y );
    }
    std::vector<int> sPos = struc->getPositions( _x, _y );
    int sPl = sPos.size() / 2;
    for( int f = 0; f < sPl; f++ ){
      int sx = sPos[ f * 2 ];
      int sy = sPos[ f * 2 + 1 ];
      for( int m = 0; m < sl; m++ ){
        if( ( sx == startPts[ m * 2 ] && sx == startPts[ m * 2 + 1 ] ) || ( sx == endPts[ m * 2 ] && sy == endPts[ m * 2 + 1 ] ) ){
          return false;
        }
      }
      int t1d = to1d( sx, sy );
      if(  copyMap[ t1d ] > -2 ){
        copyMap[ t1d ] = -2;
        newPositions.push_back( sx );
        newPositions.push_back( sy );
      }else{
        return false;
      }
    }
    filteredStructs.push_back( struc );
  }
  for( int c = 0; c < arrLength; c++ ){
    if( copyMap[ c ] > -2 ){
      copyMap[ c ] = 1;
    }
  }

  std::vector<int> filled = fillAccessMap( endPts[ 0 ], endPts[ 1 ], copyMap );
  for( int i = 0; i < arrLength; i++ ){
    if( filled[ i ] == -1 ){
      return false;
    }
  }
  int npl = newPositions.size() / 2;
  int fsl = filteredStructs.size();
  for( int u = 0; u < fsl; u++ ){
    structures.push_back( filteredStructs[ u ] );
  }
  for( int z = 0; z < npl; z++ ){
    int px = newPositions[ z * 2 ];
    int py = newPositions[ z * 2 + 1 ];
    int p1d = to1d( px, py );
    arr[ p1d ] = -2;
  }
  return true;
}

std::vector<int> Map::getStructuresPos(){
  int sl = structures.size();
  std::vector<int> ret;
  for( int i = 0; i < sl; i++ ){
    std::vector<int>  poses = structures[ i ]->getPositions( structures[ i ]->getx(), structures[ i ]->gety() );
    int pl = poses.size();
    for(int j = 0; j < pl; j++ ){
      ret.push_back( poses[ j ] );
    }
  }
  return ret;
}

v8::Local<v8::Array> Map::getStructures(){
  int sl = structures.size();
  v8::Local<v8::Array> jsArr = Nan::New<v8::Array>( sl );

  for( int i = 0; i < sl; i++ ){
    Structure *struc = structures[ i ];

    std::vector<int> poses = struc->getPositions( struc->getx(), struc->gety() );

    v8::Local<v8::Object> jsonObject = Nan::New<v8::Object>();

    v8::Local<v8::String> xProp = Nan::New("x").ToLocalChecked();
    v8::Local<v8::String> yProp = Nan::New("y").ToLocalChecked();
    v8::Local<v8::String> tProp = Nan::New("typeName").ToLocalChecked();

    v8::Local<v8::String> aProp = Nan::New("structures").ToLocalChecked();

    v8::Local<v8::Value> xValue = Nan::New(struc->getx());
    v8::Local<v8::Value> yValue = Nan::New(struc->gety());

    v8::Local<v8::Value> tValue = Nan::New(struc->getTypeName()).ToLocalChecked();

    v8::Local<v8::Array> posesArr = Nan::New<v8::Array>(poses.size());
    int pal = posesArr->Length();
    for (int p = 0; p < pal; p++) {
      int number = poses.at( p );
      v8::Local<v8::Value> jsElement = Nan::New(number);
      posesArr->Set(p, jsElement);
    }

    Nan::Set( jsonObject, xProp, xValue );
    Nan::Set( jsonObject, yProp, yValue );
    Nan::Set( jsonObject, tProp, tValue );
    Nan::Set( jsonObject, aProp, posesArr );

    jsArr->Set( i, jsonObject );
  }
  return jsArr;
}

std::vector<int> Map::fillAccessMap( int _startx, int _starty, std::vector<int>& arrMap ){

  int length = getLength();
  std::vector<int> ret( length, -1);

  int p1d = to1d( _startx, _starty );
  ret[ p1d ] = 1;
  std::vector<int> searchList;
  searchList.push_back( _startx );
  searchList.push_back( _starty );
  for( std::vector<int>::size_type n = 0; searchList.size() > 0; n = n ){
    searchList = fillAccessMapStep( ret, searchList, arrMap );
  }
  return ret;
}

std::vector<int> Map::fillAccessMapStep( std::vector<int>& ret, std::vector<int> searchList, std::vector<int>& arrMap ){
  std::vector<int> newList;
  int sl = searchList.size() / 2;
  for( int i = 0; i < sl; i++ ){
    int x  = searchList[ i * 2 ];
    int y = searchList[ i * 2 + 1 ];
    int pos = to1d( x, y );
    int value = ret[ pos ];
    if( x > 0 ){
        newList =  testAccessTile( x, y, -1, 0, value, ret, newList, arrMap );
    }
    if( x < arrW - 1 ){
        newList =  testAccessTile( x, y, 1, 0, value, ret, newList, arrMap );
    }
    if( y > 0 ){
        newList =  testAccessTile( x, y, 0, -1, value, ret, newList, arrMap );
    }
    if( y < arrH - 1 ){
      newList =  testAccessTile( x, y, 0, 1, value, ret, newList, arrMap );
    }
  }
  return newList;
}

std::vector<int>  Map::testAccessTile( int srcX, int srcY, int offsetX, int offsetY, int value, std::vector<int>& ret, std::vector<int> _list, std::vector<int>& arrMap ){
  int tX = srcX + offsetX;
  int tY = srcY + offsetY;
  int pos1d = to1d( tX, tY );
  std::vector<int> newList;
  newList = _list;
  int retTarget = ret[ pos1d ];
  if(retTarget != -1 ){
    return newList;
  }
  int srcTarget = arrMap[ pos1d ];
  if( srcTarget < -1 ){
    ret[ pos1d ] = srcTarget;
    return newList;
  }else{
    ret[ pos1d ] = 1;
    int listl = _list.size() / 2;
    for( int i = 0; i < listl; i++ ){
      if( _list[ i * 2 ] == tX && _list[ i * 2 + 1 ] == tY ){
        return newList;
      }
    }
    newList.push_back( tX );
    newList.push_back( tY );
    return newList;
  }
}
v8::Local<v8::Array> Map::getFilledAccess(){
  std::vector<int> copyArr = arr;
  std::vector<int> filled = fillAccessMap( endPts[ 0 ], endPts[ 1 ], copyArr );
  int sl = copyArr.size();
  v8::Local<v8::Array> jsArr = Nan::New<v8::Array>( sl );
  for( int i = 0; i < sl; i++ ){
    int number = filled.at( i );
    v8::Local<v8::Value> jsElement = Nan::New(number);
    jsArr->Set(i, jsElement);
  }
  return jsArr;
}
