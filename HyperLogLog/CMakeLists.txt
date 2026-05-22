cmake_minimum_required(VERSION 3.31.11)
project(HyperLogLog)
add_executable(test 
tests/test.cpp
src/hyper_log_log.cpp
)

target_include_directories(test PRIVATE
    ../Common/include
    include
)
add_executable(benchmark 
benchmark/benchmark.cpp
src/hyper_log_log.cpp
)

target_include_directories(benchmark PRIVATE
    ../Common/include
    include
)