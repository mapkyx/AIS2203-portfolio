add_library(sourceLibrary
        functions.cpp
        GUI.cpp
        UDPReceiver.cpp
)

target_include_directories(sourceLibrary PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_link_libraries(sourceLibrary PRIVATE
        nlohmann_json::nlohmann_json
        ${OpenCV_LIBRARIES}
        Boost::boost
)
