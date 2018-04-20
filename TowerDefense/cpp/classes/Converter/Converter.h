#ifndef DEF_Converter
#define DEF_Converter

class Converter {

  public:

    static std::vector<int> jsArrayToVectorInt( v8::Local<v8::Array> _arr );
    static v8::Local<v8::Array> vectorIntToJsArray( std::vector<int> _v );

};

#endif
