#include <nan.h>
#include <cmath>
const int maxValue = 10;
int numberOfCalls = 0;
const std::vector<int> turnCoords { -1,0,   -1,-1,   0,-1,    1,-1,    1,0,   1,1,   0,1,   -1,1 };

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
#ifndef DEF_PATHMAP
#define DEF_PATHMAP

class PathMap{
  public:

    void setStart( int x, int y ){
      startx = x;
      starty = y;
    }
    void clearMap(){
      map.clear();
    }
    void setValue( int index, int value ){
      map[ index ] = value;
    }
    void push( int value ){
      map.push_back( value );
    }
    std::vector<int> getStart(){
      std::vector<int> pos { startx, starty };
      return pos;
    }
    int getValue( int index ){
      return map[ index ];
    }
    std::vector<int> getMap(){
      return map;
    }

  private:
    int startx;
    int starty;
    std::vector<int> map;
};

#endif

#ifndef DEF_MAP
#define DEF_MAP
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

    int getPathMapIndex( int x, int y ){
      int size = paths.size();
      for( int i = 0; i < size; i++ ){
        PathMap& pm = paths[ i ];
        std::vector<int> pos = pm.getStart();
        if( pos[ 0 ] == x && pos[ 1 ] == y ){
          return i;
        }
      }
      return -1;
    }

    std::vector<int> getArray(){
      return arr;
    }

    int getLength(){
      return arrLength;
    }

    std::vector<int> getFilledMap( int x, int y ){
      int index = getPathMapIndex( x, y );
      return paths[ index ].getMap();
    }

    void fillMap( int x, int y ){
      int p1d = to1d( x, y);
      int pathId = getPathMapIndex( x, y );
      if( pathId < 0 ){
        PathMap newPathMap;
        newPathMap.setStart( x, y );
        paths.push_back( newPathMap );
        pathId = getPathMapIndex( x, y );
      }
      PathMap& pathMap = paths[ pathId ];
      pathMap.clearMap();
      for ( int i = 0; i < arrLength; i++ ) {
        pathMap.push( -1 );
      };
      pathMap.setValue( p1d, arr[ p1d ] );
      std::vector<int> searchList;
      searchList.push_back( x );
      searchList.push_back( y );
      for( std::vector<int>::size_type n = 0; n < searchList.size(); n++ ){
        searchList = fillMapStep(  p1d, pathMap, searchList );

      }
    }

    std::vector<int> fillMapStep( int id, PathMap& pathMap,  std::vector<int> searchList ){
      std::vector<int> newList;
      for( std::vector<int>::size_type i = 0; i < searchList.size() / 2; i++ ){
        int x  = searchList[ i * 2 ];
        int y = searchList[ i * 2 + 1 ];
        int pos = to1d( x, y );
        int value = pathMap.getValue( pos );
        if( x > 0 ){
          if( y > 0){
            newList =  testTile( x, y, -1, -1, value, pathMap, newList );
          }
          newList = testTile( x, y, -1, 0, value, pathMap, newList );
          if( y + 1 < arrH ){
            newList =  testTile( x, y, -1, 1, value, pathMap, newList );
          }
        }
        //MIDDLE
        if( y > 0 ){
          newList =  testTile( x, y, 0, -1, value, pathMap, newList );
        }
        if( y + 1 < arrH ){
          newList =  testTile( x, y, 0, 1, value, pathMap, newList );
        }
        //RIGHT
        if( x + 1 < arrW ){
          if( y > 0){
            newList =  testTile( x, y, 1, -1, value, pathMap, newList );
          }
          newList =  testTile( x, y, 1, 0, value, pathMap, newList );
          if( y + 1 < arrH ){
            newList =  testTile( x, y, 1, 1, value, pathMap, newList );
          }
        }
      }
      return newList;
    }

    std::vector<int>  testTile( int srcX, int srcY, int offsetX, int offsetY, int value, PathMap& pathMap, std::vector<int> _list ){
      int tX = srcX + offsetX;
      int tY = srcY + offsetY;
      int pos1d = to1d( tX, tY );
      std::vector<int> newList;
      newList = _list;
      int retTarget = pathMap.getValue( pos1d );
      int srcTarget = arr[ pos1d ];

      if( retTarget < 0 || value + srcTarget < retTarget ){
      //  pathMap[ pos1d ] = value + srcTarget;
        int newVal = value + srcTarget;
        pathMap.setValue( pos1d, newVal );
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

    std::vector<int> getPath( int startx, int starty, int destinationx, int destinationy ){
      int pathMapIndex = getPathMapIndex( destinationx, destinationy );
      if( pathMapIndex < 0 ){
        fillMap( destinationx, destinationy );
        pathMapIndex = getPathMapIndex( destinationx, destinationy );
      }
      PathMap& pathMap = paths[ pathMapIndex ];
      std::vector<int> ret;
      ret.push_back( startx );
      ret.push_back( starty );
      bool end = false;
      for( std::vector<int>::size_type i = 0; i < ret.size(); i+=2 ){
        int x = ret[ i ];
        int y = ret[ i + 1 ];
        int p1d = to1d( x, y );
        int val = pathMap.getValue( p1d );
        int rx = -1;
        int ry = -1;
        for( int j = 0; j < 8; j++ ){
          int tx = x + turnCoords[ j * 2 ];
          int ty = y + turnCoords[ j * 2 + 1 ];
          int t1d = to1d( tx, ty );
          int testVal = pathMap.getValue( t1d );
          if( testVal < val ){
            rx = tx;
            ry = ty;
            val = testVal;
          }
        }
        if( rx > -1 ){
          ret.push_back( rx );
          ret.push_back( ry );
        }
      }
      return ret;
    }

    private:

    std::vector<int> arr;
    int arrW;
    int arrH;
    int arrLength;
    int rangeMin;
    int rangeMax;
    std::vector<PathMap> paths;

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

  FillMapAsyncWorker(int x, int y, Map& _Map, Nan::Callback *callback) : Nan::AsyncWorker(callback) {
    this->x = x;
    this->y = y;
    this->_map = _Map;
  }

  void Execute() {
    // mimic long running task
    //delay(iterations);
    _map.fillMap( x, y );
    this->filledMap = _map.getFilledMap( x, y );
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
class SetDestinationAsyncWorker : public Nan::AsyncWorker {
  public:
    int x;
    int y;
    Map _map;

    SetDestinationAsyncWorker( int x, int y, Map& _Map, Nan::Callback *callback) : Nan::AsyncWorker(callback) {
      this->x = x;
      this->y = y;
      this->_map = _Map;
    }

    void Execute(){
      _map.fillMap( x, y );
    }

    void HandleOKCallback(){
      Nan::HandleScope scope;
      v8::Local<v8::Value> argv[] = {
        Nan::Null(), // no error occured
        Nan::New("done").ToLocalChecked()
      };
      callback->Call(2, argv);
    }

    void HandleErrorCallback(){
      Nan::HandleScope scope;
      v8::Local<v8::Value> argv[] = {
        Nan::New(this->ErrorMessage()).ToLocalChecked(), // return error message
        Nan::Null()
      };
      callback->Call(2, argv);
    }
};
class GetPathAsyncWorker : public Nan::AsyncWorker {
  public:
    int startx;
    int starty;
    int destinationx;
    int destinationy;
    Map _map;
    std::vector<int> ret;

    GetPathAsyncWorker( int sx, int sy, int dx, int dy, Map& _Map, Nan::Callback *callback) : Nan::AsyncWorker(callback){
      this->startx = sx;
      this->starty = sy;
      this->destinationx = dx;
      this->destinationy = dy;
      this->_map = _Map;
    }

    void Execute(){
      this->ret = _map.getPath( startx, starty, destinationx, destinationy );
    }

    void HandleOKCallback() {
      v8::Local<v8::Array> a = Nan::New<v8::Array>();
      for (std::vector<int>::size_type i = 0; i < ret.size(); i++ ) {
        Nan::Set(a, i, Nan::New(ret[i]));
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

NAN_METHOD( initArray ){
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
NAN_METHOD( GetArrayLength ){
  v8::Local<v8::Number> arrL = Nan::New(mainMap.getLength());
  info.GetReturnValue().Set(arrL);
}
NAN_METHOD( getArray ){
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
}

NAN_METHOD( getFilledMap ){
  if ( !info[0]->IsNumber() || !info[1]->IsNumber() ) {
    Nan::ThrowTypeError("Argument must be a number");
    return;
  }
  int x = info[0]->NumberValue();
  int y = info[1]->NumberValue();

  Nan::Callback *callback = new Nan::Callback(info[2].As<v8::Function>());

  mainMap.fillMap( x, y );
  std::vector<int> filledMap = mainMap.getFilledMap( x, y );
  int arrLength = mainMap.getLength();
  v8::Local<v8::Array> a = Nan::New<v8::Array>();
  for (int i = 0; i < arrLength; i++ ) {
    Nan::Set(a, i, Nan::New(filledMap[i]));
  }
  info.GetReturnValue().Set(a);

}

NAN_METHOD( asyncGetFilledMap ) {
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

NAN_METHOD( asyncSetDestination ){
  if ( !info[0]->IsNumber() || !info[1]->IsNumber() ) {
    Nan::ThrowTypeError("Argument must be a number");
    return;
  }
  if(!info[2]->IsFunction()) {
    return Nan::ThrowError(Nan::New("expected arg 2: function callback").ToLocalChecked());
  }
  Nan::AsyncQueueWorker(new SetDestinationAsyncWorker(
    info[0]->NumberValue(),
    info[1]->NumberValue(),
    mainMap,
    new Nan::Callback(info[2].As<v8::Function>())
  ));
}

NAN_METHOD( asyncGetPath ){
  if ( !info[0]->IsNumber() || !info[1]->IsNumber() || !info[2]->IsNumber() || !info[3]->IsNumber() ) {
    Nan::ThrowTypeError("Argument must be a number");
    return;
  }
  if(!info[4]->IsFunction()) {
    return Nan::ThrowError(Nan::New("expected arg 4: function callback").ToLocalChecked());
  }
  Nan::AsyncQueueWorker(new GetPathAsyncWorker(
    info[0]->NumberValue(),
    info[1]->NumberValue(),
    info[2]->NumberValue(),
    info[3]->NumberValue(),
    mainMap,
    new Nan::Callback(info[4].As<v8::Function>())
  ));
}

NAN_MODULE_INIT(Initialize) {
  NAN_EXPORT(target, WhoAmI);
  NAN_EXPORT(target, Increment);
  NAN_EXPORT(target, initArray);
  NAN_EXPORT(target, GetArrayLength);
  NAN_EXPORT(target, getArray);
  NAN_EXPORT(target, getFilledMap);
  NAN_EXPORT(target, asyncGetFilledMap);
  NAN_EXPORT(target, asyncSetDestination);
  NAN_EXPORT(target, asyncGetPath);
}

NODE_MODULE(module_name, Initialize)
