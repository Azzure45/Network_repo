cmake_minimum_required(VERSION 3.10.0)
project(uppgift_1 VERSION 0.1.0 LANGUAGES C CXX)

add_executable(uppgift_1 src/server.cpp)

include(CTest)
enable_testing()

