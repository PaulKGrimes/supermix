// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ********************************************************************
// sisdevice.h
//
// Declarations for the sis devices and associated classes
//
// Includes the following class:
// sis_basic_device
// sis_device (synonym for sis_basic_device)
//
// F. Rice 6/22/98
// ********************************************************************

#ifndef SISDEV_H
#define SISDEV_H

#include "global.h"
#include "junction.h"
#include "parameter.h"


// ********************************************************************
// class sis_basic_device (also sis_device)
//
// sis_basic_device is a junction of type Y_type.
// It has the SIS junction's gap voltage, normal resistance, and
// capacitance as explicit values set by the client.
//
// F. Rice 6/30/98
// ********************************************************************

class sis_basic_device : public junction
{
public:

  // The routine used to set the operating state of the sis junction.
  // The input vector is a set (indexed 0..n) of RMS voltages impressed
  // on the junction; the function returns a set of large-signal
  // currents. Index 0 is the DC component, index n represents the
  // signal at n*LO_freq.

  const Vector & large_signal(
		      const Vector & V,       // the vector of input voltages
		      double LO_freq,         // the LO frequency
		      int max_harmonics )     // the max number of harmonics
    ;


  // The routine that returns the small-signal response. It returns a
  // symmetrically-indexed square matrix representing the small-signal
  // admittance matrix. The client must call large_signal() before calling
  // this routine; it uses the large signal voltages and LO frequency
  // arguments to the most recently executed call of large_signal().

  const Matrix & small_signal(
		      double IF_freq,         // the IF frequency
		      int max_harmonics )     // the max number of harmonics
    ;


  // The routine that returns the noise correlation matrix.  It returns a
  // symmetrically-indexed square matrix representing the current noise. The
  // client must call large_signal() before calling this routine; it uses the
  // large signal voltages and LO frequency arguments to the most recently
  // executed call of large_signal().

  const Matrix & noise(
	       double IF_freq,                // the IF frequency
	       double T,                      // the temperature
	       int max_harmonics )            // the max number of harmonics
    ;


  // This function returns a flag which is nonzero if large_signal() has
  // never been called, or has not been called since one of the junction
  // parameters below has been changed.

  int call_large_signal() const;


  // The characteristics of this junction.

  sis_basic_device & set_iv(const ivcurve & iv)
  { piv = &iv; iv_data_ok = 0; return *this; }

  parameter Vn, Rn;  // the normalizing voltage and resistance used in the ivcurve

  parameter Cap;     // the junction capacitance


  // Routines to report state data for the sis junction.
  // Note that Ck() values are in symmetric-harmonic form (-k..0..k):

  const Vector & V() const  { return Voltages; }
  const Vector & I() const  { return Currents; }
  const Vector & Ck() const   { return C.Ck; }
  double alpha() const
    { return (LO_freq != 0.0) ? VoltToFreq*RmsToPeak*abs(Voltages.read(1))/LO_freq : 0.0; }

  // The constructor sets up the member variables to well-defined values
  sis_basic_device();

private:
  const ivcurve *piv;
  double Vn_, Rn_, Cap_;         // the parameter values used by large_signal
  int iv_data_ok;                // a flag used by call_large_signal()
  Vector Voltages, Currents;     // filled by large_signal()
  ckdata C;                      // filled by large_signal()
  Vector I_VLO, I_VLO_prime,     // temporary values looked up from the ivcurve
    I_pVIF, I_mVIF;
  Matrix Y, H;                   // filled by small_signal() and noise()
  double LO_freq, IF_freq;       // the values used to calculate I_VLO, etc.
};

typedef sis_basic_device sis_device;

#endif /* SISDEV_H */
