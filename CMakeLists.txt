cmake_minimum_required(VERSION 2.6)
add_definitions(-std=c99)
project(melinda)
add_library(melinda melinda.c tuplespace.c internal.c systeminfo.c)
add_subdirectory(test)
enable_testing()

