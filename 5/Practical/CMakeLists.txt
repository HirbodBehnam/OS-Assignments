cmake_minimum_required(VERSION 3.16)
project(Practical C)

set(CMAKE_C_STANDARD 11)
find_package(Threads)
add_executable(Practical main.c pipe.h)
target_link_libraries(Practical ${CMAKE_THREAD_LIBS_INIT})
