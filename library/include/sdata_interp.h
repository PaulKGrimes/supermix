// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ********************************************************************
// sdata_interp.h
//
// Contains classes which allow creation of nport objects which interpolate
// their S matrix and noise data. Values may be read from files of S, Y, or
// Z matrix data vs. frequency in any of a number of data file formats.
// (currently [12/99] only Touchstone format is recognized)
//
// FRR 10/7/99
// ********************************************************************

#ifndef SDATA_INTERP_H
#define SDATA_INTERP_H

#include "interpolate.h"
#include "io.h"
#include "nport.h"
#include "parameter/abstract_real_parameter.h"

// ********************************************************************
// class S_interp: interpolate S and C matrix data; not an nport device

class S_interp
{
public:

  explicit
  S_interp(int ports = 2) : N(ports), Znorm_(0.0), noise_(false) { }


  // ------------------------------------------------
  // fill the interpolators and prepare them for use:

  // Read in Touchstone-formatted data from the named file, convert to
  // S matrix data, and rebuild the interpolators. Returns false if the file
  // could not be read or interpreted with the proper number of ports. Uses
  // the provided frequency scale factor if none included in the file spec line.
  // Will also read in noise data and build a C matrix interpolation if it is a 2 port
  // and noise data is available in the touchstone file.
  bool touchstone(const char * name, double f_scale = GHz);

  // The function add_S() adds a single S matrix point to the S interpolator.
  // S must have the correct number of ports in order for it to be added.
  // Warns if S is improper. User must later call build() to have the
  // point included in the interpolation. Zn should be the normalization
  // impedance used to calculate S; S will then be renormalized if necessary.
  // Ignores Zn if it is not positive.
  S_interp & add_S(double f, const Matrix & S, double Zn = device::Z0);
  S_interp & add_S(double f, const sdata & Sd); // uses sdata's normalization

  // add_SC() adds both S and C matrix data to the interpolators
  S_interp & add_SC(double f, const Matrix & S, const Matrix & C, double Zn = device::Z0);
  S_interp & add_SC(double f, const sdata & Sd);

  // incorporate the matrices added using add_S() and add_C() into the interpolation.
  // (Note that touchstone() automatically builds the interpolations, so you need not call
  // this function if the data comes from a call to touchstone())
  S_interp & build() { if(Znorm_ != 0.0) {s.build(); if(noise_) c.build();} return *this; }


  // ------------------------------------------------
  // interpolate the matrix data. The matrices are normalized to Znorm().

  // interpolate S matrix.
  Matrix S(double f) const
    { return s(f); }

  // interpolate noise correlation (C) matrix. If no noise interpolation is available,
  // returns all zeroes.
  Matrix C(double f) const
    { return (noise_) ? c(f) : Matrix(N); }

  // fill Sd.S and Sd.C, set Sd normalization to Znorm(). If no noise interpolation is
  // available, leaves Sd.C unmodified. Returns true if noise interpolation
  // performed, false if only S matrix interpolated.
  bool fill(sdata & Sd, double f) const
    { Sd.S = s(f); if(noise_) Sd.C = c(f); Sd.set_znorm(Znorm_); return noise_; } 


  // ------------------------------------------------
  // manage and manipulate the interpolations

  // Clear out all data and start over; number of ports unchanged
  S_interp & clear() { Znorm_ = 0.0; noise_ = false; s.clear(); c.clear(); return *this; }

  // Clear out only noise data
  S_interp & clear_noise() { noise_ = false; c.clear(); return *this; }

  // Directly access the interpolators to set special options, etc.
  interpolator<Matrix> & S_interpolator() { return s; }
  interpolator<Matrix> & C_interpolator() { return c; }


  // ------------------------------------------------
  // status information:

  // return the normalization impedance for the interpolated matrix data. It will
  // equal the value of device::Z0 when the first data point was added, if any.
  // Returns 0 if no data has been added yet.
  double Znorm() const { return Znorm_ ; }

  // return the number of ports.
  int ports() const { return N ; }

  // Is S_interp ready to provide interpolated values?
  bool ready() const { return s.ready() && ( !noise_ || c.ready() ); }

  // Is noise data interpolation available?
  bool has_noise() const { return noise_ ; }
  

private:
  int N;          // number of ports
  double Znorm_;  // normalization impedance
  bool noise_;    // interpolating noise values
  interpolator<Matrix> s, c;
};


// ********************************************************************
// class sdata_interp: nport with interpolated S matrix data

class sdata_interp : public nport
{
public:

  // Call constructor with the number of ports for this object and reference to
  // a parameter to be used as the interpolation index.
  explicit
  sdata_interp(int ports = 2, const abstract_real_parameter & f = device::f);

  // Change the index parameter used for the interpolation
  sdata_interp & parameter(const abstract_real_parameter & f) { pf = &f; return *this; }

  // -------------------
  // Various status functions:

  // Returns true if the data has been filled and it's ready to go
  bool ready() const { return S.ready(); }

  // Return info about the sdata of the device.
  const data_info & get_data_info() { info.active = S.has_noise(); return info; }

  // Returns true if noise data is being interpolated. Returns false if noise
  // data comes from a passive noise calculation from the S matrix interpolation,
  // device::f, and device::T.
  bool active_noise() const { return S.has_noise(); }

  // -------------------
  // Various ways to fill the matrix data for the interpolation. Returns false
  // if there was a problem.

  // Read in Touchstone-formatted data from the named file, convert to
  // S (and, if available, C) matrix data, and rebuild the interpolation.
  // Returns false if the file could not be read or interpreted with the proper
  // number of ports. Uses the provided frequency scale factor if none included
  // in the file spec line.
  bool touchstone(const char * name, double f_scale = GHz)
    { return S.touchstone(name, f_scale); }

  // Copy the provided interpolator into the object's internal interpolator.
  // The argument must have the correct number of ports.
  bool copy(const S_interp & source);

  // Provide for direct modification of the internal S matrix interpolator.
  S_interp & interpolator() { return S ; }
  

private:
  const abstract_real_parameter * pf;   // points to the index variable
  S_interp S;                           // interpolates S matrix
  void recalc();
  void recalc_S();
};


// ********************************************************************
// Some generally useful utility functions for S matrix manipulation

// return a new S matrix with normalization impedance Znew by renormalizing
// S, which has normalization impedance Zold. S must be square, Index_1.
Matrix S_renormalize(const Matrix & S, double Znew, double Zold);

// function S_to_sdata(sdata & SD, const Matrix & S) : fill sdata using S.
// uses Zn, device::T, and device::f to set up SD. Noise comes from
// sdata::passive_noise(). SD must have the correct size for S.
void S_to_sdata(sdata & SD, const Matrix & S, double Zn = device::Z0);


#endif  /* SDATA_INTERP_H */
