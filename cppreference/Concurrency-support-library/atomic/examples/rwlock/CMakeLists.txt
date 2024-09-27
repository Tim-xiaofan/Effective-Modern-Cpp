project(examples)

cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS "-O2 -g -Wall -Werror -Wextra -Wshadow")

add_executable(rwlock rwlock.cpp)
target_compile_options(rwlock PRIVATE -std=c++17)
target_link_libraries(rwlock PRIVATE cryptopp)
