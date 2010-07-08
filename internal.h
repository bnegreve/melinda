/*
** internal.h
** 
** Made by bengreve
** Login   <bengreve@confiance.imag.fr>
** 
** Started on  Thu Jul  8 14:55:42 2010 bengreve
** Last update Thu Jul  8 14:55:42 2010 bengreve
*/

#ifndef   	INTERNAL_H_
#define   	INTERNAL_H_

typedef struct {
  opaque_tuple_t *data; 
  size_t data_size; 
  size_t tuple_size; 
  unsigned int nb_tuples;     
}internal_t; 

void internal_init(internal_t *i, size_t tuple_size);
void internal_destroy(); 
void internal_put(internal_t *i, opaque_tuple_t *tuple, unsigned int nb_tuples);
opaque_tuple_t *internal_get(unsigned int *nb_tuples);


#endif 	    /* !INTERNAL_H_ */
