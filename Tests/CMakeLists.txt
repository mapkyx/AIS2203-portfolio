<<<<<<< Updated upstream

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
=======
add_executable(controller_tests controller_tests.cpp
        /Users/johanneslervag/Documents/3.Året automasjon/Sanntids datateknikk/AIS2203-portfolio/Src/SpheroRVRController.cpp
        /Users/johanneslervag/Documents/3.Året automasjon/Sanntids datateknikk/AIS2203-portfolio/Src/SpheroRVRMessageHandler.cpp)
add_test(NAME controller_tests COMMAND controller_tests)
target_link_libraries(controller_tests PRIVATE Catch2::Catch2WithMain)

add_executable(helper_tests helper_tests.cpp)
add_test(NAME helper_tests COMMAND helper_tests)
target_link_libraries(helper_tests PRIVATE Catch2::Catch2WithMain)

add_executable(udp_tests udp_tests.cpp)
add_test(NAME udp_tests COMMAND udp_tests)
target_link_libraries(udp_tests PRIVATE Catch2::Catch2WithMain)

#include(CTest)
include(Catch)
catch_discover_tests(controller_tests)
catch_discover_tests(helper_tests)
catch_discover_tests(udp_tests)
>>>>>>> Stashed changes
