/*
** tuplespace.c
** 
** Made by (bengreve)
** Email   <Benjamin.Negrevergne[at]imag[dot]fr>
** 
** Started on  Mon Jul 12 12:00:51 2010 bengreve
*/

#include <stdio.h>
#include "tuplespace.h"
#include "thread.h"
#include "melinda.h" 

#define WBR __asm volatile("mfence")


static void change_nb_tuples(tuplespace_t *ts, int nb); 
static int next_internal(tuplespace_t *ts, int current); 
static void auto_close(tuplespace_t *ts); 

void m_tuplespace_init(tuplespace_t *ts, size_t tuple_size, 
		       unsigned int nb_internals, int options){
  ts->tuple_size = tuple_size; 
  ts->nb_tuples = 0; 
  ts->nb_internals = 0; 
  ts->closed = !TUPLESPACE_CLOSED; 
  ts->options = options; 
  pthread_mutex_init(&ts->mutex, NULL); 
  pthread_cond_init(&ts->cond, NULL); 
  //  memset(ts->binds, 0, TUPLESPACE_MAXINTERNALS*sizeof(int));
  memset(ts->ids, 0, TUPLESPACE_MAXINTERNALS);
  for(int i = 0; i < TUPLESPACE_MAXINTERNALS; i++){
    ts->binds[i] = -1; 
  }
  ts->nb_pending_threads = 0; 
}


void m_tuplespace_destroy(tuplespace_t *ts){
  pthread_mutex_destroy(&ts->mutex); 
  pthread_cond_destroy(&ts->cond); 
}

void m_tuplespace_put(tuplespace_t *ts, opaque_tuple_t *tuples,
		      unsigned int nb_tuples){
  

  int internal_nmbr = m_distribute(tuples); 
  //TODO add unlikely 
  if(ts->binds[internal_nmbr] == -1){
    pthread_mutex_lock(&ts->mutex); 
    if(ts->binds[internal_nmbr] == -1){
      int id;
      for(id = 0; id < TUPLESPACE_MAXINTERNALS; id++){
	if(ts->ids[id] == 0){
	  ts->ids[id]=1; 
	  break; 
	}
      }
      assert(id < TUPLESPACE_MAXINTERNALS); 
      m_internal_init(&ts->internals[id], ts->tuple_size); 
      //      WBR; //prevent instruction reordering 
      ts->binds[internal_nmbr] = id; 
      ts->nb_internals++; 
    }
    pthread_mutex_unlock(&ts->mutex); 
  }
  change_nb_tuples(ts, nb_tuples); 
  m_internal_put(&ts->internals[ts->binds[internal_nmbr]], tuples, nb_tuples);
}

int m_tuplespace_get(tuplespace_t *ts, unsigned int nb_tuples, 
		     opaque_tuple_t *tuples){

  int internal_nmbr = m_retrieve(); 
  int internal_id = ts->binds[internal_nmbr];

  for(;;){
    /* while there are some tuples */
    while(__sync_fetch_and_or(&ts->nb_tuples, 0)){
      internal_t *i = &ts->internals[internal_id];
      if(!m_internal_empty(i)){	
	int nb_out_tuples = 
	  m_internal_iget(i, nb_tuples, tuples);
	  if(nb_out_tuples > 0){
	    change_nb_tuples(ts, -(nb_out_tuples));
	    return nb_out_tuples; 
	  }
      }
      internal_id = next_internal(ts, internal_id);
    }

    /* if the tuplespace seems to be empty */ 
    pthread_mutex_lock(&ts->mutex); 
    while(ts->nb_tuples == 0){
      //printf("%d enters locked mode\n", m_thread_id()); 
      if(m_tuplespace_closed(ts)){
	pthread_mutex_unlock(&ts->mutex); 
	return TUPLESPACE_CLOSED; 
      }
      if(ts->options & TUPLESPACE_OPTIONAUTOCLOSE)
	auto_close(ts); 
      else
	pthread_cond_wait(&ts->cond, &ts->mutex); 
    }
    pthread_mutex_unlock(&ts->mutex); 
  }

  /* /\* There are (very probably) some tuples somewhere *\/ */
  /* for(int i = 0; i < TUPLESPACE_MAXINTERNALS; i++){ */
  /*   internal_t *i;  */
  /*   if(ts->binds[i] != 0){ */
  /*     i = ts->internals[ts->binds[i]]; */
  /*     if(!m_internal_empty(i)){ */
  /* 	int nb_out_tuples = m_internal_iget(i, nb_tuples, tuples);  */
  /* 	ts->nb_tuples -= nb_out_tuples;  */
  /* 	pthread_mutex_unlock(&ts->mutex);  */
  /* 	return nb_out_tuples; */
  /*     } */
  /*   } */
  /* }   */
  /* pthread_mutex_unlock(&ts->mutex);  */
}

static void change_nb_tuples(tuplespace_t *ts, int nb){
  pthread_mutex_lock(&ts->mutex); 
  ts->nb_tuples+=nb; 
  //  printf("%d change nb_tupes to %d (%d)\n", m_thread_id(), ts->nb_tuples, nb);
  assert(ts->nb_tuples >= 0); 
  if(ts->nb_tuples == 0)
    pthread_cond_broadcast(&ts->cond); 
  pthread_mutex_unlock(&ts->mutex); 
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
    assert(0);     //TODO this should be reached until removal of ts is implemented 
    WBR; //nb_internal must be fetched from the memory
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

void auto_close(tuplespace_t *ts){
  assert(ts->options & TUPLESPACE_OPTIONAUTOCLOSE); 
  if(++ts->nb_pending_threads == m_thread_nb_registred()){
    assert(ts->nb_pending_threads <= m_thread_nb_registred()); 
    assert(ts->nb_tuples == 0);
    ts->closed = TUPLESPACE_CLOSED;
    pthread_cond_broadcast(&ts->cond); 
  }
  else{
    pthread_cond_wait(&ts->cond, &ts->mutex); 
    --ts->nb_pending_threads;
  }
}
