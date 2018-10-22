// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// complex_interp.cc

#include <iostream>
#include <cmath>
#include "global.h"
#include "error.h"
#include "SIScmplx.h"
#include "units.h"
#include "device.h"
#include "complex_interp.h"
#include "io.h"

using namespace std;

// ----------------------------------------------
// Constructing the complex_interp object

// Constructors with no supplied data
//
complex_interp::complex_interp(const abstract_real_parameter &xp) :
  interpolator<complex>(), xparam(&xp)
{ }

complex_interp::complex_interp() :
  interpolator<complex>(), xparam(0)
{ }


// Constructors from input file or stream
//

complex_interp::complex_interp(const abstract_real_parameter &xp,
			       const char * const name, double x_unit,
			       double y_unit, int ycol, bool ignore_ws ) :
  interpolator<complex>(), xparam(&xp)
{ file(name, x_unit, y_unit, ycol, ignore_ws); }

complex_interp::complex_interp(const abstract_real_parameter &xp,
			       istream & s, double x_unit,
			       double y_unit, int ycol, bool ignore_ws ) :
  interpolator<complex>(), xparam(&xp)
{ file(s, x_unit, y_unit, ycol, ignore_ws); }


// Constructor from datafile
//
complex_interp::complex_interp(const datafile &df, int xcol, 
			       int ycol1, int ycol2, 
			       const abstract_real_parameter &xp, 
			       complex_interp::data_mode dmode) :
  interpolator<complex>(), xparam(&xp)
{  
  // construct from the table associated with datafile
  construct(*(df.table()), xcol, ycol1, ycol2, dmode) ;
}

// Constructor from real_matrix
//
complex_interp::complex_interp(const real_matrix &rt, int xrow, 
			       int yrow1, int yrow2, 
			       const abstract_real_parameter &xp, 
			       complex_interp::data_mode dmode) :
  interpolator<complex>(), xparam(&xp)
{  
  construct(rt, xrow, yrow1, yrow2, dmode) ;
}

// Construct from a complex_matrix
//
complex_interp::complex_interp(const complex_matrix &ct, int xrow, 
			       int yrow,
			       const abstract_real_parameter &xp) :
  interpolator<complex>(), xparam(&xp)
{
  construct(ct, xrow, yrow) ;
}

// This does the real construction work - from real table data
//
void complex_interp::construct(const real_matrix &rt, int xrow, int yrow1,
			       int yrow2, complex_interp::data_mode dmode)
{
  int npts = rt.Rmaxindex()-rt.Rminindex()+1 ; // how many data points ?
  int nrow = rt.Lmaxindex()-rt.Lminindex()+1 ; // how many rows ?

  // see if the constructor arguments are reasonable

  if(npts < 1 || nrow < 1) {
    error::warning(
    "in complex_interp constructor: table contains no data !") ;
    return ;
  }
  if(npts < 2) {
    error::warning(
    "in complex_interp constructor: table contains insufficient data!") ;
    return ;
  }
  if(nrow < 3) {
    error::warning(
    "in complex_interp constructor: table needs to have at least 3 rows !");
    return ;
  }
  if(xrow < rt.Lminindex() || xrow > rt.Lmaxindex()) {
    error::warning(
    "in complex_interp constructor: x row number out of range !") ;
    return ;
  }
  if(yrow1 < rt.Lminindex() || yrow1 > rt.Lmaxindex()) {
    error::warning(
    "in complex_interp constructor: y1 row number out of range !") ;
    return ;
  }
  if(yrow2 < rt.Lminindex() || yrow2 > rt.Lmaxindex()) {
    error::warning(
    "in complex_interp constructor: y2 row number out of range !") ;
    return ;
  }

  // pass the tests, so 
  // add the data points to the interpolator

  Complex yval ;
  for(int icol=rt.Rminindex(); icol<=rt.Rmaxindex(); icol++) {
    switch(dmode) {
    case CARTESIAN:
      yval = Complex(rt[yrow1][icol], rt[yrow2][icol]) ;
      break;
    case POLAR:
      yval = polar(rt[yrow1][icol], rt[yrow2][icol]) ;
      break;
    case DB:
      yval = polar(pow(10.0, 0.05*rt[yrow1][icol]), rt[yrow2][icol]*Degree) ;
      break;
    }
 
    add(rt[xrow][icol],yval);
  }

  build();
}

