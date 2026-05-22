cmake_minimum_required(VERSION 3.31.11)
project(HyperLogLog)
add_executable(hll 
tests/test.cpp
src/hyper_log_log.cpp
)

target_include_directories(hll PRIVATE
    ../Common/include
    include
)