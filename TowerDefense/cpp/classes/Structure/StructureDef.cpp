#include "StructureDef.h"

StructureDef::StructureDef(){}

StructureDef::StructureDef( std::string _typeName, std::vector<int> _grid, std::string _imgUrl, int _cost, int _gridWidth, int _gridHeight ){
  typeName = _typeName;
  grid = _grid;
  imgUrl = _imgUrl;
  cost = _cost;
  gridWidth = _gridWidth;
  gridHeight = _gridHeight;
  rotates = true;
}

StructureDef::StructureDef( std::string _typeName, std::vector<int> _grid, std::string _imgUrl, int _cost, int _gridWidth, int _gridHeight, bool _rotates ){
  typeName = _typeName;
  grid = _grid;
  imgUrl = _imgUrl;
  cost = _cost;
  gridWidth = _gridWidth;
  gridHeight = _gridHeight;
  rotates = _rotates;
}

StructureDef::StructureDef( std::string _typeName, std::vector<int> _grid, std::string _imgUrl, int _cost, int _gridWidth, int _gridHeight, bool _rotates, std::vector<std::string> _upgradeList ){
  typeName = _typeName;
  grid = _grid;
  imgUrl = _imgUrl;
  cost = _cost;
  gridWidth = _gridWidth;
  gridHeight = _gridHeight;
  rotates = _rotates;
  upgradeList = _upgradeList;
}

StructureDef::StructureDef(
      std::string _typeName,  std::vector<int> _grid,
      std::string _imgUrl,    int _cost,
      int _gridWidth,         int _gridHeight,
      bool _rotates,          int _level,
      std::string _upgradeImgUrl, std::vector<std::string> _upgradeList
  ){
  typeName = _typeName;
  grid = _grid;
  imgUrl = _imgUrl;
  cost = _cost;
  gridWidth = _gridWidth;
  gridHeight = _gridHeight;
  rotates = _rotates;
  level = _level;
  upgradeImgUrl = _upgradeImgUrl;
  upgradeList = _upgradeList;
}

StructureDef::StructureDef( std::string _typeName,  std::vector<int> _grid,
              std::string _imgUrl,    int _cost, int _gridWidth,
              int _gridHeight,        bool _rotates,
              int _level,             std::string _upgradeImgUrl ){

  typeName = _typeName;
  grid = _grid;
  imgUrl = _imgUrl;
  cost = _cost;
  gridWidth = _gridWidth;
  gridHeight = _gridHeight;
  rotates = _rotates;
  level = _level;
  upgradeImgUrl = _upgradeImgUrl;
}

v8::Local<v8::Object> StructureDef::toObj(){

  v8::Local<v8::Object> ret = Nan::New<v8::Object>();

  v8::Local<v8::String> typeProp = Nan::New( "typeName" ).ToLocalChecked();
  v8::Local<v8::Value> typeValue = Nan::New( typeName ).ToLocalChecked();
  ret->Set( typeProp, typeValue );

  v8::Local<v8::String> widthProp = Nan::New( "gridWidth" ).ToLocalChecked();
  v8::Local<v8::Value> widthValue = Nan::New( gridWidth );
  ret->Set( widthProp, widthValue );

  v8::Local<v8::String> heightProp = Nan::New( "gridHeight" ).ToLocalChecked();
  v8::Local<v8::Value> heightValue = Nan::New( gridHeight );
  ret->Set( heightProp, heightValue );

  v8::Local<v8::String> imgProp = Nan::New( "imgUrl" ).ToLocalChecked();
  v8::Local<v8::Value> imgValue = Nan::New( std::string("../img/") + imgUrl ).ToLocalChecked();
  ret->Set( imgProp, imgValue );

  v8::Local<v8::String> costProp = Nan::New( "cost" ).ToLocalChecked();
  v8::Local<v8::Value> costValue = Nan::New( cost );
  ret->Set( costProp, costValue );

  v8::Local<v8::String> rotProp = Nan::New( "rotates" ).ToLocalChecked();
  v8::Local<v8::Value> rotValue = Nan::New( rotates );
  ret->Set( rotProp, rotValue );

  v8::Local<v8::String> gridProp = Nan::New( "grid" ).ToLocalChecked();
  v8::Local<v8::Array> gridValue = converter::vectorIntToJsArray( grid );
  ret->Set( gridProp, gridValue );

  if( upgradeImgUrl != "null" ){
    v8::Local<v8::String> upImgProp = Nan::New( "upgradeImgUrl" ).ToLocalChecked();
    v8::Local<v8::Value> upImgValue = Nan::New( std::string("../img/structuresThumbs/") + upgradeImgUrl ).ToLocalChecked();
    ret->Set( upImgProp, upImgValue );
  }

  if( upgradeList.size() > 0 ){
    v8::Local<v8::String> upListProp = Nan::New( "upgradeList" ).ToLocalChecked();
    v8::Local<v8::Array> upListValue = converter::vectorStringToJsArray( upgradeList );
    ret->Set( upListProp, upListValue );
  }

  return ret;
}

