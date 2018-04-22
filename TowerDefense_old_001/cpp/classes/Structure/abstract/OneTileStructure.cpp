#include "OneTileStructure.h"

OneTileStructure::OneTileStructure():StructureCommons(){

}
OneTileStructure::OneTileStructure( int _x, int _y ):StructureCommons( _x, _y, std::vector<int> { 0 }, 1, 1 ){

}
