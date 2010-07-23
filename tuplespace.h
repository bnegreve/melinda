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

#ifndef   	TUPLESPACE_H_
#define   	TUPLESPACE_H_
/**\file tuplespace.h*/

#include "defines.h"

#include "internal.h"

typedef struct {
  size_t tuple_size; 
  internal_t internals[TUPLESPACE_MAXINTERNALS];   
  int binds[TUPLESPACE_MAXINTERNALVALUE]; /**< bind arbitrary value > internals */
  char ids[TUPLESPACE_MAXINTERNALS]; /**< available ids (should be a bitmap) */
  unsigned int nb_internals; 
  int options; 
  int nb_tuples; 
  int closed:1; 
  pthread_mutex_t mutex; 
  pthread_cond_t cond; 
  unsigned int nb_expected_threads; 
  unsigned int nb_pending_threads; 
} tuplespace_t;

/** 
 * \brief Intialize the tuplespace \ts.
 *
 * \warning Must be called before calling any other function in this file.  
 *
 * @param ts The tuplespace to initialize. 
 * @param tuple_size
 * @param nb_internals Number of internals to be initialy created. 
 * @param options Options (see \defines.h) if NULL, default options are used.
 */
void m_tuplespace_init(tuplespace_t *ts, size_t tuple_size, 
		       unsigned int nb_internals, int options); 

/** 
 * \brief Releases memory used by the tuplespace \ts.
 * 
 * @param ts 
 */
void m_tuplespace_destroy(tuplespace_t *ts);

/** 
 * \brief Put the tuples contained in the \tuples array inside
 * \tuplespace.
 * 
 * @param ts The tuplespace where to put the given tuples.
 * @param tuples The array of tuples.
 * @param nb_tuples Number of tuples in \tuples array. 
 */
void m_tuplespace_put(tuplespace_t *ts, opaque_tuple_t *tuples,
		    unsigned int nb_tuples); 


/** 
 * \brief Fetch tuples from \ts where label equals to \label,
 * and fill the \tuples array with them.
 * 
 * @param ts The tuplespace from which tuples are going to be fetched.
 * @param nb_tuples Maximum number of tuples to retrieve. 
 * @param tuples The array of tuples. 
 * 
 * @return number of tuples or TUPLESPACE_CLOSED if \ts is empty and is not going
 * to receive any other tuple. 
 */
int m_tuplespace_get(tuplespace_t *ts, unsigned int nb_tuples, 
		     opaque_tuple_t *tuples);


/** 
 * \brief Put the tuples contained in the \tuples array inside
 * \ts.
 * 
 * @param ts The tuplespace where to put the given tuples.
 * @param tuples The array of tuples.
 * @param nb_tuples Number of tuples in \tuples array. 
 * @param label Integer value. Used by the \distribute function to prevent mixing of tuples. See. \tuplespace_get_label(). When using \tuplespace_put(), 0 is used as label.
 */
/* void m_tuplespace_put_label(tuplespace_t ts, opaque_tuple_t *tuples, */
/* 			  unsigned int nb_tuples, int label);  */




/**  
 * \brief Fetch tuples from \ts where label equals to \label,
 * and fill the \tuples array with them.
 * 
 * @param ts The tuplespace from which tuples are going to be fetched.
 * @param label Integer value. Any valid Melinda implementation does
 * not mix tuples with different values, When using \tuplespace_get(),
 * 0 is used as label.
 * @param tuples The array of tuples. 
 * @param nb_tuples Number of tuples cointained in \tuples after the call.
 * 
 * @return TUPLESPACE_CLOSED if \ts is empty and is not going
 * to receive any other tuple. Or TUPLESPACE_NOMATCH if \ts
 * does not cointain any tuple with the given label and is not going
 * to receive any.
 */
/* int m_tuplespace_get_label(tuplespace_t ts, int label,  */
/* 			 opaque_tuple_t *tuples, unsigned int *nb_tuples);  */


/** 
 * \brief Lock the tuplespace so that all the call to put or get are 
 *  blocked. 							     
 * \warning The tuplespace is initially locked.
 * @param ts The tuplespace to be locked
 */
void m_tuplespace_lock(tuplespace_t ts); 

/* Unlock the tuple space. \warning The tuplespace is initially
   locked. Therefor this must be called at least once after
   initialization of the tuple*/
void m_tuplespace_unlock(tuplespace_t ts);



/** 
 * \brief Return a non-null value if the tuplespace \ts is closed. 
 */
int m_tuplespace_closed(tuplespace_t *ts);

/** 
 * \brief Close the tuplespace \ts.
 * TODO
 */
void m_tuplespace_close(tuplespace_t *ts);


/** 
 * \brief Inform the tuplespace how many threads it must wait for
 * before closing itself.
 *
 * When the number of threads is reached, the tuplespace \ts is closed
 * and all the pending threads are relased.
 *
 * If TUPLESPACE_AUTOCLOSE is set, it is *gareented* that no threads will be released before the first
 * call to this function.
 *
 * \warning Only makes sens if TUPLESPACE_AUTOCLOSE option is set.
 * \warning The tuplespace will *not* close if the function is never
 * called with the right value (ie. The number of threads that will
 * eventually be waiting for tuples).
 * 
 * @param ts The tuplespace.
 * @param nb_threads The number of threads to wait for before closing \ts.
 * 
 */
void  m_tuplespace_close_at(tuplespace_t *ts, unsigned int nb_threads); 


#endif 	    /* !TUPLESPACE_H_ */
