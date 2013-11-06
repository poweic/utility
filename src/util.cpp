#include <util.h>
using namespace std;

extern "C" void libutil_is_present(void) {}

namespace util {

  void appendSlashAtTheEnd(string& str) {
    if(str[str.size() - 1] != '/')
      str += "/";
  }

  int str2int(string str) {
    int n = 0;
    stringstream ss;
    ss << str;
    ss >> n;
    return n;
  }

  double str2double(string str) {
    double n = 0;
    stringstream ss;
    ss << str;
    ss >> n;
    return n;
  }

  long double str2longdouble(string str) {
    long double n = 0;
    stringstream ss;
    ss << str;
    ss >> n;
    return n;
  }

  bool str2bool(string str) {
    return (str == "true");
  }

  int ss2int(stringstream &ss) {
    int n = 0;
    ss >> n;
    return n;
  }

  string int2str(int n) {
    string str;
    stringstream ss;
    ss << n;
    ss >> str;
    return str;
  }

  string double2str(double n) {
    string str;
    stringstream ss;
    ss << n;
    ss >> str;
    return str;
  }

#define  A1  (-3.969683028665376e+01)
#define  A2   2.209460984245205e+02
#define  A3  (-2.759285104469687e+02)
#define  A4   1.383577518672690e+02
#define  A5  (-3.066479806614716e+01)
#define  A6   2.506628277459239e+00

#define  B1  (-5.447609879822406e+01)
#define  B2   1.615858368580409e+02
#define  B3  (-1.556989798598866e+02)
#define  B4   6.680131188771972e+01
#define  B5  (-1.328068155288572e+01)

#define  C1  (-7.784894002430293e-03)
#define  C2  (-3.223964580411365e-01)
#define  C3  (-2.400758277161838e+00)
#define  C4  (-2.549732539343734e+00)
#define  C5   4.374664141464968e+00
#define  C6   2.938163982698783e+00

#define  D1   7.784695709041462e-03
#define  D2   3.224671290700398e-01
#define  D3   2.445134137142996e+00
#define  D4   3.754408661907416e+00

#define P_LOW   0.02425
  /* P_high = 1 - p_low*/
#define P_HIGH  0.97575
  // Output range under the accuracy of long double
#define MAX_DEVIATION ((double) 1/0)

  long double normsinv(long double p) {
    long double x;
    long double q, r, u, e;


    if( p > 1 || p < 0) return NAN;
    if( p == 0 ) return -MAX_DEVIATION;
    if( p == 1 ) return +MAX_DEVIATION;

    if (p < P_LOW) {
      q = sqrt(-2*log(p));
      x = (((((C1*q+C2)*q+C3)*q+C4)*q+C5)*q+C6) / ((((D1*q+D2)*q+D3)*q+D4)*q+1);
    }
    else if ( p > P_HIGH ) {
      q = sqrt(-2*log(1-p));
      x = -(((((C1*q+C2)*q+C3)*q+C4)*q+C5)*q+C6) / ((((D1*q+D2)*q+D3)*q+D4)*q+1);
    }
    else {
      q = p - 0.5;
      r = q*q;
      x = (((((A1*r+A2)*r+A3)*r+A4)*r+A5)*r+A6)*q /(((((B1*r+B2)*r+B3)*r+B4)*r+B5)*r+1);
    }

    // If you are compiling this under UNIX OR LINUX, you may uncomment this block for better accuracy.
    e = 0.5 * erfc(-x/sqrt(2)) - p;
    u = e * sqrt(2*M_PI) * exp(x*x/2);
    x = x - u/(1 + x*u/2);

    return x;
  }

  void system(string command) {
      ::system(command.c_str());
  }

}
