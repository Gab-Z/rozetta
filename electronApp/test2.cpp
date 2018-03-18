#include <node.h>

const int maxValue = 10;
int numberOfCalls = 0;
int *arr;
int *pathArr;
int arrW;
int arrH;
int arrLength;

void WhoAmI(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  auto message = v8::String::NewFromUtf8(isolate, "I'm a Node Hero!");
  args.GetReturnValue().Set(message);
}

void Increment(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();

  if (!args[0]->IsNumber()) {
    isolate->ThrowException(v8::Exception::TypeError(
          v8::String::NewFromUtf8(isolate, "Argument must be a number")));
    return;
  }

  double argsValue = args[0]->NumberValue();
  if (numberOfCalls + argsValue > maxValue) {
    isolate->ThrowException(v8::Exception::Error(
          v8::String::NewFromUtf8(isolate, "Counter went through the roof!")));
    return;
  }

  numberOfCalls += argsValue;

  auto currentNumberOfCalls = v8::Number::New(isolate, static_cast<double>(numberOfCalls));

  args.GetReturnValue().Set(currentNumberOfCalls);
}

void initArray(const v8::FunctionCallbackInfo<v8::Value>& args){

  v8::Isolate* isolate = args.GetIsolate();

  if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
    isolate->ThrowException(v8::Exception::TypeError(
          v8::String::NewFromUtf8(isolate, "Argument must be a number")));
    return;
  }
  arrW = args[0]->NumberValue();
  arrH = args[1]->NumberValue();
  arrLength = arrW * arrH;

  arr = new int[ arrLength ];
  auto arrL = v8::Number::New(isolate, static_cast<double>(arrLength));
  args.GetReturnValue().Set(arrL);
}

void GetArrayLength(const v8::FunctionCallbackInfo<v8::Value>& args){
  v8::Isolate* isolate = args.GetIsolate();
  auto arrL = v8::Number::New(isolate, static_cast<double>(arrLength));
  args.GetReturnValue().Set(arrL);
}

void Initialize(v8::Local<v8::Object> exports) {
  NODE_SET_METHOD(exports, "whoami", WhoAmI);
  NODE_SET_METHOD(exports, "increment", Increment);
  NODE_SET_METHOD(exports, "initarray", initArray);
  NODE_SET_METHOD(exports, "getarraylength", GetArrayLength);
}

NODE_MODULE(module_name, Initialize)
