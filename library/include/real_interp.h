// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ********************************************************************
// real_interp.h
//
// The real_interp class is an abstract real parameter which gets its value
// by interpolating into a table. The table may be supplied at
// construction, from either an external file, a datafile object or a
// real_matrix object. The independent variable used as an index for the
// interpolation comes from an abstract real parameter, which may be
// supplied at construction.
//
// Since real_interp is an interpolator<double>, the member functions of
// that class are available for real_interp, such as spline() (the default)
// and linear(), etc.
// ********************************************************************

#ifndef REAL_INTERP_H
#define REAL_INTERP_H

#include "parameter/abstract_real_parameter.h"
#include "interpolate.h"
#include "datafile.h"

class real_interp : public abstract_real_parameter, public interpolator<double>
{
public:

  // The option enumeration definitions for use in the constructors:

  typedef enum { STD, PHASE } phase_type;      // y (dependent variable) data type flag.

  // STD   : The default. Don't do anything special with the y values.
  //
  // PHASE : Interpret the y values as phase data IN RADIANS, which may wrap
  //         around discontinuously after every 2 Pi of phase difference.
  //         Correctly interpolates across such phase discontinuities using get().


  // ----------------------------------------------
  // Constructing the real_interp object:

  // In each of the following constructors, the argument xp refers to a real
  // parameter sort of object which will be used to determine the x values used during
  // interpolations when the real_interp object is asked for its value
  // (see get(), below).

  // Also note that the (x,y) data supplied at construction need not be sorted, but all
  // x values must be unique.


  // ----------------------------------------------
  // Constructing, but supplying no data (add data using file() or table()):

  explicit
  real_interp(const abstract_real_parameter & xp, // The independent (x) variable object
	      phase_type ptype = STD              // Will the y data be phase data?
	      ) ;

  // xp refers to a real parameter sort of object which will be used to determine the x
  // value used to perform an interpolation when the real_interp object is asked for its
  // value (see get(), below).

  explicit
  real_interp(phase_type ptype = STD) ;           // Pick parameter and get data later... 


  // ----------------------------------------------
  // Constructing, taking data directly from a file or stream:

  real_interp(
	      const abstract_real_parameter &xp, // The independent (x) variable object
	      const char * const name,           // The name of the file to open
	      double x_unit = 1.0,               // Units for the x value
	      double y_unit = 1.0,               // Units for the y value
	      int ycol = 1,                      // Which y-value number on line to use
	      bool ignore_ws = true,             // Should all blank lines be ignored?
	      phase_type ptype = STD             // Is the y data phase data?
	      );

  // The above version opens and reads from the named file. Using the functionality of
  // data_parser in io.h, it always skips initial blank lines and comments in the
  // file. If ignore_ws is set to false, then any further blank or comment lines
  // once data starts being read terminates the input. Otherwise the default is to
  // read the entire file. ycol tells which number entry should be read in
  // each line; don't count the 1st column, which contains the x value (a double).
  // x and y values are scaled (multiplied) by x_unit and y_unit, respectively.

  // The following constructor is like the above, except it reads from a supplied input
  // stream. It starts from the current stream location and then skips any initial
  // blank or comment lines: 

  real_interp(
	      const abstract_real_parameter &xp, // The independent (x) variable object
	      std::istream & s,                  // The input stream
	      double x_unit = 1.0,               // Units for the x value
	      double y_unit = 1.0,               // Units for the y value
	      int ycol = 1,                      // Which complex number on line to use 
	      bool ignore_ws = true,             // Should all blank lines be ignored?
	      phase_type ptype = STD             // Is the y data phase data?
	      );


  // ----------------------------------------------
  // Constructing with data taken from existing data table variables: 

  // Constructing from a real_matrix object:

  real_interp(const real_matrix &rt,              // The real_matrix with the data points 
	      int xrow, int yrow,                 // Row references into the real_matrix 
	      const abstract_real_parameter &xp,  // The independent (x) variable object 
	      phase_type ptype = STD              // Is the y data phase data?
	      ) ;

  // The x and y row numbers, xrow and yrow, refer to LEFT INDEX values in the 
  // real_matrix. They correspond to the FIRST INDEX in a real_matrix::read(i,j) function
  // call.


  // Constructing from a datafile object:

  real_interp(const datafile &df,                 // The datafile with the data points
	      int xcol, int ycol,                 // Column references into the datafile
	      const abstract_real_parameter &xp,  // The independent (x) variable object
	      phase_type ptype = STD              // Is the y data phase data?
	      ) ;

  // The x and y column numbers, xcol and ycol, refer to the actual columns in the
  // ASCII file on the disk from which the datafile was constructed. They correspond
  // to the FIRST INDEX in a datafile::read(i,j) function call. The data table is read
  // only once, at construction, so it may refer to a temporary object. For example:
  //
  //   real_interp R(datafile("my_data"),1,2,device::f);
  //
  // reads the data from "my_data" into a temporary datafile object, uses it to build
  // the interpolation tables within R, and then discards the datafile object. The first
  // column in the file "my_data" supplies the x (independent) values, the second column
  // the y (dependent) values. The parameter device::f will be the independent variable.


  // ----------------------------------------------
  // Supplying the data or x parameter following construction:

  // parameter():

  real_interp & parameter(const abstract_real_parameter &xp)
    { xparam = &xp ; return *this ; }


  // file():

  real_interp & file(
	      const char * const name,  // The name of the file to open
	      double x_unit = 1.0,      // Units for the x value
	      double y_unit = 1.0,      // Units for the y value
	      int ycol = 1,             // Which y value on line to use 
	      bool ignore_ws = true,    // Should all blank lines be ignored?
	      phase_type ptype = STD    // Is the y data phase data?
	      );

  real_interp & file(
	      std::istream & s,         // The input stream
	      double x_unit = 1.0,      // Units for the x value
	      double y_unit = 1.0,      // Units for the y value
	      int ycol = 1,             // Which y value on line to use 
	      bool ignore_ws = true,    // Should all blank lines be ignored?
	      phase_type ptype = STD    // Is the y data phase data?
	      );


  // table():
  
  real_interp & table(
	      const real_matrix &rt,              // The real_matrix with the data points 
	      int xrow, int yrow,                 // Row references into the real_matrix 
	      phase_type ptype = STD              // Is the y data phase data?
	      )
    {  phase = ptype ; construct(rt, xrow, yrow) ; return *this ; }

  real_interp & table(
	      const datafile &df,                 // The datafile with the data points
	      int xcol, int ycol,                 // Column references into the datafile
	      phase_type ptype = STD              // Is the y data phase data?
	      )
    {  phase = ptype ; construct(*(df.table()), xcol, ycol) ; return *this ; }


  // ----------------------------------------------
  // Return an interpolated value:

  double get() const ;  // Returns a y(x) value obtained by interpolation, using the current
                        // value of the abstract_real_parameter supplied during construction
                        // as the independent (x) value into the interpolator.

  // get() implements the abstract interface declaration in the abstract_real_parameter
  // base class from which real_interp is derived.


private:
  const abstract_real_parameter *xparam ;  // this parameter gives the x value
  phase_type phase ;
  double phase_adjust(double phase) const; // bring phase into -Pi, Pi range
  void construct(const real_matrix &rt, int xrow, int yrow) ;

};

#endif /* REAL_INTERP_H */










