cmake_minimum_required(VERSION 2.6)
add_definitions(-std=c99)

set(CMAKE_C_FLAGS_DEBUG -g)
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")

>>>>>>> 368e68caf2056d4ee2cbff3ac773138cf3ca34ae:melinda/CMakeLists.txt
project(melinda)
add_library(melinda STATIC melinda.c tuplespace.c internal.c systeminfo.c thread.c)

enable_testing()
add_subdirectory(test)


add_custom_target(cleane rm -rf *~)
add_custom_target(doc doxygen doc)