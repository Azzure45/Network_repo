cmake_minimum_required(VERSION 3.10.0)
project(ovning_3 VERSION 0.1.0 LANGUAGES C)

add_executable(client_run src/client.c)

include(CTest)
enable_testing()

