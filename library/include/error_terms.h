// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ************************************************************************
// error_terms.h
//
// Concrete classes for calculating error function terms to be used with
// class error_func, in error_func.h. All are derived from abstract error
// term classes also defined in that header file. 
//
// Class:           Derived from:             Used to optimize:
//  gain_dB          error_term_mode           gain in dB
//  s_mag                  "                   S parameter magnitude
//  input_tn               "                   input noise temperature 
//  amp_k                  "                   amplifier stability "k"
//  amp_mag_delta          "                   amplifier stability "delta"
//  two_match        error_term                match two circuit S matrices
//  fts_match        scaled_match_error_term   match FTS response to data
//  iv_match         error_term                match mixer pumped IV to data
//
// 4/21/99 by John Ward
//
// Based on err_term.h written 7/22/98 by J.Z.
//
// ************************************************************************
// classes gain_dB, s_mag, and input_tn
//
// These classes are convenient for optimizing the gain, the input or
// output match, or the noise performance of a circuit. Each requires that
// an nport (circuit) be provided as the first argument in its constructor.
// Addtional arguments may be provided, but are optional. The default
// behavior of each of these classes assumes:
//
//   Input port number  =  1      change using member function in(int port)
//   Output port number =  2      change using member function out(int port)
//   Error term mode    =  MATCH  change using error_term_mode functions
//   Error term target  =  0.0    change using error_term_mode functions
//
// The error_term_mode member functions are described in file error_func.h.
//
// Here are a few examples. Note that here we only provide the circuit as the
// constructor argument. Changes to the ports and/or mode are done using
// member function calls. If you wish to use additional arguments with the
// constructor, consult the actual class declarations in this file.
//
// circuit ckt;        // this is the circuit to be optimized. Assume it is
//                     // a 2-port with input at port 1 and output at port 2.
//
// gain_dB   gain(ckt);
// gain.above(+20);    // This term generates an error if gain < 20 dB.
//
// gain_dB   flat(ckt);
// flat.flat();        // Error proportional to the variation around the mean
//                     // gain in dB.
//
// input_tn  noise(ckt);
// noise.below(4*Kelvin); // Error if input noise > 4K
//
// gain_dB   in_match(ckt);
// in_match.in(1).out(1).below(-20); // Error if input reflection > -20 dB.
//
// When concatenating member function calls using ".", always put in() and
// out() calls before calls to error_term_mode functions which set the mode
// and/or target (otherwise the compiler will complain... try it and see!).
//
// ************************************************************************
// classes amp_k and amp_mag_delta
//
// These two error term classes are used to optimize the stability of a
// 2-port amplifier design. Each requires that an nport (circuit) be
// provided as the first argument in its constructor. Addtional arguments
// may be provided, but are optional. Both use the functionality of class
// ampdata (see ampdata.h) to perform their calculations.
//
// Class amp_k compares the 2-port stability factor "K", which must be > 1
// for an unconditionally stable design, to a specified target. The default
// amp_k object assumes:
//
//   Input port number  =  1      change using member function in(int port)
//   Output port number =  2      change using member function out(int port)
//   Error term mode    =  ABOVE  change using error_term_mode functions
//   Error term target  =  3.0    change using error_term_mode functions
//
// Class amp_mag_delta compares the magnitude of the determinant of the
// 2-port scattering matrix, which must be < 1 for an unconditionally stable
// design, to a specified target. The default amp_mag_delta object assumes: 
//
//   Input port number  =  1      change using member function in(int port)
//   Output port number =  2      change using member function out(int port)
//   Error term mode    =  BELOW  change using error_term_mode functions
//   Error term target  =  0.5    change using error_term_mode functions
//
// ************************************************************************
// class two_match
//
// Class two_match simply takes the matrix difference of the scattering
// matrices returned by two nports, then totals the squared magnitudes of
// the elements of the difference. Its constructor requires two nport
// arguments.
//
// ************************************************************************
// classes fts_match and iv_match 
//
// These classes are used to compare the predictions of an SIS mixer
// model to measured FTS (Fourier Transform Spectrometer) or DC IV curve
// file data. Each has a fairly complicated constructor argument list
// which must include a mixer object (see mixer.h) one or more parameter
// objects, and a data file name, as well as additional options. When
// constructed, the error term reads in the file data and builds an
// internal real_interp (real_interp.h) interpolator of the data. When
// the error term value is calculated, it sets the operating state of
// the mixer by calling mixer::balance(), calculates the total DC bias
// current for the mixer junctions (see junction.h) and compares the
// calculation with the interpolated file data.
//
// Both classes include a public member variable named "measured", which
// is the real_interp object used to interpolate the file data. This
// interpolator is made public so that the user may adjust its behavior
// using its own member functions (see interpolate.h). If the measured
// data has a significant amount of noise, for example, it would be
// prudent to use linear rater than spline interpolation (the default).
// This interpolation mode may be set for an error term "term" by calling:
//
//   term.measured.linear().build();
//
// which rebuilds the interpolation tables for linear interpolation.
//
//
// USAGE: fts_match
//
// fts_match is derived from class scaled_match_error_term. Here's what it
// does:
//   1. At construction, read in the file data and build an interpolation
//      of it as a function of mixer LO frequency.
//   2. When reset(), set the LO power to zero, call mixer's balance() to
//      set its operating state, and get and save the total junction dark
//      current. Restore the LO power variable to its previous state.
//   3. When get() is called, set the LO power to a small value specified
//      at construction. Call the mixer's balance() and again get the total
//      junction current. Subtract off the dark current; the result is the
//      simulated response from the mixer model. Interpolate the FTS data
//      for the current LO frequency and compare the simulated and
//      interpolated values using the scaled_match_error_term algorithm
//      (see error_func.h).
//
// Here's an example of how to create an fts_match error term:
//
//   fts_match fts_term(mix, LO_f, LO_p, name, GHz, 1*Nano*Watt);
//     // mix:  the mixer object
//     // LO_f: a parameter object which contains the LO frequency
//     // LO_p: a parameter object which controls the LO power
//     // name: a char pointer to the data file name (C-style string)
//     // GHz:  a double value which gives the frequency units in the file
//     //       (in this case, the file uses GHz as its units)
//     // 1*Nano*Watt: a double value for the LO power to set when
//              simulating the FTS response with the mixer.
//
// The last two arguments are optional. If omitted, GHz and 10*Nano*Watt
// are assumed.
//
// The measured FTS data must be in a 2 column file, frequency and
// response. Units for frequency can be set with the constructor, as shown.
// The units for the response are arbitrary.
//
// The abstract_real_parameter LO_f will only be accessed using its get()
// function; it will not be written to. The parameter LO_p, on the other
// hand, will be set by the error term whenever the term's reset() or get()
// is executed. The LO_p parameter will be copied and restored to its
// original state by the the functions, however, so other parts of the
// program should not be affected.
//
// The mixer object mix will be modified whenever the term's reset() or
// get() is executed. On reset(), mix.balance() will be called with LO_p
// set to0. On get(), mix.balance() will be called with LO_p set to the LO
// power value specified at construction. Consequently, every call to the
// term's reset() or get() will change the operating state of the junctions
// used by the mixer mix.
//
// Additional member functions of fts_match:
//
//   pump(double p);
//     sets the LO power to p whenever get() is called.
//
//   f_correct(bool f);
//   f_correct();        // same as f_correct(true)
//     sets an internal flag which affects the comparison to the measured
//     data. If f is true, then apply a frequency correction to the
//     simulated response by multiplying the response value by the value of
//     the LO frequency. If f is false, then don't apply the frequency
//     correction (the default behavior). This feature is included because
//     some FTS laboratory data acquisition software applies such a
//     correction to the measured data set. If this compensation has been
//     applied to the data set in the file, then call this member function
//     before performing  error function calculations to compensate for
//     this adjustment to the data.
//
//
// USAGE: iv_match
//
// iv_match is derived from class error_term. Here's what it does:
//   1. At construction, read in the file data and build an interpolation
//      of it as a function of mixer DC bias voltage.
//   2. When get() is called, call the mixer's balance() and get the total
//      junction current. Interpolate the IV file data for the current
//      bias voltage and return the square of the difference between the
//      calculated and interpolated currents.
//
// Here's an example of how to create an iv_match error term:
//
//   iv_match iv_term(mix, V_bias, name, mVolt, Micro*Amp);
//     // mix:    the mixer object
//     // V_bias: a parameter object which contains the DC bias voltage
//     // name:  a char pointer to the data file name (C-style string)
//     // mVolt: a double value which gives the voltage units in the file
//     //       (in this case, the file uses millivolts as its units)
//     // Micro*Amp: a double value which gives the file's current units
//     //       (in this case, the file uses microamps as its units) 
//
// The last two arguments are optional. If omitted, mVolt and Micro*Amp
// are assumed.
//
// The measured IV data must be in a 2 column file, bias voltage
// and bias current.  Units for voltage and current can be set with the
// constructor. Measured data offset error corrections can be specified
// using member functions described below.
//
// The abstract_real_parameter V_bias will only be accessed using its get()
// function; it will not be written to.
//
// The mixer object mix may be modified whenever the term's get() is
// executed. On get(), mix.balance() will be called. Consequently, every
// call to the get() may change the operating state of the junctions
// used by the mixer mix.
//
// Additional member functions of iv_match:
//
//   v_offset(v);
//   i_offset(i);
//     set voltage (v_offset) or current (i_offset) offset corrections
//     to be applied to the file data. The sign convention on the offset
//     corrections is as follows: the measured data point is the true
//     data value + offset, so that:
//               I(V) == I_measured(V + V_offset) - I_offset
//     The argument may be either a double value or a pointer to an
//     abstract_real_parameter object, so that the offsets can shadow
//     external parameter objects (and thusly be controlled by the
//     optimizer). The default offsets are 0.
//
// ************************************************************************
 
