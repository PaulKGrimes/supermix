// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ************************************************************************
// powell.h
//
// defines class powell: a multidimensional minimization routine which
// provides a concrete implementation of the abstract class "minimizer"
// found in optimizer.h. The algorithm used is a modified version of that
// of Powell, as implemented in Press, et. al., Numerical Recipes in C,
// 2nd ed.
//
// 7/27/98  J.Z.
// ************************************************************************

#ifndef POWELL_H
#define POWELL_H

// supermix's real_vector class used here
#include "vector.h"

// Defines general multivariate function class abstract_error_func
// as well as the generic optimizer interface class.
#include "optimizer.h"

class powell : public minimizer
{
public:
  // Constructor needs the error function to be minimized:
  powell(abstract_error_func & aef);

  // Call this function to do minimization.
  double minimize();

  // How many iterations it took
  unsigned num_iter() { return iter ;}

  // Some variables to control the minimizer:
  // when the value of the error function changes by a relative amount less
  // than FTOL, the algorithm assumes the minimum has been found.

  double   FTOL ;       // error function relative change target         (default 1.0e-4)
  unsigned ITMAX ;      // max allowable iterations                      (default 100)


  // These variables can have a significant impact on the speed of optimizations; the
  // default values should be ok for most situations.

  unsigned CLOSENESS;   // the larger this number, the more likely it is that powell
                        // will find a local minimum close to the initial value.
                        // Very large values will slow the optimization. (default 10)
  double   FOCUS;       // Set between 0 and 1; set to 1 if the topology of the
                        // error function surface is simple; 0 if it is very complex.
                        // (default 1.0)
private:

  // some internal routines:
  double f(double) const;  // convert error function to a function of a path variable
  double fmin(double);     // find the minimum of f(x)
  double lambda();         // find an appropriate scale for parameter variation

  // some internal variables:
  unsigned LN_ITMAX ;         // max iterations for line minimizations
  double   LN_TOL ;           // targer rel error tol for line mins
  unsigned iter ;             // iteration counter
  real_vector P, N ;          // keep track of where we are in parameter space
  real_vector Pmax, Pmin, D;  // hold the parameter space limits
  mutable real_vector xf;     // used by f();
  double lambda_ ;            // will save value returned by lambda()

} ;

#endif /* POWELL_H */

