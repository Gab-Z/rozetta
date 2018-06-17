{
  "targets": [
    {
      "target_name": "towerdef",
      "sources": [  "./cpp/index.cpp",
                    "./cpp/classes/converter/converter.cpp",
                    "./cpp/classes/TowerDefense/TowerDefense.cpp",
                    "./cpp/classes/Tile/Tile.cpp",
                    "./cpp/classes/Structure/Structure.cpp",
                    "./cpp/classes/Structure/StructureDef.cpp",
                    "./cpp/classes/Structure/structuresDefList.cpp",
                    "./cpp/classes/Structure/index.cpp",
                    "./cpp/classes/Floor/Floor.cpp",
                    "./cpp/classes/Floor/floorsList.cpp",
                    "./cpp/classes/GameLevel/GameLevelBase.cpp",
                    "./cpp/classes/GameLevel/GameLevel.cpp",
                    "./cpp/classes/GameLevel/DestinationPt.cpp",
                    "./cpp/classes/GameLevel/TethaSearchTile.cpp",
                    "./cpp/classes/GameLevel/index.cpp",
                    "./cpp/classes/Character/Character.cpp",
                    "./cpp/classes/Character/CharacterType.cpp",
                    "./cpp/classes/Character/characterTypeList.cpp",
                    "./cpp/classes/Character/MovePattern.cpp",
                    "./cpp/classes/Character/Movement.cpp",
                    "./cpp/classes/Character/MoveData.cpp",
        ],
      "include_dirs" : [
          "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
