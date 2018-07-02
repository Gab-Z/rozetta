#include "CharacterType.h"

CharacterType::CharacterType( std::string _typeName ){
  typeName = _typeName;
}

CharacterType::CharacterType( std::string _typeName, int _maxLife, int _speed, void( *_moveFunc )( int _timestamp ) ){
  typeName = _typeName;
  maxLife = _maxLife;
  //movePattern = _movePattern;
}

std::string CharacterType::getTypeName(){ return typeName; };
int CharacterType::getMaxLife(){ return maxLife; };
int CharacterType::getSpeed(){ return speed; };
