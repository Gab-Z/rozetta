#include <nan.h>
#include <cmath>
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

NAN_METHOD(initArray2){
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
  }
  v8::Local<v8::Number> arrL = Nan::New(arrLength);
  info.GetReturnValue().Set(arrL);
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

void pushIfNew( std::vector<int> _arr, int x, int y ){
  bool isNew = true;
  for( std::vector<int>::size_type i = 0; i != _arr.size() / 2; i++ ){
    int tx = _arr[ i * 2 ];
    int ty = _arr[ i * 2 + 1 ];
    if( tx == x && ty == y ){
      isNew = false;
      break;
    }
  }
  if( isNew == true ){
    _arr.push_back( x );
    _arr.push_back( y );
  }
}

std::vector<int> testTile( int srcX, int srcY, int offsetX, int offsetY, int value, std::vector<int> _retMap, std::vector<int> _list ){
  int tX = srcX + offsetX;
  int tY = srcY + offsetY;
  int pos1d = to1d( tX, tY );
  int retTarget = _retMap[ pos1d ];
  int srcTarget = arr[ pos1d ];
  if( retTarget < 0 || value + srcTarget < retTarget ){
    _retMap[ pos1d ] = value + srcTarget;
    //alert(retMap[ y - 1 ][ x - 1 ])
    //pushIfNew( _list, tX, tY );

    bool isNew = true;
    for( std::vector<int>::size_type i = 0; i != _list.size() / 2; i++ ){
      int tx = _list[ i * 2 ];
      int ty = _list[ i * 2 + 1 ];
      if( tx == tX && ty == tY ){
        isNew = false;
        break;
      }
    }
    if( isNew == true ){
      _list.push_back( tX );
      _list.push_back( tY );
    }
  }
  return _list;
}

std::vector<int> fillMapStep( std::vector<int> retMap, std::vector<int> searchList ){
  std::vector<int> newList;
  for( std::vector<int>::size_type i = 0; i != searchList.size() / 2; i++ ){
    int x  = searchList[ i * 2 ];
    int y = searchList[ i * 2 + 1 ];
    int pos = to1d( x, y );
    int value = retMap[ pos ];

    if( x > 0 ){
      if( y > 0){
        testTile( x, y, -1, -1, value, retMap, newList );
      }
      testTile( x, y, -1, 0, value, retMap, newList );
      if( y + 1 < arrH ){
        testTile( x, y, -1, 1, value, retMap, newList );
      }
    }
    //MIDDLE
    if( y > 0 ){
      testTile( x, y, 0, -1, value, retMap, newList );
    }
    if( y + 1 < arrH ){
      testTile( x, y, 0, 1, value, retMap, newList );
    }
    //RIGHT
    if( x + 1 < arrW ){
      if( y > 0){
        testTile( x, y, 1, -1, value, retMap, newList );
      }
      testTile( x, y, 1, 0, value, retMap, newList );
      if( y + 1 < arrH ){
        testTile( x, y, 1, 1, value, retMap, newList );
      }
    }
  }
  return newList;
}

std::vector<int> fillMap( int x, int y ){
  std::vector<int> retMap;
  for ( int i = 0; i < arrLength; i++ ) {
    retMap.push_back( -1 );
  }
  std::vector<int> searchList;
  searchList.push_back( x );
  searchList.push_back( y );
  bool end = false;
  for( int n = 0; end == false; n++ ){
    searchList = fillMapStep( retMap, searchList );
    if( searchList.size() == 0 ){
      end = true;
    }
  }
  return retMap;
}

#ifndef DEF_PERSONNAGE
#define DEF_PERSONNAGE
class Map{

    public:

    void init( int w, int h, int depMin, int depMax ){
      arrW = w;
      arrH = h;
      arrLength = arrW * arrH;
      rangeMin = depMin;
      rangeMax = depMax;
      for (int i = 0; i < arrLength; i++){
        int b = rand() % rangeMax + rangeMin;
        arr.push_back(b);
      }
    }

    std::vector<int> fillMap( int x, int y ){
      std::vector<int> retMap;
      for ( int i = 0; i < arrLength; i++ ) {
        retMap.push_back( -1 );
      }
      std::vector<int> searchList;
      searchList.push_back( x );
      searchList.push_back( y );
      bool end = false;
      for( int n = 0; end == false; n++ ){
        searchList = fillMapStep( retMap, searchList );
        if( searchList.size() == 0 ){
          end = true;
        }
      }
      return retMap;
    }

    private:

    std::vector<int> arr;
    int arrW;
    int arrH;
    int arrLength;
    int rangeMin;
    int rangeMax;
};


#endif

NAN_METHOD(getFilledMap){
  if ( !info[0]->IsNumber() || !info[1]->IsNumber() ) {
    Nan::ThrowTypeError("Argument must be a number");
    return;
  }
  int x = info[0]->NumberValue();
  int y = info[1]->NumberValue();
  std::vector<int> filledMap = fillMap( x, y );
  v8::Local<v8::Array> a = Nan::New<v8::Array>();
  for (int i = 0; i < arrLength; i++ ) {
    Nan::Set(a, i, Nan::New(filledMap[i]));
  }
  info.GetReturnValue().Set(a);
}

NAN_MODULE_INIT(Initialize) {
  NAN_EXPORT(target, WhoAmI);
  NAN_EXPORT(target, Increment);
  NAN_EXPORT(target, initArray);
  NAN_EXPORT(target, GetArrayLength);
  NAN_EXPORT(target, getArray);
  NAN_EXPORT(target, getFilledMap);
}

NODE_MODULE(module_name, Initialize)
