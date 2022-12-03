cmake_minimum_required(VERSION 3.24)
project(OS C)

set(CMAKE_C_STANDARD 11)
find_package (Threads)
add_executable(OS
        main.c
        matrix.h)
target_link_libraries (OS ${CMAKE_THREAD_LIBS_INIT})