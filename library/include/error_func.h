// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ************************************************************************
// error_func.h
//
// Optimization error function classes for SuperMix. 
//
// Contains:
//   class error_func
//   class error_term
//   class error_term_mode
//   class scaled_match_error_term
//
// Class error_func is a fully-implemented error function class which can
// be used with optimizers derived from class minimizer (optimizer.h). It can
// be used to build complicated error functions involving multiple terms and
// sweeps over parameter ranges and provides for the optimization of any
// number of program variables.
//
// The error term classes are abstract classes which provide the interface
// between a single term in an error function calculation and the full
// calculation performed by class error_func. Concrete implementations of
// these classes for use with error_func can be found in the header file
// error_terms.h. It is also very simple to write your own custom error term
// class derived from one of these classes.
//
// 4/21/99 by John Ward
// Based on err_func.h written 7/22/98 J.Z.
//
// ************************************************************************
// Class error_func
// 
// The concrete class "error_func" is an error function class that should be
// suitable for most SuperMix simulations. Its error function value is built
// up by summing the results from any number of more primitive calculations
// (provided by objects derived from class "error_term"). Each of these
// primitive terms may be individually weighted.
//
// By using sweeper objects (see sweeper.h), error_func can include error
// terms whose values are calculated and summed over a swept range of parameter
// values (such as a sweep over a frequency range); error_func manages the
// sweepers and error terms so that the calculations are performed efficiently
// in the case that multiple terms must be swept using the same sweeper. This
// could be the case, for example, when summing a gain and a noise term over
// a design frequency band. 
//
// error_func is derived from error_func_parameters (see simple_error_func.h)
// which provides the interface between an optimizer and the program parameters
// to be optimized.
//
//
// USAGE:
//
// Construction of a complicated error function calculation for a circuit
// optimization problem using error_func can be divided into 7 steps:
//
//  1. Identify the circuit parameters which may need to be optimized and
//     define variables of type parameter to represent them.
//
//  2. Identify the other parameters which control the circuit operating
//     state (frequency, temperature, etc) and ensure that variables of
//     type parameter represent them as well.
//
//  3. Build the circuit model, using the parameters defined in steps
//     1 and 2 to control its behavior.
//
//  4. Identify the individual circuit characteristics (S11, noise, etc.)
//     that are critical to the performance of the circuit. Choose or
//     write error terms that measure the performance of each characteristic
//     and create the error term objects.
//
//  5. Define sweeper objects which cover the appropriate ranges of the
//     operating state control parameters (from step 2) for the circuit
//     performance goals. Use sweeper::sweep() for parameters to be swept
//     (eg, a frequency sweep) and sweeper::initialize() for parameters
//     to be held constant (eg, SIS bias voltage). Use of initialize() is
//     especially important if one sweeper may be adjusting a control
//     parameter which must then be reset to a specific value for another
//     sweeper.
//
//  6. Create an error_func object. Use its vary() member function to
//     define the allowable limits and initial values for the parameters
//     to be optimized (from step 1). Use its add_term() member function
//     to weight the individual error terms (step 4) and associate them
//     with the appropriate sweepers for the control parameters (step 5).
//
//  7. Pass the error_func object as a parameter to the constructor for
//     the optimizer. The optimizer will communicate with the error
//     function using the abstract_error_func interface defined in
//     optimizer.h
//
// Refer to the header files simple_error_func.h and sweeper.h for more
// information about controlling parameters (steps 1 and 2) using the
// vary() member function (step 6) and sweepers (step 5). Error terms
// (step 4) are described in more detail below; header file error_terms.h
// defines several ready-to-use error term classes suitable for circuit
// performance optimizations.
//
// Once the error function has been created and built, its value is
// calculated using abstract_error_function::operator(). Here's an
// example:
//
//   error_func ef;
//     ...                 // calls to ef.vary() and ef.add_term()
//
//   double error = ef();  // ef() calculates the error function and
//                         // returns its value.
//
// The member function get_func_breakdown() returns a real_vector object
// whose elements contain a term-by-term breakdown of the most recent
// error function calculation, allowing you to check the weighted 
// error contribution of each term.
//
//
// THE vary() MEMBER FUNCTION:
//
// Refer to the description of class error_func_parameters in header file
// simple_error_func.h for a detailed description of the vary() function.
// Here's its declaration and an example of its use, both from that file:
//
//   abstract_real_parameter * vary(double min, double init, double max,
//                                  double units = 1.0);
//
//   parameter R;          // a parameter we need the optimizer to control
//   error_func ef;        // our error function
//   R = ef.vary(10, 25, 100, Ohm);  // Range: 10*Ohm <= R <= 100*Ohm
//                                   // Initial value: R = 25*Ohm
//
//
// THE add_term() MEMBER FUNCTION:
//
// Class error_func builds up a full error function calculation by summing
// individually-weighted terms. These terms may be calculated and averaged
// over one or more sweeps of various operating state parameters. The
// scheme for identifying, weighting and sweeping error terms is described
// to an error_func object by calls to its add_term() member function.
//
// Individual error terms are created using classes derived from class
// error_term (also described in this header file). Operating state
// parameter sweeps are implemented using objects of class sweeper (see
// sweeper.h). add_term() allows error_func to take control of these
// objects.
//
// Here's how to use add_term():
// Assume you've declared 2 error terms (derived from error_term) and
// a sweeper, along with an error_func:
//
//   my_term_1   term1;
//   my_term_2   term2;
//   sweeper     sweep;   // will be used with term2
//   error_func  ef;
//
// Then:
//
//   ef.add_term(2.0, term1);         // a "sweeperless" term
//
// adds term1's error to the error function calculation with weight 2.
// Whenever ef.func_value() is called by the optimizer, term1 is reset and
// asked for its error value before any sweeps are started. Its error
// value is multiplied by its weight (2.0 in this case) and added into
// the error value to be returned by error_func ef.
//
//   ef.add_term(1.0, term2, sweep);  // a term with an associated sweeper
//
// adds term2 to the error function calculation with weight 1. Whenever
// ef.func_value() is called, First sweep and then term2 will be reset, then
// the sweeper will be swept, with term2 being calculated for each point of
// the sweep. The error values returned by term2 will be averaged over the
// total number of points in the sweep. This average value will be
// multiplied by the specified weight (1.0 in this case) and added into the
// error function value to be returned by error_func.
//
// More than one error term may be added with the same sweeper. When
// func_value() executes, it sweeps sweepers with more than one error term
// only once; for each point of the sweep the individual terms are calculated,
// weighted, and added into their individual running sums. The final results
// are divided by the number of points in the sweeper to get an average for
// each term. func_value() uses the state_tag argument to errror_term::get()
// to try to maximize the efficiency of the calculations (see the description
// of class error_term).
//
// The algorithm for error_func::func_value() is described in more detail
// in the declaration of class error_func.
//
//
// THE get_func_breakdown() MEMBER FUNCTION:
//
// Once the error function has been calculated, calling get_func_breakdown()
// returns a real_vector object (see SuperMix's vector.h) containing a
// term-by-term breakdown of the error function value. Each element of the
// real_vector contains the weighted error contribution of a single error
// term created by a call to add_term(). The real_vector uses the Index_1
// indexing mode (first valid element has index 1); the first element holds
// the weighted result of the term added by the first call to add_term().
// The breakdown can be displayed using "<<" for real_vectors, so:
//
//   cout << ef.get_func_breakdown() << endl;
//
// outputs the term-by-term breakdown on a single line, the values separated
// by " ". 
//
// ************************************************************************
// class error_term
//
// Class error_term is an abstract class that provides the interface between
// simple error function terms and class error_func. Actual, useful, concrete
// subclasses of error_term can be found in the file error_terms.h.
//
//
// USAGE: WRITING YOUR OWN ERROR TERMS USING ERROR_TERM
//
// Defining your own error terms is very simple: just derive a class from
// error_term in which you define the virtual function get(). Better yet,
// check out abstract class error_term_mode (described below). This latter
// class may actually be the class of choice for deriving your own error
// terms. If you don't need the added sophistication of error_term_mode,
// here's an example of an error term derived from class error_term:
//
// Suppose you have a function f(p1,p2) that you need to minimize. Then all
// you would need in order to define an error term for this function is:
//
//    // Define the new error term class:
//
//    class my_term_class : public error_term {
//    public:
//
//      // get() must return an error value given the current program
//      // state (eg, the current values of p1 and p2 for this example)
//
//      double get(state_tag) { return f(p1,p2); }
//
//    };   // don't forget the ";" after the "}"
//
//
//    // create an actual error term object (my_term) using the class:
//
//    my_term_class   my_term;  
//
// where f(), p1, and p2 are global identifiers declared somewhere previously
// in the source. Note that in this example we ignore the state_tag argument
// in the call to get(). The use of state_tags is described later.
//    
// IMPORTANT:  Try to avoid side effects in derived classes of error_term.
// An error term's get() function should attempt to leave the system in
// the same state as before get() was called to prevent one error_term from
// affecting another in a single error function calculation involving
// multiple error terms.
//
//
// USE OF THE error_term::reset() FUNCTION
//
// Some error term calculations involve saving results from previous get()
// calls. This is useful if the function is performing some running sum
// over a parameter sweep, for example, which it uses in its calculation.
// The virtual function reset() is used to tell the error term that it
// should clear any such previous results and prepare to start over. It is
// called by class error_func, for example, before it begins a parameter
// sweep. The default error_term::reset() does nothing, so if you don't
// need the reset capability for your error term, then ignore it.
//
//
// USE OF THE STATE_TAG ARGUMENT
//
// A state_tag is used to avoid repeating a lengthy function calculation
// when more than one error term relies on the results of that calculation
// for its error term value. Refer to the brief intro to state tags in the
// header file state_tag.h before proceeding.
//
// Let's consider a specific example involving a circuit calculation,
// although the state_tag mechanism could be useful in other contexts as
// well. Assume your error function calculation needs to know the gain and
// the noise of the circuit at a single operating condition (specific set
// of program variable values). The circuit returns both gain and noise
// when it calculates its response; this may be a complicated, lengthy
// calculation that you wouldn't want to have the program repeat
// unnecessarily. However, you use two error_term objects, one for gain
// and one for noise. By using state_tag arguments, you can have each
// term call the circuit's response function and still avoid the
// recalculation.
//
// The idea is this: you define the response function (which calculates
// both noise and gain, both returned in some data structure) so that it
// accepts a state_tag argument. When the function is called, it compares
// the state_tag argument to a saved copy of the argument provided on the
// previous function call; if the two state_tags match, then the function
// assumes that none of the parameters on which it depends has changed.
// In this case it can just return a copy of the previously returned
// output rather than perform a recalculation. This is the way that an
// nport object's get_data(state_tag) member function works (see nport.h).
//
// As for an error_term implementation, all you would do is pass on the
// state_tag argument in the error_term::get(state_tag) function to the
// response function you are calling (it, of course, has to be defined so
// that it accepts the state_tag argument as well). The error_term objects
// just assume that the caller of get() knows what it is doing if it
// sends along the same state_tag value to more than one get() call.
//
// ************************************************************************
// class error_term_mode
//
// In many cases the function to be optimized must not be simply minimized,
// but must be made to match, stay below, or exceed a given target value.
// This class provides the calculating muscle to perform these or similar
// optimizations using an error term.
//
// Class error_term_mode is a much-refined derivative of class error_term.
// Most of the concrete error terms in error_terms.h are derived from this
// class. It provides various "modes" for calculating an error term based
// on a comparison of a function value and a target. These modes are
// designed to be used in error function calculations using error_func with
// sweepers.
//
//
// CALCULATION OF THE ERROR VALUE:
//
// The basic error value (e) is given by the square of the difference between
// a function value (f) and a target value (t). How this value is used in the
// calculation of the returned error value is determined by the mode that has
// been set:
//
//   Mode                    Returned Error Value
//   ----                    --------------------
//   MATCH, M_MATCH, FLAT    return e
//   ABOVE, M_ABOVE          if (f < t) return e; else return 0
//   BELOW, M_BELOW          if (f > t) return e; else return 0
//
// The target value is set by the user before the error term's get() function
// is called, for all modes except FLAT. In the case of mode FLAT, the target
// value is calculated internally by the error term. The error calculation is
// performed by error_term_mode::checkval(double), which considers the mode,
// the target, and any internally saved results of previous error
// calculations. Its argument is the function value; it returns the
// calculated error:
//
//   checkval(double f);   // using the saved target value and other data
//                         // (depending on the error mode), return the
//                         // error value generated by function value f.
//
//
// DETAILED MODE DESCRIPTIONS:
//
//   MATCH, ABOVE, BELOW:
//    A fixed target value is specified by the user. Anytime checkval() is
//    called, the argument is compared with the target and an error value
//    is generated as described in the table above. These modes have no
//    "memory" of previous error values returned. When error_func sweeps
//    an error term using one of these modes, it calculates the average
//    value of the error over the points in the sweep.
//
//   M_MATCH, M_ABOVE, M_BELOW:
//    A fixed target value is specified by the user. These modes are
//    designed to be used in an error term with a sweeper. When error_func
//    sweeps an error term using one of these modes, then instead of
//    calculating the average value of the error over the sweep, it
//    calculates the MAXIMUM ERROR VALUE seen during the sweep. The code
//    in checkval() ensures that this result is returned by keeping a
//    running tally of the number of points seen so far in the sweep and
//    the maximum error value seen (both internal variables are cleared
//    by the reset() member function). When a new maximum error is
//    encountered, checkval() returns that value plus a correction which
//    compensates for the lower error values it had returned on previous
//    calls. Because of this correction, when error_func calculates the
//    mean error value upon sweep completion, the result is the maximum
//    error value encountered during the sweep. ( the M_ prefix is a
//    mnemonic for "maximum error").
//
//   FLAT:
//    Like the M_-prefixed modes, mode FLAT is designed to be used in an
//    error term with a sweeper. Unlike the other modes, no fixed target
//    value need be specified; FLAT uses the mean value of the function
//    as the target value, so this error term mode measures the "flatness"
//    of the function value over the range of a sweep. The code in
//    checkval() essentially keeps a running calculation of the function
//    mean value. When it returns an error value, it also includes a
//    correction for all previously returned values to compensate for
//    changes in the function's mean as the sweep proceeds. Consequently,
//    when error_func calculates the mean error value upon sweep completion,
//    the result is the mean squared deviation from the mean of the
//    function value. Calling reset() sets up the error term to start the
//    calculation over again from scratch.
//
// IMPORTANT:  One must be precise when defining "flat."  Returning a 
// simple value, such as the magnitude of S21, will minimize the absolute
// deviation from flatness. This may have the unwanted side effect of making
// S21 go to zero. To avoid this problem, either include another error term
// which gets large if S21 gets too small, or optimize the flatness of
// log(S21).  This way, the fractional deviation from flatness will be
// minimized instead of the absolute deviation.
//
//
// USAGE:
//
// Use one of the provided error terms derived from class error_term_mode
// (see error_terms.h) or derive a new class of your own. Set the error
// term mode and target value using error_term_mode member functions or
// whatever other methods are available for the specific error term you
// are using. Add the error term to an error_func object using
// error_func::add_term().
//
// Member Functions for Setting the Target and Mode:
//
//   target(t);       // set the target value to t (a double)
//
//   match()          // set the mode as indicated; target is unchanged
//   above()
//   below()
//   flat()
//   worst_match()    // set mode to M_MATCH
//   worst_above()    // M_ABOVE
//   worst_below()    // M_BELOW
//
//   match(t)         // set both the mode and target (t a double)
//   above(t)
//   below(t)
//   worst_match(t)
//   worst_above(t)
//   worst_below(t)
//
// Each of the above functions returns a reference to error_term_mode which
// refers to the error term object, so they can be concatenated using ".":
//   ef.match().target(10);  is the same as:  ef.match(10);
//
//
// WRITING YOUR OWN error_term_mode CLASS:
//
// Here's an example, similar to the one included for class error_term.
// Again assume you have some function f(p1,p2) to be optimized, only this
// time you want to compare its value to a target using the capabilities
// of class error_term_mode. You could write: 
//
//    class my_term_class : public error_term_mode {
//    public:
//
//      // get() uses error_term_mode::checkval() to do the comparison:
//      double get(state_tag) { return checkval(f(p1,p2)); }
//
//    } my_term;  // the actual error term object is "my_term"
//
// where f(), p1, and p2 are global identifiers declared somewhere previously
// in the source. Note that in this example we ignore the state_tag argument
// in the call to get(). Now we can use our object my_term with an error_func:
//
//    error_func ef;
//    sweeper sweep;
//    ef.add_term(1.0, my_term, sweep);
//    my_term.above(0.5);
//
// If you would like to be able to initialize the mode and target to some
// particular combination at construction, then you could include a constructor
// in your class definition:
//
//    my_term_class(error_term_mode::mode m, double t)
//      : error_term_mode(m,t) { }
//
// The type error_term_mode::mode is an enumeration of the available modes:
//
//    enum error_term_mode:mode
//         {MATCH, ABOVE, BELOW, FLAT, M_MATCH, M_ABOVE, M_BELOW};
//
// ************************************************************************
// class scaled_match_error_term
//
// Class scaled_match_error_term is an abstract class that provides an
// error function term to compare the shape of two curves which may not have
// the same scale. For example, it is used by class fts_match (error_terms.h)
// which compares a measured mixer FTS response in arbitrary units to that of
// a simulated mixer. It is meant to be used by error_func with a sweeper.
//
// USAGE:
//
// Derive a class from this class in which you implement functions get_a()
// and get_b(), where:
//
//   double get_a(state_tag); // returns values for the reference data
//   double get_b(state_tag); // returns values for the data to be scaled
//
// IMPORTANT:  Note that get_b() values will be scaled to match get_a()
// values. Beware that if the get_a() values are allowed to go to zero under
// control of the optimizer, then the calculation will find a scale factor
// for the get_b() values equal to zero as well, resulting in a very small
// error value. To avoid this limitation, the function get_a() should not be
// changeable by the optimizer, but should represent a fixed reference
// function. For example, get_a() may return data read from a file.
//
// The error calculation for the scaled match is:
//
//            Sum[(get_a() - K*get_b())*(get_a() - K*get_b())]
//   error = --------------------------------------------------
//                         Sum[get_a()*get_a()]
//
// Where "Sum[]" represents a sum over all terms, and we scale the return
// values from get_b() by the factor K to best match the return values from
// get_a(). The error = 0 if the get_a() all vanish, since K = 0 gives a
// perfect match regardless of the values of get_b(). Otherwise, the K which
// results in the best match is given by:
//
//   K = Sum[get_a()*get_b()]/Sum[get_b()*get_b()]
//
// Using this formula for the scale factor, we must have:
//
//   0.0 <= error <= 1.0
//
// If the get_b() all vanish, then K is undefined and error = 1.0. Since
// the error will be less than 1.0 even if the match is terrible, you will
// want to adjust the weight used with error_func::add_term() appropriately.
// A weight of ~100 might be a good starting value, depending on what other
// error terms you are also including in the error function. 
//
// The scale factor that minimizes the error can be accessed using the
// member function scale():
//
//   double scale() const;  // return the scale factor for get_b() data
//
// Since error_func will sum the error values returned by the error term
// during a sweep, scaled_match_error_term::get() only returns an error
// increment at each iteration. Since error_func divides by the number of
// points in a sweep to calculate an average, scaled_match_error_term
// inflates the error by a factor equal to the number of get() calls to
// compensate. The term's internal registers are reset by the member
// function reset(); if your derived class needs to overload reset() in
// order to perform its own internal housekeeping, then your reset() must
// remember to call the function scaled_match_error_term::zero() so that
// the scaled_match_error_term registers are properly reset. 
//
// ************************************************************************

