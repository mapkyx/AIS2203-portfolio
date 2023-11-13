cmake_minimum_required(VERSION 3.17)
project(PortfolioRobot)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(tests)

find_package(nlohmann_json CONFIG REQUIRED)
find_package(OpenCV CONFIG REQUIRED)
find_package(realsense2 CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS thread)
find_package(Catch2 3 REQUIRED)

add_executable(${PROJECT_NAME} hello_world.cpp)

if (Catch2_FOUND)
    message("Catch2 found")
    target_link_libraries(${PROJECT_NAME} PRIVATE Catch2::Catch2)
else()
    message(FATAL_ERROR "Catch2 not found")
endif ()

if(nlohmann_json_FOUND)
    message("nlohmann_json found")
    target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json nlohmann_json::nlohmann_json)
else()
    message(FATAL_ERROR "nlohmann_json not found")
endif()

if(realsense2_FOUND)
    message("Realsense2 found")
    target_link_libraries(${PROJECT_NAME} PRIVATE ${realsense2_LIBRARY})
else()
    message(FATAL_ERROR "Realsense2 not found")
endif()

if(OpenCV_FOUND)
    message("OpenCV found")
    target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV_LIBRARIES})
else()
    message(FATAL_ERROR "OpenCV not found")
endif()

if(Boost_FOUND)
    message("Boost found")
    target_link_libraries(${PROJECT_NAME} PRIVATE Boost::thread)
else()
    message(FATAL_ERROR "Boost not found")
endif()

enable_testing()