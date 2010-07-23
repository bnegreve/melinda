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

#ifndef   	THREAD_H_
# define   	THREAD_H_


/** 
 * \brief attribute a unique id between [0..].
 * Returned ids can be assumed to be consecutives and starting from 0. 
 * @return a unique id 
 */
int m_thread_register(); 

/** 
 * \brief Returns a unique id associated to the thread during a former
 * call to \m_register_thread();
 *\warning The thread must be registred. 
 * 
 * 
 * @return the unique thread id. 
 */
int m_thread_id(); 

/** 
 * \brief Returns the number of thread.
 */
int m_thread_nb_registred(); 


#endif 	    /* !THREAD_H_ */