#ifndef ERROR_FUNC_H
#define ERROR_FUNC_H

#include <vector>     // STL vector template class
#include <map>        // STL associative array template class

#include "simple_error_func.h"
#include "state_tag.h"
#include "sweeper.h"


// ************************************************************************
class error_term
{
public:

  // This is the interface. Calculate error function term.
  virtual double get(state_tag) = 0;

  // The method reset() is called before a sweep is started.  It is normally
  // not needed.  Some error terms, such as flatness_error_term, must keep
  // running sums during a sweep.  For them, reset clears these running
  // sums to 0.
  virtual void reset() { }

  // Virtual destructor is necessary to ensure proper subclass destruction.
  virtual ~error_term() { }
};


// ************************************************************************
class error_func : public error_func_parameters
{

public:

  // The add_term() member function, the major addition to the error function
  // interface provided by class error_func:

  // Add a simple, "sweeperless" error term.
  void add_term(double weight, error_term & et);

  // Add an error term whose values will be averaged over the points of the 
  // associated sweeper. The function allows the sweeper and error term
  // arguments to be listed in either order.
  void add_term(double weight, error_term & et, sweeper & swp);
  void add_term(double weight, sweeper & swp, error_term & et)
    { add_term(weight,et,swp); }

  // This function breaks down an error_func return value (calculated by
  // error_func::func_value()) into a real_vector of component terms, each
  // of which holds the contribution to the error function value from a
  // term added with add_term(). The first (lowest index) entry contains
  // the contribution of the earliest add_term() call; the last to the
  // latest add_term() call.
  real_vector get_func_breakdown() const;


