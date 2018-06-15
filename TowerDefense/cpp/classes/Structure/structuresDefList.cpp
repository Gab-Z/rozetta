#include "structuresDefList.h"

StructureDef* structuresDefList::getStructureTypeByName( std::string _typeName ){
  int l = structuresDefList::structureTypes.size();
  for( int i = 0; i < l; i++ ){
    StructureDef* strucDef = structuresDefList::structureTypes[ i ];
    if( strucDef->getTypeName() == _typeName ){
      return strucDef;
    }
  }
  return structuresDefList::structureTypes[ 0 ];
}

v8::Local<v8::Array> structuresDefList::structuresDefs(){
  int l =  structuresDefList::structureTypes.size();
  v8::Local<v8::Array> ret = Nan::New<v8::Array>( l - 1 );
  for( int i = 1; i < l; i++ ){
    int ii = i - 1;
    StructureDef* strucDef = structuresDefList::structureTypes[ i ];
    v8::Local<v8::Object> strucDefObj = strucDef->toObj();
    ret->Set( ii, strucDefObj );
  }
  return ret;
}
