cmake_minimum_required(VERSION 3.17)
project(PortfolioRobot)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_TOOLCHAIN_FILE "C:/Users/Admin/vcpkg/scripts/buildsystems/vcpkg.cmake")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(nlohmann_json CONFIG REQUIRED)
find_package(protobuf CONFIG REQUIRED)
find_package(boost CONFIG REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)

add_subdirectory(protobuf)

add_executable(${PROJECT_NAME} main.cpp
        src/Camera/camera_control.cpp
        src/Camera/camera_control.h
        src/GUI/gui.cpp
        src/GUI/gui.h
        src/RVR-Control/rvr_controller.cpp
        src/RVR-Control/rvr_controller.h)

target_include_directories(main PRIVATE "${CMAKE_SOURCE_DIR}/include")

target_link_libraries(main
        PRIVATE
        proto_msg
        nlohmann_json::nlohmann_json
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
)