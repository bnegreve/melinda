#include <cstdlib>
#include <iostream>
#include <cassert>
#include <cstring>


#include "MultiTuple.hpp" 

using std::cout; 
using std::cerr;
using std::endl; 

const unsigned int TUPLE_BUFFER_SIZE = 128; 
  
MultiTuple::MultiTuple(unsigned int closeCount){
  pthread_mutex_init(&_mutex, NULL); 
  pthread_cond_init(&_cond, NULL); 
  _closeCount = closeCount; 
  _closed = false; 
  _nbSleeping = 0;
}

MultiTuple::~MultiTuple(){
  pthread_mutex_destroy(&_mutex); 
  pthread_cond_destroy(&_cond); 
  for(int i = 0; i < trace_access_count; i++){
    cout<<trace_access[i];
  }
  cout<<endl;
}

bool MultiTuple::getAccess(){
#ifdef TRACE
  traceCollector.setTotalDurationOfPreviousTask();
  traceCollector.startNewTask(typeid(tuple).name());
#endif //TRACE
  pthread_mutex_lock(&_mutex);    
  return true; 
}

bool MultiTuple::waitTuple(int typeID){   
//   while (_data[typeID].empty()){ /* If there is nothing in _data ... */
//     if (_closed){ /* ... if the tuplespace is closed returns nothing */
//       pthread_mutex_unlock(&_mutex); 
// #ifdef TRACE
//       traceCollector.setWaitDurationOfCurrentTask();
// #endif //TRACE
//       return false; 
//     }
//     else{ /* ... if the tuplespace is not closed wait for a tuple */
//       if(++_nbSleeping == NUM_THREADS){	
//         _closed = true; 
//         pthread_mutex_unlock(&_mutex); 
//         pthread_cond_broadcast(&_cond); 
// #ifdef TRACE
//         traceCollector.setWaitDurationOfCurrentTask();
// #endif //TRACE
//         return false;
//       }
//       else
// 	  pthread_cond_wait(&_cond, &_mutex);
//     }
//   }
//   return true; 
}

int MultiTuple::get(void *&tuple, int typeID){
  int r; 
  if((r = _data[typeID].get(tuple)) == 0)
    return typeID;
  else
    return r;
}
  
//   if(getAccess() == false)
//     return SPACE_CLOSED; 
//   /* Take out an entry ... and returns it. */
  
//   TupleSpaceInternal::iterator entry = _data[typeID].begin(); 

//  //  unsigned int typeID = entry->first;
//   tuple = *entry; 
//   _data[typeID].erase(entry); 
//   pthread_mutex_unlock(&_mutex);
// #ifdef TRACE
//   string type;
//   switch(typeID) {
//   case 0: type="CFT"; break;
//   case 1: type="Saturated DAG"; break;
//   case 2: type="Saturated DAG TS"; break;
//   }
// //  std::cout<< ">>>>>>>>>>>>" << type << std::endl;
//   traceCollector.setTypeOfCurrentTask(type.c_str());
//   traceCollector.setWaitDurationOfCurrentTask();
// #endif //TRACE
//   //  return typeID; 
//   return 1; 


int MultiTuple::read(void *&tuple){
  /* Not implemented yet */
  assert(false); 
}

void MultiTuple::partialClose(){
//   pthread_mutex_lock(&_mutex);
//   if(--_closeCount == 0)
//     _closed=true;
//   pthread_mutex_unlock(&_mutex);
//   pthread_cond_broadcast(&_cond); 
}

void MultiTuple::close(unsigned int typeID){
  Internal *ts = &_data[typeID];
  ts->close(); 

//   _closed=true; 
//   pthread_mutex_unlock(&_mutex);
//   pthread_cond_broadcast(&_cond); 
}

void MultiTuple::closeAll(){
  for(int i = 0; i < MAX_INTERNALS; i++){
    _data[i].close();
  }
}

TupleRef MultiTuple::putTuple (unsigned int typeID, const Tuple &t){
  return put(t._elements.size(), new Tuple(t)); 
}

