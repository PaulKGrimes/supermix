// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ********************************************************************
// Definitions of functions in minimize1.h
//
// F. Rice 4/9/99
// ********************************************************************

#include "error.h"
#include <cmath>

// some helper functions, enclosed in a class for global namespace conservation:
struct minimize1_util
{
  // golden ratio for searches
  static inline double gold() { return 1.61803399; }
  static inline double Cgold() { return 2 - gold(); }

  // and a couple of utilities:
  static inline void shift(double & a, double & b, double & c, const double & d)
    { a = b; b = c; c = d; }
  static inline double fmax(double a, double b)
    { return (a > b) ? a : b;}
};

// ********************************************************************

template <class F> inline
int bracket_minimum( F f,  // the function
		     double &  a, double &  b, double &  c, // x values
		     double & fa, double & fb, double & fc, // f(x) values
		     unsigned n_max )
{
  // must be given two different points a and b:
  if (a == b) {
    c = a;
    fc = fb = fa = f(a);
    return 1;
  }

  // rearrange a and b so that f(a) >= f(b):
  fa = f(a);
  fb = f(b);
  if (fb > fa) {
    double t;
    minimize1_util::shift(t,a,b,t);  // note how this call simply swaps a and b
    minimize1_util::shift(t,fa,fb,t);
  }

  // find a useable third point c, such that fc != fb:
  c = minimize1_util::gold()*(b-a) + b; // default is to expand outward by golden ratio
  fc = f(c);
  unsigned n;  // iteration counter
  for (n = 0; n < n_max && fc == fb; ++n) {
    c *= minimize1_util::gold(); c -= (minimize1_util::gold()-1)*b;  // continue expanding
    fc = f(c);
  }
  if (n >= n_max) return 1;  // couldn't find a suitable c

  if (fc > fb) {
    // we may be done...
    if (fa > fb) return 0;
    else {
      // must have had fa == fb; swap c and a
      double t;
      minimize1_util::shift(t,a,c,t);
      minimize1_util::shift(t,fa,fc,t);
    }
  }

  // here, we must have either: fa >= fb > fc ; or: fa > fb >= fc

  // now refine guesses:
  for ( ; n < n_max && fb >= fc; ++n) {
    double t1 = (b-a)*(fb-fc);
    double t2 = (b-c)*(fb-fa);

    if (t1 != t2) {
      // (a,fa),(b,fb),(c,fc) are not colinear, so 
      // we try a quadratic fit to a,b,c to find a possible minimum
      double t = b - 0.5*((b-a)*t1 - (b-c)*t2)/(t1-t2);
      double ft = f(t);

      if ((b-t)*(t-c) > 0.0) {
	// t is between b and c, so we must have had fa > fb
	if (fc > ft) {
	  // t is a minimum; fix a and b and return
	  a = b; fa = fb;
	  b = t; fb = ft;
	  return 0;
	}
	else if (ft > fb) {
	  // b is a minimum
	  c = t; fc = ft;
	  return 0;
	}
	// fb >= ft >= fc, so this t is of no help; use default expansion
      }

      else if ((b-c)*(c-t) > 0.0 && ft > fc) {
	// c is a minimum between b and t
	minimize1_util::shift(a,b,c,t);
	minimize1_util::shift(fa,fb,fc,ft);
	return 0;
      }

    } // if (t1 != t2)

    // eliminate "oldest" point, add an expansion by gold and loop
    minimize1_util::shift(a,b,c, minimize1_util::gold()*(c-b)+c);
    minimize1_util::shift(fa,fb,fc, f(c));

  }  // for

  // here either fb < fc or iteration limit was reached
  return (n >= n_max || fa == fb)? 1 : 0;

}  // bracket_minimum()

// ********************************************************************

