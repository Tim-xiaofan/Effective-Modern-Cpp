project(cxx17)

cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_FLAGS "-O2 -Wall -Werror -std=c++17 ${CMAKE_CXX_FLAGS}")
add_executable(cxx17 cxx17.cpp)
add_executable(fold fold.cpp)
add_executable(braced-init-list braced-init-list.cpp)
add_executable(constexpr constexpr.cpp)
add_executable(constexpr_if constexpr_if.cpp)
add_executable(string_view string_view.cpp)

add_executable(inline_variables inline_variables.cpp)
target_link_libraries(inline_variables pthread)

add_executable(lambda_this lambda_this.cpp)

add_executable(variant variant.cpp)
add_executable(optional optional.cpp)
add_executable(any any.cpp)
add_executable(apply apply.cpp)
add_executable(make_from_tuple make_from_tuple.cpp)
add_executable(filesystem filesystem.cpp)
