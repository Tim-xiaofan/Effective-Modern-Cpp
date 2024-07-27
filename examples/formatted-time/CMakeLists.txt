cmake_minimum_required(VERSION 3.0)

if (POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)
project(formatted_time VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(APP formatted_time)
set(SRC formatted_time.cpp)

add_executable(${APP} ${SRC})
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
  target_compile_options(${APP} PRIVATE /W4)
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  target_compile_options(${APP} PRIVATE -Wall -Wextra -Wpedantic -Werror -Wshadow)
else()
  message(FATAL_ERROR "Not supported compiler ${CMAKE_CXX_COMPILER_ID}")
endif()

set_target_properties(${APP} PROPERTIES CXX_STANDARD 17
  COMPILE_WARNING_AS_ERROR ON
)