  // Refer to simple_error_func.h for the use of the vary() member function,
  // which error_func inherits.


  // the constructor:
  // Refer to the definition of abstract_error_func in optimizer.h for the
  // purpose of the argument no_limits_flag. Just using the default value
  // (ie, don't provide a constructor argument) is appropriate in nearly all
  // cases.
  error_func(bool no_limits_flag = false)
    : error_func_parameters(no_limits_flag) { }


  // func_value(): the only virtual function of abstract_error_func which
  // still needs to be defined.

  double func_value();
  // Returns the final error function, after summing over all error terms,
  // sweepers, etc. Calls error terms via their associated weighted_term
  // objects held in error_func::terms. func_value() algorithm: When called,
  // func_value() does the following:
  //  (1) initializes its return value accumulator to 0.0
  //  (2) gets a new state_tag for the "sweeperless" terms
  //  (3) Loop: for each "sweeperless" term added using add_term():
  //      (3.1) call the weighted_term's reset(),
  //            which clears its result and resets the error_term
  //      (3.2) call the weighted_term's get(state_tag), which:
  //            (3.2.1) calls the term's error_term::get(state_tag)
  //            (3.2.2) weights the error and adds it into the term's result
  //      (3.3) add the term's result into the return value
  //  (4) Loop: for each unique sweeper identified by an add_term():
  //      (4.1) reset the sweeper using its sweeper::reset()
  //      (4.2) call the associated sweeper_info::reset_terms(), which:
  //            (4.2.1) Loop: for each term added using this sweeper:
  //                    (4.2.1.1) call the weighted_term's reset()
  //      (4.3) Loop: until sweeper::is_finished() goes true:
  //            (4.3.1) get a new state_tag
  //            (4.3.2) call the associated sweeper_info::calc_terms(), which:
  //                    (4.3.2.1) Loop: for each term added using this sweeper: 
  //                              (4.3.2.1.1) weighted_term::get(state_tag)
  //            (4.3.3) increment sweeper
  //      (4.4) call the associated sweeper_info::get_error(), which:
  //            (4.4.1) Loop: for each term added using this sweeper: 
  //                    (4.4.1) divide result by the number of sweeper points
  //                    (4.4.2) add the result into the return value
  //  (5) return the accumulated return value


private:

