project(cxx11::concurrency::lockfree)

cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS "-O2 -g -ggdb3 -fno-omit-frame-pointer -Wall -Werror -std=c++11 ${CMAKE_CXX_FLAGS}")

add_executable(test_boost_lockfree_queue test_boost_lockfree_queue.cpp)
target_link_libraries(test_boost_lockfree_queue pthread)

add_executable(test_lock test_lock.cpp)
target_link_libraries(test_lock pthread)

add_executable(test_rwlock test_rwlock.cpp)
set_source_files_properties(test_rwlock.cpp PROPERTIES COMPILE_FLAGS "-std=c++17" )
target_link_libraries(test_rwlock pthread)
