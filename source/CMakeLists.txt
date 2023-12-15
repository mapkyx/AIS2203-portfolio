add_library(sourceLibrary
<<<<<<< Updated upstream
        message_handler.cpp
        KeyboardListener.cpp
        SpheroRVRController.cpp
        SpheroRVRMessageHandler.cpp
        udp_client.cpp
        )
=======
        functions.cpp
        UDPReceiver.cpp
        camera_feed.cpp
        display_rvr_state.cpp
        keyboard_listener.cpp
        SpheroRVRController.cpp
        SpheroRVRMessageHandler.cpp
)
>>>>>>> Stashed changes

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