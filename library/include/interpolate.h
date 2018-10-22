// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ********************************************************************
// interpolate.h
//
// defines generalized 1-parameter "interpolator" template class
// defines typedef "interpolation" as interpolator<double>
// 
// class interpolator<>:
//
// Template class for performing interpolation into a single-indexed
// set of numerical values. Both linear and cubic spline interpolations
// are provided. Extrapolations are performed as well; extrapolations 
// are always linear. The index must be a single double (real scalar).
// The indexed objects must be members of a "vector space" with the
// following operations defined on them (Y_type is the type of the objects,
// double is the type of the index):
//
// double x;
// Y_type A;    // construct an instance of Y_type, no particular value.
// Y_type B(A); // construct an independent copy of Y_type A;
// A = B;       // make A an independent copy of B
// A *= x;      // modify A by scaling by x.
// A += B;      // modify A by adding B to it
// A -= B;      // modify A by subtracting B from it
//
// Y_type objects must be a vector space, in that (A *= 2.0) is equivalent
// to (A += A), and (A *= 0.0) is equivalent to (A -= A) and gives the
// additive identity of Y_type, etc. 
//
// General scheme to use the interpolator:
//
// interpolator<Y_type> Y;         // an empty interpolator
// double x; Y_type y;             // independent and dependent variables
//
// (Also: interpolation Y;         // same as: interpolator<double> Y)
//
// provide tabulated data points:
//   Y.add(x1,y1).add(x2,y2)....add(xN,yN);
//   Y.add(xM,yM);         // etc.
// prepare for use (cubic spline in this case, which is the default):
//   Y.spline().build();   // or just: Y.build();
//
// y = Y(x);               // interpolate data to determine y(x)
// y = Y.prime(x);         // interpolate data to determine (d/dx)y(x)
//
// Also the following function is provided for certain special applications.
// Note that the return values are written into the variables provided. Function
// return value is a flag which tells if an extrapolation was performed.
//
// Y.val_prime(x, y1, y2); // write y(x) into y1, y'(x) into y2
// 
// Cubic spline interpolation is very loosely based on the algorithm in 
// Numerical Recipes in C, 2nd Edition (Cambridge).
//
// F. Rice 3/26/99
// ********************************************************************

#ifndef INTERPOLATE_H
#define INTERPOLATE_H

#include <list>
#include <vector>
#include <utility>

template < class Y_type >  // Y_type is the type of object we're interpolating
class interpolator
{
public:

  // ----------------------------------------------
  // Constructing or clearing the interpolator; operator =:

  interpolator();                      // an empty, default interpolator.
  interpolator(const interpolator<Y_type> &);  // copy constructor; may trigger build()
  virtual interpolator<Y_type> & clear();      // erase all data; return to default behavior.
  interpolator<Y_type> & operator=(const interpolator<Y_type> &);  // may trigger build

  // ----------------------------------------------
  // Supplying data and describing the desired interpolation:

  // Add data points. Data points may be added in any order, but the x values 
  // (of type double) must all be unique. Sorted points get added faster.
 
  virtual interpolator<Y_type> & add(const double, const Y_type &);  // a new data entry

  // Add all the data points of the supplied interpolator. All points previously
  // add()'ed to the supplied interpolator will be included. Any points of the
  // supplied interpolator with X values duplicating those already included will
  // be ignored.

  virtual interpolator<Y_type> & add(const interpolator<Y_type> &);

  // Type of interpolation (the default is cubic spline). Calling this function will
  // reset ready() until build() is called (see below). The enumeration gives mnemonics
  // for the interpolation types. Extrapolation is always linear. 

  interpolator<Y_type> & type(int);
  enum { LINEAR = 0, SPLINE = 1 };  // SPLINE is a cubic spline

  interpolator<Y_type> & linear() { return type(LINEAR); }
  interpolator<Y_type> & spline() { return type(SPLINE); }

  // Optionally specify slopes at the endpoints of the data set. These slopes are used for
  // any extrapolations, which are always linear. They also affect spline interpolation.
  // If either is called, then ready() is reset until build() is called (see below).
  // The default slope for an endpoint is calculated by the interpolator from the data
  // points using the  so-called "natural" boundary condition of vanishing curvature at
  // that endpoint.

  interpolator<Y_type> & left_slope(const Y_type &);  // set slope at left (smallest x) end
  interpolator<Y_type> & right_slope(const Y_type &); // set slope at right (largest x) end

  // ----------------------------------------------
  // Building internal data structures so that the interpolator may be used

  // Status. If this function returns 0, an attempt to use the interpolator will result
  // in a fatal error.

  virtual int ready() const {return ready_;}  // == 1 if ready to interpolate, == 0 otherwise.

  // Build tables and prepare for interpolation. If successful, sets ready().
  // Any points added following a call to build() will not be used by the
  // interpolator until build() is called again.
 
  interpolator<Y_type> & build();  // set up interpolator for points added so far

