// parameters.cc
// Illustration of the parameter classes.

// A "parameter" is a special form of scalar object (real or complex)
// which acts just like any other scalar object in a C++ expression.
// It has a couple of added features over a simple variable of type
// double or complex, however, including:
//
//   (1) The ability to "shadow", or hold a pointer to, another
//       "parameter"-like variable, so that changes to that variable
//       are automatically transferred to the shadowing variable.
//
//   (2) The ability to hold range limits, so that its value will
//       remain within a valid range regardless of attempts to set it
//       to a value outside of that range.
//
// This program will illustrate some of the features of the parameter
// classes which are used extensively throughout the SuperMix library.

// The header files defining the parameter classes are found in the
// parameter/ subdirectory of the SuperMix include/ directory. The
// header files include:
//
//   abstract_real_parameter.h
//   abstract_complex_parameter.h
//      The interfaces which provide for read access to all parameter-
//      like classes.
//
//   real_parameter.h
//   complex_parameter.h
//      The definitions of the basic parameter classes, real_parameter
//      and complex_parameter.
//
//   function_real_parameter.h
//   scaled_real_parameter.h
//      Definitions of occasionally-useful classes derived from class
//      abstract_real_parameter, these objects may be shadowed by
//      parameter objects.
//
// The following header files are in the main SuperMix include/
// directory and provide additional abstract parameter types:
//
//   parameter.h
//      Since the real_parameter class is the most often used type, this
//      header file typedefs this class to the type "parameter".
//
//   real_interp.h
//   complex_interp.h
//      Provide abstract parameter types that determine their current
//      value by interpolating into a table of values using some variable
//      of type double as an interpolation index.
//
//   simple_error_func.h
//      Declares the class "error_func_parameters". Classes derived from
//      error_func_parameters have a member function called "vary()"
//      which returns a pointer to an abstract parameter that can control
//      the value of some other parameter through the parameter class
//      shadowing mechanism. This is the means by which the error
//      function classes controlled by an optimization routine actually
//      manipulate the behavior of a simulation. 
//      See the example lna/lna_opt.cc

#include "supermix.h"
  

