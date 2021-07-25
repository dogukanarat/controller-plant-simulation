cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)

project(little-puffin)

include_directories(src)

add_subdirectory(src)
add_subdirectory(tst)
add_subdirectory(lib/googletest)