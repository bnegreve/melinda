cmake_minimum_required(VERSION 2.6)

add_definitions(-std=c99)

set(CMAKE_C_FLAGS_DEBUG -g)
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")

project(melinda)
add_library(melinda STATIC melinda.c tuplespace.c internal.c systeminfo.c thread.c)

enable_testing()
add_subdirectory(test)


add_custom_target(cleane rm -rf *~)
add_custom_target(doc doxygen doc)