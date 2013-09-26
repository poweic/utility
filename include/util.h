#ifndef _STATISTIC_H
#define _STATISTIC_H

#pragma GCC diagnostic ignored "-Wdiv-by-zero"

#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>

#include <color.h>

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))
#define PI 3.14159265357
using namespace std;

namespace util {
  class MyException {
    private:
      string m_strError;

      MyException() {}; // not meant to be called
    public:
      MyException(string strError): m_strError(strError) {}
      string getError() { return m_strError; }
  };

  template <class T>
  void swap(T& a, T& b) {
    T temp = T(a);
    a = T(b);
    b = T(temp);
  }

  void appendSlashAtTheEnd(string& str);

  int ss2int(stringstream &ss);
  int str2int(string str);
  double str2double(string str);
  long double str2longdouble(string str);
  bool str2bool(string str);

  string int2str(int x);
  string double2str(double x);
  string longdouble2str(long double x);

  void system(string command);
  long double normsinv(long double p);

  template <class T1, class T2>
  class kvPair {
    public:
      kvPair(): _key(T1()), _value(T2()) {}
      kvPair(const T1& k, const T2& v): _key(k), _value(v) {}

      kvPair<T1, T2>& operator = (const kvPair<T1, T2>& source) {
	_key = source._key;
	_value = source._value;
	return *this;
      }

      template <class U, class V>
	kvPair(const kvPair<U,V> &source): _key(source._key), _value(source.value) {}

      bool operator > (const kvPair<T1, T2>& rhs) {
	return (_value > rhs._value);
      }
      bool operator >= (const kvPair<T1, T2>& rhs) {
	return (_value >= rhs._value);
      }
      bool operator < (const kvPair<T1, T2>& rhs) {
	return (_value < rhs._value);
      }
      bool operator <= (const kvPair<T1, T2>& rhs) {
	return (_value <= rhs._value);
      }

      T1 _key;
      T2 _value;
  };

};

#endif
