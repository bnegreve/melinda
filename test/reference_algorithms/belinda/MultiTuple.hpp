//
// MultiTuple.hpp
// 
// Made by Benjamin Negrevergne
// Login   <yakomow@CUBE8>
// 
// Started on  Wed May 21 13:37:56 2008 Benjamin Negrevergne
// Last update Fri Apr  3 11:42:18 2009 Benjamin Negrevergne
//

#ifndef   	_MULTITUPLE_HPP_
#define   	_MULTITUPLE_HPP_

//#include "../common.hpp"

#include <pthread.h>

#include <list>
#include <cstdarg>
#include <list>
#include "Tuple.hpp"

#ifdef TRACE
#include <typeinfo>
#include "../TraceCollector.hpp"
#endif //TRACE

static char trace_access[6000]; 
static int trace_access_count=0; 


static const int SPACE_CLOSED = -1; 
static const int NO_MATCHES = -2; 

#include "Internal.hpp"

extern const unsigned int NUM_THREADS; 

static const int MAX_INTERNALS = 10; 

/** 
 * Represent a reference to a tuple inside the tuple space.
 */
class TupleRef : private InternalTupleRef{
public:
  friend class MultiTuple;
  TupleRef(){}

  TupleRef(unsigned short int tsIdx, InternalTupleRef it):
    InternalTupleRef(it),
    _tsIdx(tsIdx){
  }

  inline Tuple *operator*(){
    return static_cast<Tuple*>(InternalTupleRef::operator*());
  }

  inline void destroyTuple(){
    //    delete static_cast<Tuple*>(InternalTupleRef::operator*());
  }
private :
  unsigned short int _tsIdx; 
};

class MultiTuple
#ifdef TRACE
  : public TupleTrace
#endif //TRACE
{
public: 
  
  /** 
   * Contruct a MultiTupleSpace. 
   * 
   * @param closeCount The number of call to partialClose()
   * berfore effective closing of the TupleSpace.
   */
  MultiTuple(unsigned int closeCount = NUM_THREADS);

  ~MultiTuple();

public:
  /** 
   * Retreives a tuple from the TupleSpace, returning a type id. 
   * Blocs if no tuple is available and the ts. is not closed, retuns 
   * SPACE::CLOSED static value if no more tuples can ever be added to the 
   * TupleSpace (ie. if the tuple space has been closed). 
   * 
   * @param tuple A pointer to be set to the tuple. 
   * 
   * @return The tuple type. 
   */
  int get(void *&tuple, int typeID);

  int getTuple(unsigned int typeID, const Tuple &tmpl, Tuple *&pTuple);  
  
  /** 
   * \brief Same as get, but keep a copy of the tuple into the tuplespace. 
   * 
   * @param tuple A pointer to be set to the tuple. 
   * 
   * @return The tuple type. 
   */
  int read(void *&tuple);


  /** 
   * Insert a copy of tuple into the TupleSpace. 
   * 
   * @param type Type id of the tuple.
   * @param tuple Tuple data (to be casted with type). 
   */

  template<class T>
  TupleRef put(unsigned int typeID, T *t); 

  TupleRef putTuple (unsigned int typeID, const Tuple &t);

  /** 
   * Decrease the count to close the TupleSpace. The count 
   * is initialized to the number of threads by default, but 
   * it can be set to any non-null positive value see constructor. 
   * 
   */
  void partialClose();


  /** 
   * \brief Execute the function func with the tuple refered by t as a
   * parameter.
   * 
   * @param t The TupleRef to the Tuple to be used as parameter to \func.
   * @param func The function to be called with Tuple \t as a parameter. 
   * 
   * @return The return value of the function. 
   */
  int exec(TupleRef t, int (*func)(Tuple *) );


  /** 
   * \brief Removes the Tuple refered by \tr from the TupleSpace.
   * 
   * @param tr The TupleRef to the Tuple to be removed. 
   * 
   * @return Always returns 0. 
   */
  int removeTuple(TupleRef tr){
    //tr.destroyTuple();
    _data[tr._tsIdx]._tuples.erase(tr);    
    return 0; 
  }


  /** 
   * Inform that no other data will be ever added to the space. 
   */
  void close(unsigned int typeID);
  /** @returns number of elements available in the space */

  void closeAll();

  inline size_t currentSize(unsigned int typeID) const; 
  
  void dumpTupleSpace(); 

#ifdef TRACE
  int getSize(int * tab, int tailleMax) const;
#endif //TRACE
private: 

  /** 
   * \brief Private function, provide synchronized access to data. 
   *
   * If return true, the user of this function can securly access the
   * _data private member. If the space is empty and not closed it
   * will block until a new tuple is added or until the space became
   * closed in the last case it will return false.
   *
   * \warning _mutex MUST BE released by the user if returned value is true.
   *
   * @return true if secure access to _data is granted, false if the
   * space is empty and closed.
   */
  bool getAccess(); 

  void releaseAccess(); 

  bool waitTuple(int typeID); 

  pthread_mutex_t _mutex; 
  pthread_cond_t _cond; 

  unsigned int _closeCount; 
  bool _closed; 

  //TODO mettre une fille de priorite pour que l'ordre d'insertion soit concerve
  Internal _data[MAX_INTERNALS]; 

  unsigned int _nbSleeping; 
  
};


/*** FUNCTIONS DEFINITIONS ***/

template <class T>
TupleRef MultiTuple::put(unsigned int typeID, T *t){
  /* Insert a copy of t in a map indexed by his type */ 
  return TupleRef(typeID, _data[typeID].put(t)); 
}

size_t MultiTuple::currentSize(unsigned int typeID) const{
  return _data[typeID]._tuples.size(); 
}

typedef MultiTuple MultiSpace; 


#endif	    /* _MULTITUPLE_HPP_ */
