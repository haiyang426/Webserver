include_directories(${PROJECT_SOURCE_DIR}/include)

aux_source_directory(. DIR_LIB_EVENT_SRCS)

add_library(libserver_event ${DIR_LIB_EVENT_SRCS})
