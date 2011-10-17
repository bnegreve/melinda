cmake_minimum_required(VERSION 2.6)
project(melinda)

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_DEBUG "-g")

set(CMAKE_CXX_FLAGS_PROF "-O3 -g -pg")
set(CMAKE_C_FLAGS_PROF "-O3 -g -pg")
set(CMAKE_EXE_LINKER_FLAGS_PROF "-pg")


#set default build type to Release
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE "Release")
ENDIF(NOT CMAKE_BUILD_TYPE)

message(STATUS "BUILD : ${CMAKE_BUILD_TYPE}")

add_definitions(-std=c99)

add_library(melinda STATIC melinda.c tuplespace.c internal.c thread.c)

enable_testing()
add_subdirectory(test)

add_custom_target(cleane rm -rf *~)
add_custom_target(doc doxygen doc)