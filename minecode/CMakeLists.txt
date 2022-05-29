include(CTest)
enable_testing()

add_executable(minecode)
add_subdirectory(src)
target_include_directories(minecode PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
