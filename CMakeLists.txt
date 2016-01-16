cmake_minimum_required(VERSION 2.8)
project(CMakeListsGen)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(  C:/Users/Lee/Desktop/CMakeListsGen
  C:/Users/Lee/Desktop/CMakeListsGen/src
)

set(SOURCE_FILES  C:/Users/Lee/Desktop/CMakeListsGen/src/CMakeListsGen.cpp
)

add_executable(CMakeListsGen ${SOURCE_FILES})
