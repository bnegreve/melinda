/*
** tuplespace_simple.c
** 
** Made by (bengreve)
** Email   <Benjamin.Negrevergne[at]imag[dot]fr>
** 
** Started on  Mon Jul 12 17:32:22 2010 bengreve
*/

// gcc -g -std=c99 tuplespace_simple.c -lpthread ../tuplespace.c ../internal.c 

#include <stdio.h>
#include "../tuplespace.h"
#include "../melinda.h"

typedef int tuple_t; 

int m_distribute(opaque_tuple_t *tuple){
  return 0; 
}

int m_retrieve(){
  return 0; 
}


int main(){
  tuplespace_t ts; 
  m_tuplespace_init(&ts, sizeof(tuple_t), 0); 
  int i[] = {1, 2, 3, 4}; 
  m_tuplespace_put(&ts, i, 4); 

  m_tuplespace_close(&ts); 
  int n = 1; 
  int x; 
  while(m_tuplespace_get(&ts, &x, &n) != TUPLESPACE_CLOSED){
    printf("%d\n", x); 
  }
  

}
