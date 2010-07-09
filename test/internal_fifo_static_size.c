/*
** internal_fifo_static_size.c
** 
** Made by (bengreve)
** Email   <Benjamin.Negrevergne[at]imag[dot]fr>
** 
** Started on  Sat Jul 10 12:08:56 2010 bengreve
*/


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "../internal.h"


typedef struct {
  int a; 
  int b; 
  char *c; 
}tuple_t ; 

int main(){
  internal_t internal; 

  internal_init(&internal, sizeof(tuple_t)); 
  for(int i = 0; i < 10000; i++){
    tuple_t t; 
    t.a = i; 
    t.b = i*i; 

    int size = 25;//sprintf(NULL, "poulet%d\n", i);
    assert(size > 0); 
    assert(size < 100); 
    t.c = malloc(size); 
    snprintf(t.c, size, "poulet%d", i);
    
    internal_put(&internal, (opaque_tuple_t*)&t, 1); 
  }

  internal_close(&internal); 

  int nb_tuples = 1; 
  tuple_t t; 
  while(internal_get(&internal, &nb_tuples, (opaque_tuple_t*)&t) != INTERNAL_CLOSED){
    printf("%d %d %s\n", t.a, t.b, t.c); 
    free(t.c); 
  }
  
  internal_destroy(&internal); 
}
