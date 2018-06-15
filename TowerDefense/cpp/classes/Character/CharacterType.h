#ifndef DEF_CharacterType
#define DEF_CharacterType

#include<string>
#include "MovePattern.h"


class CharacterType {

  MovePattern* movePattern;
  std::string typeName;
  int maxLife;
  int speed;

  public:

    CharacterType( std::string _typeName );
    CharacterType( std::string _typeName, int _maxLife, MovePattern* _movePattern );

    std::string getTypeName();

};

#endif
