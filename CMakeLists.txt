cmake_minimum_required(VERSION 3.6)
project(Cache_oblivious)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -o0")

set(SOURCE_FILES main.cpp matrix.h segment_tree.h ho_segment_tree.h test_matrix.h test_segment_tree.h)
add_executable(Cache_oblivious ${SOURCE_FILES})