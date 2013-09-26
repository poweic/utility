#ifndef _MY_VECTOR_H
#define _MY_VECTOR_H
#include <vector>
#include <cmath>
#include "array.h"
using namespace std;

template <typename T> class MyVector;

template <typename T>
MyVector<T> operator*(double c, const MyVector<T>& rhs);

template <typename T>
class MyVector : public Array<T> {
public:
    MyVector(): Array<T>() {}
    MyVector(size_t n): Array<T>(n) {}
    MyVector(size_t n, T* ptr): Array<T>(n, ptr) {}
    MyVector(const MyVector<T>& source): Array<T>(source) {}
    MyVector(const std::vector<T>& source) {
	Array<T>::resize(source.size());
	for(int i=0; i<Array<T>::_size; ++i)
	    Array<T>::_data[i] = source[i];
    }

    friend MyVector<T> operator*<T>(double, const MyVector<T>&);

    // GET Function
    int getDim() const { return Array<T>::_size; }

    // Operator Overloading
    MyVector<T> operator + (const MyVector<T> v2) const {
	MyVector<T> tmp(*this);	return (tmp += v2);
    }

    MyVector<T> operator - (const MyVector<T> v2) const {
	MyVector<T> tmp(*this);	return (tmp -= v2);
    }

    MyVector<T> operator * (const T coeff) const {
	MyVector<T> tmp(*this);	return (tmp *= coeff);
    }

    MyVector<T> operator / (const T coeff) const {
	MyVector<T> tmp(*this);	return (tmp /= coeff);
    }

    MyVector<T>& operator += (const MyVector<T> v2) {
	for(size_t i=0; i<Array<T>::_size; ++i)
	    Array<T>::_data[i] += v2.Array<T>::_data[i];
	return *this;
    }

    MyVector<T>& operator -= (const MyVector<T> v2) {
	for(size_t i=0; i<Array<T>::_size; ++i)
	    Array<T>::_data[i] -= v2.Array<T>::_data[i];
	return *this;
    }

    MyVector<T>& operator *= (const T coeff) {
	for(size_t i=0; i<Array<T>::_size; ++i)
	    Array<T>::_data[i] *= coeff;
	return *this;
    }

    MyVector<T>& operator /= (const T coeff) {
	for(size_t i=0; i<Array<T>::_size; ++i)
	    Array<T>::_data[i] /= coeff;
	return *this;
    }

    // Static Utility Functions
    static double norm(const MyVector& v) {
	double t=0;
	for(size_t i=0; i<v.Array<T>::_size; ++i)
	    t += v.Array<T>::_data[i]*v.Array<T>::_data[i];
	return sqrt(t);
    }
    static double dis(const MyVector& v1, const MyVector& v2) { return norm(v1-v2); }
};
typedef MyVector<double> vec;

template <typename T>
MyVector<T> operator*(double c, const MyVector<T>& vector) {
    MyVector<T> temp(vector._size);
    for(size_t i=0; i<vector.Array<T>::_size; ++i)
	vector.Array<T>::_data[i] *= c;
    return temp;
}

#endif
