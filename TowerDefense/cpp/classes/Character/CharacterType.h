#ifndef DEF_CharacterType
#define DEF_CharacterType

#include<string>


class CharacterType {

  void ( *moveFunc )( int timestamp );
  std::string typeName;
  int maxLife;
  int speed;

  public:

    CharacterType( std::string _typeName );
    CharacterType( std::string _typeName, int _maxLife, int _speed, void( *_moveFunc )( int _timestamp ) );

    std::string getTypeName();
    int getMaxLife();
    int getSpeed();

};

#endif