  // This class takes care of multiplying error terms by weighting factors
  // and maintaining accumulators for their error contributions
  class weighted_term
  {
  public:
    double weight;                 // weight of this term
    error_term * et;               // pointer to the function
    double result;                 // accumulate the result for this term  

    // The constructor initializes parameters.
    weighted_term(double w, error_term & etr) : weight(w), et(&etr), result(0.0) { }

    // Reset the term and clear the accumulator
    weighted_term & reset() { result = 0.0; et->reset(); return *this; }

    // Calculate and the weighted error and add into the accumulator.
    weighted_term & get(state_tag s) { result += weight*(et->get(s)); return *this; }
  } ;

  // to make the code a bit more readable, we add typedefs using weighted_term :
  typedef  std::vector<error_func::weighted_term>  term_list;
  typedef  term_list::size_type                    term_index;
  typedef  std::vector<term_index>                 term_index_list;
  typedef  term_index_list::size_type              term_index_list_index;

  // This class has the information on terms for a single sweeper. It keeps a
  // record of the weighted_terms which use the sweeper, and a few functions
  // which loop over all of these associated terms.
  class sweeper_info
  {
  public:
    // The terms vector keeps track of all the weighted_terms to be summed
    // by this sweeper. It holds indexes into the container with all terms.
    error_func::term_index_list sweeper_terms;

