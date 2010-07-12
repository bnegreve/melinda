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

typedef tuple_t;

/** User defined function. Given a tuple this function returns the
   internal in which the tuple is to be stored*/
int m_distribute(tuple_t *tuple); 

/** User defined function. Return \nb_tuples tuples that should be
   processed by the calling thread */
tuple_t *m_retrieve(unsigned int *nb_tuples);



#endif 	    /* !MELINDA_H_ */
