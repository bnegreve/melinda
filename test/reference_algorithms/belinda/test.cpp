#include "ThreadID.hpp" 

pthread_t THREAD::_ids[100] = {0};
unsigned short int THREAD::_idx = 0;
eventData THREAD::_events[100]; 
const double THREAD::BEGIN = THREAD::getDate();
const unsigned int NUM_THREADS = 2;
int main(){
  THREAD::registerThread(); 
  THREAD::eventStart("poulet");
  sleep(1); 
  THREAD::eventEnd();
}
