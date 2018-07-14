#ifndef DEF_Vec2
#define DEF_Vec2

template<typename T>

struct Vec2 {

  T x;
  T y;

  Vec2(){};
  Vec2( T _x, T _y ):x( _x ), y( _y ){};

  friend Vec2<T> operator+( const Vec2<T>& first, const Vec2<T>& second ){
    return Vec2<T>( first.x + second.x, first.y + second.y );
  }

};


#endif
