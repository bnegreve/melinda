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



typedef struct{
  pthread_mutex_t mutex; 
<<<<<<< HEAD
=======
  pthread_t tids[SYSTEMINFO_MAX_THREADS]; 
>>>>>>> b254433... Introduced a systeminfo module, added doxygen config file, create a
  int next_tid; 
}thread_ids_t;

thread_ids_t tids; 

int m_register_thread(){
  pthread_mutex_lock(&tids.mutex); 
  tids[tids.next_tid++] = pthread_self(); 
  pthread_mutex_unlock(&tids.mutex);  
}

int m_thread_id(){
  pthread_t me =  pthread_self(); 
  int last = tid.next_tid; 
  for(int i = 0; i < last; i++){
    if(me == tids.tids[i])
      return i; 
  }
  assert(false); // if reached, means that the thread has not been registred. 
}

int m_core_id(){
  assert(false); //not implemented yet. 
}
