#Set up min CMake version
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)


#Set project name
PROJECT(SVMender)

set(CMAKE_MACOSX_RPATH 1)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_library(gssw SHARED gssw.c)
target_link_libraries(gssw)

add_library(Region SHARED Region.cpp)
target_link_libraries(Region)

add_library(Utility SHARED Utility.cpp)
target_link_libraries(Utility)

add_library(Sample SHARED Sample.cpp)
target_link_libraries(Sample)

add_library(GSSWGraph SHARED GSSWGraph.cpp)
target_link_libraries(GSSWGraph Utility Region Sample gssw)

add_executable(svmender SVMender.cc)

target_link_libraries(svmender gssw)
