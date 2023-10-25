project(cxx11::concurrency::data_struct)

cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS "-O3 -Wall -Werror -std=c++11 ${CMAKE_CXX_FLAGS}")

add_executable(test_queue test_queue.cpp)
target_link_libraries(test_queue pthread)
set_source_files_properties(test_queue.cpp PROPERTIES COMPILE_FLAGS "-std=c++14" )

add_executable(test_threadsafe_queue test_threadsafe_queue.cpp)
target_link_libraries(test_threadsafe_queue pthread)
set_source_files_properties(test_threadsafe_queue.cpp PROPERTIES COMPILE_FLAGS "-std=c++14" )

add_executable(test_loockup_table test_loockup_table.cpp)
