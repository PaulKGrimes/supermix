// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// ivcurve.cc

#include "global.h"
#include "datafile.h"
#include "junction.h"
#include "error.h"
#include <cmath>
#include <cstdio>             // for snprintf()

using namespace std;

// Maximum error message string length (in characters)
static const int msglen = 2000 ;


// Constructors:

ivcurve::ivcurve() : valid(false)
{ }

ivcurve::ivcurve(const char * const Idc_filename, const char * const Ikk_filename)
{ data(Idc_filename, Ikk_filename); }

/* Will add this one later...
ivcurve::ivcurve(char *filename)
{
}
*/


// initialize from data files:

void ivcurve::data(const char * const Idc_filename, const char * const Ikk_filename)
{
  datafile Idc_data(Idc_filename), Ikk_data(Ikk_filename);
  real_table const * pdata;
  int i, max, ix, iy;

  // set up interpolators
  Idc.clear().no_extrapolation_warning(1).type(interpolator<double>::SPLINE);
  Ikk.clear().no_extrapolation_warning(1).type(interpolator<double>::SPLINE);

  // load in Idc data:

  pdata = Idc_data.table();
  ix = pdata->Lminindex();    // index to get voltage variable
  iy = pdata->Lmaxindex();    // index to get current variable
  i  = pdata->Rminindex();    // start of data
  max = pdata->Rmaxindex();   // end of data

  // Now do some data validity checks:
  if(iy - ix != 1) {
    // Then improper number of columns in Idc data file
    char errmsg[msglen];
    snprintf(errmsg, msglen,
	     "Couldn't read correct data from file: %s",
	     Idc_filename);
    error::fatal(errmsg);
  }
  if(max - i < 3) {
    // Then not enough data to deal with
    char errmsg[msglen];
    snprintf(errmsg, msglen,
	     "Not enough IV data in file: %s",
	     Idc_filename);
    error::fatal(errmsg);
  }

  while (i <= max) {
    Idc.add((*pdata)[ix][i],(*pdata)[iy][i]);
    ++i;
  }
  Idc.build();

  // load in Ikk data:

  pdata = Ikk_data.table();
  ix = pdata->Lminindex();    // index to get voltage variable
  iy = pdata->Lmaxindex();    // index to get current variable
  i  = pdata->Rminindex();    // start of data
  max = pdata->Rmaxindex();   // end of data

  // Now do Ikk data validity checks:
  if(iy - ix != 1) {
    // Then improper number of columns in Idc data file
    char errmsg[msglen];
    snprintf(errmsg, msglen,
	     "Couldn't read correct data from file: %s",
	     Ikk_filename);
    error::fatal(errmsg);
  }
  if(max - i < 3) {
    // Then not enough data to deal with
    char errmsg[msglen];
    snprintf(errmsg, msglen,
	     "Not enough IV data in file: %s",
	     Ikk_filename);
    error::fatal(errmsg);
  }

  while (i <= max) {
    Ikk.add((*pdata)[ix][i],(*pdata)[iy][i]);
    ++i;
  }
  Ikk.build();  // this build is only useful to allow access to the sorted data

  // before completing the build of the Ikk interpolator, we need to provide endpoint
  // slopes, since the endpoint conditions don't have vanishing Ikk''(V).
  // To do this we need to calculate the extrapolation parameters for
  // the Ikk data. Derivation on pp 120,121 of FR's notebook. These will also be used
  // to get the slope for the final Ikk point; see notebook pg 109.
  // Usually, the iv curve data will be normalized so that Rn = Vgap = 1, but this
  // isn't necessary.

  // To determine Io, the current offset, use the slope and value at the final Idc
  // point; we scale by 2/Pi so we don't have to later

  double x = Idc.x(Idc.size()-1);   // largest voltage in the Idc table
  Idc.val_prime(x,Io,c0);           // use c0 to temporarily hold slope

  Io = x*c0 - Io;                   // now Io has the offset of I
  Io *= 2.0/Pi;                     // scale now so we save some time later 

  // now we need to access the sorted Ikk points (that's why we had to build it)
  // We convert the last 3 points in the Ikk table to:
  // {1/v^2, Ikk(v) - Io Log(v)} == {y,f}:
  double y[3], f[3];
  for(i = 1; i <= 3; ++i) {
    double v = Ikk.x(Ikk.size() - i);
    y[i-1] = 1/(v*v);
    f[i-1] = Ikk[Ikk.size() - i] - Io*log(v);
  }

  // Now we're able to calculate c0, c2, c4:
  double M21 = (f[2]-f[1])/(y[2]-y[1]);
  double M02 = (f[0]-f[2])/(y[0]-y[2]);
  double F = (M02-M21)/(y[0]-y[1]);

  // with these definitions, the quadratic approximation is (FR notes, pg 121):
  // f[1] + (y-y[1])*(M21+F*(y-y[2])) == c0 + c2*y + c4*y*y
  c4 = F;
  c2 = M21 - F*(y[2]+y[1]);
  c0 = f[1] - y[1]*(M21-F*y[2]);

  // Finally, the slope at the final Ikk point, using the approximating fcn:
  x = (Io - 2*y[0]*(c2 + 2*c4*y[0]))/Ikk.x(Ikk.size()-1);   // use x to hold it temporarily

  Ikk.left_slope(0.0).right_slope(x).build();

  // all finished
  valid = true;
}


// various ways to fetch IV data

double ivcurve::idcinterp(double v) const
{
  return (v < 0.0) ? -Idc(-v) : Idc(v); // Idc(v) is an odd function
}

void ivcurve::idcinterpslope(double v, double & y, double & yp) const
{
  // Idc(v) is odd, Idc'(v) is even
  if (v < 0.0) {
    Idc.val_prime(-v,y,yp);
    y *= -1;
  }
  else {
    Idc.val_prime(v,y,yp);
  }
}

double ivcurve::ikkinterp(double v) const
{
  if (v < 0.0) v *= -1;   // Ikk(v) is even

  if (v < Ikk.x(0)) {
    // v is below table lower limit, slope is horizontal here
    return Ikk[0];  // just use first current value in table
  }
  else if (v > Ikk.x(Ikk.size()-1)) {
    // v is beyond table upper limit, so do nonlinear extrapolation
    // formula:  y = Io*log(v) + c0 + c2/v^2 + c4/v^4
    double y = 1/(v*v);
    return c0 + y*(c2 + y*c4) + Io*log(v);
  }
  else {
    return Ikk(v);
  }
}

void ivcurve::ikkinterpslope(double v, double & y, double & yp) const
{
  double av = fabs(v);
  // fetch results from interpolator; test if an extrapolation to the right
  if ( Ikk.val_prime(av,y,yp) > 0) {
    // use special extrapolation formulas
    double temp = 1/(av*av);
    y  = Io*log(av) + c0 + temp*(c2 + temp*c4);
    yp = (Io - 2*temp*(c2 + 2*temp*c4))/v; // note divide by v vice av; gets sign right
  }
  else {
    // correct sign of yp
    if (v < 0.0) yp *= -1;
  }
}
