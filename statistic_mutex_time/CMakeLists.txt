project(smt)
cmake_minimum_required(VERSION 2.6)

add_definitions(-std=c++11)
add_definitions(-O2)

set(LIBRARY_OUTPUT_PATH ${RPOJECT_SOURCE_DIR})
add_library(smt smt.cpp)
