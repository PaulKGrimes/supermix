// SuperMix version 1.6 C++ source file
// Copyright (c) 1999, 2001, 2004, 2009 California Institute of Technology.
// All rights reserved.
// ************************************************************************
// sweeper.h
//
// class for sweeping parameter values
//
//
// 5/8/09  fixed silly code which assumed number of bits in an int
// 7/20/98 J.Z., F.R., J.S.W.
// ************************************************************************
// Using the sweeper class:
// 
// Sweepers manage setting one or more parameters through a desired range
// of values. They are used by the class error_func to sum error term
// calculations over a range of parameter values such as a frequency range.
// Sweepers may prove useful in other applications as well. Here is an
// example of how to use a sweeper:
//
//   parameter p1, p2;
//   sweeper s;
//   s.sweep(p1, 0.0, 1.0, 0.1);
//   s.sweep(p2, 2.0, 3.0, 0.5, GHz);
//
// We've set up sweeper s to sweep the values of p1 and p2; p1 goes from
// 0.0 to 1.0 in increments of 0.1, p2 from 2.0*GHz to 3.0*GHz in increments
// of 0.5*GHz (Note that "GHz" is just a double value that sweeper uses as a
// scale factor for the range and increment arguments). Thus we have defined
// a 2-dimensional grid of points described by ordered pairs of (p1,p2). The
// range, increment, and scale factor arguments of sweep() must evaluate to
// double values; they are only read once: at the time sweep() is executed.
//
// We can use the sweeper s to loop through the points as follows:
//
//   for(s.reset(); !s.finished(); s++) { your calculation here }
//
// s.reset() sets the two parameters to their initial values. s++ (or ++s)
// sets the two parameters to the values defined by the next point in the
// grid. The first call of s.sweep() determines the variable which changes
// fastest (the "inner loop") as the grid is traversed; in the above example
// p1 changes fastest. Each call to s++ causes the values of p1 and p2 to be
// updated until the values both "wrap" back to the beginning of the
// grid. At this time s.finished() returns true, and any subsequent s++
// attempts would leave p1 and p2 unchanged and generate warning messages
// until s.reset() is executed to set things up again.
//
// If the values of p1 and/or p2 are changed between calls to s++, they
// will be set to their proper grid values at the next call to s++ (unless
// s.finished() is true).
//
// In addition to the sweep() member function there is initialize(). This
// member function is essentially a single-valued sweep; it ensures that
// its argument parameter is set to the specified value with each call to
// reset() or ++. Here's an example. Adding:
//
//   parameter p3;
//   s.initialize(p3, 4.0);
//
// to the above code would ensure that p3 is set to 4.0 every time the
// sweeper adjusts the values of p1 and p2.
//
// There are several ways to specify the range of values for a sweep() in
// addition to the way used in the example; see the comments in the
// definition of the sweeper class below. All of the various methods for
// setting the sweep range using sweep() require that the range values be
// fully defined at the time sweep() is executed; the arguments are only
// evaluated once at the time of the sweep() call.
//
//
// The sweeper::setup() function:
//
// the setup() function is called by a sweeper just after it sets parameter
// values during reset() or ++. As defined, setup() does nothing, but it is
// declared virtual. If you need to perform some function following the
// setting of parameters to the next point on the sweep grid, then derive a
// new class from class sweep and simply define the setup function in this
// derived class to do whatever you wish. Example:
//
//   class my_sweeper : public sweeper {
//     void setup() { setup code goes here }
//   };
//  
//   my_sweeper s;
//
// ************************************************************************
                                                                          
#ifndef SWEEPER_H
#define SWEEPER_H

#include "parameter/real_parameter.h"
#include <vector>        // STL vector template class
#include <list>          // STL linked list template class
#include "vector.h"      // SuperMix numerical vector classes
#include "interpolate.h" // SuperMix interpolator<> classes

class sweeper
{
					   
public:

  // constructor takes no arguments
  sweeper();

  // sweep() defines the parameters to control and the range of values.
  // The versions which pass a container of values make local copies of
  // those values; the container must be filled with the desired values
  // at the time sweep() is executed.

  // Specify start, stop and increment values:
  void sweep(real_parameter &rp, double start, double stop, double step) ;

  // Include a scale factor (units) for the values
  void sweep(real_parameter &rp, double start, double stop, double step,
	     double units)
    { sweep(rp, start*units, stop*units, step*units); }

