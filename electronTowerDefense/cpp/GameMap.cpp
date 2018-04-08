#include "GameMap.h"

GameMap::GameMap(){

}
GameMap::GameMap( int _w, int _h ){

}
void GameMap::setStartPts( std::vector<int> _v ){
  startPts->set( _v );
}
std::vector<int> GameMap::getStartPts(){
  return startPts->get();
}
void GameMap::setEndPt( int i, int val ){
  endPts->set( i, val );
}

void GameMap::setEndPts( std::vector<int> _v ){
  endPts->set( _v );
}
std::vector<int> GameMap::getEndPts(){
  return endPts->get();
}
