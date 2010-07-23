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