int main()
{
  // *******************************************************************
  // Basic declarations and use in statements:

  parameter A;   // variable "A" behaves like a double in C++ statements

  // can assign double values to A, just like a double:
  A = 10.0;
  cout << "A = " << A << endl;    // output: "A = 10"

  // can use A in an expression, just like a double:
  double b = sqrt(A+6);
  cout << "b = " << b << endl;    // output: "b = 4"

  // however, A isn't really a double, so we must cast it if we use ?: :
  b = (b > 6) ? double(A) : 6;
  cout << "b = " << b << endl;    // output: "b = 6"

  // instead of a cast, we can call the member function get(), which
  // returns the value of the parameter:
  b = (A > 6) ? A.get() : 6;
  cout << "b = " << b << endl;    // output: "b = 10"

  // Here's a complex-number parameter:
  complex_parameter z = 3.0 + 4.0*I;
  cout << "z = " << z << ", |z| = " << abs(z) << endl;
                                  // output: "z = 3+i4, |z| = 5"

  // *******************************************************************
  // Using range limits on the real_parameter class:

  // let's assign a maximum limit to parameter A:
  A.set_max(6.0);
  cout << "A = " << A << endl;    // output: "A = 6"

  // we now can't set A to any value higher than the limit. No complaints
  // are issued, A just silently limits itself:
  A = 30;
  cout << "A = " << A << endl;    // output: "A = 6"

  // ditto for a lower limit:
  A.set_min(-1.5);
  A = -100;
  cout << "A = " << A << endl;    // output: "A = -1.5"

  // we can turn the limiting back off:
  A.no_min();
  A.no_max();
  A = 30;
  cout << "A = " << A << endl;    // output: "A = 30"
  A = -100;
  cout << "A = " << A << endl;    // output: "A = -100"

  // range limiting is not available on the complex_parameter class


  // *******************************************************************
  // Shadowing another parameter-like object

  // setting one parameter equal to another makes it a clone of the other
  parameter c = A;
  cout << "c = " << c << endl;    // output: "c = -100"
  // subsequently changing A won't affect the value of c
  A = 2;
  cout << "A = " << A << ", c = " << c << endl;
                                  // output: "A = 2, c = -100"
  
  // assigning the address of A to c makes c shadow A:
  c = & A;
  cout << "A = " << A << ", c = " << c << endl;
                                  // output: "A = 2, c = 2"
  // now changing A also changes the value returned by c
  A = -5;
  cout << "A = " << A << ", c = " << c << endl;
                                  // output: "A = -5, c = -5"

  // here's another parameter, which shadows c
  parameter d(&c);
  // now d also tracks A's value, which it gets from c
  A = 10;
  cout << "A = " << A << ", c = " << c << ", d = " << d << endl;
                                  // output: "A = 10, c = 10, d = 10"

  c = 3;
  // now c no longer shadows A, but d still shadows c
  cout << "A = " << A << ", c = " << c << ", d = " << d << endl;
                                  // output: "A = 10, c = 3, d = 3"

  // assigning A = d makes A a clone of d, so now A shadows c also:
  A = d;
  c = 20;
  cout << "c = " << c << ", d = " << d << ", A = " << A << endl;
                                  // output: "c = 20, d = 20, A = 20"

  // changing d now doesn't affect A, because A shadows c (since that's
  // what d was doing at the time of the "A = d" statement)
  d = 10;
  c = -5;
  cout << "c = " << c << ", d = " << d << ", A = " << A << endl;
                                  // output: "c = -5, d = 10, A = -5"

  // If you set limits on a parameter, it applies them to the shadowed
  // value. Here A and d both shadow c; d will have a limit:
  d.set_min(0);
  d = & c;
  c = 3;
  cout << "c = " << c << ", d = " << d << ", A = " << A << endl;
                                  // output: "c = 3, d = 3, A = 3"
  c = -2;
  cout << "c = " << c << ", d = " << d << ", A = " << A << endl;
                                  // output: "c = -2, d = 0, A = -2"
  c = 10;
  cout << "c = " << c << ", d = " << d << ", A = " << A << endl;
                                  // output: "c = 10, d = 10, A = 10"


  // *******************************************************************
  // Special features of complex_parameter objects

  // complex_parameters can shadow one another:
  complex_parameter u(&z);
  z = 2 + I*6;
  cout << "z = " << z << ", u = " << u << endl;
                                  // output: "z = 2+i6, u = 2+i6"

  // complex_parameters can also shadow real_parameters; here's a chain
  // of shadowing:
  z = & A;    // u -> z -> A -> c;
  c = 5;
  cout << "c = " << c << ", z = " << z << ", u = " << u << endl;
                                  // output: "c = 5, z = 5+i0, u = 5+i0"

  // a special constructor lets you have a complex_parameter shadow
  // TWO real parameters (note that we don't supply addresses, just the
  // names of the two real-valued parameter variables):
  u = complex_parameter(c,d);  // now u = c + I*d, as c and d vary
  c = 3; d = 4;
  cout << "c = " << c << ", d = " << d << ", u = " << u << endl;
                                  // output: "c = 3, d = 4, u = 3+i4"
  c = -5; d = 10;
  cout << "c = " << c << ", d = " << d << ", u = " << u << endl;
                                  // output: "c = -5, d = 10, u = -5+i10"

  // Now we can provide range limiting for u by limiting the ranges
  // on c and d:
  c.set_min(0).set_max(3);  // note that we can concatenate limit commands
  d.set_min(-1).set_max(1);
  cout << "c = " << c << ", d = " << d << ", u = " << u << endl;
                                  // output: "c = 0, d = 1, u = 0+i1"
  c = 2; d = 0.5;
  cout << "c = " << c << ", d = " << d << ", u = " << u << endl;
                                  // output: "c = 2, d = 0.5, u = 2+i0.5"

  // we can also construct the complex value using polar form rather
  // than cartesian; note the special 3rd argument in the constructor
  c.no_max();           // minimum for c is still 0.0
  d.no_min().no_max();
  u = complex_parameter(c,d, complex_parameter::POLAR);
  c = 2;
  d = 90*Degree;  // convert a number to radians by multiplying by Degree
  cout << "c = " << c << ", d = " << d << ", u = " << u << endl;
                        // output: "c = 2, d = 1.5708, u = 1.22465e-16+i2"
  // (note: the double precision calculation gives u a tiny real part)

  // let's display u in (magnitude,degree) form:
  Complex::out_degree();          // output format: magnitude degree
  Complex::out_delimited();       // use (,) as delimiters
  cout << "u = " << u << endl;    // output: "u = (2,90)"

}
