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
#define MAX_NB_THREADS 1000
int ids[MAX_NB_THREADS] = {0};

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
   printf("put %d\n", me);
  int x, n = 1; 

  // if((int)(rand()/RAND_MAX*20) != 1){
    while(m_tuplespace_get((tuplespace_t*)ts, 1, &x) != TUPLESPACE_CLOSED){
      ids[x] = 1; 
      printf("put %d got %d\n", me, x);
    }
    //  }
}

int main(int argc, char **argv){
  
  if(argc != 2){
    fprintf(stderr, "usage %s <nb_threads>\n", argv[0]); 
    exit(EXIT_FAILURE); 
  }
  
  int nb_threads = atoi(argv[1]); 
  if(nb_threads > MAX_NB_THREADS){
    fprintf(stderr, "max nb threads\n", MAX_NB_THREADS); 
    exit(EXIT_FAILURE); 
  }

  int count = 0; 
  tuplespace_t ts; 
  m_tuplespace_init(&ts, sizeof(int), 0, TUPLESPACE_OPTIONAUTOCLOSE); 

  pthread_t tids[MAX_NB_THREADS]; 
  for(int i = 0; i < nb_threads; i++)
    pthread_create(&tids[i], NULL, thread_func, (void*)&ts); 
  m_tuplespace_close_at(&ts, nb_threads); 

  for(int i = 0; i < nb_threads; i++)
    pthread_join(tids[i], NULL); 

  for(int i = 0; i < nb_threads; i++){
    if(ids[i] != 1){
      fprintf(stderr, "All ids (ie. tuples) are not present (eg. %d)\n", i); 
      return EXIT_FAILURE; 
    }
  }
  printf("test successfull\n"); 
  return EXIT_SUCCESS; 

}
