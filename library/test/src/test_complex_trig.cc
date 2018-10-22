// test the inverse trig and hyp fcns in SIScmplx

#include "SIScmplx.h"
#include "global.h"
#include <iostream>

using namespace std;

int main()
{
  double d1;
  complex z1;
  int i, n = 10;
  double min, dx = Pi/n;
  complex::out_degree();

  // get rid of tiny imaginary parts
  Complex::rel_tolerance = Complex::abs_tolerance = 1.0e-10;

  // compare complex and real versions of fcns
  min = -Pi/2;
  for(i = 1, d1 = min + dx; i < n; ++i, d1 += dx) {
    z1 = d1;
    cout << "sin: " << sin(z1) - sin(d1)
	 << " , tan: " << tan(z1) - tan(d1)
	 << " , cos: " << cos(z1+Pi/2) - cos(d1+Pi/2)
	 << endl;
    cout << "sinh: " << sinh(z1) - sinh(d1)
	 << " , tanh: " << tanh(z1) - tanh(d1)
	 << " , cosh: " << cosh(z1+Pi/2) - cosh(d1+Pi/2)
	 << endl;
  }
  min = -1; dx = 2.0/n;
  for(i = 1, d1 = min + dx; i < n; ++i, d1 += dx) {
    z1 = d1;
    cout << "asin: " << asin(z1) - asin(d1)
	 << " , atan: " << atan(z1) - atan(d1)
	 << " , acos: " << acos(z1) - acos(d1)
	 << endl;
    cout << "asinh: " << asinh(z1) - asinh(d1)
	 << " , atanh: " << atanh(z1) - atanh(d1)
	 << " , acosh: " << acosh(z1+2) - acosh(d1+2)
	 << endl;
  }
}
