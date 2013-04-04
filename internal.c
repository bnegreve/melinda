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

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "internal.h"

#define min(a,b)(a<=b?a:b)

/** \brief Expand an internal if needed. 
 * \warning this function is not synchronized. */
static void expand(internal_t *i);

 /** \brief Waits until a tuples arrives or the until the internal is closed. 
 * \warning this function is not synchronized.
 * @return INTERNAL_CLOSED if internal is closed. */
static int wait_tuple(internal_t *i);

/** \brief returns the size of the internal in bytes. */
static inline unsigned int size(internal_t *i) ;

/** \brief returns a pointer to the current end of data.*/
static inline opaque_tuple_t *data_end(internal_t *i) ;

static inline unsigned int dist_to_data_end(internal_t *i); 

/** \brief copy data from \i into \tuples*/
static unsigned int retrieve_tuple(internal_t *i, int nb_tuples, 
				   opaque_tuple_t *tuples);

void m_internal_init(internal_t *i, size_t tuple_size){

  i->tuple_size = tuple_size; 
  i->data_size = INTERNAL_INITIALMAXTUPLE * tuple_size; 
  i->data = malloc(i->data_size); 
  i->size = 0; 
  i->begin = 0; 
  i->closed = !INTERNAL_CLOSED; 
  i->in_usage = 0;
  pthread_mutex_init(&i->mutex, NULL); 
  pthread_cond_init(&i->cond, NULL); 
}

void m_internal_destroy(internal_t *i){
  assert(i->size == 0); 
  free(i->data); 
  pthread_mutex_destroy(&i->mutex); 
  pthread_cond_destroy(&i->cond); 
}

void m_internal_put(internal_t *i, opaque_tuple_t *tuples, unsigned int nb_tuples){

  pthread_mutex_lock(&i->mutex); 
  /* while(i->data_size < nb_tuples * i->tuple_size){ */
  /*   expand(i);  */
  /* } */

  /* check wether there is enough room */
  if(!(i->data_size >= i->size+nb_tuples*i->tuple_size)){
    fprintf(stderr, "Melinda error: \n\
cannot store more than %d tuples per internal, reduce the number of \
tuples or change the value of INTERNAL_INITIALMAXTUPLE in \
melinda/defines.h.\n",  INTERNAL_INITIALMAXTUPLE);
    exit(1); 
  }

  /* Check if there is enough contigus room for all the given tuples*/
  int first_segment_size = dist_to_data_end(i); 
  int input_data_size = nb_tuples * i->tuple_size; 
  if(first_segment_size >= input_data_size){    

    memcpy(i->data+((i->begin+i->size)%i->data_size), tuples, input_data_size);
  }
  else{
    assert(i->begin+i->size+first_segment_size <= i->data_size); 
    memcpy(i->data+i->begin+i->size, tuples, first_segment_size);
    memcpy(i->data, tuples+first_segment_size, input_data_size-first_segment_size);
  }
  
  i->size += input_data_size; 

  pthread_cond_signal(&i->cond); 
  pthread_mutex_unlock(&i->mutex); 

}

int m_internal_get(internal_t *i, unsigned int nb_tuples, opaque_tuple_t *tuples){

  pthread_mutex_lock(&i->mutex); 
  if(wait_tuple(i) == INTERNAL_CLOSED){
    pthread_mutex_unlock(&i->mutex); 
    return INTERNAL_CLOSED;
  }
  assert(i->size > 0);

  int nb_out_tuples = retrieve_tuple(i, nb_tuples, tuples);

  pthread_mutex_unlock(&i->mutex); 
  return nb_out_tuples; 
}

int m_internal_iget(internal_t *i, unsigned int nb_tuples, opaque_tuple_t *tuples){
  pthread_mutex_lock(&i->mutex); 
  if(i->size == 0){
    if(i->closed){
      pthread_mutex_unlock(&i->mutex); 
      return INTERNAL_CLOSED; 
    }
    pthread_mutex_unlock(&i->mutex); 
    return 0; 
  }

  int nb_out_tuples = retrieve_tuple(i, nb_tuples, tuples);

  pthread_mutex_unlock(&i->mutex); 
  return nb_out_tuples; 
  
}

void m_internal_close(internal_t *i){
  pthread_mutex_lock(&i->mutex); 
  i->closed = INTERNAL_CLOSED; 
  pthread_cond_broadcast(&i->cond); 
  pthread_mutex_unlock(&i->mutex); 
}

int m_internal_closed(internal_t *i){
  //TODO SYNC
  return i->closed; 
}

static void expand(internal_t *i){
  /* int effective_size = i->nb_tuples*i->tuple_size; */
  /* int data_size_new = i->data_size * INTERNAL_EXPANDFACTOR;  */
  /* opaque_tuple_t *data_new = malloc(data_size_new);  */
  /* //TODO Implement a DEQUE  instead !! */

  /* int size_to_end = i->data + i->data_size - i->begin * i->tuple_size;  */

  /* if(effective_size > size_to_end){ */
  /*   memcpy(data_new,  i->begin_offset*i->tuple_size , size_to_end);  */
  /*   memcpy(data_new+size_to_end, i->data, effective_size - size_to_end); */
  /* } */
  /* else{ */
  /*   memcpy(data_new,  i->begin_offset*i->tuple_size, effective_size);  */
  /* } */
  /* free(i->data);  */
  /* i->data = data_new;  */
  /* i->begin = 0;  */
  /* i->data_size = data_size_new;  */
}

static int wait_tuple(internal_t *i){
  while(i->size == 0){
    if(i->closed)
      return INTERNAL_CLOSED; 
    pthread_cond_wait(&i->cond, &i->mutex); 
  }
  return !INTERNAL_CLOSED;
}

inline static unsigned int size(internal_t *i) {
  return i->size; 
}

static inline opaque_tuple_t *data_end(internal_t *i) {
  return i->data + i->size; 
}

static inline unsigned int dist_to_data_end(internal_t *i){
  int begin = i->begin; 
  int size = i->size; 
  int end = (i->begin + i->size) % i->data_size; 
  if(begin <= end)
    return (i->data_size - i->begin+i->size);
  return begin-end; 
}



static unsigned int retrieve_tuple(internal_t *i, int nb_tuples, 
				   opaque_tuple_t *tuples){
  int nb_out_tuples = min(i->size/i->tuple_size, nb_tuples);
  size_t output_data_size = nb_out_tuples * i->tuple_size; 
  size_t first_segment_size = dist_to_data_end(i);
  if(output_data_size <= first_segment_size)
    memcpy(tuples, i->data+i->begin, output_data_size); 
  else{
    memcpy(tuples, i->data+i->begin, first_segment_size); 
    memcpy(tuples+first_segment_size, i->data, output_data_size-first_segment_size); 
  }
  
  i->begin = (i->begin+output_data_size)%i->data_size; 
  i->size -= output_data_size; 
  assert(size>=0);
  return nb_out_tuples; 
}


int m_internal_empty(internal_t *i){
  return i->size == 0; 
}
