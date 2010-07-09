/*
** internal.h
** 
** Made by bengreve
** Login   <bengreve@confiance.imag.fr>
** 
** Started on  Thu Jul  8 14:55:42 2010 bengreve
** Last update Thu Jul  8 14:55:42 2010 bengreve
*/

#ifndef   	INTERNAL_H_
#define   	INTERNAL_H_

#include <pthread.h>
//#define INTERNAL_USESPINLOCKS 
#define INTERNAL_INITIALMAXTUPLE 1024
#define INTERNAL_EXPANDFACTOR 2

#define INTERNAL_CLOSED 1 //1

typedef char opaque_tuple_t; 

typedef struct {
  opaque_tuple_t *data; 
  size_t data_size; 
  unsigned int begin;
  unsigned int size; 
  size_t tuple_size; 
  int closed:1;
  pthread_mutex_t mutex; 
  pthread_cond_t cond; 
} internal_t; 

void internal_init(internal_t *i, size_t tuple_size);
void internal_destroy(internal_t *i); 
void internal_put(internal_t *i, opaque_tuple_t *tuples, unsigned int nb_tuples);

/** 
 * \brief retrieve one or more tuples from the internal. 
 * 
 * 
 * @param i The internal.
 * @param nb_tuples Maximal number of tuples, after the call, number
 * of actual tuples in \tulpes array.
 * @param tuples Output array for tuples. 
 * 
 * @return INTERNAL_CLOSED
 */
int internal_get(internal_t *i, unsigned int *nb_tuples, opaque_tuple_t *tuples);

void internal_close(internal_t *i); 


#endif 	    /* !INTERNAL_H_ */