#ifndef ERROR_TERMS_H
#define ERROR_TERMS_H

#include "error_func.h"
#include "nport.h"
#include "sdata.h"

// The following are required for class fts_match:
#include "mixer.h"
#include "real_interp.h"


// ************************************************************************
//
// Gain in dB
//
// ************************************************************************
class gain_dB : public error_term_mode
{
public:
  gain_dB(nport & ckt,                        // Circuit to optimize
          int input = 1,                      // Input port index
          int output = 2,                     // Output port index
	  error_term_mode::mode etm = MATCH,  // Mode
          double tar = 0.0) :                 // Target value
    error_term_mode(etm, tar),
    in_port(input),
    out_port(output),
    np(&ckt)
  { }

  gain_dB & in(int i)  { in_port = i; return *this; }
  gain_dB & out(int i) { out_port = i; return *this; }

  double get(state_tag);

private:
  int in_port, out_port ;
  nport *np;
};


// ************************************************************************
//
// magnitude of S parameter
//
// ************************************************************************
class s_mag : public error_term_mode
{
public:
  s_mag(nport & ckt,                        // Circuit to optimize
        int input = 1,                      // Input port index
        int output = 2,                     // Output port index
	error_term_mode::mode etm = MATCH,  // Mode
        double tar = 0.0) :                 // Target value
    error_term_mode(etm, tar),
    in_port(input),
    out_port(output),
    np(&ckt)
  { }

