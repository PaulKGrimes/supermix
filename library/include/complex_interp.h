*  *  // SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
/** ********************************************************************
*   complex_interp.h
*
*   The complex_interp class is an abstract complex parameter which gets its
*   value by interpolating into a table. The table may be supplied at
*   construction, from either an external file, a complex_matrix, a datafile
*   object or a real_matrix object. The independent variable used as an index
*   for the interpolation comes from an abstract real parameter, which may be
*   supplied at construction.
*
*   Since complex_interp is an interpolator<complex>, the member functions of
*   that class are available, such as spline() (the default), linear(), etc.
*   ********************************************************************/

#ifndef COMPLEX_INTERP_H
#define COMPLEX_INTERP_H

#include <iosfwd>

#include "parameter/abstract_complex_parameter.h"
#include "parameter/abstract_real_parameter.h"
#include "interpolate.h"
#include "datafile.h"

class complex_interp : public abstract_complex_parameter, public interpolator<complex>
{
public:

  /** ----------------------------------------------
  *   Constructing the complex_interp object:

  *   In each of the following constructors, the argument xp refers to a real
  *   parameter sort of object which will be used to determine the x values used during
  *   interpolations when the complex_interp object is asked for its value
  *   (see get(), below).

  *   Also note that the (x,y) data supplied need not be sorted, but all
  *   x values must be unique.*/


  /** ----------------------------------------------
  *   Constructing, but supplying no data (add data using file() or table()): */

  explicit
  complex_interp(
	      const abstract_real_parameter &xp   // The independent (x) variable object
	      ) ;

  /** xp refers to a real parameter sort of object which will be used to determine the x
  *   value used to perform an interpolation when the complex_interp object is asked for
  *   its value (see get(), below).*/

  complex_interp() ;                              // Pick parameter and get data later...


  /** ----------------------------------------------
  *   Constructing, taking data directly from a file or stream:*/

  complex_interp(
	      const abstract_real_parameter &xp, // The independent (x) variable object
	      const char * const name,           // The name of the file to open
	      double x_unit = 1.0,               // Units for the x value
	      double y_unit = 1.0,               // Units for the y value
	      int ycol = 1,                      // Which complex number on line to use
	      bool ignore_ws = true              // Should all blank lines be ignored?
	      );

  /** The above version opens and reads from the named file. Using the functionality
  *   of data_parser in io.h, it always skips initial blank lines and comments in the
  *   file. If ignore_ws is set to false, then any further blank or comment lines
  *   once data starts being read terminates the input. Otherwise the default is to
  *   read the entire file. ycol tells which complex number entry should be read in
  *   each line; don't count the 1st column, which contains the x value (a double).
  *   The format for interpreting complex number data is given by Complex::in_mode();
  *   see SIScmplx.h for details. x and y values are scaled (multiplied) by x_unit
  *   and y_unit, respectively.*/

  /** The following constructor is like the above, except it reads from a supplied input
  *   stream. It starts from the current stream location and then skips any initial
  *   blank or comment lines: */

  complex_interp(
	      const abstract_real_parameter &xp, // The independent (x) variable object
	      std::istream & s,                  // The input stream
	      double x_unit = 1.0,               // Units for the x value
	      double y_unit = 1.0,               // Units for the y value
	      int ycol = 1,                      // Which complex number on line to use
	      bool ignore_ws = true              // Should all blank lines be ignored?
	      );


  /** ----------------------------------------------
  *   Constructing with data taken from existing data table variables:

  *   Constructing from a complex_matrix object:*/

  complex_interp(
	      const complex_matrix &ct,           // The matrix with the data points
	      int xrow,                           // Row index for the x data
	      int yrow,                           // Row index for the y data
	      const abstract_real_parameter &xp   // The independent (x) variable object
	      ) ;

  /** The x and y row numbers, xrow and yrow, refer to LEFT INDEX values in the 
  *   complex_matrix. They correspond to the FIRST INDEX in a complex_matrix::read(i,j)
  *   function call. Only the real part of the values in xrow will be used. */

  // The following option enumeration definitions for use in the remaining constructors:
  typedef enum {CARTESIAN=0, POLAR=1, DB=2} data_mode ; // for constructing from real data

  // CARTESIAN  : The default. Complex data is stored in the input real_matrix or
  //              datafile as (real, imaginary).
  //
  // POLAR      : Complex data is stored in the input real_matrix or datafile as
  //              (magnitude, phase), with the phase in RADIANS.
  //
  // DB         : Complex data is stored as (magnitude,phase),
  //              with the magnitude in DECIBELS and the phase in DEGREES.

