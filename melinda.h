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
/**\file melinda.h*/
#include "defines.h" 

/** User defined function. Given a tuple this function returns the
   internal in which the tuple is to be stored*/
int m_distribute(opaque_tuple_t *tuple); 

/** User defined function. Return \nb_tuples tuples that should be
   processed by the calling thread */
int m_retrieve();



#endif 	    /* !MELINDA_H_ */
