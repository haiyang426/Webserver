include_directories(${PROJECT_SOURCE_DIR}/include)

aux_source_directory(. DIR_LIB_HTTP_SRCS)

add_library(libserver_http ${DIR_LIB_HTTP_SRCS})