  complex_interp(
	      const real_matrix &rt,              // The real_matrix with the data points
	      int xrow, int yrow1, int yrow2,     // Row references into the real_matrix
	      const abstract_real_parameter &xp,  // The independent (x) variable object
	      data_mode dmode = CARTESIAN         // the form of (yrow1,yrow2) data
	      ) ;

  // The x and y row numbers: xrow, yrow1, and yrow2, refer to LEFT INDEX values in the
  // real_matrix. They correspond to the FIRST INDEX in a real_matrix::read(i,j) function
  // call.


  // Constructing from a datafile object:

  complex_interp(
	      const datafile &df,                 // The datafile with the data points
	      int xcol, int ycol1, int ycol2,     // Column references into the datafile
	      const abstract_real_parameter &xp,  // The independent (x) variable object
	      data_mode dmode = CARTESIAN         // the form of (ycol1,ycol2) data
	      ) ;

  // The x and y column numbers: xcol, ycol1, and ycol2, refer to the actual columns in
  // the ASCII file on the disk from which the datafile was constructed. They correspond
  // to the FIRST INDEX in a datafile::read(i,j) function call. The data table is read
  // only once, at construction, so it may refer to a temporary object. For example:
  //
  //   complex_interp C(datafile("my_data"),1,2,3,device::f,complex_interp::POLAR);
  //
  // reads the data from "my_data" into a temporary datafile object, uses it to build
  // the interpolation tables within C, and then discards the datafile object. The first
  // column in the file "my_data" supplies the x (independent) values, the second and
  // third columns the y (dependent) values; in this example the complex y values are
  // interpreted as magnitude and phase (in radians). The parameter device::f will be
  // the independent variable.


  // ----------------------------------------------
  // Supplying the data or x parameter following construction:

  // parameter():

  complex_interp & parameter(const abstract_real_parameter &xp)
    { xparam = &xp ; return *this ; }


  // file():

  complex_interp & file(
	      const char * const name,  // The name of the file to open
	      double x_unit = 1.0,      // Units for the x value
	      double y_unit = 1.0,      // Units for the y value
	      int ycol = 1,             // Which complex number on line to use
	      bool ignore_ws = true     // Should all blank lines be ignored?
	      );

  complex_interp & file(
	      std::istream & s,         // The input stream
	      double x_unit = 1.0,      // Units for the x value
	      double y_unit = 1.0,      // Units for the y value
	      int ycol = 1,             // Which complex number on line to use
	      bool ignore_ws = true     // Should all blank lines be ignored?
	      );


  // touchstone():

  // Read in the data from a Touchstone-formatted 1-port data file and set the
  // x parameter to device::f (global frequency). Interprets the Touchstone
  // specification line to interpret the data format. In the event a specification
  // line is not present (specification lines begin with '#'), uses the default
  // format which would have a specification line of:
  //   "# GHZ S MA R 50"
  // (GHz frequency scale, S parameter in Magnitude-Degree, 50 Ohm normalization)
  complex_interp & touchstone(
	      const char * const name,  // The name of the file to open
	      char conversion = 'S'     // use 'Z' to convert data to impedance, 'Y'
	                                // for admittance. Default 'S' converts to
	                                // a reflection coefficient into a normalizing
	                                // impedance of device::Z0 at time of call
	      );


  // table():

  complex_interp & table(
	      const datafile &df,                 // The datafile with the data points
	      int xcol, int ycol1, int ycol2,     // Column references into the datafile
	      data_mode dmode = CARTESIAN         // the form of (ycol1,ycol2) data
	      )
    {  construct(*(df.table()), xcol, ycol1, ycol2, dmode) ; return *this ; }

  complex_interp & table(
	      const real_matrix &rt,              // The real_matrix with the data points
	      int xrow, int yrow1, int yrow2,     // Row references into the real_matrix
	      data_mode dmode = CARTESIAN         // the form of (ycol1,ycol2) data
	      )
    {  construct(rt, xrow, yrow1, yrow2, dmode) ; return *this ; }

  complex_interp & table(
	      const complex_matrix &ct,           // The matrix with the data points
	      int xrow,                           // Row index for the x data
	      int yrow
	      )
    {  construct(ct, xrow, yrow) ; return *this ; }


  // ----------------------------------------------
  // Return an interpolated value:

  Complex get() const ; // Returns a y(x) value obtained by interpolation, using the current
                        // value of the abstract_real_parameter supplied during construction
                        // as the independent (x) value into the interpolator.

  // get() implements the abstract interface declaration in the abstract_real_parameter
  // base class from which real_interp is derived.

private:
  const abstract_real_parameter *xparam ;  // parameter gives the x value
  void construct(const real_matrix &rt, int xcol, int ycol1, int ycol2, data_mode d) ;
  void construct(const complex_matrix &ct, int xcol, int ycol) ;

};

#endif /* COMPLEX_INTERP_H */
