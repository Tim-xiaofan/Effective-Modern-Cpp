project(cxx11)

cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS "-g -O3 -Wall -Werror -std=c++11 ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_COMPILER_VERSION:${CMAKE_CXX_COMPILER_VERSION}, CMAKE_C_COMPILER_VERSION:${CMAKE_C_COMPILER_VERSION}")

# Set the minimum required compiler version
set(MIN_REQUIRED_COMPILER_VERSION "4.8.5")

# Check if the compiler version is greater than or equal to the minimum required version
if(${CMAKE_CXX_COMPILER_VERSION} VERSION_GREATER_EQUAL ${MIN_REQUIRED_COMPILER_VERSION})
    message(STATUS "Compiler version is compatible.")
else()
    message(FATAL_ERROR "Compiler version is not compatible. Minimum required version is ${MIN_REQUIRED_COMPILER_VERSION}.")
endif()

add_executable(tuple tuple.cpp)
add_executable(tuple_construct tuple_construct.cpp)
add_executable(make_tuple make_tuple.cpp)
add_executable(forward_reference forward_reference.cpp)

add_executable(ratio chrono/ratio.cpp)
add_executable(duration chrono/duration.cpp)
add_executable(time_point chrono/time_point.cpp)
add_executable(clock chrono/clock.cpp)

add_subdirectory(concurrency)
