/*
** tuplespace_simple.c
** 
** Made by (bengreve)
** Email   <Benjamin.Negrevergne[at]imag[dot]fr>
** 
** Started on  Mon Jul 12 17:32:22 2010 bengreve
*/

// gcc -g -std=c99 tuplespace_simple.c -lpthread ../tuplespace.c ../internal.c 

#include <stdio.h>
#include "tuplespace.h"
#include "melinda.h"
#include "thread.h"
typedef int tuple_t; 

int m_distribute(opaque_tuple_t *tuple){
  return 0; 
}

int m_retrieve(){
  return 0; 
}

int member(int *t, int size, int x){
  for(int i = 0; i < size; i++){
    if(t[i] == x)
      return 1; 
  }
  return 0; 
}

int main(int argc, char **argv){
  /* one argument = autoclose test*/
  int count = 0; 
  tuplespace_t ts; 
  if(argc == 2){
      m_tuplespace_init(&ts, sizeof(tuple_t), 0, TUPLESPACE_OPTIONAUTOCLOSE); 
      m_tuplespace_close_at(&ts, 1);
  }
  else
    m_tuplespace_init(&ts, sizeof(tuple_t), 0, 0); 
  int i[] = {1, 2, 3, 4}; 
  m_tuplespace_put(&ts, i, sizeof(int)); 
  
  m_thread_register(); 

  if(argc != 2)
    m_tuplespace_close(&ts); 
  int n = 1; 
  int x; 
  while(m_tuplespace_get(&ts, 1, &x) != TUPLESPACE_CLOSED){
    printf("%d\n", x); 
    count++; 
    if(!member(i, sizeof(i)/sizeof(int), x)){
      return EXIT_FAILURE; 
    }
  }

  if(count != sizeof(i)/sizeof(int))
    return EXIT_FAILURE; 
  return EXIT_SUCCESS; 


}
