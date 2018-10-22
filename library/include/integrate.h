// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ********************************************************************
// integrate.h
//
// includes classes integrator<> and CauchyPV
//
// ********************************************************************
// class integrator<>:
//
// Performs numerical integrations of functions of one real variable (double).
// The return type of the function to integrated is determined by a template
// parameter in the declaration of an integrator object. The return type
// can be any sort of object which is a member of a "vector space" with the
// following operations defined on it (Y is the type name of the function
// return type, double is the type of the independent variable):
//
// double x;
// Y A;         // construct an object of type Y, no particular value.
// Y B(A);      // construct an independent copy of (value of type Y) A;
// A = B;       // make A an independent copy of B
// A *= x;      // modify A by scaling by x.
// A += B;      // modify A by adding B to it
// A -= B;      // modify A by subtracting B from it
//
// Y type objects must be a vector space, in that (A *= 2.0) is equivalent
// to (A += A), and (A *= 0.0) is equivalent to (A -= A) and gives the
// additive identity of Y, etc. 
//
// General scheme to use the integrator:
//
// Assume you have a complex-valued function of a real variable:
// complex f(double x), that you must numerically integrate. The code could be:
//
//   integrator<complex> F;   // an integrator appropriate for f().
//
//   double a,b;              // limits of integration
//   complex z = F(f,a,b);    // use the default tolerances and integrate f().
//
//   F.abs_tolerance = 1e-6; F.rel_tolerance = 1e-4;  // adjust tolerances
//   z = F(f,a,b);            // integrate f() again, using new tolerances.
//
// Now assume that you need to integrate a real function: double g(double x),
// which has an inverse square root singularity at the origin, and decreases
// approximately like exp(-a*x), a >= 1, for x > 10. You must integrate g(x)
// between -10 and +Infinity. The code would be:
//
//   typedef integrator<double> Ig;  // shorthand will be used repeatedly below.
//   Ig G;
//
//   double answer = 0.0;
//   anwser += G.sqrtupper()(g, -10, 0);
//   anwser += G.sqrtlower()(g,  0, 10);
//   anwser += G.exp()(g,10,100);       // upper limit is ignored here
//
// Note the concatenation of the method() and operator() calls; this works
// because method returns a reference to G, which is used by operator().
// The code could have used separate statements:
//
//   G.method(Ig::SQRTUPPER); answer += G(g, -10, 0); // etc.
//
//
// Algorithms based on the Romberg integration methods described in 
// Numerical Recipes in C, 2nd Edition (Cambridge).
//
// F. Rice
// 7/8/99
//
// ********************************************************************
// Cauchy_integrate(); class CauchyPV:
//
// Performs Cauchy principle value integrations of real functions of a
// real variable (ie, like: double f(double x)) using integrator<double>.
// The location of a nonintegrable, odd singularity (pole) is specified
// in addition to the limits of integration. The function will not be
// evaluated at the limits of integration, but it must remain finite
// at them, since the integration method used is integrator<double>::OPEN.
//
// To calculate a Cauchy principal value integral:
//
// (1) using Cauchy_integrate (limits and pole are doubles, func a function):
//     double answer = Cauchy_integrate(func, low_limit, pole, high_limit);
//
// (2) using class CauchyPV:
//     CauchyPV I;  // the integrator
//     double answer = I(f,a,s,b);
//
// Use method (2) if you want to adjust integrator controls to other than
// the default values, or if you need to calculate integrals using several
// different functions.
//
// F. Rice
// 7/13/99
// ********************************************************************

#ifndef INTEGRATE_H
#define INTEGRATE_H

#include "num_utility.h"
#include <cmath>
#include <utility>
#include <vector>
#include "polynomial.h"

template <class Y>
class integrator
{
public:

  // The following enumeration provides choices for the integration method to use
  // (default is CLOSED):

  typedef enum {

    CLOSED,    // Closed interval: function will be evaluated at the limits
    OPEN,      // Open interval: function will not be evaluated at the limits
    SQRTLOWER, // Open: lower limit has an inverse square root singularity
    SQRTUPPER, // Open: upper limit has an inverse square root singularity
    INFINITE,  // Open: one limit is very large; both limits must have the same sign
    EXP        // Function must be decreasing at +infinity AT LEAST AS FAST AS 
               //   exp(-x);integrates from lower limit to +infinity
               //   (upper limit is ignored; +infinity is assumed).

  } type;


  // --------------------------------------------------------------------
  // The constructor:
  explicit integrator(type t_ = CLOSED);


  // --------------------------------------------------------------------
  // The following variables control or limit the integration process
  // (default values in parentheses)

  unsigned order;           // (4)   the order of the Romberg extrapolation
  unsigned recursion_limit; // (20)  the max allowable number of interval subdivisions
  

