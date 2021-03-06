#include "Converter.h"

std::vector<int> Converter::jsArrayToVectorInt( v8::Local<v8::Array> _arr ){
  int l = _arr->Length();
  std::vector<int> ret( l );
  for( int i = 0; i < l; i++ ){
    ret[ i ] = _arr->Get( i )->IntegerValue();
  }
  return ret;
}

v8::Local<v8::Array> Converter::vectorIntToJsArray( std::vector<int> _v ){
  int l = _v.size();
  v8::Local<v8::Array> ret = Nan::New<v8::Array>( l );
  for( int i = 0; i < l; i++ ){
    v8::Local<v8::Value> jsElement = Nan::New( _v[ i ] );
    ret->Set( i, jsElement );
  }
  return ret;
}
