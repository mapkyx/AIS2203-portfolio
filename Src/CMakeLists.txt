add_library(mylib myHeader.cpp)

target_link_libraries(mylib PUBLIC ${OpenCV_LIBARIES})
target_link_libraries(mylib PUBLIC ${Boost_LIBARIES})

target_include_directories(mylib PUBLIC ${CMAKE_SOURCE_DIR}/include)

