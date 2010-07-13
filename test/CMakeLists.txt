
cmake_minimum_required(VERSION 2.6)
add_definitions(-std=c99)

include_directories("../")
link_directories("../")
link_libraries(pthread melinda)

add_executable(internal_fifo_static_size internal_fifo_static_size.c)
add_executable(tuplespace_simple tuplespace_simple.c)

add_test(tuplespace_simple tuplespace_simple)
add_test(internal_simple internal_fifo_static_size 1000)



