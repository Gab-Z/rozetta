#include "GameMap.h"

GameMap::GameMap():Grid(){

}
GameMap::GameMap( int _w, int _h ):Grid( _w, _h ){

}
void GameMap::setStartPts( std::vector<int> _v ){
  startPts.set( _v );
}
std::vector<int> GameMap::getStartPts(){
  return startPts.get();
}
void GameMap::setEndPt( int i, int val ){
  endPts.set( i, val );
}

void GameMap::setEndPts( std::vector<int> _v ){
  endPts.set( _v );
}
std::vector<int> GameMap::getEndPts(){
  return endPts.get();
}
void GameMap::setStartPt( int i, int val ){
  startPts.set( i, val );
}
