cmake_minimum_required(VERSION 3.0.0)

project(util)

add_library(${PROJECT_NAME} src/utils.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC include)
