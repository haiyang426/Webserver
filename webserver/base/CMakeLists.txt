include_directories(${PROJECT_SOURCE_DIR}/include)

aux_source_directory(. DIR_LIB_BASE_SRCS)

add_library(libserver_base ${DIR_LIB_BASE_SRCS})
