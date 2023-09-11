project(cxx11::concurrency::thread)

cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS "-O3 -Wall -Werror -std=c++11 ${CMAKE_CXX_FLAGS}")

add_executable(condition_variable condition_variable.cpp)
target_link_libraries(condition_variable pthread)

add_executable(block_queue block_queue.cpp)
target_link_libraries(block_queue pthread)

add_executable(count_down_latch count_down_latch.cpp)
target_link_libraries(count_down_latch pthread)

add_executable(barrier barrier.cpp)
target_link_libraries(barrier pthread)
