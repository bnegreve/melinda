//
// ThreadID.hpp
// 
// Made by Benjamin Negrevergne
// Login   <bengreve@confiance.imag.fr>
// 
// Started on  Wed Apr 22 18:06:20 2009 Benjamin Negrevergne
// Last update Wed Apr 22 18:06:20 2009 Benjamin Negrevergne
//

#ifndef   	_THREADID_HPP_
# define   	_THREADID_HPP_

#include <cassert>
#include <pthread.h>
#include <sstream>
#include <iostream>
#include <time.h>
#include <sys/time.h>
#include <fstream>
extern const unsigned int NUM_THREADS; 

//#define TRACE_EVENTS

#ifdef TRACE_EVENTS
struct eventData{
public: 
  double startDate; 
  std::string eventName;
};
#endif //TRACE_EVENTS

class THREAD{
public:   
  static inline void registerThread();
  static inline unsigned short int getMyID();

  static inline void eventStart(const std::string &eventName); 
  static inline void eventEnd(); 
  static inline double getDate();

private: 
  static pthread_t _ids[100]; 
  static unsigned short int _idx; 

#ifdef TRACE_EVENTS
  static eventData _events[100]; 
  static const double BEGIN; 
#endif //TRACE_EVENTS
};

void THREAD::registerThread(){
  static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 
  pthread_mutex_lock(&mutex); 
  assert(_idx < NUM_THREADS); 
  _ids[_idx++] = pthread_self();  
  pthread_mutex_unlock(&mutex); 
}

unsigned short int THREAD::getMyID(){
  pthread_t id = pthread_self(); 
  for (int i = 0; i < NUM_THREADS; i++){
    if(_ids[i] ==  id)
      return i; 
  }
  assert(false); 
}


#ifdef TRACE_EVENTS
void THREAD::eventStart(const std::string &eventName){
  int tid = getMyID(); 
  _events[tid].startDate = getDate(); 
  _events[tid].eventName = eventName; 
}

void THREAD::eventEnd(){

  unsigned short int tid = getMyID(); 
  static std::ofstream of[100]; 
  std::ofstream &rof = of[tid]; 
  if(!rof.is_open()){
    static std::ostringstream oss[100]; 
    oss[tid]<<"trace_"<<tid;
    rof.open(oss[tid].str().c_str()); 
  }
  
  rof<<tid<<"\t"<<_events[tid].startDate - BEGIN<<"\t"<<
    getDate() - BEGIN<<"\t"<<_events[tid].eventName<<std::endl; 
  //  pthread_mutex_lock(&mutex); 
  //  rof<<ross.str()<<std::endl;  
  //  pthread_mutex_unlock(&mutex); 
  //  ross.str("");
}

double THREAD::getDate(){
  struct timeval tv; 
  gettimeofday(&tv, NULL); 
  
  return ((tv.tv_sec*1000000) + tv.tv_usec) ; 
}
#else

void THREAD::eventStart(const std::string &eventName){
}

void THREAD::eventEnd(){
}

double THREAD::getDate(){
}
#endif //TRACE_EVENTS

#endif	    /* _THREADID_HPP_ */
