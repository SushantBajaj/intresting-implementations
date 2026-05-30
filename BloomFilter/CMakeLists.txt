cmake_minimum_required(VERSION 3.31.11)
project(BloomFilter)

add_executable(test
tests/test.cpp
src/bloom_filter.cpp
../Common/src/hash.cpp
)

target_include_directories(test PRIVATE
    ../Common/include
    include
)