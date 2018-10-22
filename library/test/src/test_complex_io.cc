// ********************************************************************
// test_complex_io
//
// test various ways of formatting output of complex numbers
//
// 1/11/04:  Modified for gcc 3.x
// 7/24/98
//
// ********************************************************************

#include "SIScmplx.h"
#include <iostream>
#include <iomanip>
#include "ioext.h"

using namespace std;

int main()
{
  double x = 12345.6789, y = -9876.54321;
  complex z(12345.6789, 9876.54321);

  cout << "Default output, two doubles, one complex\n"
       << x << endl << y << endl << z << endl << endl;

  cout << "Use scientific notation\n" << scientific
       << x << endl << y << endl << z << endl << endl;

  cout << "Scientific, capital E\n" << uppercase
       << x << endl << y << endl << z << endl << endl;

  cout << "All on one line, setw(20) or setw(30) set before each number output\n"
       << setw(20) << x << setw(20) << y << setw(30) << z << endl << endl;

  cout << "Try a single space separator in complex\n";
  complex::out_space();
  cout << z << endl << endl;

  cout << "Back to general format, no uppercase e, but precision 15\n";
  cout << general;
  cout << nouppercase << setprecision(15) << showpoint
       << x << endl << y << endl << z << endl << endl;

  complex::out_delimited();
  cout << "Default precision; use delimited form, width 30, two complex\n"
       << setprecision(6)
       << setw(30) << z << setw(30) << conj(z) << endl << endl;

  complex::out_mode(complex::polar);
  cout << "Same, use polar output mode\n"
       << setw(30) << z << setw(30) << conj(z) << endl << endl;

  cout << "Polar output mode with default output strings\n";
  complex::out_polar();
  complex::out_default();
  cout << setw(30) << z << setw(30) << conj(z) << endl << endl;

  cout << "Polar output of 0, +/-1\n"
       << setw(20) << Complex(0.0) << setw(20) << Complex(1.0) 
       << setw(20) << Complex(-1.0) << endl << endl;

  complex::out_degree();
  cout << "Same, angles in degrees\n"
       << setw(20) << Complex(0.0) << setw(20) << Complex(1.0) 
       << setw(20) << Complex(-1.0) << endl << endl;

  cout << "Same, fixed format\n" << fixed
       << setw(20) << Complex(0.0) << setw(20) << Complex(1.0) 
       << setw(20) << Complex(-1.0) << endl << endl;

}
