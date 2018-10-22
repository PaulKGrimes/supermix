// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ********************************************************************
// SIScmplx.h
// Complex data type and functions upon it
//
//        *** NOT THE SAME AS THE C++ complex<> CLASS ***
//
// F. Rice 7/8/97
// ********************************************************************
//
//      The Complex data type consists of two (public) doubles.
//        The following operations are defined by this file:
//
//                  (z: Complex; d: double; n:int)
//
//
//  Basic creation, access, and assignment:
//    Note that "complex" is an alias for "Complex" (using typedef)
//
//  Complex z;          create z with initial value 0 + 0 I
//  Complex z(d);       initial value d + 0 I
//  Complex z(d1,d2);   initial value d1 + d2 I
//
//  z.real;             access parts of z (each a double)
//  z.imaginary;        (these are lvalues)
//
//  real(z);            read parts of z (each a double)
//  imag(z);            (these are rvalues)
//
//  z = z1;             assign values to z
//  z = Complex(d1,d2); z = d1 + d2 I
//  z = d;
//  z = n;              (for the last 2 cases, z.imaginary = 0.0)
//
//
//  Arithmetic operators:
//
//  z = z1 + z2;                    z = z1 - z2;
//  z = z1 + d;    z = d + z1;      z = z1 - d;    z = d - z1;
//  z = z1 + n;    z = n + z1;      z = z1 - n;    z = n - z1;
//
//  z = z1 * z2;                    z = z1 / z2;
//  z = z1 * d;    z = d * z1;      z = z1 / d;    z = d / z1;      
//  z = z1 * n;    z = n * z1;      z = z1 / n;    z = n / z1;
//
//  z = -z1;            unary minus
//  z = +z1;            unary plus
//
//
//  Additional assignment operators:
//
//  z += z1;       z -= z1;         z *= z1;       z /= z1;
//  z += d;        z -= d;          z *= d;        z /= d;
//  z += n;        z -= n;          z *= n;        z /= n;
//
//
//  Relational operators:
//
//  z == z1;       z != z1;
//  z == d;        z != d;      needs z.imaginary == 0.0 for z == d  
//
//
//  Other functions:
//
//  z = polar(rho,theta);        construct z from polar form (doubles)
//  z = conj(z1), zconj(z1);     complex conjugate of z1
//  d = abs(z1),  zabs(z);       magnitude of z
//  d = arg(z1),  zarg(z);       argument (phase) of z, in range: +/- Pi
//  d = norm(z1), zmagsq(z);	 magnitude of z squared (z*z)
//  z = sqrt(z1);                square root, arg(z) will be in +/- Pi/2
//  z = log(z1); z = exp(z1);    complex log and exponential
//  z = log10(z1);               complex common log (base 10)
//  z = pow(z1,d);               raise complex to a power (double)
//  z = pow(z1,z2);               (or complex)
//  trig and hyperbolic functions:
//    cos(z), sin(z), tan(z), cosh(z), sinh(z), tanh(z)
//  inverses:
//    acos(z), asin(z), atan(z), acosh(z), asinh(z), atanh(z)
//
//  (note that C++ will cast a d or n to Complex if used as an argument
//   to functions which require type Complex, with imaginary part = 0.0)
//
// ********************************************************************
//
//            Complex number input/output and formatting
//
//  Stream operators:
//
//  <ostream> << z;     writes z, using a format etablished by functions
//                      described below
//
//  <istream> >> z;     reads z, interpreting the input as described below
//
//
//  I/O modes:
// 
//  There are several i/o modes, declared in the enumerated type
//  Complex::io_mode.
//
//  cartesian:  the real part followed by the complex part (the default)
//  polar:      the magnitude followed by the phase in radians
//  degree:     the magnitude followed by the phase in degrees
//  db:         20*log10(magnitude) followed by phase in degrees
//
//  Setting the output mode:
//
//  Complex::out_cartesian();   set output mode to cartesian
//  Complex::out_polar();       set output mode to polar
//  Complex::out_degree();      set output mode to degree
//  Complex::out_db();          set output mode to db
//  Complex::out_mode(m);       set output mode m (type Complex::io_mode)
//  Complex::out_mode();        just report the current output mode
//
//  each of the above functions returns the previous output mode
//
//  Output prefix, separator, and suffix strings: 
//
//  The pair of numeric values to be output can be preceeded by an
//  optional character string prefix, whose default value is empty. The
//  pair may be followed by an optional suffix as well. The separator
//  string to be inserted between the pair of values may also be
//  specified; its default value is determined by the output mode and
//  is described below.
//
//  Setting the output strings:
//
//  Complex::out_prefix(s);     set the prefix to s
//  Complex::out_suffix(s);     set the suffix to s
//  Complex::out_separator(s);  set the separator to s
//
//  s may be a singe char, a C-style char string, or a C++ string type.
//  The default value for s is an empty string. Each of the above
//  functions returns a copy of the previous value for its string.
//  Each of the above functions with no argument simply returns the
//  current value for its string. Return type is a C++ string type.
//
//  The default value (empty string) for the output separator causes the
//  following output between the pair of numeric values in the Complex
//  number:
//
//  Output mode polar or degree:  a single space separates the pair.
//  Output mode cartesian: for z.imaginary >= 0, the separator is "+i";
//  for z.imaginary < 0, the separator is "-i", and the absolute value
//  of the imaginary part is output.
//
//  More ways to set the output prefix, suffix, and separator:
//
//  Complex::out_default();     set all strings to the default (empty)
//  Complex::out_space();       set the separator to " ", others empty
//  Complex::out_delimited();   set the strings to "(" ; "," ; ")"
//
//  these functions do not return anything.
//
//
//  Rounding the displayed output to pure real or pure imaginary:
//
//  The variable Complex::rel_tolerance == 0.0 (default value) is used
//  to determine whether a number which is very nearly pure real or pure
//  imaginary is output with its imaginary part or real part set to zero.
//  If abs(z.imaginary) < abs(Complex::rel_tolerance*z.real), the number
//  is output with the displayed imaginary part set equal to zero, and
//  similarly if z.real is small compared to z.imaginary. This feature
//  is disabled by default; to enable it set Complex::rel_tolerance to a
//  nonzero value.
//
//  The variable Complex::abs_tolerance == 0.0 (default value) is used
//  to determine whether either or both the real and/or imaginary part of
//  a number is displayed as zero. If abs(z.real) < 
//  abs(Complex::abs_tolerance), then z is displayed with real part = 0;
//  similarly for z.imaginary. This feature is disabled by default; to
//  enable it set Complex::abs_tolerance to a nonzero value.
//
//
//  Setting the input mode:
//
//  Complex::in_cartesian();   set input mode to cartesian
//  Complex::in_polar();       set input mode to polar
//  Complex::in_degree();      set input mode to degree
//  Complex::in_db();          set input mode to db
//  Complex::in_mode(m);       set input mode m (type Complex::io_mode)
//  Complex::in_mode();        just report the current input mode
//
//  each of the above functions returns the previous input mode.
//
//  A function to convert two reals to a complex, using the current input
//  mode in order to interpret the arguments as though they had been input
//  in order from a stream using <<:
//
//  dtoz(double d1, double d2);    return complex formed from the two doubles
//
//  The input format options:
//
//  The default input format requires two whitespace-separated doubles
//  for each complex number input; the two doubles are converted into
//  a single complex z.
//
//  The "delimited" input format reads any of the three input forms:
//
//  x     (x)    (x,y)
//
//  The first two forms convert the single double x into complex z with
//  z.real = x, z.imaginary = 0; the third form converts the two doubles
//  x and y into a single complex z. Whitespace may optionally separate
//  the various parts of the delimited forms.
//
//  For both the default and delimited input forms, two doubles are
//  converted to a complex number as follows:
//
//  If the input mode is cartesian, two doubles are interpreted as the
//  real part of z followed by the imaginary part of z. Otherwise,
//  the first double is interpreted as the magnitude of z, the second
//  as the phase of z, in either radians
//  (polar) or degrees (degree or db).
//
//  Setting the input form:
//
//  Complex::in_form(f);   set input form to f
//                         (0: default; 1: delimited)
//                         returns the previous value for the input form
//
//  Complex::in_default();    set default input form
//  Complex::in_delimited();  set delimited input form
//
//  Complex::in_form();    read the current input form
//
//
// ********************************************************************

