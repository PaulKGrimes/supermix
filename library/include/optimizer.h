// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ************************************************************************
// optimizer.h
//
// Contains abstract classes to provide an interface to multivariate error
// functions and minimize them with optimization algorithms.
//
// 7/27/98 by J.Z.
// ************************************************************************
// class minimizer:
//
// Class minimizer is an abstract class that defines an interface for
// multiparameter optimization routines. Predefined optimizers derived from
// minimizer include class "powell" in the file powell.h and class
// "montecarlo" in montecarlo.h.
//
//
// USAGE:
//
// Use one of the provided optimizers derived from class minimizer or
// derive a new class of your own.
//
// Assume you are using the optimization class optimizer_class, which
// is derived from class minimizer. Its constructor requires an error
// function. You have already declared an error function ("my_error_func")
// which is of a type derived from class abstract_error_func. Then:
//
//   optimizer_class my_optimizer(my_error_func);
//
// Creates an optimizer for your error function which is ready to use.
// You can then call any or all of the member functions of class minimizer:
//
//   my_optimizer.verbose();      // toggle to provide status output
//
//   my_optimizer.very_verbose(); // even more detailed status output
//
//   my_optimizer.quiet();        // minimal or no status output (default)
//
//   my_optimizer.set_target(10.0);  // optimizer should stop if error drops
//                                   // to or below 10.0
//
//   my_optimizer.no_target();    // use the optimizer's default method to
//                                // decide when to stop.
//
//   double e = my_optimizer.minimize();  // perform the minimization of the
//                                        // error function. Final error
//                                        // value is copied into e.
//
// After completion of the optimization, the error function object will hold
// the values of the optimization parameters which achieved the error
// function value returned by minimize(). See the description of class
// abstract_error_func for details.
//
// There is an additional member function: stop(). It is described below.
//
//
// WRITING YOUR OWN OPTIMIZER:
//
// If you write your own optimizer, then create a subclass of minimizer to
// implement your specific minimization algorithm. Fill in the code for the
// minimize() routine, which should minimize the error function. The error
// function should be derived from class abstract_error_function (see details
// below). The minimize() routine should interface with the error function
// using only the member routines declared in class abstract_error_function.
// 
// Class minimizer requires an abstract_error_function reference as an
// argument to its constructor.
//
// Here's how to derive an optimizer from class minimizer:
//
//    You must define a class (or use an already-defined class) derived from
//    class minimizer, with the following member definition details:
//
//    class my_minimizer_class : public minimizer {
//      public:
//
//      // Your constructor must provide an error function object (derived
//      // from class abstract_error_func) to minimizer, which will use it
//      // to initialize the reference variable minimizer::erf. Example:
//
//      my_minimizer_class(abstract_error_func & aef) : minimizer(aef)
//      { ... // your constructor code goes here, if needed }
//
//
//      // You provide the minimization routine by implementing virtual
//      // function minimize(). It should return the final error function
//      // value.
//
//      double minimize()
//      {
//        ... // Your code calls member functions of the abstract_error_func
//            // variable erf. When asking the error function for its
//
//            // A single-variable minimization routine is provided by the
//            // templated function minimize(), defined in minimize1.h
//            // You may want to call it as a part of your algorithm.
//
//            // Your algorithm should check variables is_verbose and
//            // is_very_verbose and output something useful if required.
//
//            // Your algorithm should use the target error function value
//            // if target_on is set.
//      }
//
//    };
//
//
// Now you can declare and use a new optimizer object:
//
//    my_minimizer_class my_minimizer(my_error_function);
//
//    double minimum = my_minimizer.minimize();
//
//
// Notes regarding minimize():
//
// Your minimizer should provide some useful status output as it executes if
// the boolean is_verbose is true. It should provide even more details if the
// boolean is_very_verbose is true. If target_on is true, then the minimizer
// should exit successfully if the error function value decreases below
// the value stored in member variable target. Otherwise it should use some
// other method to test for convergence on a minimum. Your minimize()
// routine should call the virtual function stop(int) periodically as it
// executes. The argument in the call is meant to be an iteration count
// and may be omitted (a default value of 0 is used in this case. stop()
// returns a boolean value. If stop() returns false, then minimize()
// should continue with its calculations; If stop() returns true, then
// minimize() should terminate gracefully as though it had converged on a
// minimum, using the best error function value and parameter set seen so
// far.
//
// Notes regarding stop():
//
// The virtual function stop(), as defined, simply ignores the calling
// argument and always returns false. It is provided as a means to add
// additional functionality to an implementation of minimize() in a
// derived class. By deriving and changing the definition of stop(), a
// programmer could:
//
//  (1) provide additional status information beyond that normally
//      output in the verbose and/or very_verbose modes.
//
//  (2) allow the minimization to be terminated early without aborting
//      the program run. 
//
// ************************************************************************
// class abstract_error_func:
//
// Class abstract_error_func defines the interface that all multiparameter
// error functions must implement in order to be used by classes derived
// from class minimizer.
//
// The error function can be a function of multiple real (double) variables.
// Therefore, we treat it as a function of a single real_vector object (see
// SuperMix's header file vector.h, NOT the C++ standard header <vector.h>).
// Consequently, communication between an optimization algorithm and the
// error function is accomplished using real_vector objects containing the
// relevant parameter values. In this way, the optimization algorithm is
// insulated from the details of how the parameter values are actually used
// to affect the program's behavior; these "messy" details are handled
// entirely by the error function.
//
// SuperMix provides an abstract class derived from abstract_error_func which
// handles all of the details of mapping the elements of these real_vectors
// of values and actual program variables of type "parameter" (actually a
// type_def for type "real_parameter" defined in parameter/real_parameter.h),
// since most variables controlling the behavior of circuit elements in
// SuperMix are of this type. This class is called "error_func_parameters"
// and is defined in simple_error_func.h
//
// SuperMix also provides a fully implemented, concrete error function class
// called "error_func", defined in error_func.h. It is derived from class
// error_func_parameters, so it provides for the interface and control of
// program parameter variables by the optimizer algorithm. It can calculate
// complicated error function values containing multiple weighted terms swept
// and summed over any number of ranges of multidimensional parameter space.
//
//
// INTERFACE WITH THE MINIMIZER: GETTING AND SETTING PARAMETER VALUES
//
// The minimizer algorithm may need to know not only the current parameter
// values, but also minimum, maximum, and initial values as well. Of course,
// it also needs to be able to set the values of the parameters. The
// following member functions of abstract_error_func provide for this
// interface with the minimizer. All of the get_... functions should return
// real_vectors with the same index mode and valid index range; their
// individual sizes MAY EXCEED THE VALID INDEX RANGE. The minimizer algorithm
// MUST ignore elements outside the valid index range. This means that if "V"
// is a real_vector object returned by an abstract_error_func, then the valid
// elements of V have index values from V.minindex() to V.maxindex(). If you
// have an object "erf" derived from class abstract_error_func, then:
//
//   erf.size();             // Return the number of parameters (an int
//                           // value). This gives the number of valid
//                           // elements in the real_vectors returned by
//                           // erf.get_...
//
//   erf.get_parms();        // Returns a real_vector containing the current
//                           // values of the parameters actually being used
//                           // in the program. The values will be limited by
//                           // each parameter's specified min and max value.
//
//   erf.get_min_parms();    // Returns a real_vector containing the minimum
//                           // allowable values of the parameters.
//
//   erf.get_max_parms();    // Returns a real_vector containing the maximum
//                           // allowable values of the parameters.
//
//   erf.get_initial_parms();// Returns a real_vector containing the values
//                           // of the parameters initially assigned by the
//                           // program. These values NEED NOT be limited by
//                           // each parameter's specified min and max value.
//
//   erf.set_parms(V);       // Sets the parameters using the elements of the
//                           // real_vector argument V. The first valid
//                           // element of V holds the value for the first
//                           // parameter, etc. V may attempt to set the
//                           // parameters to values outside of their
//                           // specified allowable ranges; the error
//                           // function must take care of limiting these
//                           // values appropriately.
//
// Note that none of these functions provides for setting the number of
// parameters or for setting their minimum, maximum, or initial values. These
// details must be taken care of by additional functions of classes derived
// from class abstract_error_func. This implies that a minimizer algorithm
// should not need to perform any of those functions.
//
//
// RETURNING THE ERROR FUNCTION VALUE
//
// The primary means of calculating the error function value is to use
// operator (). This is the method that should be used by a minimizer
// algorithm. Again assuming your error function is called "erf":
//
//   double e = erf();       // Using the current parameter values, calculate
//                           // and return the error function value. In this
//                           // case, copy the value into variable e.
//
//   double e = erf(V);      // Attempt to set the parameter values to those
//                           // in the real_vector argument V and return the
//                           // resulting error function value. The error
//                           // function will limit the values to their valid
//                           // ranges as in erf.set_parms(V). If V includes
//                           // a values outside the specified ranges for
//                           // the parameters, then the returned error
//                           // function value will rise exponentially from
//                           // its value at the nearest parameter space
//                           // boundary point to V. This behavior is the
//                           // default, but may be disabled by passing a
//                           // boolean = true argument to the
//                           // abstract_error_func constructor.
//
// Unlike the case of set_parms(V), the argument V in erf(V) MUST HAVE THE
// SAME INDEXING MODE AND VALID INDEX RANGE AS THE RESULT RETURNED BY
// get_parms().
//
// The calls above also increment an internal count register which can be
// examined to determine how many calls have been made to calculate the
// error function.
//
// Note that calling:
//   erf.set_parms(V);
//   double e = erf();
// is generally faster than calling:
//   double e = erf(V);
// since the error function value in the former case is always calculated at
// a point within the valid range of the parameters, so an exponential growth
// factor need not be calculated (a nontrivial calculation!). Convergence of
// an optimization algorithm may be slowed down substantially, however, if
// the algorithm continually tries to set V well outside the valid range of
// the parameters. For this reason, optimization algorithms should always
// use erf(V), unless the algorithm will not attempt to set V outside the
// error function parameter limits.
//
//
// OTHER MEMBER FUNCTIONS USEFUL FOR STATUS MONITORING
//
// Verbose output from the optimizer could include the current parameter
// values along with a count of the number of calls made to the error
// function. Here are functions which help with generating this output:
//
//   erf.set_count();        // Reset the internal count register to 0. At 
//                           // creation of the error function object, the
//                           // count register is already reset.
//
//   erf.set_count(n);       // Set the count register to (unsigned) n.
//
//   erf.count();            // Returns the (unsigned) value of the count
//                           // register. This count is incremented with
//                           // each call to operator(), ie: erf() or erf(V).
//
//   erf.get_parms_user();   // Return a real_vector of parameter values,
//                           // but first convert each parameter into a
//                           // number which has physical meaning to the
//                           // user. In other words, if a parameter
//                           // represents a physical resistance, then
//                           // get_parms_user() may return its value in
//                           // Ohms. Note that the error function erf
//                           // must determine how to convert the values.
//
//   erf.get_units();        // Returns a vector of the scaling factors
//                           // erf uses when converting parameter values
//                           // in get_parms_user(). These "units" work
//                           // as described in the header file units.h
//                           // for units conversion.
//
// Since get_parms() and set_parms() manipulate parameters in their internal
// SuperMix representation and get_parms_user() provides an external,
// physical value, we have the relation:
//   erf.get_parms_user()[i] * erf.get_units()[i] == erf.get_parms()[i]
// for each element [i] of the real_vectors. An optimizer's verbose output
// code may include a statement like:
//   cerr << erf.get_parms_user() << endl;
// To display the values of the parameters in easy-to-interpret form.
//
//
// WRITING YOUR OWN ERROR FUNCTION CLASS
//
// If you write your own error function class, you must derive it from
// class abstract_error_func and provide implementations of the following
// virtual functions:
//   virtual void         set_parms(const real_vector & pv);
//   virtual real_vector  get_parms();
//   virtual real_vector  get_min_parms();
//   virtual real_vector  get_max_parms();
//   virtual real_vector  get_initial_parms();
//   virtual real_vector  get_parms_user();
//   virtual real_vector  get_units();
//   virtual int          size();
//   virtual double       func_value();
//
// A very viable approach to the possibly daunting task of implementing all
// these functions is to derive your error function from class
// "error_func_parameters" described in simple_error_func.h; in this case
// you'll only have to implement func_value(). See simple_error_func.h for
// details.
//
// The function func_value() calculates the value of the error function
// at the point specified by the most recent call to set_parms(), or using
// the initial parameter values as if the call:
//   set_parms(get_initial_parms())
// had been made. In any case, get_parms() should ALWAYS return the point
// at which a call to func_value() would calculate the error function.
// It should never be the case that func_value() would attempt to calculate
// the error function for a point with a parameter value outside of its
// allowable range as defined by the return values of get_min_parms() and
// get_max_parms(), since set_parms() is required to limit the values
// stored to their valid ranges.
//
// It is probably appropriate (although not necessary) that the returned
// real_vector results of the get_... functions use indexing mode Index_1.
// In this case, the valid index range of the returned vectors would be
// from 1 to size(). Note that set_parms() must accept a real_vector using
// any indexing mode, not just the indexing mode used in the return values
// of the get_... functions.
//
// The relationship between the return values of get_parms(), get_units(),
// and get_parms_user() was described earlier. If this scaling scheme is
// not appropriate for the parameters being optimized, then get_units()
// should return a real_vector filled with 1's and get_parms_user()
// should just return the same result as get_parms().
//
// Refer to the comments in the declaration of abstract_error_func below
// for further details.
//
// ************************************************************************


