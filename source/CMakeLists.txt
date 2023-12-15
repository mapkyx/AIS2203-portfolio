add_library(sourceLibrary
        camera_feed.cpp
        display_rvr_state.cpp
        keyboard_listener.cpp
        message_handler.cpp
        SpheroRVRController.cpp
        SpheroRVRMessageHandler.cpp
        udp_client.cpp
        udp_listener.cpp
)

target_include_directories(sourceLibrary PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_link_libraries(sourceLibrary PRIVATE
        nlohmann_json::nlohmann_json
        ${OpenCV_LIBRARIES}
        Boost::boost
        ws2_32
)