add_executable(controller_tests controller_tests.cpp
        source/SpheroRVRController.cpp
        source/SpheroRVRMessageHandler.cpp
        )
add_test(NAME controller_tests COMMAND controller_tests)
target_link_libraries(controller_tests PRIVATE Catch2::Catch2WithMain)

add_executable(helper_tests helper_tests.cpp)
add_test(NAME helper_tests COMMAND helper_tests)
target_link_libraries(helper_tests PRIVATE Catch2::Catch2WithMain)

add_executable(udp_tests udp_tests.cpp
        source/udp_client.cpp)
add_test(NAME udp_tests COMMAND udp_tests)
target_link_libraries(udp_tests PRIVATE Catch2::Catch2WithMain)

#include(CTest)
include(Catch)
catch_discover_tests(controller_tests)
catch_discover_tests(helper_tests)
catch_discover_tests(udp_tests)
