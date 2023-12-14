add_library(udpLibrary
        udp_client.cpp
        udp_listener.cpp
)

target_include_directories(udpLibrary PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
)