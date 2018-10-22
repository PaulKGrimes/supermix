// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ************************************************************************
// montecarlo.h
// simple Monte Carlo optimizer class
//
// Defines class "montecarlo", which can attempt to find a global minimum
// using a random search in parameter space. The search scheme employs one
// or more local minimizer methods (powell is the default) at each random
// point chosen so that hopefully minima are found even in compilcated
// error function topologies. Here's what it does:
//
//  (1) Picks a point in parameter space using a uniformly-distributed
//      random distribution. 
//
//  (2) Calculates the error function value at the point. If the value
//      exceeds a threshold, the point is discarded and another point
//      is chosen - back to step (1).
//
//  (3) If the initial error function value is small enough in step (2),
//      it runs the local optimizer referenced by the member function
//      rough() on the point. rough() is an internally defined powell
//      minimizer.
//
//  (4) The minimum error function value found by rough() is compared to
//      a second threshold. If it is smaller than this threshold, a
//      second local optimizer is run on the point. This optimizer
//      defaults to the internally defined powell minimizer p(), but may
//      be set to a user-specified optimizer by the constructor.
//
//  (5) The final error function value found by either (2), (3) or (4) is
//      compared to the best value found previously. If it is an improvement
//      the value and the corresponding point in parameter space are saved.
//
// The first point picked by "montecarlo" is always the initial values of
// the parameters in the error function. The fine optimization is always
// run on this point. Only the second and subsequent points are chosen
// randomly.
//
// The output of montecarlo defaults to "display(false).verbose()", which
// outputs a report of the error function and parameter values each time
// an improved minimum is found. The effect of the display() member function
// is described in the comments near its declaration below. Calling
// very_verbose() additionally sets verbose() on the local optimizer called
// for every candidate point.
//
// When montecarlo::minimize() exits, it sets the parameter values to the
// best values found and calculates the error function one last time,
// returning the resulting error function value. It does this so that the
// error function's internal state will be consistent with the value
// returned. 
//
// Also included is class "do_nothing_optimizer", which can be given to
// montecarlo in order to just evaluate test points without running a
// local optimizer on them.
//
// 3/21/00 FRR and JSW
// ************************************************************************
 

#ifndef MONTECARLO_H
#define MONTECARLO_H

#include "powell.h"

class montecarlo : public minimizer
{
public:
  // NOTE: Constructors have the side effect of seeding the random number
  // generator used by drand48() (defined in <stdlib.h>).

  // Constructor needs the error function to be minimized:
  montecarlo(abstract_error_func & aef);

  // This contructor lets you specify a subordinate minimizer to call:
  montecarlo(abstract_error_func & aef, minimizer & method);

  // How many random starting points to try (default 100):
  montecarlo & npoints(unsigned);

  // The first error function threshold factor for performing a
  // rough optimization. If the error function value exceeds this
  // factor times the current best value, no further optimization
  // is performed to refine the point. Defaults to 1.0e4.
  montecarlo & rough_threshold(double v) { t1 = v; return *this; }

  // The second error function threshold factor for performing a
  // fine optimization. If the error function value is less than
  // this factor times the current best value, further optimization
  // is performed. Defaults to 100.
  montecarlo & fine_threshold(double v) { t2 = v; return *this; }

  // Type of verbose output on each iteration (default is false):
  //    set to true:  output each local minimum found
  //    set to false: output only if a better minimum found
  montecarlo & display(bool);

  // Type of verbose output on each iteration. Calling this function
  // causes verbose output to display those local minima whose value
  // is no greater than 1+r times the value of the best minimum seen
  // so far. Example: display(0.1) will cause all minima whose values
  // are no greater than 1.1 of the best seen so far to be displayed.
  // If the best minimum has a negative value, uses 1-r times the value
  // rather than 1+r times. If r <= 0.0, the function's effect is the
  // same as calling display(false). 
  montecarlo & display(double r);

  // The internal powell minimizer used for fine optimization if none
  // is specified by the constructor. You can access the powell
  // minimizer using this function so that you may modify its
  // conversion properties. See powell.h for details.
  powell & p() { return p_ ; }

  // The internal powell minimizer used for rough optimization.  You
  // can access the powell minimizer using this function so that you
  // may modify its conversion properties. See powell.h for details.
  // The default is to run only two iterations of this optimizer.
  powell & rough() { return q_ ; }

  // Performs the global minimization:
  double minimize();

  // A virtual destructor ensures proper subclass destruction.
  virtual ~montecarlo() { }

private:
  powell q_;            // local minimizer for rough()
  powell p_;            // local minimizer for fine optimization
  minimizer * m;        // user-specified minimizer for fine optimization
  unsigned n;           // number of random points
  bool display_f;       // display flag for the type of verbose output
  double display_r;     // display ratio for the type of verbose output
  double best_e;        // holds best error function result seen
  real_vector best_x;   // holds parameter values yielding best_e
  void randomize();     // randomize the error function parameters
  double t1, t2;        // rough and fine thresholds
};

inline montecarlo & montecarlo::npoints(unsigned np)
{ n = np; return *this; }

inline montecarlo & montecarlo::display(bool f)
{ display_f = f; display_r = 0.0; return *this; }

inline montecarlo & montecarlo::display(double r)
{ display_f = false; display_r = (r <= 0.0) ? 0.0 : r; return *this; }

// ************************************************************************

class do_nothing_optimizer : public minimizer
{
public:
  // Constructor needs the error function to be minimized:
  do_nothing_optimizer(abstract_error_func & aef) : minimizer(aef) { };

  // Don't do anything!
  double minimize() { return erf(); }
};

#endif /* MONTECARLO_H */
