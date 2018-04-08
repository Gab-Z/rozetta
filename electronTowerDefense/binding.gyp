{
  "targets": [
    {
      "target_name": "towerdef",
      "sources": [  "./cpp/towerdef_Main.cpp",
                    "./cpp/towerdef.cpp",
                    "./cpp/towerdef_Map.cpp",
                    "./cpp/towerdef_PathMap.cpp",
                    "./cpp/towerdef_Structure.cpp",
                    "./cpp/towerdef_oneTileStruct.cpp",
                    "./cpp/towerdef_Struct_Wall.cpp",
                    "./cpp/Grid.cpp",
                    "./cpp/GameMap.cpp",
                    "./cpp/PointList.cpp"
                    ],
      "include_dirs" : [
          "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
