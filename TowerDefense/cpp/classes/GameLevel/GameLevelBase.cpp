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

Structure* GameLevelBase::getStructureById( int _id ){
  int l = structures.size();
  for( int i = 0; i < l; i++ ){
    if( structures[ i ]->getId() == _id ){
      return structures[ i ];
    }
  }
  return structures[ 0 ];
}

std::vector<int> GameLevelBase::getStartByIndex( int _i ){
  int l = startPoints.size() / 2;
  if( _i < 0 || _i >= l ){
    return std::vector<int> { startPoints[ 0 ], startPoints[ 1 ] };
  }else{
    return std::vector<int> { startPoints[ _i * 2 ], startPoints[ _i * 2 + 1 ] };
  }
}

v8::Local<v8::Array> GameLevelBase::getCommonTextures(){
  v8::Local<v8::Array> ret = Nan::New<v8::Array>();

  v8::Local<v8::Object> destroyStructObj = Nan::New<v8::Object>();
    v8::Local<v8::String> nameProp = Nan::New("name").ToLocalChecked();
    v8::Local<v8::Value> nameVal = Nan::New( std::string( "destroyStructure" ) ).ToLocalChecked();
    destroyStructObj->Set( nameProp, nameVal );

    v8::Local<v8::String> destroyProp = Nan::New("url").ToLocalChecked();
    v8::Local<v8::Value> destroyVal = Nan::New( std::string("../img/") + std::string( "Hammer.png" ) ).ToLocalChecked();
    destroyStructObj->Set( destroyProp, destroyVal );

  ret->Set( 0, destroyStructObj );

  return ret;

}

v8::Local<v8::Object> GameLevelBase::getWays(){
  v8::Local<v8::Object> ret = Nan::New<v8::Object>();

  v8::Local<v8::String> startProp = Nan::New("startPoints").ToLocalChecked();
  v8::Local<v8::Array> startArr = converter::vectorIntToJsArray( startPoints );
  ret->Set( startProp, startArr );

  v8::Local<v8::String> endProp = Nan::New("endPoints").ToLocalChecked();
  v8::Local<v8::Array> endArr = converter::vectorIntToJsArray( endPoints );
  ret->Set( endProp, endArr );

  return ret;
}

bool GameLevelBase::isPointOnStructureById( int _id, int _x, int _y ){
  return getStructureById( _id )->testPoint( _x, _y );
}

int GameLevelBase::destroyStructById( int _id ){
  std::vector<Structure*>::iterator searchedIterator;
  Structure* searchedStruct;
  bool found = false;
  for( std::vector<Structure*>::iterator i = structures.begin(); i < structures.end(); ++i ){
    if( (*i)->getId() == _id ){
      searchedStruct = (*i);
      searchedIterator = i;
      found = true;
      break;
    }
  }
  if( found == true ){
    delete searchedStruct;
    structures.erase( searchedIterator );
    return 1;
  }
  return 0;
}
