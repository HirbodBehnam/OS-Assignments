cmake_minimum_required(VERSION 3.22)
project(Practical C)

set(CMAKE_C_STANDARD 11)
find_package(Threads)
add_executable(Practical main.c linked_list.c util.c)
target_link_libraries(Practical ${CMAKE_THREAD_LIBS_INIT})