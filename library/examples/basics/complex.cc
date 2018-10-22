// complex.cc

// Brief introduction to SuperMix's complex number class

// We access the capabilities of the library with the following
// #include statement:

#include "supermix.h"

// SuperMix header files are found in the SuperMix "include/"
// directory.

// The complex number class used by SuperMix is NOT the same as
// the class provided with the standard C++ class library
// (loaded with a statement like " #include <complex> " )
// DO NOT TRY TO USE BOTH THE SuperMix COMPLEX CLASS AND THE
// STANDARD LIBRARY COMPLEX CLASS IN THE SAME PROGRAM! We plan
// for a future supermix release to make the two types compatible.

// The SuperMix header file "SIScmplx.h" defines the SuperMix
// complex number class. It contains extensive comments describing
// the capabilities of the class; this program provides a simple
// introduction to some of these capabilities.

int main()
{
  // -----------------------------------------------------------------
  // BASIC CREATION AND MANIPULATION OF COMPLEX NUMBERS

  // Declaring a complex number is easy:

  Complex z;

  // The name of the type is "Complex". There is a typedef in SIScmplx.h
  // which allows you to use all lowercase, ie: "complex", should you
  // wish.

  // A Complex number is stored internally in Cartesian form. We can
  // access and manipulate the two components directly (each component
  // is of type "double"):

  z.real = 3;
  z.imaginary = z.real + 1;


  // Here we output the value of z. The default output format writes
  // out the Cartesian representation:

  cout << "z = " << z << endl;

  // Note that we didn't use "std::cout" to access the output stream.
  // The header file "supermix.h" includes the C++ standard i/o header
  // files, and ends with "using namespace std;".

  //When we declare a complex variable we can give it an initialization:

  Complex u(2,1);    // u = 2+i1
  Complex v(3);      // v = 3+i0
  Complex w = 5;     // w = 5+i0

  cout << "u = " << u << ", v = " << v << ", w = " << w << endl;


  // We can manipulate complex numbers using the same sorts of
  // formulas we'd use with doubles:

  z = u + 2*v;
  cout << "u = " << u << ", v = " << v << ", u + 2*v = " << z << endl;

  w += z/u;
  cout << "z = " << z << ", u = " << u << ", w += z/u = " << w << endl;


  // The predefined consant "I" (defined in "global.h") can be used in
  // formulas:

  w = 3.0 + I*4.0;   // w = 3+i4


  // A temporary, unnamed complex value can be created by using the
  // complex() constructor:

  u = Complex(5,6);  // u = 5+i6

  // This can also be used to cast a double value to type Complex,
  // when you need a complex argument for a function:

  z = sqrt(Complex(-4.0)); // z = 0+i2


  // To construct a complex number from a magnitude and phase (in radians),
  // use the function polar():

  z = polar(1,Pi/2);       // "Pi" is another predefined constant
  v = polar(2,45*Degree);  // the constant "Degree" == Pi/180

  cout << "w = " << w << ", u = " << u
       << ", z = " << z << ", v = " << v << endl;


  // -----------------------------------------------------------------
  // OUTPUT FORMATTING OF COMPLEX NUMBERS, I/O INFORMATION

  // We can output complex numbers in several different formats. The
  // format affects only the way the number is displayed, not the
  // internal representation.

  Complex::out_cartesian();   // this sets the default form used above
  cout << "Cartesian format: " << v << endl;

  Complex::out_polar();       // magnitude phase(radians) format
  cout << "polar format:     " << v << endl;

  Complex::out_degree();      // magnitude phase(degrees) format
  cout << "degree format:    " << v << endl;

  Complex::out_db();          // magnitude(dB) phase(degrees) format
  cout << "db format:        " << v << endl;

  // Note that the decibel conversion is appropriate for wave
  // amplitude, not power. The formula used is 20*log10(magnitude).

  // Changing the leading and trailing characters as well as the
  // separator character is possible; see the comments in the file
  // "SIScmplx.h".

  // For example, to get rid of the "+i" between the Cartesian
  // components in the output and replace it with a single space,
  // just use:

  Complex::out_space();      // separate output numbers with " ".
  Complex::out_cartesian();  // back to Cartesian output mode.

  cout << "Cartesian, with just a space as a separator: " << v << endl;

  Complex::out_default();    // back to using "+i" separator.

  // Similar conversions are available during complex number input.
  // See the "SIScmplx.h" header file for details.


  // -----------------------------------------------------------------
  // FUNCTIONS OF COMPLEX NUMBERS

  // Complex number versions of many of the standard C++ numerical
  // functions are implemented by SuperMix. Here are some examples;
  // a complete list is provided in "SIScmplx.h".

  z = I;
  cout << "z        = " << z        << endl
       << "sqrt(z)  = " << sqrt(z)  << endl
       << "log(z)   = " << log(z)   << endl     // natural logarithm
       << "log10(z) = " << log10(z) << endl     // common logarithm
       << "exp(z)   = " << exp(z)   << endl
       << "pow(z,I) = " << pow(z,I) << endl
       << "cos(z)   = " << cos(z)   << endl
       << "sinh(z)  = " << sinh(z)  << endl
       << "atanh(z) = " << atanh(z) << endl;

  // Other functions are especially for complex numbers:
  cout << "conj(z)  = " << conj(z)  << endl
       << "arg(z)   = " << arg(z)   << endl;    // in radians

  // The absolute value function returns the magnitude. abs(z)
  // is equivalent to the C++ standard abs(double).
  cout << "abs(z)   = " << abs(z)   << endl;     // magnitude  
    
}
