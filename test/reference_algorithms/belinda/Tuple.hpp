//
// Tuple.hpp
// 
// Made by Benjamin Negrevergne
// Login   <bengreve@confiance.imag.fr>
// 
// Started on  Tue Mar 24 09:45:47 2009 Benjamin Negrevergne
// Last update Thu Apr  2 15:37:34 2009 Benjamin Negrevergne
//

#ifndef   	_TUPLE_HPP_
#define   	_TUPLE_HPP_

#include <cstdarg>
#include <cassert>
#include <cstddef>
#include <vector>





class element;

class Tuple{
public :
  friend std::ostream &operator<<(std::ostream &, const Tuple &);
  friend class MultiTuple;
  friend class TupleSpace;

  Tuple(const char *fmt, ...);
  
  Tuple(const Tuple &t);
  
  ~Tuple();

  void destroy(); 
  bool matches(const Tuple &cmp) const;
 
  void fill(const Tuple &t);

  template<class T>
  T &getValue(int index);

  inline void print(){
    std::cout<<*this<<std::endl;
  }
  
  


#ifdef CHECK_INTEGRITY
  inline int computeCheckSum(); 
  inline void checkMyIntegrity(); 
  inline void checkIntegrity(int checkSum);
#endif

private: 
  void _buildTuple(const char *fmt, std::va_list ap);
  
  size_t _parseNext(const char *&fmt); 

  char *_data; 
  size_t _size;
  std::vector<element> _elements;
  bool _any; 
#ifdef CHECK_INTEGRITY
  int _sum; 
#endif

};


/** \class element
 * 
 * \brief Enhance a tuple entry (deal with typing).
 */
class element{
public:
  element(char _elementName, unsigned int _dataSize, unsigned short int _offset):
    elementName(_elementName), 
    dataSize(_dataSize), 
    offset(_offset){
  }

  char elementName;
  int dataSize;
  unsigned short int offset; 
};


/* TEMPLATE DEFINITIONS */

template<class T>
T &Tuple::getValue(int index) {
  assert(index < _elements.size()); 
  return  *(T*)(this->_data+_elements[index].offset); 

}



#ifdef CHECK_INTEGRITY
int Tuple::computeCheckSum(){
  int sum = 0; 
  for(int i = 0; i < _size; i++){
    sum += this->_data[i]; 
  }

  for(int i = 0; i < _elements.size(); i++){
    sum += _elements[i].dataSize;
    sum += _elements[i].elementName;
  }
  return sum; 
}

void Tuple::checkMyIntegrity(){
  checkIntegrity(_sum); 
}

void Tuple::checkIntegrity(int checkSum){  
  int sum = computeCheckSum(); 
  if(sum != checkSum){  
    std::cerr<<"Check Integrity failed : "<<checkSum<<" != "<<sum<<")."<<std::endl;
    assert(false); 
  }
  //  else
    //    std::cerr<<"Tuple OK"<<std::endl;
}
#endif //CHECK_ITEGRITY


/*** OPERATORS<< ***/ 
std::ostream &operator<<(std::ostream &, const Tuple &); 


#endif	    /* _TUPLE_HPP_ */