    // Method reset() loops through sweeper_terms, calling reset() for each.
    // It is called with a reference to error_func::terms
    void reset_terms(error_func::term_list &);

    // Method get_terms loops through sweeper_terms, calling get() for each.
    void calc_terms(state_tag, error_func::term_list &);

    // Method get_error averages the terms by dividing each term's result
    // by npoints; it adds them all and returns the sum
    double get_error(int npoints, error_func::term_list &);
  };
  

  // Here we store all the weighted_terms added using add_term:
  term_list terms;

  // Here we store all the sweepers and their associated information in an
  // STL associative array (map)
  std::map<sweeper *, sweeper_info> swp_map ;

  // Keep a vector of all the terms that don't have sweepers.
  term_index_list sweeperless_terms;

} ;


// ************************************************************************
class error_term_mode : public error_term
{
public:
  enum mode {MATCH, ABOVE, BELOW, FLAT, M_MATCH, M_ABOVE, M_BELOW};

  // Constructor sets mode and target, initializes other values to 0.
  // If the mode is FLAT, then target_val will be ignored.
  error_term_mode(mode m = MATCH, double t = 0.0):
    flag(m), target_val(t)
  { reset(); }

  // Method to set the target.
  error_term_mode & target(double t) { target_val = t; return *this; }

