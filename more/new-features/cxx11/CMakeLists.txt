project(cxx11)

cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS "-O2 -Wall -Werror -std=c++11 ${CMAKE_CXX_FLAGS}")

add_executable(tuple tuple.cpp)
add_executable(tuple_construct tuple_construct.cpp)
add_executable(make_tuple make_tuple.cpp)
add_executable(forward_reference forward_reference.cpp)
