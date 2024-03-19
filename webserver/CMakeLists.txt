include_directories(${PROJECT_SOURCE_DIR}/include)

aux_source_directory(. DIR_SRCS)

# add_executable(WebServer ${DIR_SRCS})

# target_link_libraries(WebServer libserver_base, libserver_http)
add_library(libserver_webserver ${DIR_SRCS})
target_link_libraries(libserver_webserver libserver_base libserver_http)

add_subdirectory(base)
add_subdirectory(http)
