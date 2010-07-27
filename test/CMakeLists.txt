
cmake_minimum_required(VERSION 2.6)
add_definitions(-std=c99 -g )
cmake_policy(SET CMP0015 NEW)
include_directories("../")
link_directories("../")
link_libraries(pthread melinda)

add_executable(internal_simple internal_simple.c)
add_executable(tuplespace_simple tuplespace_simple.c)
add_executable(thread_register thread_register.c)
add_executable(tuplespace_multi tuplespace_multi.c)


add_test(tuplespace_simple tuplespace_simple)
add_test(tuplespace_simple_autoclose tuplespace_simple x)
add_test(internal_simple internal_simple 1000)
add_test(thread_register thread_register)
add_test(tuplespace_multi tuplespace_multi 10)
add_test(many_run many_run.sh 50 16)
add_test(valgrind valgrind -q ./tuplespace_multi 10)

add_custom_target(
  check 
  COMMAND ${CMAKE_CTEST_COMMAND}
  DEPENDS 
  tuplespace_simple
  internal_simple
  thread_register
  tuplespace_multi
  )

set_tests_properties ( 
  internal_simple 
  tuplespace_simple
  tuplespace_simple_autoclose
  thread_register
  tuplespace_multi
  PROPERTIES TIMEOUT 2
  )


set_tests_properties ( 
  many_run
  valgrind
  PROPERTIES TIMEOUT 7
  )
