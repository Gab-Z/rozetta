#ifndef DEF_structuresDefList
#define DEF_structuresDefList

#include <nan.h>
#include "StructureDef.h"

namespace structuresDefList {

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
                      "shape_L.png", 20, 2, 3 ),

    new StructureDef( "canon_lvl1",
                      std::vector<int>{ 1 },
                      "canon_lvl1.png", 20, 1, 1, false, std::vector<std::string> { "canon_lvl2", "canon_lvl2b" } ),

    new StructureDef( "canon_lvl2",
                      std::vector<int>{ 1 },
                      "canon_lvl2.png", 40, 1, 1, false, 2, "thumb_canon_lvl2.png" ),

    new StructureDef( "canon_lvl2b",
                      std::vector<int>{ 1 },
                      "canon_lvl2b.png", 40, 1, 1, false, 2, "thumb_canon_lvl2b.png" )

  };

};

#endif
