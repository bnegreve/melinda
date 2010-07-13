/*
** thread.c
** 
** Made by (bengreve)
** Email   <Benjamin.Negrevergne[at]imag[dot]fr>
** 
** Started on  Tue Jul 13 16:20:52 2010 bengreve
*/

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#include "thread.h"
#define NB_THREADS 128


int ids[NB_THREADS] = {0}; 

void *thread_func(void *x){
  int a = m_thread_register(); 
  printf("%d\n", a); 


  if(a != m_thread_id()){
    exit(EXIT_FAILURE); 
  }
  if(a < 0 || a >= NB_THREADS){
    fprintf(stderr, "a is not in the interval [0 .. NB_THREADS[ (a=%d)\n", a); 
    exit(EXIT_FAILURE); 
  }

  ids[a] = 1; 
}

int main(){
  pthread_t tids[NB_THREADS]; 
  for(int i = 0; i < NB_THREADS; i++){
    pthread_create(&tids[i], NULL, thread_func, NULL); 
  }

  for(int i = 0; i < NB_THREADS; i++)
    pthread_join(tids[i], NULL); 


  for(int i = 0; i < NB_THREADS; i++){
    if(ids[i] != 1){
      fprintf(stderr, "all ids have not been attributed (eg. %d)\n", i); 
      return EXIT_FAILURE; 
    }
  }



  return EXIT_SUCCESS; 
}
