subdir('src')
minecode = executable('minecodec',
  sources,
  include_directories: include_directories('include'),
  dependencies:[
    dependency('tinyasm'),
    dependency('tinyutil'),
    subproject('nlohmann_json').get_variable('nlohmann_json_dep'),
  ]
)