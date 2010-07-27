cmake_minimum_required(VERSION 2.6)
add_definitions(-std=c99)

add_library(melinda STATIC melinda.c tuplespace.c internal.c thread.c)

enable_testing()
add_subdirectory(test)

add_custom_target(cleane rm -rf *~)
add_custom_target(doc doxygen doc)