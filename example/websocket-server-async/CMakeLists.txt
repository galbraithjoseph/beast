# Part of Beast

GroupSources(include/beast beast)
GroupSources(example/common common)
GroupSources(example/websocket-server-async "/")

add_executable (websocket-server-async
    ${BEAST_INCLUDES}
    ${COMMON_INCLUDES}
    websocket_server_async.cpp
)

target_link_libraries(websocket-server-async
    Beast
    )
