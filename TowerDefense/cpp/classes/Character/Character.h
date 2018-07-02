#ifndef DEF_Character
#define DEF_Character

#include "../geometry/Vec2.h"
#include "CharacterType.h"


class Character {

  static int idCounter;

  int id;
  int life;

  Vec2<float> pos;
  Vec2<float> dir;
  Vec2<float> startPos;
  float startTime;
  Vec2<float> targetPos;
  float moveDuration;
  Vec2<int> targetTilePos;
  CharacterType* type;

  public:

    Character( CharacterType* _type );
    void setPos( float _x, float _y );
    void addPos( float _x, float _y );
    Vec2<float> getPos();
    void setDir( float _x, float _y );
    Vec2<float> getDir();
    void setStartPos( float _x, float _y );
    Vec2<float> getStartPos();
    void setStartTime( float _startTime );
    float getStartTime();
    void setTargetPos( float _x, float _y );
    Vec2<float> getTargetPos();
    void setMoveDuration( float _duration );
    float getMoveDuration();
    void setTargetTilePos( int _x, int _y );
    Vec2<int> getTargetTilePos();

};

#endif
