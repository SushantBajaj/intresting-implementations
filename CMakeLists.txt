cmake_minimum_required(VERSION 3.31.11)

project(InterestingImplementations)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(
    -fsanitize=address
    -g
)


add_link_options(
    -fsanitize=address
)

add_subdirectory(BloomFilter)
add_subdirectory(CountMinSketch)
add_subdirectory(HyperLogLog)
add_subdirectory(AVLtree)
