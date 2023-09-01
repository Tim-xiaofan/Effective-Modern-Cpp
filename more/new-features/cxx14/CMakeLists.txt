project(cxx14)

cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS "-O2 -Wall -Werror -std=c++14 ${CMAKE_CXX_FLAGS}")

add_executable(cxx14 cxx14.cpp)
add_executable(integer_sequence integer_sequence.cpp)
add_executable(UDLs UDLs.cpp)
