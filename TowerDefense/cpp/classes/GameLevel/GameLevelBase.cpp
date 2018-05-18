#include "GameLevelBase.h"

GameLevelBase::GameLevelBase(){};
GameLevelBase::GameLevelBase( int _width, int _height, std::vector<int> _startPts, std::vector<int> _endPts ){
  mapWidth = _width;
  mapHeight = _height;
  startPoints = _startPts;
  endPoints = _endPts;
  size = mapWidth * mapHeight;
}

int GameLevelBase::width(){
  return mapWidth;
}

int GameLevelBase::height(){
  return mapHeight;
}

int GameLevelBase::to1d( int _x, int _y ){
  int pos = _y * mapWidth + _x;
  return pos;
}

std::vector<int> GameLevelBase::to2d( int _idx ){
  int _y = std::floor( _idx / mapWidth );
  int _x = _idx - (_y * mapWidth );
  std::vector<int> pos { _x, _y };
  return pos;
}

int GameLevelBase::getSize(){
  return size;
}

void GameLevelBase::pushStructure( Structure* _structure ){
  structures.push_back( _structure );
}

int GameLevelBase::structuresSize(){
  return structures.size();
}

Structure* GameLevelBase::getStructure( int _i ){
  return structures[ _i ];
}

std::vector<int> GameLevelBase::getStartByIndex( int _i ){
  int l = startPoints.size() / 2;
  if( _i < 0 || _i >= l ){
    return std::vector<int> { startPoints[ 0 ], startPoints[ 1 ] };
  }else{
    return std::vector<int> { startPoints[ _i * 2 ], startPoints[ _i * 2 + 1 ] };
  }
}
