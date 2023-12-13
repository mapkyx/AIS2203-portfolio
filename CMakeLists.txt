cmake_minimum_required(VERSION 3.17)
project(PortfolioRobot)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(Boost_NO_WARN_NEW_VERSIONS ON)

enable_testing()

find_package(nlohmann_json CONFIG REQUIRED)
find_package(OpenCV CONFIG REQUIRED)
find_package(realsense2 CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS thread)
find_package(Catch2 3 REQUIRED)

if(MSVC)
    add_compile_definitions("_WIN32_WINNT=0x0601")
endif()

if (Catch2_FOUND)
    message("Catch2 found")
else()
    message(FATAL_ERROR "Catch2 not found")
endif()

if(nlohmann_json_FOUND)
    message("nlohmann_json found")
else()
    message(FATAL_ERROR "nlohmann_json not found")
endif()

if(realsense2_FOUND)
    message("Realsense2 found")
else()
    message(FATAL_ERROR "Realsense2 not found")
endif()

if(OpenCV_FOUND)
    message("OpenCV found")
else()
    message(FATAL_ERROR "OpenCV not found")
endif()

if(Boost_FOUND)
    message("Boost found")
else()
    message(FATAL_ERROR "Boost not found")
endif()

include_directories(include)

add_subdirectory(app)
add_subdirectory(source)
add_subdirectory(tests)