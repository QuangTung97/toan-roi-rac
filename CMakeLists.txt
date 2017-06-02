cmake_minimum_required(VERSION 3.0)

project(toan-roi-rac)

include_directories(include)

set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_FLAGS "-O3")

add_subdirectory(src)
add_executable(main src/main.cpp)

target_link_libraries(main src)
