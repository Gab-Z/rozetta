#include <nan.h>
#include <cmath>
const int maxValue = 10;
int numberOfCalls = 0;
std::vector<int> arr;
std::vector<std::vector<int>> pathArrays;

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

NAN_METHOD(initArray){
  if (!info[0]->IsNumber() || !info[1]->IsNumber() || !info[2]->IsNumber() || !info[3]->IsNumber() ) {
    Nan::ThrowTypeError("Argument must be a number");
    return;
  }
  arrW = info[0]->NumberValue();
  arrH = info[1]->NumberValue();
  arrLength = arrW * arrH;

  int rangeMin = info[2]->NumberValue();
  int rangeMax = info[3]->NumberValue();
  for (int i =0; i < arrLength; i++){
    int b = rand() % rangeMax + rangeMin;
    arr.push_back(b);
    std::vector<int> pathArr( arrLength );
    pathArrays.push_back( pathArr );
  }
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

int to1d( int x, int y ){
  int pos = y * arrW + x;
  return pos;
}

std::vector<int> to2d( int p ){
  std::vector<int> pos;
  int y = std::floor( p / arrW );
  int nbCellRows = y * arrW;
  int x = p - nbCellRows;
  pos.push_back(x);
  pos.push_back(y);
  return pos;
}

NAN_METHOD(getFilledMap){
  if ( !info[0]->IsNumber() || !info[1]->IsNumber() ) {
    Nan::ThrowTypeError("Argument must be a number");
    return;
  }
  int x = info[0]->NumberValue();
  int y = info[1]->NumberValue();
  int pos = to1d( x, y );
  pathArrays[ pos ].clear();
  for ( int i = 0; i < arrLength; i++ ) {
    pathArrays[ pos ].push_back( -1 );
  }
  std::vector<int> searchList;
  searchList.push_back( x );
  searchList.push_back( y );
  std::vector<int> pathArr;
  v8::Local<v8::Array> a = Nan::New<v8::Array>();

}

NAN_MODULE_INIT(Initialize) {
  NAN_EXPORT(target, WhoAmI);
  NAN_EXPORT(target, Increment);
  NAN_EXPORT(target, initArray);
  NAN_EXPORT(target, GetArrayLength);
  NAN_EXPORT(target, getArray);
}

NODE_MODULE(module_name, Initialize)
