// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ************************************************************************
// minimize1.h
//
// Functions for finding local minima of a real function of 1 real variable.
//
// Contains:
// minimize1(), bracket_minimum(), refine_minimum()
// ************************************************************************


#ifndef MINIMIZE1_H
#define MINIMIZE1_H

// ************************************************************************
// Here's the main minimizer routine:
// Given initial guesses x1 and x2 for the location of a minimum of f(x),
// use:
//      minimize(f, x1,x2, tol);  // tol is optional
// 
// to locate a local minimum to within a fractional tolerance of tol. The
// coordinate of the minimum is returned in x1; minimize() returns f(x)
// at that point. Not supplying the optional tol, or setting it < 1.0e-7
// will use 1.0e-7 as the tolerance. The optional n_max sets the maximum
// number of iterations to attempt; the default value is 1000.

template <typename F> inline
double minimize1( F f, // the function
		 double & x1, double x2, // x values
		 double tol, unsigned n_max);

template <typename F> inline
double minimize1( F f, // the function
		 double & x1, double x2, // x values
		 double tol)
{ return minimize1(f,x1,x2,tol,1000); }

template <typename F> inline
double minimize1( F f, // the function
		 double & x1, double x2  // x values
		)
{ return minimize1(f,x1,x2,1.0e-7,1000); }


// ************************************************************************
// A couple of more primitive routines used by minimize(), using algorithms
// adapted from Press, et.al., Numerical Recipes in C, 2nd ed.:


// bracket_minimum(f, a,b,c, fa,fb,fc):
//
// Given initial guesses a and b, find new a, b, and c such that
// f(b) < f(a) and f(b) < f(c), that is: a and c bracket a local minimum
// of f(x), and b is a point between a and c. Returns 0 if successful,
// 1 if failed (no other error messages are issued if search failed).
// The optional n_max sets the maximum number of iterations to attempt;
// the default value is 1000.

template <typename F> inline
int bracket_minimum( F f,  // the function
		     double &  a, double &  b, double &  c, // x values
		     double & fa, double & fb, double & fc, // f(x) values
		     unsigned n_max );

template <typename F> inline
int bracket_minimum( F f,  // the function
		     double &  a, double &  b, double &  c, // x values
		     double & fa, double & fb, double & fc  // f(x) values
		    )
{ return bracket_minimum(f,a,b,c,fa,fb,fc,1000); }


// refine_minimum(f, a,b,c);    // (tol defaults to 1.0e-7)
//
// Given a triplet of guesses a,b,c with b between a and c, and f(b) < f(a)
// and f(b) < f(c) (so a local minimum of f(x) must lie between a and c),
// adjusts the values of a,b,c until a <= b <= c and f(b) is the minimum of
// f(x), ie: b estimates the local minimum of f(x), within +/- (tol*b).
// Also, a and c will bracket the minimum, with c - a <= 2*(tol*b).
// Returns f(b). Uses Brent's method. Setting the optional tol < 1.0e-7
// will use 1.0e-7 as the tolerance. The optional n_max sets the maximum
// number of iterations to attempt; the default value is 1000.

template <typename F> inline
double refine_minimum( F f, // the function
		       double & a, double & b, double & c, // x values
		       double tol, unsigned n_max);

template <typename F> inline
double refine_minimum( F f, // the function
		       double & a, double & b, double & c, // x values
		       double tol)
{ return refine_minimum(f,a,b,c,tol,1000); }

template <typename F> inline
double refine_minimum( F f, // the function
		       double & a, double & b, double & c  // x values
		      )
{ return refine_minimum(f,a,b,c,1.0e-7,1000); }


// ************************************************************************
#include "numerical/num_minimize1.h"

#endif  /* MINIMIZE1_H */