#ifndef SISCOMPLEX_H
#define SISCOMPLEX_H

#include <iosfwd>
#include <cmath>
#include <string>

//---------------------------------------------------------------------
// The Complex class:

class Complex {
public:

  //the data:

  double real, imaginary;


  //constructors:

  Complex(void) : real(0.0), imaginary(0.0) { }

  //(uses default copy constructor, Complex(Complex) )//

  Complex(double r, double i = 0.0) : real(r), imaginary(i) { }


  //assignment:

  inline Complex & operator =(const Complex & z)
  {
    real = z.real;
    imaginary = z.imaginary;
    return *this;
  }

  inline Complex & operator =(const double d)
  {
    real = d;
    imaginary = 0;
    return *this;
  }

  inline Complex & operator =(const int n)
  {
    real = n;
    imaginary = 0;
    return *this;
  }


  // i/o formatting:

  enum io_mode {cartesian = 0, polar = 1, degree = 2, db = 3};

  static io_mode out_mode();           // return the current output mode
  static io_mode out_mode(io_mode m);  // change to mode m; return previous
  static inline io_mode out_polar()     { return out_mode(polar); }
  static inline io_mode out_degree()    { return out_mode(degree); }
  static inline io_mode out_db()        { return out_mode(db); }
  static inline io_mode out_cartesian() { return out_mode(cartesian); }

