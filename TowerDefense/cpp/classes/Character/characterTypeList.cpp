#include "characterTypeList.h"

CharacterType* characterTypeList::getCharacterTypeByName( std::string _typeName ){
  int l = characterTypeList::characterTypes.size();
  for( int i = 0; i < l; i++ ){
    if( characterTypeList::characterTypes[ i ]->getTypeName() == _typeName ){
      return characterTypeList::characterTypes[ i ];
    }
  }
  return characterTypeList::characterTypes[ 0 ];
}
