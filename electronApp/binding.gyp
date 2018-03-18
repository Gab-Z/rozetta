{
  "targets": [
    {
      "target_name": "module",
      "sources": [ "test.cpp" ],
      "include_dirs" : [
          "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
