/*
** melinda.c
** 
** Made by (bengreve)
** Email   <Benjamin.Negrevergne[at]imag[dot]fr>
** 
** Started on  Thu Jul  8 14:49:27 2010 bengreve
*/

#include "melinda.h"


typedef struct {
  internal_t *internals; 
  unsigned int nb_iternals; 

}tuplespace_t;

void tuplespace_init(tuplespace_t *ts, unsigned int nb_internals){
  for(int i = 0; i < nb_internals; i++)
}
