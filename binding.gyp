{
  "targets": [
    {
      "target_name": "addon",
      "sources": [
        "native/src/addon.cpp",
        "native/src/PrinterBuilder.cpp",
        "native/src/PrinterInterface.cpp"
      ],
      "include_dirs": [
        "native/src",
        "<!(node -p \"require('node-addon-api').include\")"
      ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').targets\"):node_addon_api_maybe"
      ],
      "cflags!": ["-fno-exceptions"],
      "cflags_cc!": ["-fno-exceptions"],
      'msvs_settings': {
          'VCCLCompilerTool': {
            'ExceptionHandling': '1',    
            'AdditionalOptions': ['/EHsc']
          }
      },
      "conditions": [
        [
          "OS=='win'",
          {
            "sources": ["native/src/PrinterWin.cpp"],
            "defines": ["_WIN32"]
          }
        ],
        [
          "OS!='win'",
          {
            "sources": ["native/src/PrinterPosix.cpp"],
            "libraries": ["-lcups"]
          }
        ]
      ]
    }
  ]
}
