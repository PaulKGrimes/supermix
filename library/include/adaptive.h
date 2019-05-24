// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
/** ********************************************************************
* adaptive.h : contains class adaptive<> and function adaptive_fill()
* ********************************************************************
* adaptive_fill():
*
* Fills and builds an interpolator (using class adaptive<>) so that it
* approximates a given function.
*
* usage:
*
* Given a function f() returning a Y_type:
*        Y_type f(double x);
*
* And an interpolator of Y_types:
*        interpolator<Y_type> Y;
*
* calling:
*        adaptive_fill(Y, f, min_x, max_x);  // to use default tolerances
* (or)   adaptive_fill(Y, f, min_x, max_x, abs_error, rel_error);
*
* will build Y so that Y(x) approximates f(x) within the range
* [min_x .. max_x]. adaptive_fill() returns an int which equals 0 if all
* went well. The tolerances will be calculated using the default global
* norm() function for Y_type objects: double norm(Y_type);
*
* ********************************************************************
* Class adaptive<>:
*
* A templated class used to adaptively build an interpolator
* (cf interpolate.h) of an arbitrary function which meets specified
* accuracy targets. If the default norm() function is adequate, function
* adaptive_fill() is a simple alternative (see below).
*
* Given a function f(x) which calculates some Y_type object as a function
* of the double x, we wish to approximate this function using an
* interpolator Y(x). Class adaptive accomplishes the task of building
* Y(x).
*
* Here's how to use it:
*
* double x;
* Y_type f(double);       // The function to be interpolated. It could
*                         // return a const Y_type & instead.
*
*  ** Note that if f() is a MEMBER FUNCTION of a class object, it can't **
*  ** be used directly by class adaptive. It must be used as described  **
*  ** below!!!                                                          **
*
* interpolator<Y_type> Y;  // declare the interpolator object, then
* adaptive<Y_type> a(Y);   // construct an adaptive object from it.
*
* a.min_x = x1;
* a.max_x = x2;            // set the interpolation limits
*
* int bad = a(f);          // this call fills and builds Y to approximate f.
*     or                   // The optional second argument is a function for
* int bad = a(f,n);        // the "squared magnitude" or "norm" of a Y_type y.
*                          // If no second argument is provided, a will use
*                          // the global function norm(y), if it is defined.
*
* if(bad) ...              // bad == 1 if a problem occured with the build.
*
* If either the function to be interpolated or the norm to be used is
* defined as a member function, it must be converted for use with adaptive.
* If, for example, the function to be interpolated is a member function
* of the object Obj of class Cl: ie, f(x) is really Obj.f(x), then the
* function must be passed to a() as follows:
*
*   a(member_function(& Cl::f, Obj))
*
* member_function() returns a function object properly bound to Obj.f() for
* use by adaptive. This extra step is required because pointers to member
* functions (ie, the name of a member function) are not pointers to functions.
* The same conversion must be applied to a norm function as well, if it is
* really a member function of some class.
*
*
* Now we can use the interpolator to approximate f(x):
*
* Y_type approx_f = Y(x);
*
* If you wish to change the interpolation, use Y.clear() to throw away
* the old points, then a(f) to rebuild Y.
*
* There are member variables for adaptive which control the accuracy and
* other aspects of approximation; see the class declaration below for
* details.
*
* ********************************************************************
* Change history:
*
* 1/31/04:  Fixed for gcc 3.x
* 7/19/99:  Added adaptive_fill()
* 7/8/99:   Moved some definitions into num_utility.h
*
* ********************************************************************/

#ifndef ADAPTIVE_H
#define ADAPTIVE_H

#include "interpolate.h"
#include "matmath.h"
#include "error.h"
#include <set>
#include "num_utility.h"

template < class Y_type >  // Y_type is the type of object we're interpolating
class adaptive
{
public:

  // the constructor must be called with an interpolator<Y_type>, which it
  // will at a later time adaptively fill with data to meet the desired
  // interpolation accuracy.

  explicit adaptive( interpolator<Y_type> & Y_interp );


  // the following variables control or limit the adaptive sampling process
  // (default values in parentheses)

  unsigned min_points;      // (25)       the minimum number of points in the interpolator
  unsigned max_points;      // (1000)     the maximum number of points allowed
  unsigned recursion_limit; // (10)       the max allowable number of interval subdivisions

    // if either of the following accuracy limits is met, then accuracy is o.k:
  double abs_tolerance;  // (1.0e-100) the max absolute error target in the interpolation
  double rel_tolerance;  // (1.0e-6)   the max relative error target in the interpolation

  double min_x;          // (0.0)      the minimum x for the range being interpolated
  double max_x;          // (0.0)      the maximum x for the range being interpolated


  // The following function (operator ()) actually builds the interpolator, filling
  // it with appropriate data points in order to meet the accuracy targets. It returns
  // 0 if all went well, 1 if something went wrong (an appropriate error msg will be
  // generated in this case as well.) It must be called with a function object to be
  // interpolated and a function object returning a double "norm", or "magnitude
  // squared" of a Y_type object. The function object f must have an operator ()
  // defined which takes a single double argument and returns a Y_type or a const
  // reference to a Y_type, that is, it will be called like: "Y_type y = f(x)",
  // x a double. The function object n must have an operator () defined which takes a
  // single Y_type argument and returns a double which represents the squared
  // "magnitude" of the argument; it will be compared to rel_tolerance*rel_tolerance
  // and abs_tolerance*abs_tolerance. If no norm function is specified, a default of
  // the globally-defined funtion double norm(Y_type) is used, if it exists.

