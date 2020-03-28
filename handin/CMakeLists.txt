cmake_minimum_required(VERSION 3.0.0)
project(GIS VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 14)

add_executable(GIS main.cpp)

add_subdirectory(lib/core)
add_subdirectory(lib/data_structure)
add_subdirectory(lib/util)

target_link_libraries(GIS core data_structure util)
