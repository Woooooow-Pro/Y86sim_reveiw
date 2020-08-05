# ./src/programstack/CMakeLists.txt

aux_source_directory(. DIR_SRC_PRGSTK)
add_library(PrgStack STATIC ${DIR_SRC_PRGSTK})
target_link_libraries(PrgStack Utility)