add_library(sourceLibrary
        message_handler.cpp
        KeyboardListener.cpp
        SpheroRVRController.cpp
        SpheroRVRMessageHandler.cpp
        udp_client.cpp
        )

add_subdirectory(udp)

target_include_directories(sourceLibrary PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
        )

target_link_libraries(sourceLibrary PRIVATE
        nlohmann_json::nlohmann_json
        ${OpenCV_LIBRARIES}
        Boost::boost
        ws2_32
        udpLibrary
        )