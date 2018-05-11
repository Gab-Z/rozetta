#include "StructuresDefList.h"

/*
std::vector<StructureDef*> StructuresDefList::structureTypes = {

  new StructureDef( "null", std::vector<int>(), "null", 0, 0, 0 ),

  new StructureDef( "Wall", std::vector<int>{ 1 }, "wall.jpg", 10, 1, 1 ),

  new StructureDef( "Cross", std::vector<int>{
    0,  1,  0,
    1,  1,  1,
    0,  1,  0
  }, "cross.png", 60, 3, 3 )

};
*/
StructureDef* StructuresDefList::getStructureTypeByName( std::string _typeName ){
  int l = StructuresDefList::structureTypes.size();
  for( int i = 0; i < l; i++ ){
    StructureDef* strucDef = StructuresDefList::structureTypes[ i ];
    if( strucDef->getTypeName() == _typeName ){
      return strucDef;
    }
  }
  return StructuresDefList::structureTypes[ 0 ];
}
v8::Local<v8::Array> StructuresDefList::structuresDefs(){
  int l =  StructuresDefList::structureTypes.size();
  v8::Local<v8::Array> ret = Nan::New<v8::Array>( l - 1 );
  for( int i = 1; i < l; i++ ){
    int ii = i - 1;
    StructureDef* strucDef = StructuresDefList::structureTypes[ i ];
    v8::Local<v8::Object> strucDefObj = strucDef->toObj();
    ret->Set( ii, strucDefObj );
  }
  return ret;
}
