project(my_grep)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(grep)
target_sources(grep PRIVATE grep.cpp thread_pool.cpp thread_pool.h)
