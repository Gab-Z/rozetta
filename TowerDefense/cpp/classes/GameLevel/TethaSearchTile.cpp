#include "TethaSearchTile.h"

TethaSearchTile::TethaSearchTile(){};

TethaSearchTile::TethaSearchTile( int _parentPos, float _hVal ){
  parentPos = _parentPos;
  hVal = _hVal;
};

void TethaSearchTile::setParentPos( int _parentPos ){
  parentPos = _parentPos;
}

void TethaSearchTile::setHVal( float _hVal ){
  hVal = _hVal;
}

void TethaSearchTile::set( int _parentPos, float _hVal ){
  parentPos = _parentPos;
  hVal = _hVal;
}
