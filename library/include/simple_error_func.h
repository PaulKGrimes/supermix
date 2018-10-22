// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ************************************************************************
// simple_error_func.h
//
// Contains the abstract class "error_func_parameters", which provides an
// implementation of the parameter manipution virtual functions of class
// abstract_error_func, by controlling the values of program variables
// of type parameter.
//
// 4/21/99 by John Ward, J.Z.
// ************************************************************************
// class error_func_parameters
//
// Class error_func_parameters implements the required parameter interface
// to an optimizer derived from class minimizer (see optimizer.h). It
// interfaces the elements of the real_vector parameter lists of the virtual
// member functions set_parms(), get_parms(), etc., of class
// abstract_error_func with individual program variables of class parameter.
//
// By using an error function derived from this class, an optimizer can
// indirectly control the values of any number of program variables
// contained in parameter class objects. Since most variables controlling
// the behavior of the various SuperMix circuit elements are parameter
// objects, the interface provided by error_func_parameters is powerful
// enough for most optimization needs.
// 
// Class error_func_parameters does not, however, provide code for actual
// calculation of an error function value (absract_error_func::func_value()).
// This function must be defined in a class derived from error_func_parameters.
// An error function which does this is class "error_func", defined in
// error_func.h. It provides a ready to use, powerful error function
// capability suitable for many optimization problems.
//
//
// USAGE:
//
// Use class err_func, derived from class error_func_parameters, or
// derive a new class of your own from error_func_parameters.
//
//
// CONTROLLING PROGRAM PARAMETER VARIABLES
//
// Assume you are using the error function class ef_class, which is
// derived from class error_func_parameters. Then you can give your
// optimizer control of various program parameters using the vary()
// member function as in the following example:
//
// Assume your program has to optimize a resistance R, a length L, and
// a coupling parameter K. In order to be manipulated by your error
// function, these variables must be held in objects of type "parameter";
// they may be global, local, or members of some class object or objects:
//
//   parameter R, L, K;  // these are the variables to optimize
//
// Assume that the appropriate units for R and L are ohms and centimeters,
// respectively; assume K is a pure number between 0 and 1. As you are
// familiar with the units definitions in the header file units.h, you
// give your error function control of R, L, and K as follows:
//
//   ef_class ef;       // declare your error function object
//
//   R = ef.vary(10, 25, 100, Ohm);  // Range: 10*Ohm <= R <= 100*Ohm
//                                   // Initial value: R = 25*Ohm
//
//   L = ef.vary(0, .1, 1.2, Centi*Meter);
//
//   K = ef.vary(0, .5, 1.0);  // no units needed for K
//
// The vary() member function returns a pointer to abstract_real_parameter;
// the assignment actually causes the parameter on the left hand side of
// the equal sign to "shadow" the abstract_real_parameter, so its value
// changes along with changes to the value of the shadowed variable. The
// pointer refers to an internal variable created within the error function
// object; each call to vary() creates another internal variable. Calls by
// the optimizer to ef.set_parms() change the values of the internal
// variables; the shadowing mechanism causes these changes to in turn
// change the values of the variables R, L and K.
//
//
// INTERFACE TO THE OPTIMIZER
//
// The member functions of class abstract_error_func described in
// optimizer.h provide the interface between an optimization routine and
// the error function's control of the program parameters. The following
// functions, declared virtual in abstract_error_func, are implemented by
// class error_func_parameters:
//    set_parms()          get_min_parms()       get_units()
//    get_parms()          get_max_parms()       size()
//    get_parms_user()     get_init_parms()
//
// The get_.. functions return real_vectors with index mode Index_1; the
// number of valid elements is equal to the number of calls to vary()
// executed since the error function was created, one element for each
// vary() call. The first valid element corresponds to the program
// variable associated with the first vary() call, the last valid element
// to the most recent vary() call. The same scheme applies, of course, to
// the real_vector argument of set_parms().
//
// Continuing with the previous example, where we have initialized R,L,K to
// 25*Ohm, 0.1*Centi*Meter, and 0.5, respectively, a call to ef.get_parms()
// would return a real_vector V with contents:
//
//   V = ef.get_parms();
//   V[1] == 25*Ohm;      V[2] == 0.1*Centi*Meter;  V[3] == 0.5;
//
// Calls to other get_.. functions would return:
//
//   V = ef.get_parms_user();
//   V[1] == 25.0;        V[2] == 0.1;              V[3] == 0.5;
//
//   V = ef.get_units();
//   V[1] == Ohm;         V[2] == Centi*Meter;      V[3] == 1.0;
//
//   V = ef.get_min_parms();
//   V[1] == 10*Ohm;      V[2] == 0.0*Centi*Meter;  V[3] == 0.0;
//
// etc.
//
//
// CONTROLLING A COMPLEX-VALUED VARIABLE
// 
// Often you will need an optimization of a complex variable. Since the
// optimizer classes are designed to manipulate real-valued variables,
// you need to map two real variables into your complex variable. To do
// this you simply use the class complex_parameter, defined in header file
// parameter/complex_parameter.h, which has the capability of shadowing two
// abstract_real_parameters.
//
// Example:
//   complex_parameter C;    // the variable we need to optimize
//
// First save the return values from two vary() calls:
//   abstract_real_parameter *p1, *p2;
//   p1 = ef.vary(...);
//   p2 = ef.vary(...);
//
// Here's where the optimizer gets control of the variable:
//   // either control the real and imaginary parts (Cartesian scheme):
//   C.shadow(*p1, *p2);
//   // or control the magnitude and phase(radians) (polar scheme):
//   C.shadow(*p1, *p2, complex_parameter::POLAR);
//
// The arguments you provide to the vary() calls will, of course, depend
// on how you plan to control the complex number, ie: Cartesian or polar
// scheme.
//
// ************************************************************************

