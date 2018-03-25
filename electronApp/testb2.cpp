
#include <nan.h>
#include <cmath>

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
        std::vector<int> path( arrLength );
        paths.push_back( path );
      }
    }
    std::vector<int> getArray(){
      return arr;
    }
    int getLength(){
      return arrLength;
    }
    std::vector<int> getFilledMap( int x, int y ){
      int pos1d = to1d( x, y );
      return paths[ pos1d ];
    }

    void fillMap( int x, int y ){
      int p1d = to1d( x, y);
      paths[ p1d ].clear();
      for ( int i = 0; i < arrLength; i++ ) {
        paths[ p1d ].push_back( -1 );
      };
      paths[ p1d ][ p1d ] = arr[ p1d ];
      std::vector<int> searchList;
      searchList.push_back( x );
      searchList.push_back( y );
      for( std::vector<int>::size_type n = 0; n < searchList.size(); n++ ){
        searchList = fillMapStep(  p1d, searchList );

      }
    }

    std::vector<int> fillMapStep( int id, std::vector<int> searchList ){
      std::vector<int> newList;
      for( std::vector<int>::size_type i = 0; i < searchList.size() / 2; i++ ){
        int x  = searchList[ i * 2 ];
        int y = searchList[ i * 2 + 1 ];
        int pos = to1d( x, y );
        int value = paths[ id ][ pos ];
        if( x > 0 ){
          if( y > 0){
            newList =  testTile( x, y, -1, -1, value, id, newList );
          }
          newList = testTile( x, y, -1, 0, value, id, newList );
          if( y + 1 < arrH ){
            newList =  testTile( x, y, -1, 1, value, id, newList );
          }
        }
        //MIDDLE
        if( y > 0 ){
          newList =  testTile( x, y, 0, -1, value, id, newList );
        }
        if( y + 1 < arrH ){
          newList =  testTile( x, y, 0, 1, value, id, newList );
        }
        //RIGHT
        if( x + 1 < arrW ){
          if( y > 0){
            newList =  testTile( x, y, 1, -1, value, id, newList );
          }
          newList =  testTile( x, y, 1, 0, value, id, newList );
          if( y + 1 < arrH ){
            newList =  testTile( x, y, 1, 1, value, id, newList );
          }
        }
      }
      return newList;
    }

    std::vector<int>  testTile( int srcX, int srcY, int offsetX, int offsetY, int value, int id, std::vector<int> _list ){
      int tX = srcX + offsetX;
      int tY = srcY + offsetY;
      int pos1d = to1d( tX, tY );
      std::vector<int> newList;
      newList = _list;
      int retTarget = paths[id][ pos1d ];
      int srcTarget = arr[ pos1d ];

      if( retTarget < 0 || value + srcTarget < retTarget ){
        paths[id][ pos1d ] = value + srcTarget;

        bool isNew = true;
        for( std::vector<int>::size_type i = 0; i < _list.size() / 2; i++ ){
          int tx = _list[ i * 2 ];
          int ty = _list[ i * 2 + 1 ];
          if( tx == tX && ty == tY ){
            isNew = false;
            break;
          }
        }
        if( isNew == true ){
          newList.push_back( tX );
          newList.push_back( tY );
        }
      }
      return newList;
    }

    private:

    std::vector<int> arr;
    int arrW;
    int arrH;
    int arrLength;
    int rangeMin;
    int rangeMax;
    std::vector<std::vector<int>> paths;

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

};

#endif

#ifndef DEF_fillMapAsyncWorker
#define DEF_fillMapAsyncWorker

class FillMapAsyncWorker : public Nan::AsyncWorker {
  public:
  int x;
  int y;
  Map _map;
  std::vector<int> filledMap;

  FillMapAsyncWorker(int x, int y, Map _Map, Nan::Callback *callback) : Nan::AsyncWorker(callback) {
    this->x = x;
    this->y = y;
    this->_map = _Map;
  }

  void Execute() {
    // mimic long running task
    //delay(iterations);
    _map.fillMap( x, y );
    filledMap = _map.getFilledMap( x, y );
  }

