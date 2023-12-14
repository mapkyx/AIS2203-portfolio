add_library(sourceLibrary
        functions.cpp
<<<<<<< Updated upstream
        GUI.cpp
        UDPReceiver.cpp
=======
        camera_feed.cpp
        display_rvr_state.cpp
        keyboard_listener.cpp
>>>>>>> Stashed changes
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
