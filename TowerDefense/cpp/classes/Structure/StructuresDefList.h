#ifndef DEF_StructuresDefList
#define DEF_StructuresDefList

#include <nan.h>
#include "StructureDef.h"

namespace StructuresDefList {

  StructureDef* getStructureTypeByName( std::string _typeName );
  v8::Local<v8::Array> structuresDefs();

  static std::vector<StructureDef*> structureTypes = {

    new StructureDef( "null",
                      std::vector<int>(),
                      "null", 0, 0, 0 ),

    new StructureDef( "Wall",
                      std::vector<int>{ 1 },
                      "wall.jpg", 10, 1, 1 ),

    new StructureDef( "Cross",
                      std::vector<int>{
                        0,  1,  0,
                        1,  1,  1,
                        0,  1,  0 },
                      "cross.png", 60, 3, 3 ),

    new StructureDef( "shapeL",
                      std::vector<int>{
                        0,  1,
                        0,  1,
                        1,  1 },
                      "shape_L.png", 20, 2, 3 )
  };

};

#endif
