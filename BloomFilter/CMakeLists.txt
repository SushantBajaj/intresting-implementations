cmake_minimum_required(VERSION 3.31.11)
project(BloomFilter)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(bloom_filter_lib
src/bloom_filter.cpp
../Common/src/hash.cpp
)

target_include_directories(bloom_filter_lib PUBLIC
    ../Common/include
    include
)

add_executable(test
tests/test.cpp
)

target_link_libraries(test PRIVATE
bloom_filter_lib
)

add_executable(benchmark
benchmark/benchmark.cpp
)

target_link_libraries(benchmark PRIVATE
bloom_filter_lib
)
