project(cxx11::concurrency::thread)

cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS "-O3 -Wall -Werror -std=c++11 ${CMAKE_CXX_FLAGS}")

add_executable(atomic1 atomic.cpp)
target_link_libraries(atomic1 pthread atomic)

add_executable(compare_exchange_weak compare_exchange_weak.cpp)
target_link_libraries(compare_exchange_weak pthread atomic)

add_executable(relaxed-order relaxed-order.cpp)
target_link_libraries(relaxed-order pthread atomic)

add_executable(release-acquire-order release-acquire-order.cpp)
target_link_libraries(release-acquire-order pthread atomic)

add_executable(release-consume-order release-consume-order.cpp)
target_link_libraries(release-consume-order pthread atomic)