// This does the real construction work - from complex table data
//
void complex_interp::construct(const complex_matrix &ct, int xrow, int yrow)
{
  int npts = ct.Rmaxindex()-ct.Rminindex()+1 ; // how many data points ?
  int nrow = ct.Lmaxindex()-ct.Lminindex()+1 ; // how many rows ?

  // see if the constructor arguments are reasonable

  if(npts < 1 || nrow < 1) {
    error::warning(
    "in complex_interp constructor: table contains no data !") ;
    return ;
  }
  if(npts < 2) {
    error::warning(
    "in complex_interp constructor: table contains insufficient data!") ;
    return ;
  }
  if(nrow < 2) {
    error::warning(
    "in complex_interp constructor: table needs to have at least 2 rows !");
    return ;
  }
  if(xrow < ct.Lminindex() || xrow > ct.Lmaxindex()) {
    error::warning(
    "in complex_interp constructor: x row number out of range !") ;
    return ;
  }
  if(yrow < ct.Lminindex() || yrow > ct.Lmaxindex()) {
    error::warning(
    "in complex_interp constructor: y row number out of range !") ;
    return ;
  }

  // pass the tests, so 
  // add the data points to the interpolator

  for(int icol=ct.Rminindex(); icol<=ct.Rmaxindex(); icol++) {
    add(ct[xrow][icol].real,ct[yrow][icol]);
  }

  build();
}


// ----------------------------------------------
// read in data from an input stream or file

complex_interp & complex_interp::file(istream & s, double xu, double yu, int yc, bool ws)
{
  if(!s) {
    error::warning("complex_interp: couldn't read from input.");
    return *this;
  }

  data_parser d(s);
  unsigned n = d.skip_parse();
  if(!n) {
    error::warning("complex_interp: no data found in input.");
    return *this;
  }

  while(n) {
    double x;
    Vector y;
    int m;

    m = d.convert(x, y, xu, yu);
    if(m < yc) {
      error::warning("complex_interp: input stream doesn't have enough columns of data.");
    }
    else {
      add(x,y.read(yc));
    }

    n = (ws) ? d.skip_parse() : d.parse();
  }
  build();
  return *this;
}


complex_interp & 
complex_interp::file(const char * const name, double xu, double yu, int yc, bool ws)
{
  ifstream f(name);
  if(!f) {
    error::warning("complex_interp: couldn't open file", name);
    return *this;
  }
  file(f,xu,yu,yc,ws);
  return *this;
}


// ----------------------------------------------
// read in data from a Touchstone-formatted 1-port file

complex_interp & complex_interp::touchstone(const char * const name, char conversion)
{
  // use a touchstone_read object to read file
  touchstone_read file;
  if ( !file.open(name,1) ) {
    // couldn't open file properly. file issues a warning with details
    error::warning("complex_interp::touchstone(): couldn't deal with file", name);
    return *this;  // this line won't get executed unless you change the one above
  }

  // file opened ok, so read data into the interpolator
  Matrix M;
  double f;

  switch ( conversion ) {
  default:
    // unrecognized character
    error::warning("complex_interp::touchstone(): unknown conversion option; using S");
    // fall through to case 'S'
  case 'S':
  case 's':
    // use S parameter
    while ( file.Svalue(f,M) ) add(f,M[1][1]);
    break;
  case 'Z':
  case 'z':
    // use Z value
    while ( file.Zvalue(f,M) ) add(f,M[1][1]);
    break;
  case 'Y':
  case 'y':
    // use Y value
    while ( file.Yvalue(f,M) ) add(f,M[1][1]);
    break;
  }

  build();
  parameter(device::f);
  return *this;
}


// ----------------------------------------------
// Return an interpolated value:
// Define the get() virtual function for abstract_complex_parameter

Complex complex_interp::get() const
{
  if(!(ready() && xparam)) {
    error::warning(
    "attempted use of a complex_interp object that was not properly constructed");
    return(0.) ;
  }

  return (*this)(xparam->get());
}
