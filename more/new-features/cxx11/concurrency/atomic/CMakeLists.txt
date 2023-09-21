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

add_executable(seq_cst-order seq_cst-order.cpp)
target_link_libraries(seq_cst-order pthread atomic)

add_executable(ATOMIC_XXX_LOCK_FREE ATOMIC_XXX_LOCK_FREE.cpp)
target_link_libraries(ATOMIC_XXX_LOCK_FREE pthread atomic)

add_executable(atomic_flag atomic_flag.cpp)
target_link_libraries(atomic_flag pthread atomic)

add_executable(atomic_bool atomic_bool.cpp)
target_link_libraries(atomic_bool pthread atomic)

add_executable(atomic_int atomic_int.cpp)
set_source_files_properties(atomic_int.cpp PROPERTIES COMPILE_FLAGS "-g" )
target_link_libraries(atomic_int pthread atomic)

add_executable(rwlock rwlock.cpp)
target_link_libraries(rwlock pthread atomic)

add_executable(barrier1 barrier.cpp)
target_link_libraries(barrier1 pthread atomic)
