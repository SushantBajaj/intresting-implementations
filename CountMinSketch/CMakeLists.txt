cmake_minimum_required(VERSION 3.31.11)
project(CountMinSketch)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(count_min_sketch_lib
./src/count_min_sketch.cpp
../Common/src/hash.cpp
)

target_include_directories(count_min_sketch_lib PUBLIC
./include
../Common/include
)

add_executable(count_min_sketch_test
./tests/test.cpp
)
target_link_libraries(count_min_sketch_test PRIVATE
count_min_sketch_lib
)
add_executable(count_min_sketch_benchmark
./benchmark/benchmark.cpp
)
target_link_libraries(count_min_sketch_benchmark PRIVATE
count_min_sketch_lib
)

