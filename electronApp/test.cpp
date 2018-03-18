#include <nan.h>

const int maxValue = 10;
int numberOfCalls = 0;
std::vector<int> arr;

int arrW;
int arrH;
int arrLength;

NAN_METHOD(WhoAmI) {
    // Create an instance of V8's String type
    auto message = Nan::New("Hello from C++! Dear Node Hero").ToLocalChecked();
    // 'info' is a macro's "implicit" parameter - it's a bridge object between C++ and JavaScript runtimes
    // You would use info to both extract the parameters passed to a function as well as set the return value.
    info.GetReturnValue().Set(message);
}

NAN_METHOD(Increment) {
  if (!info[0]->IsNumber()) {
    Nan::ThrowTypeError("Argument must be a number");
    return;
  }
  double argsValue = info[0]->NumberValue();
  if (numberOfCalls + argsValue > maxValue) {
    Nan::ThrowTypeError("Counter went through the roof!");
    return;
  }

  numberOfCalls += argsValue;

//  auto currentNumberOfCalls = Nan::New(numberOfCalls).ToLocalChecked();
  v8::Local<v8::Number> currentNumberOfCalls = Nan::New(numberOfCalls);
  info.GetReturnValue().Set(currentNumberOfCalls);
}
NAN_METHOD(fillArray){
  for (int i = 0; i < arrLength; i++ ) {
    arr[i] = rand() % 10 + 1;
  }
}

NAN_METHOD(initArray){
  if (!info[0]->IsNumber() || !info[1]->IsNumber()) {
    Nan::ThrowTypeError("Argument must be a number");
    return;
  }
  arrW = info[0]->NumberValue();
  arrH = info[1]->NumberValue();
  arrLength = arrW * arrH;

  //vector<int> myVector;
  srand((unsigned)time(NULL));
  for (int i =0; i < arrLength; i++){
    int b = rand() % 10 + 1;
    arr.push_back(b);
  //  cout << arr[i] << endl;
  }
//auto arrL = Nan::New(arrLength).ToLocalChecked();
  v8::Local<v8::Number> arrL = Nan::New(arrLength);
  info.GetReturnValue().Set(arrL);
}

NAN_METHOD(GetArrayLength){
  //auto arrL = Nan::New(arrLength).ToLocalChecked();
  v8::Local<v8::Number> arrL = Nan::New(arrLength);
  info.GetReturnValue().Set(arrL);
}

NAN_METHOD(getArray){
  //v8::Local<v8::Array> a = Nan::New(arr);
  //v8::Local<v8::Array> a = New<v8::Array>(arrLength);
  v8::Local<v8::Array> a = Nan::New<v8::Array>();

  //Local<Array> a = New<v8::Array>(arrLength);
  for (int i = 0; i < arrLength; i++ ) {
    //a[i] = arr[i];
    Nan::Set(a, i, Nan::New(arr[i]));
  }

  info.GetReturnValue().Set(a);
}

NAN_METHOD(getFilledMap){
  std::vector<int> pathArr;
}

NAN_MODULE_INIT(Initialize) {
  NAN_EXPORT(target, WhoAmI);
  NAN_EXPORT(target, Increment);
  NAN_EXPORT(target, initArray);
  NAN_EXPORT(target, GetArrayLength);
  NAN_EXPORT(target, fillArray);
  NAN_EXPORT(target, getArray);
}

NODE_MODULE(module_name, Initialize)
