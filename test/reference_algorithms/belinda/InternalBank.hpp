//
// Internal.hpp
// 
// Made by Benjamin Negrevergne
// Login   <bengreve@confiance.imag.fr>
// 
// Started on  Thu Apr  9 16:50:44 2009 Benjamin Negrevergne
// Last update Thu Apr  9 16:50:44 2009 Benjamin Negrevergne
//

#ifndef   	_INTERNAL_HPP_
#define   	_INTERNAL_HPP_

#include <pthread.h>

#include <list>
#include "Bank.hpp"

typedef Bank internal_t; 
typedef int InternalTupleRef; 

extern const int SPACE_CLOSED; 
extern const int NO_MATCHES; 

#define INTERNAL_USE_SPINLOCKS

class Internal{
public:   
  friend class MultiTuple; 
  friend class TupleSpace; 

  inline
  Internal();

  inline
  ~Internal(); 

  inline
  InternalTupleRef put(void *t);

  inline
  int get(void *&t);

  inline 
  void *iGet();

  inline
  void close();

  inline
  bool getAccess();

  inline
  void releaseAccess();

  inline
  bool waitTuple();

  inline bool 
  empty();

private:
  internal_t _tuples; 
#ifdef INTERNAL_USE_SPINLOCKS
  pthread_spinlock_t _spin; 
#else
  pthread_mutex_t _mutex;
  pthread_cond_t _cond;
#endif
  bool _closed; 
};


/*** FUNCTIONS DEFINITIONS ***/

Internal::Internal(){
  std::cout<<"Contructing Internal Bank"<<std::endl;
#ifdef INTERNAL_USE_SPINLOCKS
  pthread_spin_init(&_spin, NULL); 
#else
  pthread_mutex_init(&_mutex, NULL); 
  pthread_cond_init(&_cond, NULL);   
#endif
  _closed=false;
}

Internal::~Internal(){
#ifdef INTERNAL_USE_SPINLOCKS
  pthread_spin_destroy(&_spin); 
#else
  pthread_mutex_destroy(&_mutex); 
  pthread_cond_destroy(&_cond); 
#endif
}


InternalTupleRef Internal::put(void *t){
  int tr; 
#ifdef INTERNAL_USE_SPINLOCKS
  pthread_spin_lock(&_spin); 
  //  InternalTupleRef tr(_tuples.insert(_tuples.end(), t)); 
  _tuples.pushBack(t); 
  pthread_spin_unlock(&_spin);
#else
  pthread_mutex_lock(&_mutex);
  //  InternalTupleRef tr(_tuples.insert(_tuples.end(), t)); 
  _tuples.pushBack(t); 
  pthread_cond_signal(&_cond); 
  pthread_mutex_unlock(&_mutex); 
#endif
  return tr; 
}

int Internal::get(void *&t){

#ifdef INTERNAL_USE_SPINLOCKS
  pthread_spin_lock(&_spin); 
  if(!waitTuple()){
    pthread_spin_unlock(&_spin); 
    return SPACE_CLOSED;
  }
//   t = _tuples.front();
//   _tuples.pop_front(); 
  t = _tuples.popFront(); 
  pthread_spin_unlock(&_spin); 
  return 0; 
#else
  pthread_mutex_lock(&_mutex); 
  if(!waitTuple()){
    pthread_mutex_unlock(&_mutex); 
    return SPACE_CLOSED;
  }
//   t = _tuples.front();
//   _tuples.pop_front(); 
  t = _tuples.popFront(); 
  pthread_mutex_unlock(&_mutex); 
  return 0; 
#endif
}

inline 
void *Internal::iGet(){
#ifdef INTERNAL_USE_SPINLOCKS
  void *tmp;
  pthread_spin_lock(&_spin); 
  if(! _tuples.empty()){
    tmp = _tuples.popFront(); 
    //    tmp = _tuples.front(); 
    //    _tuples.pop_front(); 
  }
  else{
    tmp = NULL; 
  }
  pthread_spin_unlock(&_spin);
  return tmp;
#else
  void *tmp;
  pthread_mutex_lock(&_mutex); 
  if(! _tuples.empty()){
    tmp = _tuples.popFront(); 
    //    tmp = _tuples.front(); 
    //    _tuples.pop_front(); 
  }
  else{
    tmp = NULL; 
  }
  pthread_mutex_unlock(&_mutex);
  return tmp;   
#endif
}

void Internal::close(){
#ifdef INTERNAL_USE_SPINLOCKS
  pthread_spin_lock(&_spin); 
  _closed = true; 
  pthread_spin_unlock(&_spin);   
#else
  pthread_mutex_lock(&_mutex); 
  _closed = true; 
  pthread_cond_broadcast(&_cond); 
  pthread_mutex_unlock(&_mutex);   
#endif
}

bool Internal::getAccess(){
#ifdef INTERNAL_USE_SPINLOCKS
  pthread_spin_lock(&_spin); 
#else
  pthread_mutex_lock(&_mutex); 
#endif
  return true; 
}

void Internal::releaseAccess(){

#ifdef INTERNAL_USE_SPINLOCKS
  pthread_spin_unlock(&_spin); 
#else
  pthread_mutex_unlock(&_mutex); 
#endif
}

bool Internal::waitTuple(){
  while(_tuples.empty()){
    if(_closed)
      return false; 
#ifndef INTERNAL_USE_SPINLOCKS
    pthread_cond_wait(&_cond, &_mutex); 
#endif
  }
  return true;
}

bool Internal::empty(){
  return _tuples.empty();
}
#endif	    /* _INTERNAL_HPP_ */
