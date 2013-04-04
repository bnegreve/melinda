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
/*
** defines.h
** 
** Made by bengreve
** Login   <bengreve@confiance.imag.fr>
** 
** Started on  Mon Jul 12 11:25:01 2010 bengreve
** Last update Mon Jul 12 11:25:01 2010 bengreve
*/

#ifndef   	DEFINES_H_
#define   	DEFINES_H_

/**\file defines.h*/
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define TUPLESPACE_MAXINTERNALS 128 /**< maximum number of internal */
#define TUPLESPACE_MAXINTERNALVALUE 128	/**< mamixum value that can be returned by distribute */
#define TUPLESPACE_CLOSED 0
#define TUPLESPACE_OPTIONAUTOCLOSE 1 //1
#define TUPLESPACE_OPTIONSTARTLOCKED 1<<1 //1<<1 // NOT IMPLEMENTED


#define INTERNAL_INITIALMAXTUPLE (1<<17)
#define INTERNAL_EXPANDFACTOR 2 //not in use 
#define INTERNAL_CLOSED -1 //-1


#define THREAD_MAXTHREADS 128
#define THREAD_UNREGISTRED -1 //-1

typedef void opaque_tuple_t;

#endif 	    /* !DEFINES_H_ */
