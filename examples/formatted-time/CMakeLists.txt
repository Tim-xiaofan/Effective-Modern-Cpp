if (POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)
project(formatted_time VERSION 1.0)

cmake_minimum_required(VERSION 3.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(APP formatted_time)
set(SRC formatted_time.cpp)

add_executable(${APP} ${SRC})
target_compile_options(${APP} PRIVATE -Wall -Werror -g -O2 -std=c++17)
