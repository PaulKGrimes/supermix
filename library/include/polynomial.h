// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ********************************************************************
// polynomial.h
//
// includes classes for generating and manipulating polynomials
//
// F. Rice
// 7/8/99
// ********************************************************************
#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <bits/stl_pair.h>
#include <vector>
#include "error.h"
#include <cmath>

// ********************************************************************
// class poly_fit
//
// 1-dimensional polynomial interpolation and extrapolation from a set of
// supplied data points. The x (independent variable) values must be doubles;
// the y (dependent variable) objects must be members of a "vector space" with
// the following operations defined on them (Y_type is the type of the objects):
//
// Y_type A;    // construct an instance of Y_type, no particular value.
// Y_type B(A); // construct an independent copy of Y_type A;
// A = B;       // make A an independent copy of B
// A *= x;      // modify A by scaling by the double x.
// A += B;      // modify A by adding B to it
// A -= B;      // modify A by subtracting B from it
//
// Y_type objects must be a vector space, in that (A *= 2.0) is equivalent
// to (A += A), and (A *= 0.0) is equivalent to (A -= A) and gives the
// additive identity of Y_type, etc. 
//
// The data point (x,y) pairs are passed to poly_fit using an STL vector
// object of STL pairs; typedefs are provided to streamline the declarations
// of these objects.
//
// The returned value is an STL pair of y values; the first element is the
// interpolated value, the second is an estimate of the error in the
// interpolation. For example:
//
// poly_fit<complex> Z;
//
// poly_fit<complex>::result answer = Z(x);  // do an interpolation
//
// complex   z     = answer.first; 
// complex   error = answer.second;
// 
//
// Algorithm based on routine polint(), from Numerical Recipes in C, 2nd ed.
//
// F. Rice
// 7/8/99
//
// ********************************************************************

template <class Y>
class poly_fit
{
public:

  // data types used by poly_fit:
  typedef std::pair<double,Y>          entry;      // hold an (x,Y) point 
  typedef std::vector<entry>           entries;    // vector of (x,Y) points
  typedef typename entries::const_iterator const_iterator;// iterator into supplied points
  typedef std::pair<Y,Y>               result;     // hold result and error estimate
  
  // fit a polynomial P to the points in STL vector v, and return P(x):
  result operator()(double x, const entries & v);

  // fit a polynomial to the n points of the STL vector starting with p, return P(x):
  result operator()(double x, const_iterator p, unsigned n);

  // this works with a C array of entry for p:
  result operator()(double x, const entry p[], unsigned n);

  // fit a polynomial to the most recently provided points (saved internally),
  // return P(x):
  result operator()(double x) const;
  
private:

  // hold the result returned by operator():
  mutable result save_result;

  // hold a vector of points that were last fit:
  entries data;
  unsigned N;    // one greater than order of fit

  // hold forward and backward differences:
  mutable std::vector<Y> fd, bd;

};  // class poly_fit

template <class Y>
inline typename poly_fit<Y>::result poly_fit<Y>::operator()(double x) const
{
  Y & y  = save_result.first;   // aliases for the result
  Y & dy = save_result.second;  // and the error estimate

  if(!N) {
    // size()==0, so no data to fit
    error::warning("No data points passed to poly_fit.");
    y = dy = Y();               // "empty" return value
    return save_result;
  }

  // first step: loop through data, find nearest neighbor of x while
  //initializing fd and bd.

  unsigned fm = fd.size();  // bd is the same size as fd

  double d = fabs(x - data[0].first);  // assume for now that x0 is nearest 
  unsigned n = 0;                      // n is a node index into difference tree

  for(unsigned i = 0; i < N; ++i) {

    const Y & yd = data[i].second;
    if (i < fm) 
      fd[i] = bd[i] = yd; // no constructor calls here
    else {
      fd.push_back(yd);
      bd.push_back(yd);
    }

    double xd = data[i].first;
    if(fabs(x - xd) < d) {
      d = fabs(x - xd);
      n = i;
    }
  }

  // 0th order approx is that nearest neighbor is the answer:
  y = data[n].second;

  // loop generates ever higher order approximations:
  for(unsigned order = 1; order < N; ++order) {
 
    // this loop generates new differences from the old:
    for(unsigned i = 0; i < N-order; ++i) {
      fd[i] = fd[i+1]; fd[i] -= bd[i];  // now fd[i] == fd[i+1]-bd[i]
      bd[i] = fd[i];
      double dx = data[i].first - data[i+order].first;  // ie: x[i]-x[i+order]
      if (dx == 0.0) {
	// two x values match; no polynomial fit is possible
	error::fatal("x values in data points passed to poly_fit must be unique.");
      }
      dx = 1.0/dx;    // so we really divide by dx below:
      fd[i] *= dx*(data[i].first - x);       // ie: (x[i]-x)/dx
      bd[i] *= dx*(data[i+order].first - x); // ie: (x[i+order]-x)/dx
    }

    // Now use the new differences to update solution,
    // taking a path which keeps node n near x:
    dy = ( 2*n < N-order ) ? fd[n] : bd[--n];
    y += dy;
  }

  return save_result;

}  // poly_fit::operator()


// the tricky part for the other operator()'s is to minimize calls to constructors
// and destructors for the elements:

template <class Y>
inline typename poly_fit<Y>::result poly_fit<Y>::operator()(double x, const entries & v)
{ 
  N = v.size();
  unsigned i;
  unsigned m = (N > data.size()) ? data.size() : N;

  for (i = 0; i < m; ++i) data[i] = v[i];       // no constructor calls here
  for (     ; i < N; ++i) data.push_back(v[i]); 
  return operator()(x);
}

template <class Y>
inline typename poly_fit<Y>::result poly_fit<Y>::operator()
     (double x, const_iterator p, unsigned n)
{ 
  N = n;
  unsigned i;
  unsigned m = (N > data.size()) ? data.size() : N;

  for (i = 0; i < m; ++i) data[i] = *(p++);       // no constructor calls here
  for (     ; i < N; ++i) data.push_back(*(p++)); 
  return operator()(x);
}

template <class Y>
inline typename poly_fit<Y>::result poly_fit<Y>::operator()
     (double x, const entry p[], unsigned n)
{ 
  N = n;
  unsigned i;
  unsigned m = (N > data.size()) ? data.size() : N;

  for (i = 0; i < m; ++i) data[i] = *(p++);       // no constructor calls here
  for (     ; i < N; ++i) data.push_back(*(p++)); 
  return operator()(x);
}

#endif /* POLYNOMIAL_H */
