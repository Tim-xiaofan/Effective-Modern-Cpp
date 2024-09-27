project(ticketlock)

cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS "-O2 -g -Wall -Werror -Wextra -Wshadow")

add_executable(ticketlock ticketlock.cpp)
target_compile_options(ticketlock PRIVATE -std=c++17)
