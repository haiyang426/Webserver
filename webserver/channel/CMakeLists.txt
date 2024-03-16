include_directories(${PROJECT_SOURCE_DIR}/include)

aux_source_directory(. DIR_LIB_CHANNEL_SRCS)

add_library(libserver_channel ${DIR_LIB_CHANNEL_SRCS})
