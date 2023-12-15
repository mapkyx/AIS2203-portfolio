add_executable(PortfolioRobot main.cpp)

target_link_libraries(PortfolioRobot PRIVATE
        sourceLibrary
        udpLibrary
        nlohmann_json::nlohmann_json
        ${OpenCV_LIBRARIES}
        ${realsense2_LIBRARY}
        Boost::thread
        SDL2::SDL2
        SDL2::SDL2main
        ws2_32
        )
