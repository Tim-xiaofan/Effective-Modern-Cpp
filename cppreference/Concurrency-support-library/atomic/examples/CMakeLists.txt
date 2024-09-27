project(examples)

cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS "-O2 -g -Wall -Werror -Wextra -Wshadow")

add_executable(spinlock spinlock.cpp)
target_compile_options(spinlock PRIVATE -std=c++17)
