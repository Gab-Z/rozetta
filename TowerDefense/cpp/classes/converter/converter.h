#ifndef DEF_Converter
#define DEF_Converter

#include <nan.h>
#include "../geometry/Vec2.h"

namespace converter {

    std::vector<int> jsArrayToVectorInt( v8::Local<v8::Array> _arr );
    v8::Local<v8::Array> vectorIntToJsArray( std::vector<int> _v );
    v8::Local<v8::Array> vectorBoolToJsArray( std::vector<bool> _v );
    v8::Local<v8::Array> vectorStringToJsArray( std::vector<std::string> _v );
    v8::Local<v8::Array> vectorFloatToJsArray( std::vector<float> _v );
    template<typename T>
    v8::Local<v8::Array> vectorVec2ToJsArray( std::vector<Vec2<T>> _v ){
      int l = _v.size();
      v8::Local<v8::Array> ret = Nan::New<v8::Array>( l );
      for( int i = 0; i < l; i++ ){
        Vec2<T> v = _v[ i ];
        v8::Local<v8::Value> jsElementx = Nan::New( v.x );
        ret->Set( i * 2, jsElementx );
        v8::Local<v8::Value> jsElementy = Nan::New( v.y );
        ret->Set( i * 2 + 1, jsElementy );
      }
      return ret;
    }

    template<typename T>
    std::vector<Vec2<T>> flatVecToVec2s( std::vector<T> _vec ){
      int l = _vec.size() / 2;
      std::vector<Vec2<T>> ret( l );
      for( int i = 0; i < l; i++ ){
        ret[ i ] = Vec2<T>( _vec[ i * 2 ], _vec[ i * 2 + 1 ] );
      }
      return ret;
    };
};

#endif
