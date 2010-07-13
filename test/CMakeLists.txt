
cmake_minimum_required(VERSION 2.6)
add_definitions(-std=c99)

include_directories("../")
link_directories("../")
link_libraries(pthread melinda)

add_executable(internal_simple internal_simple.c)
add_executable(tuplespace_simple tuplespace_simple.c)


add_test(tuplespace_simple tuplespace_simple)
add_test(internal_simple internal_simple 1000)

add_custom_target(
  check 
  COMMAND ${CMAKE_CTEST_COMMAND}
  DEPENDS 
  tuplespace_simple
  internal_simple
  )

set_tests_properties ( 
  internal_simple 
  tuplespace_simple
  PROPERTIES TIMEOUT 1
  )
