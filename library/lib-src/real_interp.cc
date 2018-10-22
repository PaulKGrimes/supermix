// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// real_interp.cc

#include <cmath>
#include "global.h"
#include "error.h"
#include "parameter/abstract_real_parameter.h"
#include "datafile.h"
#include "real_interp.h"
#include "io.h"

using namespace std;

// ----------------------------------------------
// Constructing the real_interp object

// Constructors with no supplied data
//
real_interp::real_interp(const abstract_real_parameter &xp, 
                         real_interp::phase_type ptype) :
  interpolator<double>(), xparam(&xp), phase(ptype)
{ }

real_interp::real_interp(real_interp::phase_type ptype) :
  interpolator<double>(), xparam(0), phase(ptype)
{ }


// Constructors from input file or stream
//

real_interp::real_interp(const abstract_real_parameter &xp,
			 const char * const name, double x_unit,
			 double y_unit, int ycol, bool ignore_ws, 
			 real_interp::phase_type ptype ) :
  interpolator<double>(), xparam(&xp), phase(ptype)
{ file(name, x_unit, y_unit, ycol, ignore_ws, ptype); }

real_interp::real_interp(const abstract_real_parameter &xp,
			 istream & s, double x_unit,
			 double y_unit, int ycol, bool ignore_ws, 
			 real_interp::phase_type ptype ) :
  interpolator<double>(), xparam(&xp), phase(ptype)
{ file(s, x_unit, y_unit, ycol, ignore_ws, ptype); }


// Constructor from datafile
//
real_interp::real_interp(const datafile &df, int xcol, int ycol, 
                         const abstract_real_parameter &xp, 
                         real_interp::phase_type ptype) :
  interpolator<double>(), xparam(&xp), phase(ptype)
{
  construct(*(df.table()), xcol, ycol) ;
}


// Constructor from real_table
//
real_interp::real_interp(const real_table &rt, int xrow, int yrow, 
                         const abstract_real_parameter &xp, 
                         real_interp::phase_type ptype) :
  interpolator<double>(), xparam(&xp), phase(ptype)
{
  construct(rt, xrow, yrow) ;
}


// This does the real construction work
//
void real_interp::construct(const real_table &rt, int xrow, int yrow)
{
  int npts = rt.Rmaxindex()-rt.Rminindex()+1 ; // how many data points ?
  int nrow = rt.Lmaxindex()-rt.Lminindex()+1 ; // how many rows ?

  // see if the arguments are reasonable

  if(npts < 1 || nrow < 1) {
    error::warning(
  "in real_interp constructor: table contains no data !") ;
    return ;
  }
  if(npts < 2) {
    error::warning(
"in real_interp constructor: table contains insufficient data!") ;
    return ;
  }
  if(nrow < 2) {
    error::warning(
    "in real_interp constructor: table contains only one row!") ;
    return ;
  }
  if(xrow < rt.Lminindex() || xrow > rt.Lmaxindex()) {
    error::warning(
    "in real_interp constructor: x column number out of range !") ;
    return ;
  }
  if(yrow < rt.Lminindex() || yrow > rt.Lmaxindex()) {
    error::warning(
    "in real_interp constructor: y column number out of range !") ;
    return ;
  }

  // passed the tests, so...
  // add the data points to the interpolator

  double xval, yval ;

  for(int icol=rt.Rminindex(); icol<=rt.Rmaxindex(); icol++) {
    xval = rt[xrow][icol] ;
    yval = rt[yrow][icol] ;
    add(xval, yval) ;
  }

  // now adjust y values if they are phase data:
  if (phase == PHASE) {
    double yprev = 0.0 , ydelta ;
    for(data_iter i = data.begin(); i != data.end(); ++i) {
      ydelta = i->second - yprev ;
      ydelta = phase_adjust(ydelta) ;  // force delta(phase) to -Pi, Pi
      i->second = yprev += ydelta ;
    }
  }

  // finally build the interpolator:
  build() ;  
}


// ----------------------------------------------
// read in data from an input stream or file

real_interp & 
real_interp::file(istream & s, double xu, double yu, int yc,
		  bool ws, real_interp::phase_type ptype)
{
  phase = ptype;

  if(!s) {
    error::warning("real_interp: couldn't read from input.");
    return *this;
  }

  data_parser d(s);
  unsigned n = d.skip_parse();
  if(!n) {
    error::warning("real_interp: no data found in input.");
    return *this;
  }

  while(n) {
    double x;
    real_vector y;
    int m;

    m = d.convert(x, y, xu, yu);
    if(m < yc) {
      error::warning("real_interp: input stream doesn't have enough columns of data.");
    }
    else {
      add(x,y.read(yc));
    }

    n = (ws) ? d.skip_parse() : d.parse();
  }

  // now adjust y values if they are phase data:
  if (phase == PHASE) {
    double yprev = 0.0 , ydelta ;
    for(data_iter i = data.begin(); i != data.end(); ++i) {
      ydelta = i->second - yprev ;
      ydelta = phase_adjust(ydelta) ;  // force delta(phase) to -Pi, Pi
      i->second = yprev += ydelta ;
    }
  }

  // finally build the interpolator:
  build();
  return *this;
}


real_interp & 
real_interp::file(const char * const name, double xu, double yu, int yc,
		  bool ws, real_interp::phase_type ptype)
{
  phase = ptype;

  ifstream f(name);
  if(!f) {
    error::warning("real_interp: couldn't open file", name);
    return *this;
  }
  file(f,xu,yu,yc,ws,ptype);
  return *this;
}


// ----------------------------------------------
// Return an interpolated value:
// Define the get() virtual function for abstract_real_parameter
//
double real_interp::get() const
{
  if(!(ready() && xparam)) {
    error::warning(
    "attempted use of a real_interp object that was not properly constructed");
    return(0.) ;
  }

  double xval = xparam->get() ;  // find out x value
  double yval = (*this)(xval) ;

  if (phase==PHASE) {
    yval = phase_adjust(yval) ;  // force range to -Pi, Pi
  }

  return(yval) ;
}


double real_interp::phase_adjust(double phase) const
{
  return( phase - 2.*Pi*floor((phase+Pi)/(2.*Pi))) ;
}