int MultiTuple::getTuple(unsigned int typeID, const Tuple &tmpl, Tuple *&pTuple){
  int dataIdx = tmpl._elements.size();
  Internal *internal = &_data[dataIdx];
  internal->getAccess();
  while(internal->waitTuple()){
//     TupleSpaceInternal::iterator entry = _data.lower_bound(typeID); 
//     TupleSpaceInternal::iterator uBound = _data.upper_bound(typeID); 
//    while(entry  != uBound){

    for(internal_t::iterator entry = internal->_tuples.begin();
	entry != internal->_tuples.end(); ++entry){

      //assert(entry->first == typeID);
      Tuple *t = static_cast<Tuple*>(*entry);
      if(tmpl.matches(*t)){
	internal->_tuples.erase(entry); /* Remove tuple from tuplespace */
	trace_access[trace_access_count++] = 'G';
	internal->releaseAccess(); 
	pTuple = t;
#ifndef NDEBUG
#ifdef CHECK_INTEGRITY
	t->checkMyIntegrity(); 
#endif
#endif 
	return typeID; 
      }
    }
    internal->releaseAccess();
    return NO_MATCHES; 
  }
  internal->releaseAccess();
  return SPACE_CLOSED;
}

void MultiTuple::releaseAccess(){
  assert(false); 
}

int MultiTuple::exec(TupleRef tr, int (*func)(Tuple *) ){
  _data[tr._tsIdx].getAccess();
  Tuple *t = *tr;
  _data[tr._tsIdx].releaseAccess();
  return func(t);
}

void MultiTuple::dumpTupleSpace(){
//   for(TupleSpaceInternal::const_iterator it = _data.begin(); 
//       it != _data.end(); ++it){
//     std::cout<<"DUMPING "<<std::endl;
//     std::cout<<*static_cast<Tuple*>(*it)<<std::endl;
//   }
}

#ifdef TRACE
int MultiTuple::getSize(int * tab, int taille) const {
  int i = 0;
  for (i =0; i < 3; i++) {
    tab[i] = _data.count(i);
  }
  return 3;
}
#endif //TRACE

typedef struct {
  int a; 
  int b; 
  int c; 
  double d; 

}test_t;


#ifdef TEST_TUPLE

#define CHECK_INTEGRITY

const unsigned int NUM_THREADS = 8; 

int func(Tuple *t){
  std::cout<<"EXEC"<<*t<<"CHMOUT"<<std::endl;
  delete t;
}

static void thread(MultiTuple *ts){
  static Tuple pat("???"); 
  Tuple *t; 
  while( (ts->getTuple(1, pat, t)) == 1){
    //    std::cout<<t->getValue<int>(0)<<" "<<t->getValue<int>(1)<<" "<<t->getValue<int>(2)<<std::endl;
    for(int i = 0; i < t->getValue<int>(0); i++);
  }
  
  delete t;
}

int main(){

  MultiTuple ts; 

  pthread_t tids[NUM_THREADS];
  
  for(int i = 0; i < NUM_THREADS; i++)
    pthread_create(&tids[i], NULL, (void*(*)(void*))thread, &ts); 

  for(int i = 0; i < 1000000; i++){
    ts.putTuple(1, Tuple ("iii", 1000, i, i));
  }
  ts.closeAll();

  for(int i = 0; i < NUM_THREADS; i++)
    pthread_join(tids[i], NULL);


  int a; 

  static const int TYPE_INT = 0;
  static const int TYPE_DOUBLE = 1;


//   double d = 3.141592654;
//   mt.put(TYPE_DOUBLE, &d); 

//   void *test; 
//   mt.get(test, 1); 
  
//  cout<<*(double*)test<<endl;
//  exit(1); 

//   mt.partialClose(); 
//   mt.partialClose();


//   int type;

//   int x[2] = {3, 14}; 
//   int x2[2] = {3, 15}; 

  
  
//   TupleRef r =  mt.putTuple(4, Tuple("iii", 3, 14, 16)); 
//    mt.putTuple(4, Tuple("iii", 3, 15, 16)); 
//    mt.putTuple(4, Tuple("iii", 5, 17, 18)); 

//    //mt.putTuple(4, Tuple("iiii", 3, 14, 16, 159)); 

//    mt.closeAll();
  

//   mt.exec(r, func); 
//   mt.removeTuple(r);
  
// //   exit(1);
// //   int xx[2] = {0};
// //   int k; 

// //  Tuple x("????");
//   Tuple t("???");
//   for(;;){    
//     Tuple *tup; 
// //     if(mt.getTuple(4, x, tup) == 4){
// //       cout<<*tup<<endl;
// //       std::cout<<"DELETING tup"<<std::endl;
// //       delete tup; 
// //     }
// //     else
// //        break ;

//     if(mt.getTuple(4, t, tup) == 4){
//       cout<<*tup<<endl;
//       std::cout<<"DELETING tup"<<std::endl;
//       delete tup; 
//     }
//     else
//        break ;
    
//    }

//   exit(1); 

}
#endif //0