#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "matmath.h"
#include <cmath>

// ************************************************************************
// abstract_error_func:

class abstract_error_func
{
public:

  // If the constructor is called with a true argument, parameter limits are
  // ignored in operator() calculations
  explicit abstract_error_func(bool no_limits_flag = false)
    : limit_flag(!no_limits_flag)  {} ;

  // All of the get_... functions should return real_vectors with the same
  // index mode and valid index range; their individual sizes may exceed the
  // valid index range. The minimizer algorithm should ignore elements
  // outside the valid index range.

  // Get and set the current parameter values. Note that calling
  // set_parms(get_parms()) should not change the parameter values used in
  // error function value calculations.
  virtual void set_parms(const real_vector & pv) = 0;
  virtual real_vector get_parms() = 0;

  // We sometimes want to get the parameters in units convenient for the
  // user instead of machine units.  This is usually for the purpose of
  // displaying them on screen.
  virtual real_vector get_parms_user() = 0;

  // Get the minimum, maximum, and initial parameter values in machine
  // units. Note that the methods for setting these values are not defined
  // by this interface.
  virtual real_vector get_min_parms() = 0;
  virtual real_vector get_max_parms() = 0;
  virtual real_vector get_initial_parms() = 0;

  // The parameters can be scaled by a scaling factor, usually corresponding
  // to some desired units (such as gigahertz.)  If this feature is not
  // needed, get_units() should return a vector of all 1's. 
  virtual real_vector get_units() = 0;

