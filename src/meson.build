minecode = executable('minecodec',
  [
    'src/asm.cpp', 
    'src/condeval.cpp', 
    'src/condutil.cpp', 
    'src/eval.cpp', 
    'src/main.cpp', 
    'src/mcl.cpp', 
    'src/myassert.cpp', 
    'src/optimizer.cpp', 
    'src/parserCore.cpp', 
    'src/parserTypes.cpp', 
    'src/parserWrap.cpp', 
    'src/stmtProcessor.cpp', 
    'src/syntaxError.cpp', 
    'src/util.cpp'
  ],
  include_directories: include_directories('include'),
  dependencies:[
    dependency('tinyasm'),
    dependency('tinyutil')
  ]
)