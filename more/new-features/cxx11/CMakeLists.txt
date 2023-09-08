project(cxx11)

cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS "-O3 -Wall -Werror -std=c++11 ${CMAKE_CXX_FLAGS}")

add_executable(tuple tuple.cpp)
add_executable(tuple_construct tuple_construct.cpp)
add_executable(make_tuple make_tuple.cpp)
add_executable(forward_reference forward_reference.cpp)

add_executable(ratio chrono/ratio.cpp)
add_executable(duration chrono/duration.cpp)
add_executable(time_point chrono/time_point.cpp)
add_executable(clock chrono/clock.cpp)

add_subdirectory(concurrency)