  // Return the number of parameters to optimize. How this number is
  // determined is up to the actual error function derived from this
  // abstract interface class.
  virtual int size() = 0;

  // primitive error function value calculation; called by operator().
  virtual double func_value() = 0 ;


  // Optionally set the parameters and calculate the function values. If the
  // vector pv determines a point outside of the valid parameter space as
  // defined by get_min_parms() and get_max_parms(), and parameter limits
  // are active (the default), then operator()(pv) will blow up the value
  // of the error function exponentially; this encourages a minimizer to
  // rapidly return the parameter values within the specified limits.
  // Note that for the limiting to work properly, pv must have the same size
  // and indexing mode as the real_vector returned by get_parms().

  double operator()(const real_vector & pv);  // use parameter values in pv.
  double operator()();                        // use the previously set
                                              // parameter values.


  // maintain a count of the number of times that operator() has been called
  unsigned count() { return count_; }
  void set_count(unsigned c = 0) { count_ = c; }

  // A virtual destructor ensures proper subclass destruction.
  virtual ~abstract_error_func() { }


private:
  unsigned count_;                // count calls to func_value();
  bool limit_flag;                // should we consider parameter limits?
  double calc_f();                // manages count_; calls func_value()
  double calc_f(const real_vector & P);  // consider limits with point P
  real_vector Ptemp;              // temporary used by calc_f(P)
} ;

