cmake_minimum_required(VERSION 3.6)
project(cpstresstest)

set(SOURCE_FILES src/libcpstresstest.cpp src/libcpstresstest.h src/popen2.h)

# C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_library(cpstresstest STATIC ${SOURCE_FILES})
target_include_directories(cpstresstest PRIVATE src/include)