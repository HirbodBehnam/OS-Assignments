cmake_minimum_required(VERSION 3.16)
project(Practical3 C)

set(CMAKE_C_STANDARD 99)

SET(OBJS Practical_Q_1f.o)

add_executable(Practical3 ${OBJS} main.c single_process.h external.h file_util.h)

SET_SOURCE_FILES_PROPERTIES(
        ${OBJS}
        PROPERTIES
        EXTERNAL_OBJECT true
        GENERATED true
)

target_link_libraries(Practical3 m)
