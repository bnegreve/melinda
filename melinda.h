/*
** melinda.h
** 
** Made by bengreve
** Login   <bengreve@confiance.imag.fr>
** 
** Started on  Wed Jul  7 16:52:07 2010 bengreve
** Last update Wed Jul  7 16:52:07 2010 bengreve
*/

#ifndef   	MELINDA_H_
#define   	MELINDA_H_

typedef struct {} tuple_t; 


void tuplespace_init(unsigned int nb_internals); 


/** 
 * \brief Put the tuples contained in the \tuples array inside
 * \tuplespace.
 * 
 * @param ts The tuplespace where to put the given tuples.
 * @param tuples The array of tuples.
 * @param nb_tuples Number of tuples in \tuples array. 
 */
void tuplespace_put(tuplespace_t ts, tuple_t *tuples,
		    unsigned int nb_tuples); 


/** 
 * \brief Fetch tuples from \ts where label equals to \label,
 * and fill the \tuples array with them.
 * 
 * @param ts The tuplespace from which tuples are going to be fetched.
 * @param tuples The array of tuples. 
 * @param nb_tuples Number of tuples cointained in \tuples after the call.
 * 
 * @return TUPLESPACE_CLOSED if \ts is empty and is not going
 * to receive any other tuple. Or TUPLESPACE_NOMATCH if \ts
 * does not cointain any tuple with the default label and is not going
 * to receive any.
 */
int tuplespace_get(tuplespace_t ts, tuples_t *tuples, 
		   unsigned int *nb_tuples); 


/** 
 * \brief Put the tuples contained in the \tuples array inside
 * \ts.
 * 
 * @param ts The tuplespace where to put the given tuples.
 * @param tuples The array of tuples.
 * @param nb_tuples Number of tuples in \tuples array. 
 * @param label Integer value. Used by the \distribute function to prevent mixing of tuples. See. \tuplespace_get_label(). When using \tuplespace_put(), 0 is used as label.
 */
void tuplespace_put_label(tuplespace_t ts, tuple_t *tuples,
			  unsigned int nb_tuples, int label); 




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
int tuplespace_get_label(tuplespace_t ts, int label, 
			 tuples_t *tuples, unsigned int *nb_tuples); 


/** 
 * \brief Lock the tuplespace so that all the call to put or get are 
 *  blocked. 							     
 * \warning The tuplespace is initially locked.
 * @param ts The tuplespace to be locked
 */
void tuplespace_lock(tuplespace_t ts); 

/* Unlock the tuple space. \warning The tuplespace is initially
   locked. Therefor this must be called at least once after
   initialization of the tuple*/
void tuplespace_unlock(tuplespace_t ts);


/* /\* User defined function. Given a tuple this function may return 1 if */
/*    the tuple is to be kept or 0 if the tuple is to be discarded. *\/ */
/* int input_filter(tuple_t *tuple);  */

/* User defined function. Given a tuple this function returns the
   internal in which the tuple is to be stored*/
int distribute(tuple_t *tuple); 

/* User defined function. Return \nb_tuples tuples that should be
   processed by the calling thread */
tuple_t *retrieve(unsigned int *nb_tuples);



#endif 	    /* !MELINDA_H_ */
