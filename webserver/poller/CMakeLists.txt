include_directories(${PROJECT_SOURCE_DIR}/include)

aux_source_directory(. DIR_LIB_POLL_SRCS)

add_library(libserver_poll ${DIR_LIB_POLL_SRCS})
