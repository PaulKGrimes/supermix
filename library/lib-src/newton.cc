// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// newton.cc
//
// Implementation of Newton-Raphson solver for class newton
// Based on Numerical Recipes in C, Section 9.7

#include <cmath>
#include "error.h"
#include "newton.h"

// the following includes are needed only for generating random numbers
#include <cstdlib>
#include <ctime>

using namespace std;


// helper routines in their own namespace

namespace newton_helper	 {
  // return the larger of two doubles
  inline double fmax(double a, double b)
  { return (a > b) ? a : b; }

  // return maximum absolute value found in a vector
  inline double vabs_max(const real_vector &v)
  { return sqrt(max_norm(v)); }
}

// ************************************************************************
// default constructor initializes the parameters which control
// the root finder and seeds drand48()

newton::newton() :
  max_iter(100),
  f_tol(1.e-6),  
  F_tol(1.e-8),
  dx_tol(1.e-7),
  rate_factor(1.e-4)
{ srand48(time(0)); }



// ************************************************************************
// solve(): the main solver routine

void newton::solve()
{
  solution_flag = 1 ;             // No solution found yet

  int ixmin = xlast.minindex() ;  // index limits on xlast; we'll use often
  int ixmax = xlast.maxindex() ;
  int ixnum = ixmax - ixmin +1 ;

  // check that xlast isn't empty
  if(ixmax < ixmin) {
    error::warning("Empty vector xlast in newton::solve()") ;
  }

  // calculate fval and Jacobian matrix at initial point, in xlast
  calc() ;

  // ---------------------------------------------------------------------
  // Check if fval and Jacobian have the right size and indexing

  int ifmin = fval.minindex() ;
  int ifmax = fval.maxindex() ;
  if(ifmax-ifmin != ixmax-ixmin) {
    error::warning("Number of equations and unknowns do not match in"
		   " newton::solve()") ;
    return ;
  }
  if(Jacobian.Rminindex() != ixmin || 
     Jacobian.Rmaxindex() != ixmax) {
    error::warning("Right index of Jacobian does not match xlast in"
		   " newton::solve()") ;
    return ;
  }
    if(Jacobian.Lminindex() != ifmin || 
     Jacobian.Lmaxindex() != ifmax) {
    error::warning("Left index of Jacobian does not match fval in"
		   " newton::solve()") ;
    return ;
  }

  // Check if by sheer luck we're already at the solution
  if(newton_helper::vabs_max(fval) < 0.01*f_tol) {
    solution_flag = 0 ;
    return ;
  }


  // ---------------------------------------------------------------------
  // now the fun begins:

  real_vector x(xlast); // will store solution here
  double fold ;         // result from previous iteration
  real_vector xold(x);  // result from previous iteration
  real_vector gradf(x); // will hold the gradient of f
  real_vector p(x);     // will hold the Newton-Raphson step

  // solve() tries to make sure the following value is always shrinking
  double f = 0.5*norm(fval) ; // norm of fval should be 0 at the solution

  // some variables we will need in the main iteration loop:
  double slope ;       // directional derivative of f along step direction
  double test, temp ;  // used to find a value from vector elements
  int i ;              // loop index over vector elements
  double lambda_min, lambda ;      // control size of step
  double rhs1, rhs2, a, b, disc ;  // used in polynomial calculations
  double f2 = 0, fold2 = 0, lambda_tmp = 0, lambda2 = 0 ; // misc temp's

  // ---------------------------------------------------------------------
  // Here's the main iteration loop

  for(int its = 1; its <= max_iter; ++its) {

    gradf = fval * Jacobian ;  // gradf = 1/2 gradient(fval*fval)
    xold = x ;
    fold = f ;

    // ---------------------------------------------------------------------
    // Calculate ordinary Newton-Raphson step (using matmath's solve() here)
    p = ::solve(Jacobian, -fval) ;

    // check if Jacobian was singular
    if(p.maxindex()-p.minindex() != ixnum-1) {
      // p is not the right size, so it was. Take a random search direction
      p.resize(xlast).maximize() ;
      for(i=p.minindex(); i<=p.maxindex(); i++)  p[i] = drand48()*maxstep ;
    }

    // Limit length of p to no more than maxstep
    test = sqrt(norm(p)) ;
    if(test > maxstep)  p *= maxstep/test ;


    // ---------------------------------------------------------------------
    // Do a search along the direction of p for a good value of lambda

    // Calculate rate of change of f along p
    slope = dot(gradf, p) ;
    if(slope > 0.) {    // should only happen for random search directions
      slope = -slope ;
      p = -p ;
    }
      
    // Compute lambda_min
    test = 0. ;
    for(i = ixmin; i <= ixmax; ++i) {
      temp  = fabs(p[i])*newton_helper::fmax(fabs(xold[i]),1.0) ;
      if(temp > test)  test = temp ;
    }
    lambda_min = dx_tol/test ;
   
    // Here's the search:
    bool check = false ;  // goes true if we might be at a local min
    bool done  = false ;  // goes true when we've found a lambda
    lambda = 1.0 ;        // the full Newton-Raphson step
    while(!done) {

      x = xold + lambda*p ;   // Try new position x
      calc(x) ;               // and calculate function & Jacobian
      f = 0.5*norm(fval) ;    // and a new f.

      if(lambda < lambda_min) {
	// The new x and old x are very close
	x = xold ;
	check = done = true ;
      }

      else if(f <= fold + rate_factor*lambda*slope) {
	// OK, function decreasing
	done = 1;
      }

      else {
	// Not OK, function not decreasing, we need to backtrack
	if(lambda == 1.0)    
	  // first time through while(), use quadratic approx
	  lambda_tmp = -slope/(2.0*(f-fold-slope)) ;
	else {
	  // not first time, so use cubic approx
	  rhs1 = f-fold-lambda*slope ;
	  rhs2 = f2-fold2-lambda2*slope ;
	  a = (rhs1/(lambda*lambda)-rhs2/(lambda2*lambda2))/(lambda-lambda2) ;
	  b = (-lambda2*rhs1/(lambda*lambda)+lambda*rhs2/(lambda2*lambda2))
	    /(lambda-lambda2) ;
	  if(a == 0.) 
	    lambda_tmp = -slope/(2.*b) ;
	  else {
	    disc = b*b-3.*a*slope ;
	    if(disc<0.0) {
	      error::warning("Roundoff problem in newton::solve()") ;
	      return ;
	    }
	    else
	      lambda_tmp = (-b+sqrt(disc))/(3.*a) ;
	  }
	  if(lambda_tmp > 0.5*lambda)
	    lambda_tmp = 0.5*lambda ;
	}

      } // else Not OK, ...

      lambda2 = lambda ;
      f2 = f ;
      fold2 = fold ;
      lambda = newton_helper::fmax(lambda_tmp, 0.1*lambda) ;

    } // while(!done)


    // ---------------------------------------------------------------------
    // Now we perform the convergence checks

    // Test for convergence of function values
    if(newton_helper::vabs_max(fval) < f_tol) {
      solution_flag = 0 ;
      return ;
    }
    
    // Test if the gradient of f is almost zero
    if(check) {
      test = 0. ;
      for(i = ixmin; i <= ixmax; i++) {
	temp = fabs(gradf[i])*newton_helper::fmax(fabs(x[i]),1.0) ;
	if (temp > test) test = temp ;
      }
      test /= newton_helper::fmax(ixnum*0.5, f);

      if(test < F_tol) {  // Too bad, didn't find a solution...
	error::warning("newton::solve() converged to a local minimum"
		       " instead of finding a root");
	return ;
      }
    }

    // Test convergence on x
    test = 0. ;
    for(i = ixmin; i <= ixmax; i++) {
      temp = fabs(x[i] - xold[i])/newton_helper::fmax(fabs(x[i]),1.0) ;
      if(temp > test)   test = temp ;
    }
    if(test < dx_tol) {  // We've converged
      solution_flag = 0 ;
      return ;
    }

  } // Main for(;;) loop

  // ---------------------------------------------------------------------
  // We've dropped out of the iteration loop without converging, so:
  error::warning("Maximum number of iterations exceeded in newton::solve()") ;
  return ;
  
}