inline double abstract_error_func::operator()() 
{ return calc_f(); }
inline double abstract_error_func::operator()(const real_vector & pv) 
{ return calc_f(pv); }

inline double abstract_error_func::calc_f()
{ ++count_ ; return func_value(); }

inline double abstract_error_func::calc_f(const real_vector & P)
{
  // When the parameters are set to P, the error function limits them
  // to within the parameter space boundaries:
  set_parms(P);
  double erf = calc_f();

  if (limit_flag) {
    Ptemp = get_parms();  // Get the parameters, limited by boundaries
    Ptemp -= P;  // we assume the sizes and indexing modes are compatible
    // Now if Ptemp != 0, P is outside the parameter space
    double d = norm(Ptemp);
    if (d != 0.0) {
      // We inflate the error function value exponentially:
      Ptemp = get_max_parms(); Ptemp -= get_min_parms();
      // Now Ptemp has the size of the parameter space
      double s = norm(Ptemp); // scale factor
      erf += (1+fabs(erf))*(exp(d/s)-1);  // exponentially increase erf
    }
  }
  return erf;
}

// ************************************************************************
// minimizer:

class minimizer
{

public:
  // The constructor needs to be passed a reference to an error function.
  minimizer(abstract_error_func & aef)
    : erf(aef), is_verbose(false), is_very_verbose(false),
      target_on(false), target(0.0)
    { }

