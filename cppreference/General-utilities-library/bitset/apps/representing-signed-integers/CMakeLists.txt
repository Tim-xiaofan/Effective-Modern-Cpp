# Set the CMP0048 policy to NEW
cmake_policy(SET CMP0048 NEW)

project(signed-integers VERSION 0.0.1)
cmake_minimum_required(VERSION 3.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (MSVC)
    # warning level 4
    add_compile_options(/W4)
else()
    # additional warnings
    message(STATUS "GCC")
    add_compile_options(-Wall -Wextra -Wpedantic -Wall -Werror -Wshadow)
endif()

include_directories(encode PRIVATE ${CMAKE_SOURCE_DIR})
link_directories(encode PRIVATE ${CMAKE_SOURCE_DIR})

# encode
add_executable(encode encode.cpp common.cpp)
target_link_libraries(encode PRIVATE "")

# add
add_executable(add add.cpp common.cpp)
target_link_libraries(add PRIVATE "")

# decode
add_executable(decode decode.cpp common.cpp)
target_link_libraries(decode PRIVATE "")
