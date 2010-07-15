#include <cassert>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include "Tuple.hpp"

#include <cstring>


static const int TUPLE_MAX_FORMAT_SIZE = 128; 
static const int TUPLE_MAX_DATA_SIZE = 1024; 



Tuple::Tuple(const char *fmt, ...){
  va_list ap;
  va_start(ap, fmt);
  _buildTuple(fmt, ap);
  va_end(ap);

#ifdef CHECK_INTEGRITY
  this->_sum = computeCheckSum(); 
#endif

}

Tuple::Tuple(const Tuple &t):
  _elements(t._elements), 
  _size (t._size), 
  _data ((char*)malloc(t._size)), 
  _any(t._any){
  memcpy(this->_data, t._data, t._size);  

#ifdef CHECK_INTEGRITY
  _sum = t._sum; 
#endif 

}

void Tuple::_buildTuple(const char *fmt, va_list ap){
  int d;
  void *p; 
  void *pArray; 

  char dBuf[TUPLE_MAX_DATA_SIZE]; 
  char *pd = dBuf; 

  _any = true; 
 
  while(*fmt){
    element *pElement; 
    switch(*fmt++){
    case 'i':
      _any = false; 
      _elements.push_back( element('i', sizeof(int), NULL)); 
      pElement = &_elements.back(); 
      if(*fmt == '#'){ /* if array of integers */
	pArray = va_arg(ap, void *); /* pointer to the array */
	pElement->dataSize = va_arg(ap, int) * sizeof(int); /* Number of elments in the array */
	memcpy(pd, pArray, pElement->dataSize);	
	pElement->offset = (pd - dBuf); /* Store the offset of element in the data field of each element */
	pd+=pElement->dataSize; 	
	fmt++;
      }
      else{ /* else if it is just a scalar integer */
	d = va_arg(ap, int); 
	memcpy(pd, &d, sizeof(int));
	pElement->offset = (pd - dBuf); /* Store the offset of element in the data field of each element */
	pd+=sizeof(int); 
      }
      break; 
    case 'p':
      _any = false; 
      _elements.push_back( element('p', sizeof(void*), NULL)); 
      pElement = &_elements.back(); 
      if(*fmt == '#'){ /* if array of integers */
	assert(false);
      }
      else{ /* else if it is just a scalar integer */
	p = va_arg(ap, void*); 
	memcpy(pd, &p, sizeof(void*));
	pElement->offset = (pd - dBuf); /* Store the offset of element in the data field of each element */
	pd+=sizeof(void*); 
      }
      break; 
    case 'c':
      _any = false; 
      _elements.push_back(element('c', 1, NULL)); 
      pElement = &_elements.back(); 
      if(*fmt == '#'){
	pArray = va_arg(ap, void *); /* pointer to the array */
	pElement->dataSize = va_arg(ap, int); /* Number of elments in the array */	
	memcpy(pd, pArray, pElement->dataSize);
	pElement->offset = (pd - dBuf); /* Store the offset of element in the data field of each element */
	pd+=pElement->dataSize;
	fmt++;
      }
      else{
	assert(false); 
      }
      break;
    case '?':
      _elements.push_back(element('?', 0, NULL));
      break;
    default:
      std::cerr<<"[Belinda] Unknown type identifier : "<<*(fmt-1)<<std::endl;
      exit(EXIT_FAILURE);
    }
  }
  
  this->_size = pd-dBuf; 
  this->_data = (char*)malloc(this->_size); 
  memcpy(this->_data, dBuf, this->_size); 
//   for(std::vector<element>::iterator it = this->_elements.begin(); 
//       it != this->_elements.end(); it++){
//     //offset form _data is stored in the data field of each elemet :
//     it->data = this->_data + (unsigned int)it->data;
//   }
}

void Tuple::destroy(){
  free(_data); 
}

Tuple::~Tuple(){
  free(_data); 
}

bool Tuple::matches(const Tuple &cmp) const{

  if(this->_elements.size() != cmp._elements.size())
    return false; 

  if(this->_any || cmp._any)
    return true; 

  char *p1 = this->_data; 
  char *p2 = cmp._data; 

  for(unsigned int i = 0; i < this->_elements.size(); i++){
    if(this->_elements[i].elementName == cmp._elements[i].elementName){
      if(this->_elements[i].dataSize == cmp._elements[i].dataSize)
	if(!memcmp(p1, p2, this->_elements[i].dataSize)){
	  p1+=this->_elements[i].dataSize; 
	  p2+=this->_elements[i].dataSize; 
	  continue; /* Everything is equal go on the next element */
	}
      return false; 
    }
    else{ /* if the two elements are not the same type */
      if(this->_elements[i].elementName == '?')
	p2+=cmp._elements[i].dataSize;
      else
	if(this->_elements[i].elementName == '?')
	  p1+=cmp._elements[i].dataSize; 
	else
	  return false; 
    }
  }
  return true; 
}

void Tuple::fill(const Tuple &t){
//   assert(this->matches(t)); 
//   free(this->_data); 
//   this->_elements.clear();
//   this->_elements = t._elements;
  
//   this->_size = t._size;
// //   this->_data = (char*)malloc(t._size); 
// //   memcpy(this->_data, t._data, t._size);
//   this->_data = t._data; 

//   //  std::cout<<*this<<std::endl;
}



/*** OPERATORS<< ***/

std::ostream &operator<<(std::ostream &os, const element &e){
  os<<"Element : "<<e.elementName<<" ("<<e.dataSize<<"bythes)";
  return os; 
}

std::ostream &operator<<(std::ostream &os, const Tuple &t){
  for(std::vector<element>::const_iterator it = t._elements.begin(); 
      it != t._elements.end(); ++it){
    os<<*it<<" -> "; 
    switch(it->elementName){
    case 'c':
      assert(it->dataSize > 1);
      os<<std::string(t._data+it->offset, 0, it->dataSize); 
      //      offset+=it->dataSize; 
      break; 
    case 'i': 
      if(it->dataSize / sizeof(int) == 1){
	os<<*(int*)(t._data+it->offset);
      }
      else{
	os<<"[ ";
	for(int i = 0; i < (it->dataSize)/sizeof(int); i++){
	  os<<*(int*) (t._data+it->offset)<<" ";
	}
	os<<"]";
      }
      break; 
    default :
      break;
    }
    std::cout<<std::endl;
  }
  return os; 
}





//TEST PROGRAM
#if 0

using namespace std; 
int main(){
  char *poulet = "poulet"; 
  Tuple t("c#i", poulet, 7, 4); 

  Tuple t2("?i", 4);
  
  t2.fill(t); 

  cout<<t2<<endl;
  
  cout<<*t2.getValue<int>(1)<<endl;
  cout<<t2.getValue<char>(0)<<endl;



}
#endif
