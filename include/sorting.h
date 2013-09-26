#ifndef __UTIL_SORTING_H_
#define __UTIL_SORTING_H_

#include <util.h>
#include <iostream>
using namespace std;

namespace util {
  class sorting {
    public:
      enum sortingOrder {
	ascend,
	descend
      };

      template <class T>
      static void sort(Array<T> &arr, sortingOrder order) {
	switch (order) {
	  case ascend:
	    _quicksort_ascend(arr, 0, arr.size() - 1); break;
	  case descend:
	    _quicksort_descend(arr, 0, arr.size() - 1); break;
	}
      }

      template <class T>
      static void sort(Array<T> &arr, sortingOrder order, Array<int> &permutation) {
	Array<T> temp(arr);
	sort(arr, order);

	permutation.resize(arr.size());
	for(size_t i=0; i<arr.size(); ++i)
	  permutation[i] = temp.find(arr[i]);
      }

    private:
      sorting() {}
      template <class T>
      static void _quicksort_ascend(Array<T> &arr, int left, int right) {
	_quicksort_descend(arr, left, right);
	arr.reverse();
      }

      template <class T>
      static void _quicksort_descend(Array<T> &arr, int left, int right) {
	if(left < right) { 
	  int i = left; 
	  int j = right + 1; 

	  while(1) { 
	    while(i + 1 < (int) arr.size() && arr[++i] > arr[left]) ;  
	    while(j - 1 > -1 && arr[--j] < arr[left]) ;  
	    if(i >= j) 
	      break; 
	    swap(arr[i], arr[j]); 
	  } 
	  swap(arr[left], arr[j]); 

	  _quicksort_descend(arr, left, j-1);
	  _quicksort_descend(arr, j+1, right);
	} 
      }
  };
};

#endif // __UTIL_SORTING_H_
