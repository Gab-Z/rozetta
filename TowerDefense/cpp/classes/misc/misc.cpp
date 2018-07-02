#include "misc.h"

void misc::buffer_delete_callback(char* data, void* the_vector) {
  delete reinterpret_cast<std::vector<char>*> (the_vector);
}
Nan::Persistent<v8::FunctionTemplate> constructor;
