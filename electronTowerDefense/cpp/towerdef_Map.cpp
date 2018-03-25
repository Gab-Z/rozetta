#include "towerdef_Map.h"

const std::vector<int> turnCoords { -1,0,   -1,-1,   0,-1,    1,-1,    1,0,   1,1,   0,1,   -1,1 };

void Map::init( int w, int h, int depMin, int depMax ){
  arrW = w;
  arrH = h;
  arrLength = arrW * arrH;
  rangeMin = depMin;
  rangeMax = depMax;
  for (int i = 0; i < arrLength; i++){
    int b = rand() % rangeMax + rangeMin;
    arr.push_back(b);
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
  for( std::vector<int>::size_type n = 0; n < searchList.size(); n++ ){
    searchList = fillMapStep(  p1d, pathMap, searchList );

  }
}

std::vector<int> Map::fillMapStep( int id, PathMap& pathMap,  std::vector<int> searchList ){
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
  int retTarget = pathMap.getValue( pos1d );
  int srcTarget = arr[ pos1d ];

  if( retTarget < 0 || value + srcTarget < retTarget ){
  //  pathMap[ pos1d ] = value + srcTarget;
    int newVal = value + srcTarget;
    pathMap.setValue( pos1d, newVal );
    bool isNew = true;
    for( std::vector<int>::size_type i = 0; i < _list.size() / 2; i++ ){
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