  s_mag & in(int i) { in_port = i ; return *this ; }
  s_mag & out(int i) { out_port = i ; return *this ; }

  double get(state_tag);

private:
  int in_port, out_port ;
  nport *np;

} ;


// ************************************************************************
//
// input noise temperature
//
// ************************************************************************
class input_tn : public error_term_mode
{
public:
  input_tn(nport & ckt,                        // Circuit to optimize
           int input = 1,                      // Input port index
           int output = 2,                     // Output port index
           error_term_mode::mode etm = MATCH,  // Mode
           double tar = 0.0) :                 // Target value
    error_term_mode(etm, tar),
    in_port(input),
    out_port(output),
    np(&ckt)
  { }

  input_tn & in(int i) { in_port = i ; return *this ; }
  input_tn & out(int i) { out_port = i ; return *this ; }

  double get(state_tag);

private:
  int in_port, out_port ;
  nport *np;

} ;


// ************************************************************************
//
// Amplifier "k," useful for optimizing for unconditional stability.
//
// ************************************************************************
class amp_k: public error_term_mode
{
public:
  amp_k(nport & ckt,                        // Circuit to optimize
        int input = 1,                      // Input port index
        int output = 2,                     // Output port index
        error_term_mode::mode etm = ABOVE,  // Mode should usually be ABOVE
        double tar = 3.0) :                 // Target value
    error_term_mode(etm, tar),
    in_port(input),
    out_port(output),
    np(&ckt)
  { }

  amp_k & in(int i) { in_port = i ; return *this ; }
  amp_k & out(int i) { out_port = i ; return *this ; }

  double get(state_tag);

private:
  int in_port, out_port ;
  nport *np;

} ;


// ************************************************************************
//
// Amplifier "delta," useful for optimizing for unconditional stability.
//
// ************************************************************************
class amp_mag_delta: public error_term_mode
{
public:
  amp_mag_delta(nport & ckt,                        // Circuit to optimize
                int input = 1,                      // Input port index
                int output = 2,                     // Output port index
                error_term_mode::mode etm = BELOW,  // Mode usually BELOW
                double tar = 0.5) :                 // Target value
    error_term_mode(etm, tar),
    in_port(input),
    out_port(output),
    np(&ckt)
  { }

  amp_mag_delta & in(int i) { in_port = i ; return *this ; }
  amp_mag_delta & out(int i) { out_port = i ; return *this ; }

  double get(state_tag);

private:
  int in_port, out_port ;
  nport *np;

} ;


