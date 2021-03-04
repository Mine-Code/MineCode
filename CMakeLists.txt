cmake_minimum_required( VERSION 2.8 )

project(MineCode CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(lib/Assembler)
include_directories("src/include")

add_executable(MineCode
    src/asm.cpp
    src/condeval.cpp
    src/condutil.cpp
    src/eval.cpp
    src/main.cpp
    src/mcl.cpp
    src/myassert.cpp
    src/optimizer.cpp
    src/parserCore.cpp
    src/parserTypes.cpp
    src/parserWrap.cpp
    src/stmtProcessor.cpp
    src/syntaxError.cpp
    src/util.cpp
)
target_link_libraries(MineCode ppcasm)