{
  "targets": [
    {
      "target_name": "towerdef",
      "sources": [  "./cpp/index.cpp",
                    "./cpp/classes/Grid/GridDimensions.cpp",
                    "./cpp/classes/Grid/GridIntVec.cpp",
                    "./cpp/classes/Grid/GridIntFK.cpp",
                    "./cpp/classes/Grid/GridIndicesList.cpp",
                    "./cpp/classes/TowerDefense/TowerDefense.cpp",
                    "./cpp/classes/PointList/PointList.cpp",
                    "./cpp/classes/Structure/abstract/StructureCommons.cpp",
                    "./cpp/classes/Structure/abstract/OneTileStructure.cpp"
                    ],
      "include_dirs" : [
          "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