  static std::string out_prefix();    // return the current output prefix
  static std::string out_prefix(const std::string &);
  static std::string out_prefix(const char * const);
  static std::string out_prefix(const char);

  static std::string out_separator(); // return the current output separator
  static std::string out_separator(const std::string &);
  static std::string out_separator(const char * const);
  static std::string out_separator(const char);

  static std::string out_suffix();    // return the current output suffix
  static std::string out_suffix(const std::string &);
  static std::string out_suffix(const char * const);
  static std::string out_suffix(const char);

  static void out_default();
  static void out_space();
  static void out_delimited();

  static io_mode in_mode();     // return the current input mode
  static io_mode in_mode(io_mode m);  // change to mode m; return previous
  static inline io_mode in_polar()     { return in_mode(polar); }
  static inline io_mode in_degree()    { return in_mode(degree); }
  static inline io_mode in_db()        { return in_mode(db); }
  static inline io_mode in_cartesian() { return in_mode(cartesian); }

  static int in_form();  // report current input format (0 == default)
  static int in_form(int f);  // set delimited input if f != 0, else default 
  static inline int in_default()   { return in_form(0); }
  static inline int in_delimited() { return in_form(1); }

  static double rel_tolerance;   // = 1.0e-14 by default
  static double abs_tolerance;   // = 0.0 by default


  //destructor:

  //(uses default destructor)//

};

//---------------------------------------------------------------------
//Access functions: real(), imag():

inline double real( const Complex & z ) { return z.real; }

inline double imag( const Complex & z ) { return z.imaginary; }


//---------------------------------------------------------------------
// Construct from polar form: polar():

inline Complex polar( const double rho , const double theta )
{
  return Complex (rho * std::cos(theta), rho * std::sin(theta));
}


//---------------------------------------------------------------------
// Conjugation: zconj(), conj():

inline Complex zconj( const Complex & z )
{
  return Complex (z.real, -z.imaginary);
}

inline Complex conj( const Complex & z ) { return zconj(z); }

//---------------------------------------------------------------------
// Basic Math Operator Overloading:

// addition:

inline Complex operator +(const Complex & z1, const Complex & z2)
{
  return Complex (z1.real+z2.real, z1.imaginary+z2.imaginary);
}

inline Complex operator +(const Complex & z, const double d)
{
  return Complex (z.real+d, z.imaginary);
}

inline Complex operator +(const double d, const Complex & z)
{
  return Complex (z.real+d, z.imaginary);
}

inline Complex operator +(const Complex & z, const int n)
{
  return Complex (z.real+n, z.imaginary);
}

inline Complex operator +(const int n, const Complex & z)
{
  return Complex (z.real+n, z.imaginary);
}


// subtraction:

inline Complex operator -(const Complex & z1, const Complex & z2)
{
  return Complex (z1.real-z2.real, z1.imaginary-z2.imaginary);
}

inline Complex operator -(const Complex & z, const double d)
{
  return Complex (z.real-d, z.imaginary);
}

inline Complex operator -(const double d, const Complex & z)
{
  return Complex (d-z.real, -z.imaginary);
}

inline Complex operator -(const Complex & z, const int n)
{
  return Complex (z.real-n, z.imaginary);
}

inline Complex operator -(const int n, const Complex & z)
{
  return Complex (n-z.real, -z.imaginary);
}


// unary minus and plus:

inline Complex operator -(const Complex & z)
{
  return Complex (-z.real, -z.imaginary);
}

inline Complex operator +(const Complex & z)
{
  return z;
}


// multiplication:

inline Complex operator *(const Complex & z1, const Complex & z2)
{
  return Complex (z1.real*z2.real - z1.imaginary*z2.imaginary,
		  z1.imaginary*z2.real + z1.real*z2.imaginary);
}

inline Complex operator *(const Complex & z, const double d)
{
  return Complex (z.real*d, z.imaginary*d);
}

inline Complex operator *(const double d, const Complex & z)
{
  return Complex (z.real*d, z.imaginary*d);
}

inline Complex operator *(const Complex & z, const int n)
{
  return Complex (z.real*n, z.imaginary*n);
}

