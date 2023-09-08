project(cxx11::concurrency::async)

cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS "-O3 -Wall -Werror -std=c++11 ${CMAKE_CXX_FLAGS}")

add_executable(replace_thread replace_thread.cpp)
target_link_libraries(replace_thread pthread)

add_executable(promise promise.cpp)
target_link_libraries(promise pthread)

add_executable(packaged_task packaged_task.cpp)
target_link_libraries(packaged_task pthread)

add_executable(async async.cpp)
target_link_libraries(async pthread)
