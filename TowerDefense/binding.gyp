{
  "targets": [
    {
      "target_name": "towerdef",
      "sources": [  "./cpp/index.cpp",
                    "./cpp/classes/Converter/Converter.cpp",
                    "./cpp/classes/TowerDefense/TowerDefense.cpp"
                    ],
      "include_dirs" : [
          "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
