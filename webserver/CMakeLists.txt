include_directories(${PROJECT_SOURCE_DIR}/include)

aux_source_directory(. DIR_SRCS)

add_executable(WebServer ${DIR_SRCS})

target_link_libraries(WebServer libserver_base)


add_subdirectory(base)
# add_subdirectory(channel)
# add_subdirectory(event)
# add_subdirectory(poller)