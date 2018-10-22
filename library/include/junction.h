// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ********************************************************************
// junction.h
//
// contains declarations for generic nonlinear device classes:
// junction; ivcurve; ckdata;
//
// F. Rice 6/29/98
// ********************************************************************

#ifndef JUNCTION_H
#define JUNCTION_H

#include "global.h"
#include "matmath.h"
#include "interpolate.h"


// ********************************************************************
// class junction
//
// junction is the abstract interface to nonlinear circuit elements
// (used by mixer). All nonlinear circuit elements should be derived
// from this class if they are to be used with a mixer object.
//
// F. Rice 6/29/98
//
// ********************************************************************

class junction
{
public:

  // junction::Y_type means the element returns currents given input
  // voltages, ie: has an admittance representation. Z_type means the
  // element returns voltages given currents, an impedance representation.

  enum { Y_type = 0, Z_type = 1 };     // define these symbolic constants

  virtual int type() const {return Y_type;}  // most junctions are of this type


  // The routine used to set the operating state of the nonlinear element.
  // The input vector is a set (indexed 0..n) of RMS signals impressed on
  // the element; the function sets whatever internal state variables it
  // maintains and then returns a set of RMS large-signal responses, also
  // indexed from 0 to n. Index 0 is the DC component, index n represents the
  // signal at n*LO_freq.

  virtual const Vector & large_signal(
		      const Vector & input,   // the vector of RMS inputs
		      double LO_freq,         // the LO frequency
		      int max_harmonics )     // the max number of harmonics
    = 0;


  // The routine that returns the small-signal response, which is a
  // linearized approximation about the operating state. It returns a
  // symmetrically-indexed square matrix representing either the small-
  // signal admittance matrix (if type()==Y_type) or impedance matrix
  // (if type()==Z_type). The client must call large_signal() before calling
  // this routine in order to set the device's operating state.

  virtual const Matrix & small_signal(
		      double IF_freq,         // the IF frequency
		      int max_harmonics )     // the max number of harmonics
    = 0;


  // The routine that returns the noise correlation matrix.  It returns a
  // symmetrically-indexed square matrix representing either the current
  // noise (type()==Y_type) or voltage noise (type()==Z_type).  The client
  // must call large_signal() before calling this routine to set the
  // device's operating state.

  virtual const Matrix & noise(
	       double IF_freq,                // the IF frequency
	       double T,                      // the temperature
	       int max_harmonics )            // the max number of harmonics
    = 0;


  // Routines to report state data for the junction, as a result of the
  // most recent execution of large_signal(). Whether V() and I() represent
  // input or output of large_signal() depends on type(). The indexing mode
  // will be Index_C, with index 0 representing DC, index n the state at
  // frequency n*LO_freq, with LO_freq being the value of that argument in
  // the most recent call to large_signal().

  virtual const Vector & V() const = 0;  // DC and RMS harmonic voltages
  virtual const Vector & I() const = 0;  // DC and RMS harmonic currents


  // This function returns a flag which is nonzero if the junction device has
  // has detected a change to its parameters which invalidate its current
  // operating state. large_signal() must be called in order for the device
  // to correctly calculate a new operating state. Note that calling either
  // small_signal() or noise() during this condition may result in a fatal
  // error.

  virtual int call_large_signal() const { return 0; }


  // Virtual destructor
  virtual ~junction() { };

}; // class junction


// ********************************************************************
// class ivcurve
//
// class ivcurve reads files with current response data and generates
// I(V), a complex DC current I from a real DC voltage V
//
// F. Rice 2/12/98
//
// ********************************************************************

class ivcurve
{
public:

  // creates an ivcurve object by reading and saving the file data for
  // the reponse to real, positive voltages. Interpolates or extrapolates using
  // techniques appropriate for  SIS I(V) curves: the imaginary part is the
  // simple, measured DC current Idc(V) (an odd function of V); the real part
  // is the Kramers-Kronig transform of that current response, Ikk(V) (an even
  // function of V).