  // The following methods may be called to set the mode.
  error_term_mode & match() {flag = MATCH; return *this;}
  error_term_mode & above() {flag = ABOVE; return *this;}
  error_term_mode & below() {flag = BELOW; return *this;}
  error_term_mode & flat()  {flag = FLAT;  return *this;}
  error_term_mode & worst_match() {flag = M_MATCH; return *this;}
  error_term_mode & worst_above() {flag = M_ABOVE; return *this;}
  error_term_mode & worst_below() {flag = M_BELOW; return *this;}

  // The following methods may be called to set both the mode and target.
  error_term_mode & match(double t) {flag = MATCH; target_val = t; return *this;}
  error_term_mode & above(double t) {flag = ABOVE; target_val = t; return *this;}
  error_term_mode & below(double t) {flag = BELOW; target_val = t; return *this;}
  error_term_mode & worst_match(double t) {flag = M_MATCH; target_val = t; return *this;}
  error_term_mode & worst_above(double t) {flag = M_ABOVE; target_val = t; return *this;}
  error_term_mode & worst_below(double t) {flag = M_BELOW; target_val = t; return *this;}

  // Compute the error for the given mode.
  double checkval(double x);

  // Reset is called before a sweep, and clears all memory of past calculations.
  void reset()
  {
    sum_xx = 0.0;
    sum_x = 0.0;
    n = 0;
    old_error = 0.0;
  }