  // If either of the following accuracy limits is met, then accuracy is o.k:
  double abs_tolerance;  // (1.0e-100) the max absolute error target in the integration
  double rel_tolerance;  // (1.0e-6)   the max relative error target in the integration

    
  // Select or change the integration method to use:
  integrator & closed()    { t = CLOSED; return *this; }
  integrator & open()      { t = OPEN; return *this; }
  integrator & sqrtlower() { t = SQRTLOWER; return *this; }
  integrator & sqrtupper() { t = SQRTUPPER; return *this; }
  integrator & infinite()  { t = INFINITE; return *this; }
  integrator & exp()       { t = EXP; return *this; }
  integrator & method(type t_) { t = t_; return *this; }


  // --------------------------------------------------------------------
  // Perform the integration: must pass a function and limits,
  // as well as a norm function for error determination. If no norm function
  // is specified, the default is to use a global double norm(Y), if defined.
  // Warning: do not call a given integrator object recursively; if you want
  // to do a multiple integral, create an independent integrator object for
  // each level of the integration.

  template <class F, class N> 
  Y operator()(
	       F f,        // the function to be integrated
	       double a,   // the lower integration limit
	       double b,   // the upper integration limit
	       N norm      // function returning the "magnitude squared" of
	                   //   an argument of type Y, for error estimates
	       );

  template <class F> 
  Y operator()(
	       F f,        // the function to be integrated
	       double a,   // the lower integration limit
	       double b    // the upper integration limit
	       )
  { return (*this)(f,a,b,norm); } // use the default norm
  

  // --------------------------------------------------------------------
private:

  type t;                    // the integration method
  std::pair<Y,Y> result;     // the final answer and error estimate
  typedef std::pair<double,Y> entry;
  std::vector<entry> s;      // vector of partial sums for extrapolation
  poly_fit<Y> p;             // the polynomial extrapolator

  // the following functional provides access to the overloaded global norm().
  default_norm<Y> norm ;

  // the following function is used to swap integration limits, to ensure
  // lower limit <= upper limit; operator() will handle the sign correctly.
  int qswap(double & a, double & b)
  { if (a > b) { double t = a; a = b; b = t; return 1; } else return 0; }

  // method implementations:
  template <class F, class N> 
  void romberg_closed(F f, double a, double b, N n,
		    void (integrator<Y>::*method)(F,double,double,int));
  template <class F, class N> 
  void romberg_open(F f, double a, double b, N n,
		    void (integrator<Y>::*method)(F,double,double,int));

  template <class F> void cl(F f, double a, double b, int step);
  template <class F> void op(F f, double a, double b, int step);
  template <class F> void sl(F f, double a, double b, int step);
  template <class F> void su(F f, double a, double b, int step);
  template <class F> void in(F f, double a, double b, int step);
  template <class F> void ex(F f, double a, double b, int step);

  // a class to perform change of variable needed by several of the methods:
  struct {
    Y r;

    // 2*x*f(a+x*x), used by sl:
    template <class F> Y sql(F f, double x, double a)
    { r = f(a+x*x); r *= 2.0*x; return r; }

    // 2*x*f(b-x*x), used by su:
    template <class F> Y squ(F f, double x, double b)
    {
      r = f(b-x*x);
      r *= (2.0*x);
      return r;
    }

    // (1/x^2)*f(1/x), used by in():
    template <class F> Y inv(F f, double x)
    { r = f(1.0/x); r *= 1.0/(x*x); return r; }

    // (1/x)*f(-ln(x)), used by ex():
    template <class F> Y exf(F f, double x)
    { r = f(-log(x)); r *= 1.0/x; return r; }
  } Func;

}; // class intergrator<>

// ********************************************************************
// A helper class for CauchyPV calculations

// a class to calculate the symmetrized function value for CauchPV
template <class F>
class CauchyPV_calculator { 
public:
  F op;
  double x;
  CauchyPV_calculator(F f, double s) : op(f), x(s) { }
  double operator()(double t) const { return op(x+t) + op(x-t); }
};


// ********************************************************************
class CauchyPV
{
  typedef integrator<double> I;

public:
  double & abs_tolerance;     // same functionality as for integrator<>
  double & rel_tolerance;
  unsigned  & order;
  unsigned  & recursion_limit;

  CauchyPV();                 // simple constructors
  CauchyPV(const CauchyPV &);


  // operator() performs the Cauchy Principal Value integration:

  template <class F> 
  double operator()(
		    F f,       // the function to integrate 
		    double a,  // lower integration limit
		    double s,  // the singularity location
		    double b   // upper integration limit
		    );

private:

  I integ;  // the integrator

}; // class CauchyPV

template <class F>
inline double Cauchy_integrate(F f, double a, double s, double b)
{ static CauchyPV I; return I(f,a,s,b); }

#include "numerical/num_integrate.h"

#endif /* INTEGRATE_H */
