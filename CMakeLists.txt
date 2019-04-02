cmake_minimum_required(VERSION 3.5)

project(sexpresso)

set(CMAKE_CXX_STANDARD 11)

add_library(sexpresso ${CMAKE_CURRENT_SOURCE_DIR}/sexpresso/sexpresso.cpp)
target_include_directories(sexpresso PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/sexpresso)
