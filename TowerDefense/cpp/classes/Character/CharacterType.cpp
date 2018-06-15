#include "CharacterType.h"

CharacterType::CharacterType( std::string _typeName ){
  typeName = _typeName;
}
CharacterType::CharacterType( std::string _typeName, int _maxLife, MovePattern* _movePattern ){
  typeName = _typeName;
  maxLife = _maxLife;
  movePattern = _movePattern;
}
std::string CharacterType::getTypeName(){ return typeName; };
