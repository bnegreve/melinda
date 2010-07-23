/*
** thread.h
** 
** Made by bengreve
** Login   <bengreve@confiance.imag.fr>
** 
** Started on  Tue Jul 13 16:10:36 2010 bengreve
** Last update Tue Jul 13 16:10:36 2010 bengreve
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
