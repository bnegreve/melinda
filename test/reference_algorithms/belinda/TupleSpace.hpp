//
// TupleSpace.hpp
// 
// Made by Benjamin Negrevergne
// Login   <bengreve@confiance.imag.fr>
// 
// Started on  Fri Apr 17 11:59:24 2009 Benjamin Negrevergne
// Last update Fri Apr 17 11:59:24 2009 Benjamin Negrevergne
//

#ifndef   	_TUPLESPACE_HPP_
#define   	_TUPLESPACE_HPP_

#include "Tuple.hpp"
#include "Internal.hpp"
#include "ThreadID.hpp"

const unsigned short int NUM_INTERNALS = NUM_INTERNALS_MACRO; 

extern const int SPACE_CLOSED; 
extern const int NO_MATCHES; 

extern const unsigned int NUM_THREADS; 

//#define TUPLESPACE_USE_SPINLOCKS
#define AUTO_TERMINATE

typedef int TupleRef;

class TupleSpace{
public: 
  TupleSpace(); 
  ~TupleSpace();

 

  void put(void *tuple, unsigned int listID = THREAD::getMyID());
  void putTuple(const Tuple &t, unsigned int listID = THREAD::getMyID());


  int get(void *&tuple); 
  int getTuple(unsigned int typeID, const Tuple &tmpl, Tuple *&pTuple);  

  inline
  void tupleCountInc(); 

  inline
  void tupleCountDec(); 

  inline void close();

private: 
  unsigned int _nbTuples;
  unsigned short int _workingInternal; 
  bool _closed;
  unsigned int _nbWorking; 
#ifdef TUPLESPACE_USE_SPINLOCKS
  pthread_spinlock_t _spin; 
#else
  pthread_mutex_t _mutex; 
  pthread_cond_t _cond; 
#endif
  Internal _internals[NUM_INTERNALS];  
};

/*** INLINE DEFINITIONS ***/

void TupleSpace::tupleCountInc(){
//   if(__sync_fetch_and_add(&_nbTuples, 1) == 0){
//     pthread_cond_signal(&_cond); 
//   }

#ifdef TUPLESPACE_USE_SPINLOCKS
  pthread_spin_lock(&_spin);
  _nbTuples++; 
  pthread_spin_unlock(&_spin);
#else
  pthread_mutex_lock(&_mutex); 
  if(_nbTuples++ == 0)
    pthread_cond_signal(&_cond); 
  pthread_mutex_unlock(&_mutex); 
#endif
}

void TupleSpace::tupleCountDec(){
  //  __sync_fetch_and_sub(&_nbTuples, 1); 
#ifdef TUPLESPACE_USE_SPINLOCKS
  pthread_spin_lock(&_spin);
  _nbTuples--; 
  //  std::cout<<_nbTuples<<std::endl;
  pthread_spin_unlock(&_spin);
#else
  pthread_mutex_lock(&_mutex); 
  _nbTuples--; 
  //  std::cout<<_nbTuples<<std::endl;
  pthread_mutex_unlock(&_mutex); 
#endif
}

void TupleSpace::close(){
  assert(!_closed); 
  _closed = true; 
#ifndef TUPLESPACE_USE_SPINLOCKS
  pthread_cond_broadcast(&_cond); 
#endif
}


#endif	    /* _TUPLESPACE_HPP_ */
