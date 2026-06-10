cmake_minimum_required(VERSION 3.31.11)
project(AVLtree)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(avl_tree_lib
    src/avl_tree.cpp
)

target_include_directories(avl_tree_lib PUBLIC
    include
)

add_executable(avl_tree_test
    test/test.cpp
)

target_link_libraries(avl_tree_test PRIVATE
    avl_tree_lib
)
