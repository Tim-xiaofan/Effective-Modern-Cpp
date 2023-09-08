project(cxx11::concurrency::thread)

cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS "-O3 -Wall -Werror -std=c++11 ${CMAKE_CXX_FLAGS}")

add_executable(sample sample.cpp)
target_link_libraries(sample pthread)

add_executable(joinable joinable.cpp)
target_link_libraries(joinable pthread)

add_executable(native_handle native_handle.cpp)
target_link_libraries(native_handle pthread)

add_executable(hardware_concurrency hardware_concurrency.cpp)
target_link_libraries(hardware_concurrency pthread)

add_executable(thread_local thread_local.cpp)
target_link_libraries(thread_local pthread)