  void HandleOKCallback() {

    v8::Local<v8::Array> a = Nan::New<v8::Array>();
    for (std::vector<int>::size_type i = 0; i < filledMap.size(); i++ ) {
      Nan::Set(a, i, Nan::New(filledMap[i]));
    }
    Nan::HandleScope scope;
    v8::Local<v8::Value> argv[] = {
      Nan::Null(), // no error occured
      a
    };
    callback->Call(2, argv);
  }

  void HandleErrorCallback() {
    Nan::HandleScope scope;
    v8::Local<v8::Value> argv[] = {
      Nan::New(this->ErrorMessage()).ToLocalChecked(), // return error message
      Nan::Null()
    };
    callback->Call(2, argv);
  }

};

#endif

Map mainMap;

NAN_METHOD(initArray){
  if (!info[0]->IsNumber() || !info[1]->IsNumber() || !info[2]->IsNumber() || !info[3]->IsNumber() ) {
    Nan::ThrowTypeError("Argument must be a number");
    return;
  }
  int arrW = info[0]->NumberValue();
  int arrH = info[1]->NumberValue();
  int rangeMin = info[2]->NumberValue();
  int rangeMax = info[3]->NumberValue();
  mainMap.init( arrW, arrH, rangeMin, rangeMax );
  int arrLength = mainMap.getLength();
  v8::Local<v8::Array> a = Nan::New<v8::Array>();
  std::vector<int> thisArr = mainMap.getArray();
  for (int i = 0; i < arrLength; i++ ) {
    Nan::Set(a, i, Nan::New(thisArr[i]));
  }
  info.GetReturnValue().Set(a);
}
NAN_METHOD(GetArrayLength){
  v8::Local<v8::Number> arrL = Nan::New(mainMap.getLength());
  info.GetReturnValue().Set(arrL);
}
NAN_METHOD(getArray){
  //v8::Local<v8::Array> a = Nan::New(arr);
  //v8::Local<v8::Array> a = New<v8::Array>(arrLength);
  v8::Local<v8::Array> a = Nan::New<v8::Array>();
  int length = mainMap.getLength();
  std::vector<int> map = mainMap.getArray();
  //Local<Array> a = New<v8::Array>(arrLength);
  for (int i = 0; i < length; i++ ) {
    //a[i] = arr[i];
    Nan::Set(a, i, Nan::New(map[i]));
  }
  info.GetReturnValue().Set(a);
  //info.GetReturnValue().Set(a);
}
NAN_METHOD(getFilledMap){
  if ( !info[0]->IsNumber() || !info[1]->IsNumber() ) {
    Nan::ThrowTypeError("Argument must be a number");
    return;
  }
  int x = info[0]->NumberValue();
  int y = info[1]->NumberValue();
  mainMap.fillMap( x, y );
  std::vector<int> filledMap = mainMap.getFilledMap( x, y );
  int arrLength = mainMap.getLength();
  v8::Local<v8::Array> a = Nan::New<v8::Array>();
  for (int i = 0; i < arrLength; i++ ) {
    Nan::Set(a, i, Nan::New(filledMap[i]));
  }
  info.GetReturnValue().Set(a);
}
NAN_METHOD(asyncGetFilledMap) {
  if ( !info[0]->IsNumber() || !info[1]->IsNumber() ) {
    Nan::ThrowTypeError("Argument must be a number");
    return;
  }
  if(!info[2]->IsFunction()) {
    return Nan::ThrowError(Nan::New("expected arg 2: function callback").ToLocalChecked());
  }

  // starting the async worker
  Nan::AsyncQueueWorker(new FillMapAsyncWorker(
    info[0]->NumberValue(),
    info[1]->NumberValue(),
    mainMap,
    new Nan::Callback(info[2].As<v8::Function>())
  ));
}
NAN_MODULE_INIT(Initialize) {
  NAN_EXPORT(target, initArray);
  NAN_EXPORT(target, GetArrayLength);
  NAN_EXPORT(target, getArray);
  NAN_EXPORT(target, getFilledMap);
  NAN_EXPORT(target, asyncGetFilledMap);
}
NODE_MODULE(module_name, Initialize)
