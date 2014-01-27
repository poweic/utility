#include <array.h>
using namespace std;

// ======================== Specialization =========================== 
// [string]
template <> Array<string>::Array(size_t s): _data(0), _size(0), _capacity(0) {
    _size = s;
    _capacity = _calcSuitableCapacity(s);
    _data = new string[_capacity];
}

extern "C" void libarray_is_present(void) {}

#define _optimize_constructor_for_type(T)   \
template <> Array<T>::Array(size_t s) {	    \
  _init(s);				    \
  memset(_data, 0, _capacity*sizeof(T));    \
}

#define _optimize_copy_constructor_for_type(T)	    \
template <> Array<T>::Array(const Array<T>& arr) {  \
  _init(arr._size);				    \
  memcpy(_data, arr._data, _size*sizeof(T));	    \
}


#define _optimize_expand_for_type(T)			\
template <> void Array<T>::expand(size_t c) {		\
  T* temp = new T[_capacity = c];\
  memcpy(temp, _data, _size*sizeof(T));			\
  delete [] _data;					\
  _data = temp;						\
}

#define _optimize_operator_equal_for_type(T)		  \
template <> Array<T>& Array<T>::operator = (const Array<T>& arr) {  \
  if(this == &arr)			      \
    return *this;			      \
					      \
  _capacity = arr._capacity;		      \
  _size = arr._size;			      \
  if(_data != NULL)			      \
    delete [] _data;			      \
  _data = new T[_capacity];		      \
					      \
  memcpy(_data, arr._data, _size*sizeof(T));  \
  return *this;				      \
}					    

#define _optimize_append_for_type(T) \
template <> void Array<T>::append(const Array<T>& array) { \
    size_t offset = _size; \
    this->resize(_size + array._size); \
    memcpy(_data + offset, array._data, (array._size) * sizeof(T)); \
}

#define _optimize_functions_for_type(T)	  \
  _optimize_constructor_for_type(T);	  \
  _optimize_copy_constructor_for_type(T); \
  _optimize_append_for_type(T);		  \
  _optimize_operator_equal_for_type(T);	  \
  _optimize_expand_for_type(T);		

#define _optimize_for_POD		  \
_optimize_functions_for_type(int);	  \
_optimize_functions_for_type(double);	  \
_optimize_functions_for_type(long double);

_optimize_for_POD;

