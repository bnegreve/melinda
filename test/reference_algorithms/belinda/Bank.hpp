//
// Banc.hpp
// 
// Made by Benjamin Negrevergne
// Login   <bengreve@confiance.imag.fr>
// 
// Started on  Thu Apr 23 11:22:21 2009 Benjamin Negrevergne
// Last update Thu Apr 23 11:22:21 2009 Benjamin Negrevergne
//

#ifndef   	_BANK_HPP_
# define   	_BANK_HPP_

#include <sys/mman.h>
#include <cstdio>
#include <cassert>
#include <cstring>
#include <iostream>
#include <cstdlib>
class Bank{
public: 

  inline 
  Bank(unsigned int maxSize = 4096); 
  

  inline void pushBack(void *x);
  
  inline void *popFront(); 

  inline bool empty(); 
private: 
  const unsigned int _maxSize; 
  unsigned int _start; 
  unsigned int _end; 
  char *_data; 
  
};

/*** TEMPLATE DEFINITIONS ***/

Bank::Bank(unsigned int maxSize): 
  _maxSize(maxSize), 
  _start(0), 
  _end(0), 
  _data((char*)mmap(0, maxSize, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS,0, 0)){
  
  if(_data == (void*)-1){
    perror("Unable to allocate memory for banc : ");
  }
  assert( (maxSize % (sizeof(void*))) == 0);

}



void Bank::pushBack(void *x){
  //*(_data + _end) = x;
  memcpy(_data+_end, &x, sizeof(void*));
  _end = (_end+sizeof(void*)) % _maxSize;
  if(_end==_start){
    std::cerr<<"FATAL : Bank full"<<std::endl;
    exit(1); 
  }
    
  //  std::cout<<"New end "<<_end<<std::endl;
}

void *Bank::popFront(){
  void *x; 
  x = *(void**)(_data + (_start));
  _start = (_start+sizeof(void*)) % _maxSize; 
  //  std::cout<<"New start "<<_start<<std::endl;
  return x; 
}


bool Bank::empty(){
  return (_start == _end);
}

#endif	    /* _BANK_HPP_ */
