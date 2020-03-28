cmake_minimum_required(VERSION 3.0.0)

project(data_structure)

add_library(${PROJECT_NAME} src/GeoFeature.cpp src/QuadTree.cpp src/ScriptCommand.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC include ../util/include)