#ifndef SIMPLE_ERR_FUNC_H
#define SIMPLE_ERR_FUNC_H

#include "optimizer.h"
#include "vector.h"
#include <list>

// Need to use parameters for the "vary" method.
#include "parameter/abstract_real_parameter.h"


class error_func_parameters : public abstract_error_func
{
						
public:

  // The vary() member function gives error_func_parameter objects the mechanism
  // to actually control parameter values external to it, thus giving an
  // optimizer routine control through the abstract_error_func interface.

  // Each call to the vary command creates a new internal abstract_real_parameter
  // object which can have its value controlled by an optimizer. The vary command
  // returns a pointer to the abstract_real_parameter it creates, so that external
  // parameter variables may shadow it (using assignment).

  // vary() allows the user to enter parameters in any convenient units, specifying
  // the scaling factor as the last parameter. Note: min > max and units <= 0.0
  // are fatal errors.

  abstract_real_parameter * vary(double min, double init, double max,
                                 double units = 1.0) ;  


  // What follows are the implementations of the abstract parameter manipulation
  // member functions required to be defined for a class derived from
  // abstract_error_func (see optimizer.h). The following functions return
  // Index_1 real vectors with parameter values. The sizes of the real_vectors
  // may exceed the number of valid elements, but maxindex() will be correctly
  // set to the maximum valid element index.

  // Return the current parameter values in machine units.
  real_vector get_parms() { return parms; }

  // Return the current parameter values in user-friendly units.
  real_vector get_parms_user();

  // Return the minimum allowed parameter values in machine units.
  real_vector get_min_parms() { return minimum; }

  // Return the minimum allowed parameter values in machine units.
  real_vector get_max_parms() { return maximum; }

  // Return the initial parameter values in machine units.
  real_vector get_initial_parms() { return initial; }

  // Return the scaling factors to use in get_parms_user().
  real_vector get_units() { return units; }

  // Return the number of parameters to be varied.
  int size() { return num_parms; }

  // Set parameter values, taking a vector with any type of indexing. Here is
  // how the mapping between a parameter and a real_vector index works: the lowest
  // valid index corresponds to the parameter associated with the first vary()
  // call; the last valid index corresponds to the parameter associated with the
  // most recent vary() call.
  void set_parms(const real_vector & pv) ;

  // constructor
  error_func_parameters(bool no_limits_flag = false) :
    abstract_error_func(no_limits_flag),
    minimum(0,Index_1),
    maximum(0,Index_1),
    initial(0,Index_1),
    parms(0,Index_1),
    units(0,Index_1),
    num_parms(0)
  { }


private:

  // This class is just a double wrapped in an abstract_real_parameter.
  // It is needed so that an external parameter that must be varied
  // during the course of an optimization can shadow it.
  class vary_parameter : public abstract_real_parameter
  {
  private:
    // The value of the parameter
    double value ;

  public:

    // The constructor needs an initial value.
    vary_parameter(double initial) : value(initial) { }

    // Return the parameter value.
    double get() const { return value; }

    // Set the parameter value.
    void set(double x) { value = x; }
  } ;

  // WARNING: current needs to be a list<>, since we take pointers to the
  // individual vary_parameter elements which must remain valid as the list
  // is expanded.

  // Current parameter values:
  std::list<vary_parameter> current;

  // Store information about the parameters in vectors.
  // Minimum and maximum parameter values.
  real_vector minimum;
  real_vector maximum;

  // Initial parameter values.
  real_vector initial;

  // Current parameter values.
  real_vector parms;

  // Parameter units, or scaling factor.
  real_vector units;

  // The number of parameters added to the parameter vectors.
  int num_parms;

  // Return a limited init so that min <= init <= max. Used by vary().
  double limit(double min, double init, double max)
    { return (init < min) ? min : ((init > max) ? max : init); }
  
} ;

typedef error_func_parameters simple_error_func;  // backward compatibility

#endif /* SIMPLE_ERR_FUNC_H */
