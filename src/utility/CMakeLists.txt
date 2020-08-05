# ./src/utility/CMakeLists.txt

aux_source_directory(. DIR_SRC_UTILITY)
add_library(Utility STATIC ${DIR_SRC_UTILITY})