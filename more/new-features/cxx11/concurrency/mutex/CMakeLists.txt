project(cxx11::concurrency::thread)

cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS "-O3 -Wall -Werror -std=c++11 ${CMAKE_CXX_FLAGS}")

add_executable(mutex mutex.cpp)
target_link_libraries(mutex pthread)

add_executable(lock_guard lock_guard.cpp)
target_link_libraries(lock_guard pthread)

add_executable(unique_lock unique_lock.cpp)
target_link_libraries(unique_lock pthread)

add_executable(dead_lock dead_lock.cpp)
target_link_libraries(dead_lock pthread)
