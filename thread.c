/*
** PLCM/Melinda
** (c) Benjamin Negrevergne, 2010
** Original LCM algorithm from Takaki Uno and Hiroki Arimura.
**
** The program was design and implemented in colaboration with
** Alexandre Termier and Jean-François Méhaut.
** 
** See the README file for installation, usage, and details.
** See the LICENSE file for licensing.
*/

#include <pthread.h>
#include "thread.h"
#include "defines.h"

typedef struct{
  pthread_mutex_t mutex; 
  pthread_t tids[THREAD_MAXTHREADS]; 
  int next_tid; 
}thread_ids_t;

thread_ids_t tids; 

int m_thread_register(){
  pthread_mutex_lock(&tids.mutex); 
  int tmp = tids.next_tid; 
  tids.tids[tids.next_tid++] = pthread_self(); 
  assert(tids.next_tid <= THREAD_MAXTHREADS); 
  pthread_mutex_unlock(&tids.mutex);  
  return tmp; 
}

int m_thread_id(){
  pthread_t me =  pthread_self(); 
  int last = tids.next_tid; 
  for(int i = 0; i < last; i++){
    if(me == tids.tids[i])
      return i; 
  }
  return THREAD_UNREGISTRED; 
}

int m_thread_nb_registred(){
  int tmp; 
  //TODO locking is quite useless here, a memory barrier should be enough 
  pthread_mutex_lock(&tids.mutex); 
  tmp = tids.next_tid; 
  pthread_mutex_unlock(&tids.mutex); 
  return tmp; 
}