  // The following function returns 1 if there is more data available that is not being
  // used in the interpolation (that is, add() has been called since the latest call to
  // build()).

  int more_data() const {return (data.size() > table.size()); }

  // ----------------------------------------------
  // Performing an interpolation

  // Use: interpolator A; double x; Y_type y;  ... (build A) ...; y = A(x); 
  // This is the operator to perform an interpolation. It will generate a fatal error
  // if called while ready() == 0 (not set). 

  // return an interpolated/extrapolated Y_type estimate of Y(x):

  Y_type operator()(double x) const;

  // return an interpolated/extrapolated Y_type estimate of Y'(x):

  Y_type prime(double x) const;

  // Note that prime(x) will be continuous even for linear interpolations. Consequently,
  // integrating these estimated Y'(x) will not reproduce the estimates to Y(x). If
  // this property is required, use a spline interpolation.

  // Write operator()(x) into y, prime(x) into y_prime. Executes faster than the two
  // independent function calls. Returns: (-1) if extrapolating below minimum x; (0) if
  // interpolating; (1) if extrapolating above the maximum x in the internal table.

  int val_prime(double x, Y_type & y, Y_type & y_prime) const;

  // ----------------------------------------------
  // Other miscellaneous functions

  // The following functions turn on or off warnings regarding extrapolation. The default
  // behavior is to warn whenever an extrapolation is performed. Does not affect
  // ready() status.

  interpolator<Y_type> & verbose() { no_warn_ = false; return *this; }
  interpolator<Y_type> & quiet() { no_warn_ = true; return *this; }

  // the following function is retained for backward compatibilty.  An argument != 0
  // will turn off the extrapolation warning:

  interpolator<Y_type> & no_extrapolation_warning(int f)  // f == 1: no warnings
    { no_warn_ = (f != 0); return *this; }

  // The following functions provide direct read access to the sorted data used for
  // actual interpolations (they do not include data add()ed following build()):
  // x(0) returns the smallest x value; x(size()-1) returns the largest x value

  unsigned size() const                 // the number of points used in the interpolation
    { return unsigned(table.size()); }

  double x(unsigned i) const                          // returns x[i] ( 0 <= i < size() )
    { return table[i].first; }                        // NO BOUNDS CHECKING ON i

  const Y_type & operator[](unsigned i) const         // returns y[i] ( 0 <= i < size() )
    { return table[i].second.first->second; }         // NO BOUNDS CHECKING ON i


  // virtual destructor for proper subclass destruction
  virtual ~interpolator() { }

  // debugging functions; may be removed in a later release
  void list_data_list() const;   // send entries to cout
  void list_lists() const;       // send entries to cout, use table as reference


protected:

  typedef std::pair< double, Y_type >                 data_type;
  typedef std::list< data_type >                      data_list;
  typedef typename data_list::iterator                data_iter;
  typedef typename data_list::const_iterator          const_data_iter;
  typedef std::pair< double, std::pair< data_iter, data_iter > >  entry;
  typedef std::vector<entry>                          interp_table;
  typedef typename interp_table::iterator             table_iter;
  typedef typename interp_table::const_iterator       const_table_iter;

  data_list       data, aux;     // hold sorted data and auxilliary info
  interp_table    table;         // provides random access to data and aux
  mutable Y_type  result;        // holds the interpolation result
  bool            ready_;        // internal ready flag
  bool            no_warn_;      // don't warn if extrapolating
  int type_;                     // type of interpolation (0 => linear)
  Y_type lslope, rslope;         // slopes for extrapolations and boundary conditions
  bool user_ls, user_rs;         // were endpoint slopes supplied by user?
  void build_linear();           // called by build for linear interpolation
  void build_spline();           // called by build for spline interpolation

  // the following binary search routine returns an index i such that
  // table[i-1].x < x <= table[i].x
  unsigned long bsearch(double x) const;

  // the following functions write into the Y_type argument:
  void linear(unsigned long,double,Y_type &) const;       // linear interpolator
  void spline(unsigned long,double,Y_type &) const;       // spline interpolator
  void lextrapolate(double,Y_type &) const;       // linear extrapolator to left
  void rextrapolate(double,Y_type &) const;       // linear extrapolator to right
  void prime_linear(unsigned long,double,Y_type &) const; // dy/dx using linear interpolator
  void prime_spline(unsigned long,double,Y_type &) const; // dy/dx using spline interpolator

  // the following functions write into result:
  void linear(unsigned long i,double x) const
    { linear(i,x,result); }
  void spline(unsigned long i,double x) const
    { spline(i,x,result); }
  void lextrapolate(double x) const
    { lextrapolate(x,result); }
  void rextrapolate(double x) const
    { rextrapolate(x,result); }
  void prime_linear(unsigned long i,double x) const
    { prime_linear(i,x,result); }
  void prime_spline(unsigned long i,double x) const
    { prime_spline(i,x,result); }

};

typedef interpolator<double> interpolation;

#include "numerical/num_interpolate.h"

#endif /* INTERPOLATE_H */