  // Pass a C-style array of values with a length:
  void sweep(real_parameter &rp, double values[], int length) ;

  // Pass a SuperMix real_vector of values:
  void sweep(real_parameter &rp, const real_vector & values) ;

  // Pass an STL vector<double> of values:
  void sweep(real_parameter &rp, const std::vector<double> & values) ;

  // Pass a SuperMix interpolator<>; uses the x values of the
  // points used in the interpolation. The interpolator must be filled and
  // its build() function must have been called. See interpolate.h for info
  // about creating and building an interpolator<>.
  template <class T>
  void sweep(real_parameter &rp, const interpolator<T> & values)
    {
      if(!values.ready()) {
	error::fatal("sweeper::sweep(): Must build interpolator before"
		     "passing it to sweep().");
      }
      
      // not the most efficient way, but it works
      std::vector<double> v;
      int n  = values.size();
      for(int i = 0; i < n; ++i) 
	v.push_back(values.x(i));
      sweep(rp, v);
    }


  // This gives a parameter and a value to which it will be set each time
  // the sweeper adjusts the values of its swept parameters. 
  void initialize(real_parameter &rp, double value) ;

  // Include a scale factor (units) for the value
  void initialize(real_parameter &rp, double value, double units)
    { initialize(rp, value*units); }


  // This gives the number of points in the grid over with this sweeper
  // sweeps. Always returns at least 1, even if no sweeps defined.
  int npoints() { return num_values; }

  // This static function gives the max allowed number of points in the sweep.
  // The max may be be changed by using the form with an argument
  static int maxpoints() { return max_points; }
  static int maxpoints(int newmax) {return (max_points = newmax); }

  // The following functions actually control and execute the sweep of
  // the parameter values:

  // reset() - start parameter values at the beginning. Always call this
  // function first when you start a sweep of the parameters.
  void reset() ;

  // increment operators -- go to next set of parameter values
  // in the sweep
  sweeper & operator ++(int);
  sweeper & operator ++() {return((*this)++);}

  // finished() goes true when a call to ++ wraps all parameter values
  // back to their starting values; it is reset to false by a call to
  // reset(). Calls to ++ when finished() is true generate a warning
  // message and leave the parameter values unchanged.
  bool finished() {return alldone;}

  // Derive a class from sweeper and define a useful version of this
  // function there if you need to perform some additional set up tasks
  // following the adjustment of swept parameters during a sweep
  // operation. This function is called by reset() and ++.
  virtual void setup() { }

  // Since setup() is virtual, we need a virtual destructor:
  virtual ~sweeper() { }

private:

  // One of these is created and pushed to a list for every call to
  // sweep() or initialize():
  class sweep_parameter
    {
    private:
      real_parameter * rpr ;       // hold a pointer to the actual parameter
      bool vecmode ;               // vecmode==false for startv, stopv, step; 
      double startv, stopv, step ; // startv, stopv, step for value
      std::vector<double> values ; // or use a vector of values instead
				   // (in that case vecmode==1)
      int num_values ;             // number of values
      int index ;                  // index for current value
      bool wrapped ;               // index wrapped to zero on last increment
      double getval() const ;      // return current value
      // default constructor is hidden
      sweep_parameter() { }
    public:
      // constructor using startv, stopv, step
      sweep_parameter(real_parameter &rp, double startvp, 
                      double stopvp, double stepp);
      // constructor using C-style array of values
      sweep_parameter(real_parameter &rp, double *valp, int nump);
      // constructor using Frank Rice's real_vector class
      sweep_parameter(real_parameter &rp, const real_vector & rvec);
      // constructor using STL vector<double> array of values
      sweep_parameter(real_parameter &rp, const std::vector<double> & val);
      // postfix increment operator
      sweep_parameter & operator ++(int) ;
      // set parameter value without incrementing
      sweep_parameter & touch();
      // set index to zero
      void reset() ;
      // to check if value wrapped back to start 
      bool check_wrap() { return wrapped ; }
      // number of points in this sweep parameter's range
      int npoints() { return num_values; }
    };

  // Here are the actual private member variables:
  std::list<sweep_parameter> parms ;  // the list of sweep_parameters
  bool alldone ;                      // goes true when a sweep is completed
  static int max_points ;             // maximum allowed number of sweep values
  int  num_values ;                   // total number of points in a sweep
} ;

#endif /* SWEEPER_H */