  template <class F, class N>
  int operator ()(F f, N n) { return build(f,n); }

  template <class F>
  inline int operator ()(F f) { return (*this)(f,norm); }


private:

  interpolator <Y_type> & Y;  // the interpolator to build
  std::set<double> chk_pts;   // temporarily holds x values during builds
  template <class F, class N> int build(F, N);
  template <class F>  double init(F);
  template <class F, class N> void check_and_add(F, N, double);

  // the following functional provides access to the overloaded global norm().
  default_norm<Y_type> norm ;

};  // end of adaptive class declaration


template < class Y_type >
inline adaptive<Y_type>::adaptive( interpolator<Y_type> & Y_interp )
  :
  min_points      ( 25       ),
  max_points      ( 1000     ),
  recursion_limit ( 10       ),
  abs_tolerance   ( 1.0e-100 ),
  rel_tolerance   ( 1.0e-6   ),
  min_x           ( 0.0      ),
  max_x           ( 0.0      ),
  Y               ( Y_interp )
{ }


template < class Y_type > template <class F, class N>
inline int adaptive<Y_type>::build(F f, N n)
{
  int r;                // counts down recursive interval subdivisions
  double dx = init(f);  // fill interpolator with initial points

  // the recursive build loop
  for (r = recursion_limit; r && chk_pts.size(); --r, dx /= 2.0) {
    check_and_add(f, n, dx);  // checks accuracy and picks new points to add
    Y.build();                // rebuild the interpolator with more points
    if(Y.size() >= max_points) break;
    // normal exit is for chk_pts to be empty
  }

  // check for an abnormal termination of the build loop
  if(chk_pts.size()) {
    if (r == 0)
      error::warning("Recursion limit reached in adaptive interpolator build.");
    else if (Y.size() >= max_points)
      error::warning("Table size limit reached in adaptive interpolator build.");
    return 1;
  }

  // everthing went as planned
  return 0;

}


template < class Y_type > template < class F >
inline double adaptive<Y_type>::init(F f)
{
  // initializes:
  //  (1) the interpolator with min_points/2 + 1 equally-spaced points
  //  (2) chk_pts with the midpoints of the intervals between points
  //  (3) dx (returned value) of 1/4 the interval between points
  // Since any points in chk_pts get added to the interpolator, we
  // anticipate this by not using min_points initially.

  double x1, x2, dx;
  unsigned i;

  dx = (max_x - min_x)/(min_points/2); // interval between points
  Y.add(min_x,f(min_x));
  for (i = 1, x1 = min_x+dx, x2 = min_x+dx/2.0;
       i <= min_points/2;
       ++i, x1 += dx, x2 += dx) {
    Y.add(x1,f(x1));     // adds points to the interpolator
    chk_pts.insert(x2);  // interval midpoints to be checked for accuracy
  }

  Y.build();      // initial build of the interpolator
  return dx/4.0;  // the interval at which new check points should be added to s
}


template < class Y_type > template <class F, class N>
inline void adaptive<Y_type>::check_and_add(F f, N n, double dx)
{
  // steps through the points in chk_pts (which are sorted and unique, since
  // chk_pts is of type set. The value of dx SHOULD BE 1/4 the minimum
  // separation between adjacent points, and determines the spacing to new
  // points which may be added. n() is the norm function for a Y_type.

  std::set<double>::iterator i;  // iterator into chk_pts
  double x;
  double abs_lim = abs_tolerance*abs_tolerance;  // since norm() = magnitude^2
  double rel_lim = rel_tolerance*rel_tolerance;

  // examine and delete each point in chk_pts; add others if required
  for (i = chk_pts.begin(); i != chk_pts.end(); /* no increment here */) {

    // destructively fetch the next x from s and increment iterator:
    x = *i;
    chk_pts.erase(i++);

    // get f(x) and Y(x) and compare them
    Y_type y = f(x);
    Y.add(x,y);           // add it to the interpolator, since we calculated it
    double normf = n(y);  // for relative error calculation
    y -= Y(x);            // the error
    double error = n(y);  // the norm of the error

    // examine norm of the error and add points to s if necessary
    if (error > abs_lim && (normf == 0.0 || error/normf > rel_lim)) {
      // accuracy insufficient; add additional points
      chk_pts.insert(i, x+dx);
      chk_pts.insert(i, x-dx);
    }
  }
}


// ********************************************************************
// fill an interpolator with values to interpolate the function f:

template < class Y_type, class F >
inline void adaptive_fill(
			  interpolator<Y_type> & fi,  // the interpolator to fill
			  F f,                        // the function to interpolate
			  double min, double max,     // interpolation x limits
			  double abs_error = 0.0,     // target absolute error
			  double rel_error = 0.0      // target relative error
			  )                           //   0 gives the default errors
{
  adaptive<Y_type> a(fi);
  a.min_x = min; a.max_x = max;
  if (abs_error != 0.0) a.abs_tolerance = abs_error;
  if (rel_error != 0.0) a.rel_tolerance = rel_error;
  a(f);
}


#endif /* ADAPTIVE_H */