std::string StructureDef::getTypeName(){
  return typeName;
}

std::vector<int> StructureDef::getGrid(){
  return grid;
}

std::vector<int> StructureDef::getGrid( int _rotation ){
  int l = grid.size();
  std::vector<int> ret( l );
  for( int i = 0; i < l; i++ ){
    int baseInt = grid[ i ];
    std::vector<int> basePos = to2d( i );
    std::vector<int> rotPos( 2 );
    if( _rotation == 1 ){
      rotPos[ 0 ] = gridHeight - 1 - basePos[ 1 ];
      rotPos[ 1 ] = basePos[ 0 ];
    }else if( _rotation == 2 ){
      rotPos[ 0 ] = gridWidth - 1 - basePos[ 0 ];
      rotPos[ 1 ] = gridHeight - 1 - basePos[ 1 ];
    }else if( _rotation == 3 ){
      rotPos[ 0 ] = basePos[ 1 ];
      rotPos[ 1 ] = gridWidth - 1 - basePos[ 0 ];
    }else{
      rotPos = basePos;
    }
    int rotPos1d = to1d( rotPos[ 0 ], rotPos[ 1 ], _rotation );
    ret[ rotPos1d ] = baseInt;

  }
  return ret;
}
int StructureDef::to1d( int _x, int _y ){
  int pos = _y * gridWidth + _x;
  return pos;
}

std::vector<int> StructureDef::to2d( int _idx ){
  int _y = std::floor( _idx / gridWidth );
  int _x = _idx - (_y * gridWidth );
  std::vector<int> pos { _x, _y };
  return pos;
}

std::vector<int> StructureDef::rotPoint( int _x, int _y, int _rotation ){
  std::vector<int> rotPos( 2 );
  if( _rotation == 1 ){
    rotPos[ 0 ] = gridHeight - 1 - _y;
    rotPos[ 1 ] = _x;
  }else if( _rotation == 2 ){
    rotPos[ 0 ] = gridWidth - 1 - _x;
    rotPos[ 1 ] = gridHeight - 1 - _y;
  }else if( _rotation == 3 ){
    rotPos[ 0 ] = _y;
    rotPos[ 1 ] = gridHeight - 1 - _x;
  }else{
    rotPos[ 0 ] = _x;
    rotPos[ 1 ] = _y;
  }
  return rotPos;
}

std::vector<int> StructureDef::to2d( int _idx, int _rotation ){

  int _gridWidth;
  if( _rotation == 1 || _rotation == 3 ){
    _gridWidth = gridHeight;
  }else{
    _gridWidth = gridWidth;
  }
  int _y = std::floor( _idx / _gridWidth );
  int _x = _idx - ( _y * _gridWidth );
  std::vector<int> pos { _x, _y };
  return pos;

}

int StructureDef::to1d( int _x, int _y, int _rotation ){
  int _gridWidth;
  if( _rotation == 1 || _rotation == 3 ){
    _gridWidth = gridHeight;
  }else{
    _gridWidth = gridWidth;
  }
  int pos = _y * _gridWidth + _x;
  return pos;
}

std::string StructureDef::getImgUrl(){
  return imgUrl;
}

int StructureDef::getGridWidth(){
  return gridWidth;
}

int StructureDef::getGridHeight(){
  return gridHeight;
}

bool StructureDef::isRotating(){
  return rotates;
}

std::string StructureDef::getUpgradeImgUrl(){
  return upgradeImgUrl;
}

std::vector<std::string> StructureDef::getUpgradeList(){
  return upgradeList;
}

bool StructureDef::testPoint( int _x, int _y, int _rotation ){
  //std::vector<int> grd = getGrid( _rotation );
  if( getGrid( _rotation )[ to1d( _x, _y, _rotation ) ] == 1 ){
    return true;
  }
  return false;
}

v8::Local<v8::Object> StructureDef::getUpgradeData(){
  v8::Local<v8::Object> ret = Nan::New<v8::Object>();

  v8::Local<v8::String> typeProp = Nan::New( "typeName" ).ToLocalChecked();
  v8::Local<v8::Value> typeValue = Nan::New( typeName ).ToLocalChecked();
  ret->Set( typeProp, typeValue );

  v8::Local<v8::String> imgProp = Nan::New( "upgradeImgUrl" ).ToLocalChecked();
  v8::Local<v8::Value> imgValue = Nan::New( upgradeImgUrl ).ToLocalChecked();
  ret->Set( imgProp, imgValue );

  v8::Local<v8::String> costProp = Nan::New( "cost" ).ToLocalChecked();
  v8::Local<v8::Value> costValue = Nan::New( cost );
  ret->Set( costProp, costValue );

  v8::Local<v8::String> lvlProp = Nan::New( "level" ).ToLocalChecked();
  v8::Local<v8::Value> lvlValue = Nan::New( level );
  ret->Set( lvlProp, lvlValue );

  return ret;
}
