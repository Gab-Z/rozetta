#ifndef DEF_Converter
#define DEF_Converter

#include <nan.h>
#include "../Structure/Structure.h"

namespace Converter {

    std::vector<int> jsArrayToVectorInt( v8::Local<v8::Array> _arr );
    v8::Local<v8::Array> vectorIntToJsArray( std::vector<int> _v );
    v8::Local<v8::Object> getDefinitionObj( Structure* struc );

};

#endif
