cmake_minimum_required(VERSION 2.8)

project(Y86-64-simulator)

set (CMAKE_CXX_STANDARD 17)

aux_source_directory(. DIR_SRC)
add_subdirectory(programstack)
add_subdirectory(utility)
add_executable(test ${DIR_SRC})
target_link_libraries(test PrgStack Utility)