// ************************************************************************
//
// match the S-matrices of two circuits
//
// ************************************************************************
class two_match : public error_term
{
public:
  two_match(nport &ckt1, nport &ckt2) : np1(&ckt1), np2(&ckt2) { }

  double get(state_tag);

private:
  nport *np1, *np2;

} ;


// ************************************************************************
//
// Match a simulated mixer response to a measured FTS curve.
//
// ************************************************************************
class fts_match : public scaled_match_error_term
{
public:

  // The constructor.
  // The data file must have 2 columns, frequency and measured response.
  fts_match(mixer &m,                  // The mixer to simulate.
            const abstract_real_parameter
	      &freq,                   // Parameter controlling the LO frequency.
            parameter &pow,            // Parameter controlling the LO power.
            const char*const filename, // File holding measured FTS response.
            double units=GHz,          // Units of frequency used in data file.
            double pump=10.*Nano*Watt) // Value of LO power to simulate FTS.
    :
    measured(freq, filename, units), mix(&m), f(false), LO_power(&pow),
    LO_freq(&freq), pumped_power(pump), dark_current(0), currents(0)
    { }

  // Set a different value for the pump LO power
  fts_match & pump(double p) { pumped_power = p; return *this; }

  // Set the frequency "correction" factor flag; If true, the
  // calculated response is multiplied by the LO frequency to
  // compensate for a similar factor in the measued data.
  fts_match & f_correct(bool flag = true) { f = flag; return *this; }

  // The measured curve is interpolated from the data read from the file
  // as a function of LO frequency. This is a public member variable, so
  // the user can access it to set special options, the interpolation
  // mode, etc.
  real_interp measured;

  // Reset the base class; calculate and save the dark current.
  void reset();

  // Return the measured FTS response at the LO frequency.
  double get_a(state_tag) { return measured; }

  // Return the simulated FTS response = pumped current - dark current.
  double get_b(state_tag);

private:
  mixer *mix;   // The mixer.
  bool  f;      // If true, measured response has a freq "correction" factor.

  // Setting this parameter sets the LO power in the mixer.
  parameter *LO_power;

  // This parameter tells us the LO frequency
  const abstract_real_parameter *LO_freq;

  // Value of the LO power to use for simulating the FTS response.
  double pumped_power;

  // The dark current is calculated by method reset().
  double dark_current;

  // Hold the mixer's junction DC currents
  Vector currents;
};


// ************************************************************************
//
// Match a simulated mixer IV curve to a measured IV curve
// (pumped or unpumped IV data can be matched)
//
// The error term value is the square of the difference between the
// file's bias current and the calculated bias current at the specified
// bias voltage.
//
// ************************************************************************
class iv_match : public error_term
{
public:

  // The constructor.
  // The data file must have 2 columns, frequency and measured response.
  iv_match(mixer &m,                  // The mixer to simulate.
           const abstract_real_parameter
	     &bias,                   // Parameter controlling bias voltage
	   const char *const filename,// File holding measured pumped IV.
	   double V_units=mVolt,      // Units of file's bias voltage data
	   double I_units=Micro*Amp)  // Units of file's current data
    :
    measured(bias, filename, V_units, I_units), mix(&m), V(&bias),
    V_off(0.0), I_off(0.0)
    { }

  // The measured curve is interpolated from the data read from the file
  // as a function of bias voltage. This is a public member variable, so
  // the user can access it to set special options, the interpolation
  // mode, etc.
  real_interp measured;

  // Set offset errors for the measured data. A measured value is given by
  // the actual value + the offset. Note that you can give a pointer to an
  // abstract_real_parameter so that the offsets can shadow external
  // parameters (and be determined by the optimizer). Be sure to include
  // units when providing a value!
  iv_match & v_offset(double v) { V_off = v; return *this; }  // bias voltage
  iv_match & i_offset(double i) { I_off = i; return *this; }  // bias current
  iv_match & v_offset(const abstract_real_parameter *pv)
    { V_off = pv; return *this; }
  iv_match & i_offset(const abstract_real_parameter *pi)
    { I_off = pi; return *this; }

  // Return the measured pumped IV at the bias voltage corrected for offsets
  double get_a() { return measured(V_off + *V) - I_off; }

  // Return the simulated pumped IV at the bias voltage
  double get_b();

  // Return the error term value
  double get(state_tag);

private:
  mixer *mix;   // The mixer.

  // This parameter tells us the bias voltage
  const abstract_real_parameter *V;

  // These are offset errors in the measured data, using the formula:
  //  (measured value) = (real value) + (offset)
  parameter V_off;  // measured bias voltage offset error
  parameter I_off;  // measured bias current offset error

  // Hold the mixer's junction DC currents
  Vector currents;
};

#endif /* ERROR_TERMS_H */

