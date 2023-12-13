add_executable(PortfolioRobot main.cpp)

target_link_libraries(PortfolioRobot PRIVATE
        sourceLibrary # Replace with the actual names of your libraries
        nlohmann_json::nlohmann_json
        ${OpenCV_LIBRARIES}
        ${realsense2_LIBRARY}
        Boost::thread
)
