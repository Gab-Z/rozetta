{
  "targets": [
    {
      "target_name": "towerdef",
      "sources": [  "./cpp/index.cpp",
                    "./cpp/classes/misc/misc.cpp",
                    "./cpp/classes/converter/converter.cpp",
                    "./cpp/classes/TowerDefense/TowerDefense.cpp",
                    "./cpp/classes/Tile/Tile.cpp",
                    "./cpp/classes/Structure/Structure.cpp",
                    "./cpp/classes/Structure/StructureDef.cpp",
                    "./cpp/classes/Structure/structuresDefList.cpp",
                    "./cpp/classes/Structure/index.cpp",
                    "./cpp/classes/Floor/Floor.cpp",
                    "./cpp/classes/Floor/floorsList.cpp",
                    "./cpp/classes/GameLevel/Level.cpp",
                    "./cpp/classes/GameLevel/DestinationPt.cpp",
                    "./cpp/classes/GameLevel/TethaSearchTile.cpp",
                    "./cpp/classes/GameLevel/index.cpp",
                    "./cpp/classes/Character/Character.cpp",
                    "./cpp/classes/Character/CharacterType.cpp",
                    "./cpp/classes/Character/characterTypeList.cpp",
                    "./cpp/classes/Character/moveFuncs.cpp",
                    "./cpp/classes/geometry/Vec2.cpp",
                    "./cpp/classes/geometry/GridPos.cpp"

        ],
      "include_dirs" : [
          "<!(node -e \"require('nan')\")"
      ]
    }

  ]
}
