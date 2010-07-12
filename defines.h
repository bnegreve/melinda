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
<<<<<<< HEAD

/**\file defines.h*/
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define TUPLESPACE_MAXINTERNALS 128  
#define TUPLESPACE_CLOSED 0

#define INTERNAL_INITIALMAXTUPLE 1024 //not in use
#define INTERNAL_EXPANDFACTOR 2 //not in use 
#define INTERNAL_CLOSED -1 //-1


#define SYSTEMINFO_MAXTHREADS 128


typedef void opaque_tuple_t;
=======
#include <assert.h>


#define SYSTEMINFO_MAX_THREADS 128

>>>>>>> b254433... Introduced a systeminfo module, added doxygen config file, create a

#endif 	    /* !DEFINES_H_ */