  // A virtual destructor ensures proper subclass destruction.
  virtual ~minimizer() { }

  // Control the verbosity of the minimization routine.
  void verbose() { is_verbose = true ; return ;}
  void very_verbose() { is_verbose = is_very_verbose = true ; return ;}
  void quiet() { is_verbose = is_very_verbose = false ; return ;}

  // Set a target error function value.  Minimizer should stop when this
  // value is achieved.  If set_target is not called, the minimizer will use
  // some other method to test for convergence.
  void set_target(double t) { target = t; target_on = true;} ;
  void no_target() {target_on = false;}

  // minimize() is where the subclass implements the actual optimization
  // algorithm.  The user will call this to do a minimization.
  virtual double minimize() = 0 ;    // call this to do minimization

  // stop() is a function which should be called periodically by
  // minimize() as it executes (once per iteration, perhaps). It is called
  // with a count of the current iteration number, and returns a boolean.
  // If stop returns false, then minimize() should continue with its
  // calculation. If it returns true, then minimize() should exit
  // gracefully as though it had successfully converged, using the best
  // parameter set and error function value seen so far. The default
  // version provided here always returns false, but is virtual, so it
  // may be made more useful in derived minimizer classes.
  virtual bool stop(int = 0) { return false; }


protected:
  // All minimizers hold a reference to an error function.
  abstract_error_func & erf; 

  // Verbosity flags, give verbose output if is_verbose.
  bool is_verbose;
  bool is_very_verbose;  // additional debugging info is output if set

  // The minimizer should compare the error function value to target to test
  // for convergence if the following variable is true.
  bool target_on;

  // Target error function value.
  double target;
} ;

#endif /* OPTIMIZER_H */
