#include "StructuresDefList.h"

StructureDef* StructuresDefList::getStructureTypeByName( std::string _typeName ){
  int l = StructuresDefList::structureTypes.size();
  for( int i = 0; i < l; i++ ){
    StructureDef* strucDef = StructuresDefList::structureTypes[ i ];
    if( strucDef->getTypeName() == _typeName ){
      return strucDef;
    }
  }
  return StructuresDefList::structureTypes[ 1 ];
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
