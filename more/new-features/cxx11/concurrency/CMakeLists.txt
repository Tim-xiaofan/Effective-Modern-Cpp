project(cxx11::concurrency)

cmake_minimum_required(VERSION 3.0)

add_subdirectory(thread)
add_subdirectory(async)
add_subdirectory(mutex)
add_subdirectory(condition_variable)
add_subdirectory(atomic)
