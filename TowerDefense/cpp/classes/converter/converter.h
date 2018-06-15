#ifndef DEF_Converter
#define DEF_Converter

#include <nan.h>

namespace converter {

    std::vector<int> jsArrayToVectorInt( v8::Local<v8::Array> _arr );
    v8::Local<v8::Array> vectorIntToJsArray( std::vector<int> _v );
    v8::Local<v8::Array> vectorBoolToJsArray( std::vector<bool> _v );
    v8::Local<v8::Array> vectorStringToJsArray( std::vector<std::string> _v );
    v8::Local<v8::Array> vectorFloatToJsArray( std::vector<float> _v );
};

#endif
