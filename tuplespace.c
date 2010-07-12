/*
** tuplespace.c
** 
** Made by (bengreve)
** Email   <Benjamin.Negrevergne[at]imag[dot]fr>
** 
** Started on  Mon Jul 12 12:00:51 2010 bengreve
*/

#include "tuplespace.h"
#include "melinda.h" 

static void change_nb_tuples(tuplespace_t *ts, int nb); 
static int next_internal(tuplespace_t *ts, int current); 

void m_tuplespace_init(tuplespace_t *ts, size_t tuple_size, 
		       unsigned int nb_internals){
  ts->tuple_size = tuple_size; 
  ts->nb_internals = 0; 
  ts->closed = !TUPLESPACE_CLOSED; 
  pthread_mutex_init(&ts->mutex, NULL); 
  pthread_cond_init(&ts->cond, NULL); 
  memset(ts->binds, 0, TUPLESPACE_MAXINTERNALS*sizeof(int));
  memset(ts->ids, 0, TUPLESPACE_MAXINTERNALS);
}


void m_tuplespace_destroy(tuplespace_t *ts){
  pthread_mutex_destroy(&ts->mutex); 
  pthread_cond_destroy(&ts->cond); 
}

void m_tuplespace_put(tuplespace_t *ts, opaque_tuple_t *tuples,
		      unsigned int nb_tuples){
  

  int internal_nmbr = m_distribute(tuples); 
  //TODO add unlikely 
  if(ts->binds[internal_nmbr] == 0){
    pthread_mutex_lock(&ts->mutex); 
    if(ts->binds[internal_nmbr] == 0){
      int id;
      for(id = 0; id < TUPLESPACE_MAXINTERNALS; id++){
	if(ts->ids[id] == 0){
	  ts->ids[id]=1; 
	  break; 
	}
      }
      assert(id < TUPLESPACE_MAXINTERNALS); 
      m_internal_init(&ts->internals[ts->binds[internal_nmbr]], ts->tuple_size); 
      ts->binds[internal_nmbr] = id; 
      ts->nb_internals++; 
    }
    pthread_mutex_unlock(&ts->mutex); 
  }
  m_internal_put(&ts->internals[ts->binds[internal_nmbr]], tuples, nb_tuples);
  change_nb_tuples(ts, nb_tuples); 
}

int m_tuplespace_get(tuplespace_t *ts, opaque_tuple_t *tuples, 
		     unsigned int *nb_tuples){

  int internal_nmbr = m_retrieve(); 
  int internal_id = ts->binds[internal_nmbr];

  for(;;){
    //TODO correct this
    /* while there are some tuples */
    while(__sync_fetch_and_or(&ts->nb_tuples, 0)){
      internal_t *i = &ts->internals[internal_id];
      if(!m_internal_empty(i)){
	int nb_out_tuples = 
	  m_internal_iget(i, *nb_tuples, tuples);
	  if(nb_out_tuples){
	    change_nb_tuples(ts, -(nb_out_tuples));
	    return nb_out_tuples; 
	  }
      }
      internal_id = next_internal(ts, internal_id);
    }

    /* if the tuplespace seems to be empty */ 
    pthread_mutex_lock(&ts->mutex); 
    while(ts->nb_tuples == 0){
      if(m_tuplespace_closed(ts)){
	pthread_mutex_unlock(&ts->mutex); 
	return TUPLESPACE_CLOSED; 
      }
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
  assert(ts->nb_tuples >= 0); 
  if(ts->nb_tuples == 0)
    pthread_cond_broadcast(&ts->cond); 
  pthread_mutex_unlock(&ts->mutex); 
}

static int next_internal(tuplespace_t *ts, int current){
  for(int i = current+1, count = 0; 
      count < TUPLESPACE_MAXINTERNALS; count++, ++i){
    i=(i%TUPLESPACE_MAXINTERNALS); 
    if(ts->ids[i])
      return i; 
  }
  assert(0); 
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
