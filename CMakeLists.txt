cmake_minimum_required(VERSION 3.17)
project(Lab1)

set(CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread -O2 -g")

add_subdirectory(benchmark)

include_directories(benchmark/googletest/googletest/include/gtest)

add_executable(FindIfBenchmark
        FindIf/find_if.cpp
        FindIf/find_if_bench.cpp)

target_link_libraries(FindIfBenchmark benchmark::benchmark)

add_executable(FindIfTests
        benchmark/googletest/googletest/src/gtest_main.cc
        FindIf/find_if.cpp
        FindIf/find_if_tests.cpp)

target_link_libraries(FindIfTests gtest)

add_executable(HashCollisionBenchmark
        HashCollision/hash.cpp
        HashCollision/hash_bench.cpp
        HashCollision/hash_map.cpp
        HashCollision/hash_map.h)

target_link_libraries(HashCollisionBenchmark benchmark::benchmark)

add_executable(HashCollisionTests
        benchmark/googletest/googletest/src/gtest_main.cc
        HashCollision/hash.cpp
        HashCollision/hash_tests.cpp
        HashCollision/hash_map.cpp)

target_link_libraries(HashCollisionTests gtest)

add_executable(PromiseTests
        benchmark/googletest/googletest/src/gtest_main.cc
        UnkeptPromises/promise_tests.cpp
        UnkeptPromises/promise.h
        UnkeptPromises/thread_variables.h)

target_link_libraries(PromiseTests gtest)
