#ifndef DEFF_misc
#define DEFF_misc

#include <vector>
#include <nan.h>

namespace misc {

  void buffer_delete_callback(char* data, void* the_vector);
  static Nan::Persistent<v8::FunctionTemplate> constructor;
};

#endif
