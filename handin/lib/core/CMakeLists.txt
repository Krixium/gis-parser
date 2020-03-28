cmake_minimum_required(VERSION 3.0.0)

project(core)

add_library(${PROJECT_NAME} src/Database.cpp src/FileTokenizer.cpp src/Gis.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC include ../data_structure/include ../util/include)
