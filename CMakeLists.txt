cmake_minimum_required(VERSION 3.21)

project(Sphero_rvr)

set(CMAKE_CXX_STANDARD 17)

option(NETWORKING_BUILD_TESTS "Enable tests" ON)
set(Boost_NO_WARN_NEW_VERSIONS 1)

find_package(OpenCV CONFIG REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(realsense2 CONFIG REQUIRED)


include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${boost_INCLUDE_DIRS})
include_directories(${nlohmann_json_INCLUDE_DIRS})
include_directories(${realsense2_INCLUDE_DIRS})

add_subdirectory(Src)
add_subdirectory(Tests)
add_subdirectory(App)
add_subdirectory(Include)



include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(Catch2)

enable_testing()