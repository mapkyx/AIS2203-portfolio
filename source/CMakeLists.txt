# Assuming main.cpp is the source file for your main executable
add_executable(${PROJECT_NAME} ../main.cpp
        GUI.cpp)

# Link libraries to your main executable
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} PRIVATE ${realsense2_LIBRARY})
target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::thread)
