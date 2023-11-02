project(cxx11::concurrency::data_struct)

cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS "-O2 -g -Wall -Werror -std=c++11 ${CMAKE_CXX_FLAGS}")

add_executable(test_queue test_queue.cpp)
target_link_libraries(test_queue pthread)
set_source_files_properties(test_queue.cpp PROPERTIES COMPILE_FLAGS "-std=c++14" )

add_executable(test_threadsafe_queue test_threadsafe_queue.cpp)
target_link_libraries(test_threadsafe_queue pthread)
set_source_files_properties(test_threadsafe_queue.cpp PROPERTIES COMPILE_FLAGS "-std=c++14" )

add_executable(test_lookup_table test_lookup_table.cpp)

add_executable(test_threadsafe_lookup_table test_threadsafe_lookup_table.cpp test_lookup_table_utils.cpp)

add_executable(test_threadsafe_lookup_table1 test_threadsafe_lookup_table1.cpp test_lookup_table_utils.cpp)
