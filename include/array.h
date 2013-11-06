/****************************************************************************
  FileName     [ array.h ]
  PackageName  [ util ]
  Synopsis     [ Define dynamic array package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-2010 LaDs(III), GIEE, NTU, Taiwan ]
 ****************************************************************************/

#ifndef ARRAY_H
#define ARRAY_H
#include <iostream>
#include <fstream>
#include <cstring>
#include <util.h>
#include <iomanip>
#include <cmath>
#include <vector>
using namespace std;

template <class T>
class Array {
public:

  Array(): _data(0), _size(0), _capacity(0) {
    _init(0);
  }

  Array(size_t s) {
    _init(s);
    for(size_t i=0; i<_capacity; ++i)
      _data[i] = T();
  }
  
  Array(size_t s, T* ptr) {
    _init(s);
    if(ptr == NULL)
      throw util::MyException("Failed to contruct Array from NULL pointer.");

    memcpy(_data, ptr, _capacity*sizeof(T));
  }
  
  Array(string filename) {
    _init(0);
    fstream iFile(filename.c_str(), ios::in);

    T buf;
    while(iFile >> buf)
      push_back(buf);

    iFile.close();
  }
  
  Array(const Array<T>& arr) {
    _init(arr._size);
    for(size_t i=0; i<_size; ++i)
      _data[i] = arr._data[i];
  }

  ~Array() {
    delete [] _data; 
  }

  bool isEmpty() const {
    return (_size == 0);
  }

  size_t size() const {
    return _size;
  }

  size_t capacity() const {
    return _capacity;
  }

  T& operator [] (size_t i) {
    return _data[i];
  }

  T const& operator [] (size_t i) const {
    return _data[i]; 
  }

  operator std::vector<T> () {
    std::vector<T> vec(_size);
    for (size_t i=0; i<_size; ++i)
      vec[i] = _data[i];
    return vec;
  }

  Array<T>& operator = (const Array<T>& arr) {

    if(this == &arr)
      return *this;

    _capacity = arr._capacity;
    _size = arr._size;
    if(_data != NULL)
      delete [] _data;
    _data = new T[_capacity];

    for(size_t i=0; i<_size; ++i)
      _data[i] = arr._data[i];
    return *this;
  }

  bool operator == (const Array<T>& rhs) {
    for(size_t i=0; i<_size; ++i) {
      if(!(_data[i] == rhs._data[i]))
	return false;
    }
    return true;
  }

  bool operator != (const Array<T>& rhs) {
    return !(*this == rhs);
  }

  void pop_front(){
    if(isEmpty())
      return;

    for(size_t i=0; i<_size; ++i)
      _data[i] = _data[i+1];
    --_size;
  }

  void pop_back() {
    if(isEmpty())
      return;

    _data[_size-1] = _data[_size];
    --_size;
  }

  void push_back(const T& x) {
    if(_size + 1 >= _capacity) 
      expand(_calcSuitableCapacity(_size + 1));

    _data[_size+1] = _data[_size];
    _data[_size] = x;
    ++_size;
  }

  void erase(size_t idx) {
    if(_size == 0)
      return;

    for(size_t i=idx; i<_size-1; ++i)
      _data[i] = _data[i+1];
    --_size;
  }

  int find(const T x) const{
    for(size_t i=0; i<_size; ++i) {
      if(_data[i] == x)
	return i;
    }
    return -1;
  }

  void print(size_t element_per_line = 5) const {
    cout << "[Element]: " << endl;
    cout.precision(4);
    cout << fixed;

    size_t n = (size_t) ceil(log10(_size));
    for(size_t i=0; i<_size; ++i) {
      cout << "[" << setw(n) << i << "] = " << _data[i] << "\t";
      if(i % element_per_line == element_per_line - 1)
	cout << endl;
    }
    cout << endl;
  }

  void clear() {
    _data[0] = T(_data[_size]);
    _size = 0;
  }

  void expand(size_t c) {
    T* temp = new T[_capacity = c];

    for(size_t i=0; i<_size; ++i)
      temp[i] = _data[i];

    delete [] _data;
    _data = temp;
  }

  void resize(size_t n) {
    if(n+1 >= _capacity)
      expand(_calcSuitableCapacity(n));
    _size = n;
  }

  void reverse() {
    Array<T> temp(*this);
    for(size_t i=0; i< _size; ++i)
      _data[_size - 1 - i] = temp._data[i]; 
  }

  void saveas(string filename) {
    fstream oFile;
    oFile.open(filename.c_str(), fstream::out);

    for(size_t i=0; i<_size; ++i)
      oFile << _data[i] << endl;

    oFile.close();
  }

  void saveAsBinary(string filename) const {
    FILE* filePtr = fopen(filename.c_str(), "w");
    fseek (filePtr, 0L, SEEK_SET);

    fwrite( &_size, sizeof(_size), 1, filePtr);

    fwrite(_data, sizeof(T), _size, filePtr);

    fclose(filePtr);
  }
  
  void readAsBinary(string filename) {

    FILE* filePtr = fopen(filename.c_str(), "r");
    if(filePtr == NULL)
      throw util::MyException("Failed to read binary file. File: " + filename + " may not exist");
    fseek (filePtr, 0L, SEEK_SET);

    size_t size;
    fread( &size, sizeof(size), 1, filePtr);
    this->resize(size);

    fread(_data, sizeof(T), _size, filePtr);

    fclose(filePtr);
  }

  void append(const Array<T>& array) {

    size_t offset = _size;
    this->resize(_size + array._size);
    for(size_t i=0; i<array._size; ++i)
      _data[offset + i] = array._data[i];
  }

  void reserve(size_t capacity) {
    if(capacity > _capacity)
      expand(capacity);
  }

  template <typename S>
  friend fstream& operator << (fstream& fs, const Array<S>& arr);

protected:
    T*           _data;
    size_t       _size;       // number of valid elements
    size_t       _capacity;   // max number of elements
private:

  void _init(size_t s) {
    _size = s;
    _capacity = _calcSuitableCapacity(s);
    _data = new T[_capacity];
  }

  static size_t _calcSuitableCapacity(size_t s) {
    size_t capacity = 1;
    while((s = s >> 1) > 0)
      capacity = capacity << 1;
    return capacity << 1;
  }
};
  
template <class T>
fstream& operator << (fstream& fs, const Array<T>& arr) {
  for(size_t i=0; i<arr._size; ++i)
    fs << arr[i];
  return fs;
}

#endif // ARRAY_H
