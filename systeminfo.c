/*
** system_info.c
** 
** Made by (bengreve)
** Email   <Benjamin.Negrevergne[at]imag[dot]fr>
** 
** Started on  Mon Jul 12 11:22:19 2010 bengreve
*/

#include <pthread.h>
#include "systeminfo.h"
#include "defines.h"


typedef struct{
  pthread_mutex_t mutex; 
  pthread_t tids[SYSTEMINFO_MAXTHREADS]; 
  int next_tid; 
}thread_ids_t;

thread_ids_t tids; 

int m_register_thread(){
  pthread_mutex_lock(&tids.mutex); 
  tids.tids[tids.next_tid++] = pthread_self(); 
  pthread_mutex_unlock(&tids.mutex);  
}

int m_thread_id(){
  pthread_t me =  pthread_self(); 
  int last = tids.next_tid; 
  for(int i = 0; i < last; i++){
    if(me == tids.tids[i])
      return i; 
  }
  assert(0); // if reached, means that the thread has not been registred. 
}

int m_core_id(){
  assert(0); //not implemented yet. 
}
