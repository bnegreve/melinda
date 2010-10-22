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

#include <stdio.h>
#include "tuplespace.h"
#include "thread.h"
#include "melinda.h" 

#define WBR __asm volatile("mfence")


static void change_nb_tuples(tuplespace_t *ts, int nb); 
static int next_internal(tuplespace_t *ts, int current); 
static int auto_close(tuplespace_t *ts); 

void m_tuplespace_init(tuplespace_t *ts, size_t tuple_size, 
		       unsigned int nb_internals, int options){
  #ifndef NDEBUG
  fprintf(stderr, "MELINDA DEBUG\n"); 
  #endif
  ts->tuple_size = tuple_size; 
  ts->nb_tuples = 0; 
  ts->nb_internals = 0; 
  ts->closed = !TUPLESPACE_CLOSED; 
  ts->options = options; 
  pthread_mutex_init(&ts->mutex, NULL); 
  pthread_cond_init(&ts->cond, NULL); 
  //  memset(ts->binds, 0, TUPLESPACE_MAXINTERNALS*sizeof(int));
  memset(ts->ids, 0, TUPLESPACE_MAXINTERNALVALUE);
  for(int i = 0; i < TUPLESPACE_MAXINTERNALVALUE; i++){
    ts->binds[i] = -1; 
  }
  ts->nb_expected_threads = -1;
  ts->nb_pending_threads = 0; 
}


void m_tuplespace_destroy(tuplespace_t *ts){
  pthread_mutex_destroy(&ts->mutex); 
  pthread_cond_destroy(&ts->cond); 
}

void m_tuplespace_put(tuplespace_t *ts, opaque_tuple_t *tuples,
		      unsigned int nb_tuples){
  

  int internal_nmbr = m_distribute(tuples); 
  assert(internal_nmbr < TUPLESPACE_MAXINTERNALVALUE); 
  assert(internal_nmbr >= 0); 
  //TODO add unlikely 
  int id;
  pthread_mutex_lock(&ts->mutex); 
  if(ts->binds[internal_nmbr] == -1){

    for(id = 0; id < TUPLESPACE_MAXINTERNALS; id++){
      if(ts->ids[id] == 0){
	assert(id < TUPLESPACE_MAXINTERNALS); 
	m_internal_init(&ts->internals[id], ts->tuple_size); 
	ts->ids[id]=1; 
	break; 
      }
    }
    //WBR; //prevent instruction reordering 
    ts->binds[internal_nmbr] = id; 
    ts->nb_internals++; 
    
    change_nb_tuples(ts, nb_tuples); 
    assert(!m_tuplespace_closed(ts));
    m_internal_put(&ts->internals[id], tuples, nb_tuples);
  }
  else{
    assert(ts->binds[internal_nmbr] != -1);
    change_nb_tuples(ts, nb_tuples); 
    m_internal_put(&ts->internals[ts->binds[internal_nmbr]], tuples, nb_tuples);
  }
  pthread_mutex_unlock(&ts->mutex);
}