  ivcurve(const char * const I_imaginary_filename, const char * const I_real_filename);

  ivcurve();  // just construct; wait until later to specify data files

  // here we can specify or change data files following construction:
  void data(const char * const I_imaginary_filename, const char * const I_real_filename);


  // We'll implement this one later...
  //  ivcurve(char *filename);


  // Return a current given a voltage.
  // In the case of SIS junctions, the complex current: Ikk(V) + I*Idc(V)

  Complex operator()(double V) const 
  {check(); return Complex(ikkinterp(V),idcinterp(V));}
  Complex I(double V) const
  {check(); return Complex(ikkinterp(V),idcinterp(V));}


  // Return I(V) in Y, I'(V) in Yp; for the value and the slope of the
  // I(V) curve at voltage V.
  
  void Iprime(double V, complex & Y, complex & Yp) const
  {
    check();
    ikkinterpslope(V, Y.real, Yp.real); 
    idcinterpslope(V, Y.imaginary, Yp.imaginary);
  }

private:
  bool valid;                     // Are Idc and Ikk valid?
  interpolator<double> Idc, Ikk;  // The Idc and Ikk data interpolators
  double Io, c0, c2, c4;          // Extrapolation parameters for Ikk
  double idcinterp(double v) const;  // interpolate Idc value
  double ikkinterp(double v) const;  // interpolate Ikk value
  void idcinterpslope(double, double &, double &) const;
  void ikkinterpslope(double, double &, double &) const;

  void check() const
    { if(!valid) error::fatal("No IV data provided to ivcurve object."); }

};  // class ivcurve


// ********************************************************************
// class ckdata
//
// ckdata calculates and holds Ck matrix values for an SIS
// junction; used in the mixer analysis and harmonic balance
//
// Note that if the ratio of (Voltage Amplitude)/(Photon Voltage) >> 1,
// then the size of the Ck vector will be on the order of 2.5 times the
// value of this ratio; even this will not be large enough for accurate
// convolutions, since the Ck's returned will all have values ~ 1/Sqrt(r)
// where r is the above ratio, and r >~ 10. Convolution sums will be
// accurate to better than ~ +/1 1e-6 for r = 20; above r ~ 30, the
// accuracy degrades very rapidly, to only about +/- 0.2 at a ratio of 50.
//
// This behavior may be investigated using test_ck and test_ck_prompt.
//
// F. Rice 2/12/98
// ********************************************************************

class ckdata
{
public:
  Vector Ck;  // the container of the Ck vector results
  double Tol; // the tolerance to which the Ck results are accurate
              // following calc(): Tol == ZEROTOL, #defined in global.h 

  // constructors:
  ckdata()
    : Ck(0,Index_S), Tol(0.0),
      bessel_values(0,0,Index_C,Index_C), Amj_values(0,Index_C)
  { }
  ckdata(const ckdata & old)
    : Ck(old.Ck), Tol(old.Tol),
      bessel_values(0,0,Index_C,Index_C), Amj_values(0,Index_C)
  { }

  // destructor: default destructor is fine

  // assignment operator: only copies Ck vector data and Tol
  inline ckdata & operator = (const ckdata & old)
  { (Ck = old.Ck).resize(old.Ck); Tol = old.Tol; return *this; }

  // access the vector data using array indexing:
  inline Complex & operator [] (const int i) { return Ck.get(i); }

  // calculate the Ck data:
  ckdata & calc(
		const double   fLO,  // The Large-Signal (LO) frequency
		const Vector & V     // The Large-Signal (LO) harmonic RMS voltages
		);

private:

  // holds results of bessel():
  real_table bessel_values;

  // holds results of fillA():
  Vector Amj_values;

  // determines number of bessel fcns to calculate and sizes bessel_values:
  int find_max_bessel_n(const double);

  // fills bessel_values with bessel fcns and their derivatives:
  int bessel(const double);

  // converts bessel_values to Amj coefficients, in Amj_values:
  int fillA(const Complex a);

};

#endif /* JUNCTION_H */
