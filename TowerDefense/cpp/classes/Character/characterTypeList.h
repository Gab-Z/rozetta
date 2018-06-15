#ifndef DEF_characterTypeList
#define DEF_characterTypeList

#include "CharacterType.h"
#include <vector>

namespace characterTypeList {

  static std::vector<CharacterType*> characterTypes = {
    new CharacterType( "null" )
  };

  CharacterType* getCharacterTypeByName( std::string _typeName );

};

#endif
