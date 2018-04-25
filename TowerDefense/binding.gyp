{
  "targets": [
    {
      "target_name": "towerdef",
      "sources": [  "./cpp/index.cpp",
                    "./cpp/classes/Converter/Converter.cpp",
                    "./cpp/classes/TowerDefense/TowerDefense.cpp",
                    "./cpp/classes/Tile/Tile.cpp",
                    "./cpp/classes/Structure/Structure.cpp",
                    "./cpp/classes/Structure/Wall.cpp",
                    "./cpp/classes/Structure/Cross.cpp",
                    "./cpp/classes/Structure/index.cpp",
                    "./cpp/classes/Floor/Floor.h"
        ],
      "include_dirs" : [
          "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
