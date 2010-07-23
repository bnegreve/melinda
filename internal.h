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

#ifndef   	INTERNAL_H_
#define   	INTERNAL_H_

#include <pthread.h>
/**\file internal.h*/

#include "defines.h" 

//#define INTERNAL_USESPINLOCKS 

typedef struct {
  opaque_tuple_t *data; 
  size_t data_size; 
  unsigned int begin;
  unsigned int size; 
  size_t tuple_size; 
  int closed:1;
  int in_usage:1; 
  pthread_mutex_t mutex; 
  pthread_cond_t cond; 
} internal_t; 

void m_internal_init(internal_t *i, size_t tuple_size);
void m_internal_destroy(internal_t *i); 
void m_internal_put(internal_t *i, opaque_tuple_t *tuples, unsigned int nb_tuples);

/** 
 * \brief Retrieve one or more tuples from the internal. Blocking call. 
 * 
 * 
 * @param i The internal.
 * @param nb_tuples Maximal number of tuples, after the call, number
 * of actual tuples in \tulpes array.
 * @param tuples Output array for tuples. 
 * 
 * @return The number of retrieved tuples or INTERNAL_CLOSED if the
 * tuple is empty and closed.
 */
int m_internal_get(internal_t *i, unsigned int nb_tuples, opaque_tuple_t *tuples);


/** 
 * \brief Retrieve one or more tuples from the internal. Non-blocking call.
 * 
 * 
 * @param i The internal.
 * @param nb_tuples Maximal number of tuples, after the call, number
 * of actual tuples in \tulpes array.
 * @param tuples Output array for tuples. 
 *
 * @return The number of retrieved tuples (possibly 0 if internal is
 * currently emtpy) or INTERNAL_CLOSED if the internal is emtpy and closed.
 */
int m_internal_iget(internal_t *i, unsigned int nb_tuples, opaque_tuple_t *tuples);



/** 
 * TODO
 * 
 * @param i 
 */
void m_internal_close(internal_t *i); 


/** 
 * TODO
 * 
 * @param i 
 * 
 * @return 
 */
int m_internal_closed(internal_t *i); 

/** 
 * TODO
 * 
 * 
 * @return 
 */
int m_internal_empty(internal_t *i); 
#endif 	    /* !INTERNAL_H_ */
