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
#include <pthread.h>
#include "../tuplespace.h"
#include "../melinda.h"
#include "thread.h"
#define NB_THREADS 4

int ids[NB_THREADS] = {0};

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

void *thread_func(void *ts){
  m_thread_register(); 
  int me = m_thread_id(); 
  m_tuplespace_put((tuplespace_t*)ts, (opaque_tuple_t*)&me, 1); 

  int x, n = 1; 
  while(m_tuplespace_get((tuplespace_t*)ts, 1, &x) != TUPLESPACE_CLOSED){
    ids[x] = 1; 
  }

}

int main(int argc, char **argv){
  /* one argument = autoclose test*/
  int count = 0; 
  tuplespace_t ts; 
  m_tuplespace_init(&ts, sizeof(int), 0, TUPLESPACE_OPTIONAUTOCLOSE); 

  pthread_t tids[NB_THREADS]; 
  for(int i = 0; i < NB_THREADS; i++)
    pthread_create(&tids[i], NULL, thread_func, (void*)&ts); 

  for(int i = 0; i < NB_THREADS; i++)
    pthread_join(tids[i], NULL); 

  for(int i = 0; i < NB_THREADS; i++){
    if(ids[i] != 1){
      fprintf(stderr, "All ids (ie. tuples) are not present (eg. %d)\n", i); 
      return EXIT_FAILURE; 
    }
  }
  return EXIT_SUCCESS; 

}