inline Complex operator *(const int n, const Complex & z)
{
  return Complex (z.real*n, z.imaginary*n);
}


// division:

inline Complex operator /(const Complex & z, const double d)
{
  return Complex (z.real/d, z.imaginary/d);
}

inline Complex operator /(const Complex & z1, const Complex & z2)
{
  return (z1 * zconj(z2))/(z2.real*z2.real + z2.imaginary*z2.imaginary);
}

inline Complex operator /(const double d, const Complex & z)
{
  return (zconj(z) * (d/(z.real*z.real + z.imaginary*z.imaginary)));
}

inline Complex operator /(const Complex & z, const int n)
{
  return Complex (z.real/n, z.imaginary/n);
}

inline Complex operator /(const int n, const Complex & z)
{
  return (zconj(z) * (n/(z.real*z.real + z.imaginary*z.imaginary)));
}

//---------------------------------------------------------------------
// Relational Operator Overloading:

inline int operator ==(const Complex & z1, const Complex & z2)
{
  return (z1.real == z2.real) && (z1.imaginary == z2.imaginary);
}

inline int operator !=(const Complex & z1, const Complex & z2)
{
  return !(z1 == z2);
}

//---------------------------------------------------------------------
// Assignment Operator Overloading:

// +=

inline Complex & operator +=(Complex & z1, const Complex & z2)
{
  z1.real += z2.real;
  z1.imaginary += z2.imaginary;
  return z1;
}

inline Complex & operator +=(Complex & z, const double d)
{
  z.real += d;
  return z;
}

inline Complex & operator +=(Complex & z, const int n)
{
  z.real += n;
  return z;
}


// -=

inline Complex & operator -=(Complex & z1, const Complex & z2)
{
  z1.real -= z2.real;
  z1.imaginary -= z2.imaginary;
  return z1;
}

inline Complex & operator -=(Complex & z, const double d)
{
  z.real -= d;
  return z;
}

inline Complex & operator -=(Complex & z, const int n)
{
  z.real -= n;
  return z;
}


// *=

inline Complex & operator *=(Complex & z1, const Complex & z2)
{
  z1 = z1 * z2;
  return z1;
}

inline Complex & operator *=(Complex & z, const double d)
{
  z.real *= d; z.imaginary *= d;
  return z;
}

inline Complex & operator *=(Complex & z, const int n)
{
  z.real *= n; z.imaginary *= n;
  return z;
}


// /=

inline Complex & operator /=(Complex & z1, const Complex & z2)
{
  z1 = z1 / z2;
  return z1;
}

inline Complex & operator /=(Complex & z, const double d)
{
  z.real /= d; z.imaginary /= d;
  return z;
}

inline Complex & operator /=(Complex & z, const int n)
{
  z.real /= n; z.imaginary /= n;
  return z;
}


//---------------------------------------------------------------------
// i/o stream Operator Overloading, and dtoz():

std::ostream & operator <<(std::ostream & out_file, Complex z);

std::istream & operator >>(std::istream & in_file, Complex & z);

Complex dtoz(double d1, double d2);


//---------------------------------------------------------------------
// Other Useful functions:

// absolute value (magnitude):

double zabs( const Complex & z );

inline double abs( const Complex & z ) { return zabs(z); }


// argument (phase) in +/- Pi:

double zarg( const Complex & z );

inline double arg( const Complex & z ) { return zarg(z); }


// magnitude squared:

inline double zmagsq( const Complex & z )
{
  return ( z.real*z.real + z.imaginary*z.imaginary );
}

inline double norm( const Complex & z ) { return zmagsq(z); }


// logs and exponentials:

Complex log( const Complex & z );

Complex exp( const Complex & z );

Complex log10( const Complex & z );


// powers:

Complex pow( const Complex & z, const double d );

Complex pow( Complex z1, const Complex & z2 );

inline Complex sqrt( const Complex & z )
{
  return pow(z, 0.5);
}


// trig and hyperbolic functions and inverses:

Complex cos( const Complex & z );

Complex sin( const Complex & z );

Complex tan( Complex z );

Complex cosh( const Complex & z );

Complex sinh( const Complex & z );

Complex tanh( Complex z );

Complex acos( const Complex & z );

Complex asin( const Complex & z );

Complex atan( Complex z );

Complex acosh( const Complex & z );

Complex asinh( const Complex & z );

Complex atanh( Complex z );



//---------------------------------------------------------------------
// Alias for the complex type:
// Comment out the following line if you want to use the standard
// complex type as well:

typedef Complex complex;

//---------------------------------------------------------------------

#endif /* SISCOMPLEX_H */
