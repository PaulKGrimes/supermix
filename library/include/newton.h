// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ********************************************************************
// newton.h
//
// class newton: Newton-Raphson solver for a system of nonlinear
//               equations.
//
// This is an abstract class from which useful classes may be derived
// to solve any particular system of real equations  of a set of real
// variables. Given a nonlinear real, vector function F(X) of a real
// vector X, where F(X) and X have the same number of elements (ie, the
// number of equations == the number of unknowns), and the matrix
// function J(X) which returns the Jacobian matrix of F evaluated at X,
// class newton attempts to find a solution of F(X) == 0.
//
// Class newton defines several member veriables which provide the
// interface to the Newton-Raphson algorithm. These variables must be
// initialized and managed by the class objects derived from newton as
// follows:
//
// A derived class must:
//
//   (1) provide an implementation of the virtual function calc(), which
//       will read the X vector in "xlast" and then write F(X) into the
//       vector "fval" and the Jacobian matrix dF(X)/dX into the matrix
//       "Jacobian". This function will be called by solve() as a
//       solution is sought. The vectors and matrix must have the same
//       index range.
//
//   (2) provide an initial guess of a solution vector in the variable
//       "xlast", and set the control variable "maxstep" to some
//       appropriate value.
//
// The user of the derived class can then:
//
//   (3) adjust the convergence control variables "max_iter", "f_tol",
//       "F_tol", "dx_tol", and "rate_factor" to values other than
//       their default values, if desired.
//
//   (4) call the function solve() to generate a solution.
//
//   (5) examine the status of the solution and access the solution
//       vector using the functions:
//            no_solution()
//            get_x()
//            get_fval()
//            get_Jacobian()
//
// Note that constructing an object derived from class newton will have the
// side effect of seeding the random number generator srand48().
//
// 7/13/98 J.Z.
// ********************************************************************

#ifndef NEWTON_H
#define NEWTON_H

#include "global.h"
#include "matmath.h"

class newton
{
protected:

  // These variables store the results of the calculations
  // Declaring them protected allows derived classes to access these
  // (for instance, to store the calculated results), but otherwise
  // they are private.
  //
  // Note the index convention for the Jacobian:
  // Jacobian[i][j] = d fval[i] / d x[j]

  real_vector xlast;      // point X used for latest calculation
  real_vector fval ;      // the values of the functions at X
  real_matrix Jacobian ;  // Jacobian matrix at X 
  double maxstep ;        // maximum step size in X
  int    solution_flag ;  // = 1 if solution not found; = 0 if found

public:
  // these member variables control the root finder algorithm
  // These are initialized in the constructor to reasonable values.
  // They are declared public so the user can vary them if necessary. The
  // default values are shown in parentheses:

  int    max_iter ;    // Maximum number of iterations       (100)
  double f_tol ;       // convergence test: fval[i] < f_tol  (1.e-6)
  double F_tol ;       // test for a local min of norm(fval) (1.e-8)
  double dx_tol ;      // convergence test: delta in xlast   (1.e-7)
  double rate_factor ; // min rate of approach toward zero   (1.e-4)

  // The constructor just initializes values of the public member variables
  newton() ;

  // Recalculate function values and Jacobian at xlast. This is a virtual function
  // that the programmer has to implement for a particular system of equations:
  virtual void calc() = 0; 

  // This interface allows the user to calculate the functions and Jacobian
  // at a point x 
  void calc(const real_vector & x) {xlast = x; calc(); return;} 

  // solve() attempts to solve the nonlinear system using the Newton-Raphson
  // algorithm. The value of the vector X which solves the system can be obtained
  // using get_x(). The user should check no_solution() to ensure result is valid
  void solve() ;                       // uses existing xlast as initial guess
  void solve(const real_vector & x)    // uses argument x as initial guess
     {xlast = x; solve(); return;}

  // Routines to allow users to look at the results
  const real_vector & get_x() { return xlast ;}
  const real_vector & get_fval() { return fval ;}
  const real_table & get_Jacobian() { return Jacobian ;}

  // To check the status of the solution:
  // no_solution() == 1 if no solution found
  //               == 0 if solution was found
  int no_solution() {return solution_flag ;}

  // virtual functions demand a virtual destructor:
  virtual ~newton() { }
};

#endif /* NEWTON_H */


