{
  "targets": [
    {
      "target_name": "towerdef_module",
      "sources": [ "./cpp/towerdef_Main.cpp" ],
      "include_dirs" : [
          "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
