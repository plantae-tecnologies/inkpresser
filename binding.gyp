{
  "targets": [
    {
      "target_name": "addon",
      "sources": ["src/addon.cpp", "src/printer.cpp"],
      "include_dirs": [
        "<!(node -p \"require('node-addon-api').include\")"
      ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').targets\"):node_addon_api_maybe"
      ],
      "cflags!": ["-fno-exceptions"],
      "cflags_cc!": ["-fno-exceptions"],
      "conditions": [
        ["OS=='win'", { "defines": ["_WIN32"] }],
        ["OS!='win'", { "libraries": ["-lcups"] }]
      ]
    }
  ]
}
