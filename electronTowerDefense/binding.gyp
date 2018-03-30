{
  "targets": [
    {
      "target_name": "towerdef",
      "sources": [  "./cpp/towerdef_Main.cpp",
                    "./cpp/towerdef.cpp",
                    "./cpp/towerdef_Map.cpp",
                    "./cpp/towerdef_PathMap.cpp"
                    ],
      "include_dirs" : [
          "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
