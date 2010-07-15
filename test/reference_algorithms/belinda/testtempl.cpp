#include <iostream>
#include <map>
using namespace std; 

template<typename K,typename T>
class test{
public:  
  void f();
};

template <typename K , typename T> 
void test<K,T>::f(){
  map<K,T>::iterator it; 
}


int main(){
  test<int, int> a; 
}