  // Virtual destructor is necessary to ensure proper subclass destruction.
  virtual ~error_term_mode() { }

private:
  // Which type of error to calculate.
  mode flag;

  // Target for MATCH, minimum for ABOVE, etc.
  // If the mode is FLAT, then target_val is ignored.
  double target_val;

  // The following variables are used for FLAT, M_MATCH, M_ABOVE, M_BELOW.
  double sum_xx;      // Keep a running sum of the squares of the values.
  double sum_x;       // Keep a running sum of the values.
  unsigned n;         // The number of points since the last reset() call.
  double old_error;   // The value of the error from the last iteration.

};


// ************************************************************************
class scaled_match_error_term : public error_term
{
protected:
  // Zero is called by reset, and clears all running sums to 0.
  // This function is provided for subclasses that overload reset().
  void zero()
  {
    sum_aa = 0.0;
    sum_bb = 0.0;
    sum_ab = 0.0;
    old_error = 0.0;
    n = 0;
  }

public:
  // Constructor sets initial values to 0.
  scaled_match_error_term() :
  sum_aa(0.0), sum_bb(0.0), sum_ab(0.0), old_error(0.0), n(0)
  { }

  // Reset is called before a sweep, and clears all running sums to 0.
  // Any derived classes that implement reset() must remember to call zero().
  virtual void reset() { zero(); }

  // Return the values that we want to match.
  // These are the functions that must be defined in concrete subclasses.

  virtual double get_a(state_tag) = 0;  // The reference data
  virtual double get_b(state_tag) = 0;  // The data to be scaled and compared

  // This function returns the scale factor for b to make it match a
  double scale() const
  { return (sum_bb == 0.0 && sum_ab == 0.0) ? 1.0 : sum_ab/sum_bb; }

  // The method get will calculate the error function by calling get_value.
  // It should not be modified or redefined in subclasses.
  double get(state_tag);

  // Virtual destructor is necessary to ensure proper subclass destruction.
  virtual ~scaled_match_error_term() { }

private:
  double sum_aa;      // Keep a running sum of the squares of value a.
  double sum_bb;      // Keep a running sum of the squares of value b.
  double sum_ab;      // Keep a running sum of (value a) * (value b).
  double old_error;   // The value of the error from the last iteration.
  unsigned n;         // iteration counter
};

#endif /* ERROR_FUNC_H */
