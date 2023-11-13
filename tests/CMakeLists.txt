cmake_minimum_required(VERSION 3.17)

project(unit_testing)

# Prepare "Catch2" library for other executables
add_library(Catch2 INTERFACE)
add_library(Catch2::Test ALIAS Catch2)
target_include_directories(Catch2 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})