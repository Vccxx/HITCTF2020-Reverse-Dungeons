cmake_minimum_required(VERSION 3.0.0)
project(maze VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(maze main.cpp maze.cpp)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")
set(CMAKE_GENERATOR_PLATFORM x64)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
