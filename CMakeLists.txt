cmake_minimum_required(VERSION 3.8)
project(lab6)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c mem.c mem_debug.c)
add_executable(lab6 ${SOURCE_FILES})