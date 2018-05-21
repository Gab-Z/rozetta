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
                    "./cpp/classes/Structure/StructuresDefList.cpp",
                    "./cpp/classes/Structure/index.cpp",
                    "./cpp/classes/Floor/Floor.cpp",
                    "./cpp/classes/Floor/FloorsList.cpp",
                    "./cpp/classes/GameLevel/GameLevelBase.cpp",
                    "./cpp/classes/GameLevel/GameLevel.cpp",
                    "./cpp/classes/GameLevel/index.cpp"
        ],
      "include_dirs" : [
          "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
