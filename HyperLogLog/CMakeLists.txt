cmake_minimum_required(VERSION 3.31.11)
project(HyperLogLog)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(hyper_log_log_test 
tests/test.cpp
src/hyper_log_log.cpp
)

target_include_directories(hyper_log_log_test PRIVATE
../Common/include
include
)
add_executable(hyper_log_log_benchmark 
benchmark/benchmark.cpp
src/hyper_log_log.cpp
)

target_include_directories(hyper_log_log_benchmark PRIVATE
../Common/include
include
)