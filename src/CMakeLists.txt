cmake_minimum_required(VERSION 3.10)

project(Y86-64-simulator)

set (CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_BUILD_TYPE RelWithDebInfo)

aux_source_directory(. DIR_SRC)
add_subdirectory(stages)
add_subdirectory(programstack)
add_subdirectory(utility)
add_executable(test ${DIR_SRC})
target_link_libraries(test PrgStack Stages Utility)