template <class F> inline
double refine_minimum( F f, // the function
		       double & a, double & b, double & c, // x values
		       double tol, unsigned n_max)
{
  // order a and c:
  if (a > c) {
    double t;
    minimize1_util::shift(t,a,c,t);  // note how this call simply swaps a and c
  }

  // fix up a too small tol:
  tol = minimize1_util::fmax(tol, 1.0e-7);

  // temporary working variables:
  double x = b,  // location with the min value of f(x) seen so far
         w = b,  // location with the 2nd least f(x) seen
         v = b,  // mostly the previous value of w
         u,      // location where f(x) most recently evaluated
    dx = 0, ddx = 0,  // delta x moved on this step and mostly step before last
    fx = f(b), fw = fx, fv = fx, fu;  // function values

  for (unsigned n = 0; n < n_max; ++n) {
    double abs_tol = tol*(fabs(x) + .001); // .001 keeps abs tol reasonable for small x
    double mid = 0.5*(a+c);

    // check for convergence
    if(fabs(x - mid) <= 2*abs_tol - 0.5*(c-a)) {
      // half interval meets the tolerance requirement for convergence
      b = x; return fx;
    }

    // if ddx is big enough, try parabolic acceleration
    if (fabs(ddx) > abs_tol) {
      double old_ddx = ddx;  ddx = dx;
      
      double    t1 = (x-w)*(fx-fv);
      double    t2 = (x-v)*(fx-fw);
      double   num = (x-w)*t1 - (x-v)*t2;
      double denom = 2.0*(t2-t1);  // (num/denom) is the parabolic step
      if (denom < 0.0) { num *= -1; denom *= -1; } // make denom positive

      // check that parabolic step is small and keeps us within (a,c):
      if ( (fabs(num) < fabs(denom * 0.5 * old_ddx))  // step less than 0.5 ddx
	   && (num > denom*(a-x))                 // stay within (a,c) 
	   && (num < denom*(c-x)) ) {
	// take the parabolic step
	dx = num/denom;
	u = x + dx;       // use u as a temporary
	if (u - a < 2*abs_tol || c - u < 2*abs_tol)
	  // don't move too close to the endpoints
	  dx = (mid > x) ? abs_tol : -abs_tol;
      }
      else {  // parabolic step fails tests
	// take a golden section step instead
	ddx = (mid > x) ? c-x : a-x; 
	dx = minimize1_util::Cgold() * ddx;
      }
    }

    else {  // ddx too small
      // take a golden section step instead
      ddx = (mid > x) ? c-x : a-x; 
      dx = minimize1_util::Cgold() * ddx;
    }
    
    // make sure dx is at least abs_tol; compute f()
    if (fabs(dx) < abs_tol) dx = (dx < 0) ? -abs_tol : abs_tol;
    u = x + dx;
    fu = f(u);

    // now shrink the interval and decide what to do with u,x,v,w:
    if (fu <= fx) {
      // x becomes a new endpoint; u becomes x
      if (u < x) c = x; else a = x;
      minimize1_util::shift(v,w,x,u);
      minimize1_util::shift(fv,fw,fx,fu);
    }

    else {
      // u becomes a new endpoint; x still the min seen so far
      if (u < x) a = u; else c = u;
      if (fu <= fw || w == x) {
	// then u becomes w
	v = w; fv = fw; w = u; fw = fu;
      }
      else if (fu <= fv || v == x || v == w) {
	// we'll at least make some use of u and f(u)
	v = u; fv = fu;
      }
    }

  } // for

  // normal exit of loop means there were too many iterations:
  error::warning("Iteration count reached in min_1d::refine_minimum().");
  b = x; return fx;

}  // refine_minimum()
  
// ********************************************************************

template <class F> inline
double minimize1( F f, // the function
		 double & x1, double x2, // x values
		 double tol, unsigned n_max)
{
  double a = x2, c, fa, fx1, fc;
  int flag = bracket_minimum(f,a,x1,c,fa,fx1,fc,n_max);
  if (flag) {
    error::warning("Failed to find a minimum in minimize().");
    return fx1;
  }
  else {
    return refine_minimum(f,a,x1,c,tol,n_max);
  }
}