int m_tuplespace_get(tuplespace_t *ts, unsigned int nb_tuples, 
		     opaque_tuple_t *tuples){

#ifndef SMART_GET

  int internal_nmbr = m_retrieve(); 
  pthread_mutex_lock(&ts->mutex);
  for(;;){
    int internal_id = ts->binds[internal_nmbr];
    int nb_internals = ts->nb_internals; 

    if(internal_id == -1)
      internal_id = next_internal(ts, internal_id);

    for(int count = 0; count < nb_internals && ts->nb_tuples; count++){
      internal_t *i = &ts->internals[internal_id];
      if(!m_internal_empty(i)){	
	int nb_out_tuples = 
	  m_internal_iget(i, nb_tuples, tuples);
	if(nb_out_tuples > 0){
	  change_nb_tuples(ts, -(nb_out_tuples));
	  pthread_mutex_unlock(&ts->mutex); 
	  return nb_out_tuples; 
	}
      }      
      internal_id = next_internal(ts, internal_id);
    }
    /* Reach if no tuples are available nowhere */
    if(m_tuplespace_closed(ts)){
      pthread_mutex_unlock(&ts->mutex); 
      return TUPLESPACE_CLOSED; 
    }
    if(TUPLESPACE_OPTIONAUTOCLOSE & ts->options){
      if(auto_close(ts) == TUPLESPACE_CLOSED){
	pthread_mutex_unlock(&ts->mutex); 
	return TUPLESPACE_CLOSED;
      }
    }
    else
      pthread_cond_wait(&ts->cond, &ts->mutex); 
  }
#else
for(;;){
  int internal_nmbr = m_retrieve(); 
  int internal_id = ts->binds[internal_nmbr];

  /* while there are some tuples */
  while(__sync_fetch_and_or(&ts->nb_tuples, 0)){
    internal_t *i = &ts->internals[internal_id];
    assert(internal_id != -1); 
    if(!m_internal_empty(i)){	
      int nb_out_tuples = 
	m_internal_iget(i, nb_tuples, tuples);
      if(nb_out_tuples > 0){
	pthread_mutex_lock(&ts->mutex); 
	change_nb_tuples(ts, -(nb_out_tuples));
	pthread_mutex_unlock(&ts->mutex); 
	return nb_out_tuples; 
      }
    }
    if((internal_id = next_internal(ts, internal_id)) == -1)
      break; 
  }

  /* Reached if there are no avaible tuples or internals */

  pthread_mutex_lock(&ts->mutex); 
  while(ts->nb_tuples == 0){
    //printf("%d enters locked mode\n", m_thread_id()); 
    if(m_tuplespace_closed(ts)){
      pthread_mutex_unlock(&ts->mutex); 
      return TUPLESPACE_CLOSED; 
    }
    if(TUPLESPACE_OPTIONAUTOCLOSE & ts->options)
      auto_close(ts); 
    else
      pthread_cond_wait(&ts->cond, &ts->mutex); 
  }
  pthread_mutex_unlock(&ts->mutex); 
 }
#endif //SMART_GET
}

static void change_nb_tuples(tuplespace_t *ts, int nb){
  ts->nb_tuples+=nb; 
  //  printf("%d change nb_tupes to %d (%d)\n", m_thread_id(), ts->nb_tuples, nb);
  assert(ts->nb_tuples >= 0); 
  if(nb > 0 || ts->nb_tuples == 0)
    pthread_cond_broadcast(&ts->cond); 
}

static int next_internal(tuplespace_t *ts, int current){
  for(;;){
    int nb_internals = ts->nb_internals; 
    for(int i = current+1, count = 0; 
	count < ts->nb_internals; ++i){
      i=(i%nb_internals); 
      if(ts->ids[i])
	return i; 
    }
    return -1; 
    assert(0);     //TODO this should be reached until removal of ts is implemented 
    //    WBR; //nb_internal must be fetched from the memory
  }
}


int m_tuplespace_closed(tuplespace_t *ts){
  return ts->closed == TUPLESPACE_CLOSED; 
}

void m_tuplespace_close(tuplespace_t *ts){
  pthread_mutex_lock(&ts->mutex); 
  //TODO add lot of asserts
  ts->closed = TUPLESPACE_CLOSED; 
  pthread_cond_broadcast(&ts->cond); 
  pthread_mutex_unlock(&ts->mutex); 
}

int auto_close(tuplespace_t *ts){
  assert(ts->options & TUPLESPACE_OPTIONAUTOCLOSE); 
  if(++ts->nb_pending_threads == ts->nb_expected_threads){
    assert(ts->nb_pending_threads <= ts->nb_expected_threads); 
    assert(ts->nb_tuples == 0);
    ts->closed = TUPLESPACE_CLOSED;
    pthread_cond_broadcast(&ts->cond); 
    return TUPLESPACE_CLOSED; 
  }
  else{
    pthread_cond_wait(&ts->cond, &ts->mutex); 
    --ts->nb_pending_threads;
    return !TUPLESPACE_CLOSED; 
  }
}

void  m_tuplespace_close_at(tuplespace_t *ts, unsigned int nb_threads){
  assert(ts->options&TUPLESPACE_OPTIONAUTOCLOSE); 
  pthread_mutex_lock(&ts->mutex); 
  ts->nb_expected_threads; 
  ts->nb_expected_threads = nb_threads;  
  pthread_cond_signal(&ts->cond); 
  pthread_mutex_unlock(&ts->mutex); 
}
