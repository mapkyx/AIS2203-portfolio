
include_directories("${CMAKE_SOURCE_DIR}/include")
link_libraries(Catch2::Catch2)

if (UNIX)
    link_libraries(pthread)
endif()

add_executable(test_controller controller_tests.cpp)
add_test(NAME test_controller COMMAND test_controller)

add_executable(test_helper helper_tests.cpp)
add_test(NAME test_helper COMMAND test_helper)

add_executable(test_udp udp_tests.cpp)
add_test(NAME test_udp COMMAND test_udp)