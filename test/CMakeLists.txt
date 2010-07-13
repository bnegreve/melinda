
cmake_minimum_required(VERSION 2.6)
add_definitions(-std=c99)

include_directories("../")
link_directories("../")
link_libraries(internal_fifo_static_size pthread melinda)

add_executable(internal_fifo_static_size internal_fifo_static_size.c)


ADD_TEST(internal_simple internal_fifo_static_size)