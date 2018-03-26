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
