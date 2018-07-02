#include "Character.h"

int Character::idCounter = 0;

Character::Character( CharacterType* _type ){
  type = _type;
  Character::idCounter++;
  id = Character::idCounter;
  life = type->getMaxLife();
}

void Character::setPos( float _x, float _y ){
  pos.x = _x; pos.y = _y;
}
void Character::addPos( float _x, float _y ){
  pos.x += _x; pos.y += _y;
}
Vec2<float> Character::getPos(){
  return pos;
}
void Character::setDir( float _x, float _y ){
  dir.x = _x; dir.y = _y;
}
Vec2<float> Character::getDir(){
  return dir;
}
void Character::setStartPos( float _x, float _y ){
  startPos.x = _x; startPos.y = _y;
}
Vec2<float> Character::getStartPos(){
  return startPos;
}
void Character::setStartTime( float _startTime ){
  startTime = _startTime;
}
float Character::getStartTime(){
  return startTime;
}
void Character::setTargetPos( float _x, float _y ){
  targetPos.x = _x; targetPos.y = _y;
}
Vec2<float> Character::getTargetPos(){
  return targetPos;
}
void Character::setMoveDuration( float _duration ){
  moveDuration = _duration;
}
float Character::getMoveDuration(){
  return moveDuration;
}
void Character::setTargetTilePos( int _x, int _y ){
  targetTilePos.x = _x; targetTilePos.y = _y;
}
Vec2<int> Character::getTargetTilePos(){
  return targetTilePos;
}
/*

int id;
int life;

//float posx;
//float posy;
Vec2<float> pos;

//float dirx;
//float diry;
Vec2<float> dir;

//float startx;
//float starty;
Vec2<float> startPos;
float startTime;

//float endx;
//float endy;
Vec2<float> targetPos;
float moveDuration;

//int targetTilex;
//int targetTiley;
Vec2<int> targetTilePos;

CharacterType* type;


*/
