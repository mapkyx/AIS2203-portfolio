add_executable(tests tests.cpp)

# Link libraries to your tests executable
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(tests PRIVATE nlohmann_json nlohmann_json::nlohmann_json)
target_link_libraries(tests PRIVATE ${realsense2_LIBRARY})
target_link_libraries(tests PRIVATE ${OpenCV_LIBRARIES})
target_link_libraries(tests PRIVATE Boost::thread)

# Register the tests
include(CTest)
include(Catch)
catch_discover_tests(tests)
