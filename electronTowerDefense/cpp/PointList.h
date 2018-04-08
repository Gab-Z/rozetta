class PointList {
  std::vector<int> vec;

  public:
    PointList();
    PointList( std::vector<int> v );
    void set( std::vector<int> v );
    void set( int i, int val );
    void set( int _x, int _y, int val );
    std::vector<int> get();
    int get( int i );
    int get( int _x, int _y );
    void push( int val );
};
