// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// powell.cc

#include "powell.h"
#include "matmath.h"
#include "error.h"
#include "minimize1.h"
#include "num_utility.h"
#include <iostream>
#include <cmath>

using namespace std;

powell::powell(abstract_error_func & aef) :
  minimizer(aef),
  FTOL(1.0e-4),
  ITMAX(100),
  CLOSENESS(10),
  FOCUS(1.0),
  iter(0)
{ }


double powell::minimize()
{
  // set up control limits:
  FTOL = fabs(FTOL);
  if (FTOL < 1.0e-12) FTOL = 1.0e-12;
  if (FTOL > 0.1) FTOL = 0.1;
  LN_ITMAX = (ITMAX < 100) ? 100 : ITMAX;
  LN_TOL   = sqrt(sqrt(FTOL));
  if (FOCUS < 0.0) FOCUS = 0.0;
  double focus = FOCUS;
  
  // get initial values of parameters
  P = erf.get_parms();
  Pmax = erf.get_max_parms();
  Pmin = erf.get_min_parms();
  D = Pmax - Pmin; D /= CLOSENESS+1;
  lambda_ = sqrt(norm(D)); if (lambda_ < 1.0) lambda_ = 1.0;

  real_vector Ps(P), Pext(P) ;   // Ps saves old P; Pext: extrapolation of P

  
  // Create a set of direction vectors -- initially this is diagonal
  N.resize(P) ;                     // N: a "delta P" direction vector chosen from Nset
  real_matrix Nset(N.size, N.mode); // Nset holds the set of directions
  Nset.diagonal(1.) ;

  erf.set_count(0);
  double f = erf() ;  // uses the initial parameter values here
  if(target_on && f < target) return f; // we're already below the target value

  // OK, perform the minimization:
  for(iter=1; 1 ; ++iter) {

    int imin = P.minindex(), imax = P.maxindex();
    double fP;
    double del;   // will be largest delta f()
    int ibig;     // will be direction with largest delta f()

    if(is_verbose) {  // spit stuff out every iteration if verbose
      error::stream() << "Iteration " << iter << endl ;
      error::stream() << "Parameters: " << endl << erf.get_parms_user() << endl; 
      error::stream() << "Function value: " << f << endl ;
      if(is_very_verbose) {
	error::stream() << "Calls to error function: " << erf.count() << endl;
      }
      error::stream() << endl ;
    }

    // minimize in turn along each direction in Nset; determine ibig and del
    fP = f;
    del = 0.0;
    ibig = imin;
    lambda_ *= 1;
    for (int i = imin; i <= imax; ++i) {
      for (int j = imin; j <= imax; ++j) N[j] = Nset[i][j]; // get direction
      double lastf = f ;
      f = fmin(lambda());   // perform the line minimization
      //      P = erf.get_parms();  // limit P to the valid parameter range of erf
      if ( fabs(lastf - f) > del) {
	// biggest decrease so far
	del = fabs(lastf - f);
	ibig = i;
      }
    }

    // check for termination:
    if(stop(iter)) return f;
    if(target_on && f < target) return f;
    if(2.0*fabs(fP-f) <= FTOL*(fabs(fP)+fabs(f))) return f;
    if (iter >= ITMAX) {
      if(is_verbose) error::warning("powell exceeding maximum iterations.");
      return f;
    }

    // test extrapolated P and update Nset if warranted
    N = P - Ps;
    Pext = P + N;
    double fPext = erf(Pext);    // one more call to erf
    if (fPext < fP) {

      // test if a new coordinate direction is warranted; note that focus must be >= 1.0:
      if ((2.0*(fP-2.0*f+fPext)*norm(fP-f-del)-del*norm(fP-fPext) < 0.0) && focus >= 1.0) {
	f = fmin(lambda());
	for (int j = imin; j <= imax; ++j) {
	  Nset[ibig][j] = Nset[imax][j];
	  Nset[imax][j] = N[j];
	}
      }

      // else, if fPext is better don't discarg it!
      else if (fPext < f) {
	P = Pext; f = fPext;
      }
    }

    // for the next iteration:	
    Ps = P;
    focus += FOCUS;

  } // for(iter)

} // powell::minimize()


// calculate error function values along a vector with direction N from point P:
double powell::f(double x) const
{
  xf = N; xf *= x; xf += P;
  return erf(xf);
}


// find the minimum of f(x) and update P to be the location of the minimum
double powell::fmin(double lambda)
{
  double x1, x2, fmin;
  x1 = lambda;
  x2 = 0.0;
  double a = x2, c, fa, fx1, fc;
  unsigned count = erf.count();
  if(is_very_verbose) {
    error::stream() << "Starting X values: " << a << " " << x1 << endl;
    error::stream() << "Starting f(X) val: " << f(a) << " " << f(x1) << endl;
  }
  erf.set_count(0);
  bracket_minimum(member_function(& powell::f, *this),a,x1,c,fa,fx1,fc,LN_ITMAX);
  if(is_very_verbose) {
    error::stream() << "Calls to erf by bracket_minimum(): " << erf.count() << endl;
    error::stream() << "X values: " << a << " " << x1 << " " << c << endl;
    error::stream() << "f(X) val: " << fa << " " << fx1 << " " << fc << endl;
  }
  count += erf.count(); erf.set_count(0);
  fmin = refine_minimum(member_function(& powell::f, *this),a,x1,c,LN_TOL, LN_ITMAX);
  if(is_very_verbose) {
    error::stream() << "Calls to erf by refine_minimum(): " << erf.count() << endl;
    error::stream() << "Minimum X value: " << x1 << " , fmin: " << fmin << endl;
  }
  count += erf.count(); erf.set_count(count);
  N *= x1 ;
  P += N ;
  return(fmin) ;
}


// find an appropriate initial increment multiplier for the line minimizer
// must initialize P, Pmax, Pmin, N before calling
double powell::lambda()
{
  int imin = P.minindex(), imax = P.maxindex();
  for (int i = imin; i <= imax; ++i) {
    if(N[i] == 0.0 || D[i] <= 0.0) continue;
    double L = D[i]/N[i];
    if (fabs(L) < fabs(lambda_)) lambda_ = L;
  }
  return lambda_ ;
}
