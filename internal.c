/*
** internal.c
** 
** Made by (bengreve)
** Email   <Benjamin.Negrevergne[at]imag[dot]fr>
** 
** Started on  Thu Jul  8 16:04:57 2010 bengreve
*/

#include "internal.h"

#define INTERNAL_INITIALDATASIZE 128
#define INTERNAL_EXPANDFACTOR 2

/* No synch ! */
static expand(internal_t *i);

/* Wait until a tuple arrives or the tuplesapce is closed */
static wait_tuple() 
void internal_init(internal_t *i, size_t tuple_size){
  i->nb_tuples = 0; 
  i->tuple_size = tuple_size; 
  i->data = malloc(tuple_size * INTERNAL_INITIALDATASIZE); 
  i->data_size = INTERNAL_INITIALDATASIZE; 
}

void internal_put(internal_t *i, opaque_tuple_t *tuple, unsigned int nb_tuples){
  if(i->data_size - i->nb_tuples < nb_tuples){
    expand(i); 
  }



#ifdef INTERNAL_USESPINLOCKS
  pthread_spin_lock(&_spin); 
  if(!waitTuple()){
    pthread_spin_unlock(&_spin); 
    return SPACE_CLOSED;
  }
  t = _tuples.front();
  _tuples.pop_front(); 
  pthread_spin_unlock(&_spin); 
  return 0; 
#else
  pthread_mutex_lock(&_mutex); 
  if(!waitTuple()){
    pthread_mutex_unlock(&_mutex); 
    return SPACE_CLOSED;
  }
  t = _tuples.front();
  _tuples.pop_front(); 
  pthread_mutex_unlock(&_mutex); 
  return 0; 
#endif

}


static expand(internal_t *i){
  i->data = realloc(i->data, i->size * EXPAND_FACTOR); 
}
