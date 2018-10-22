// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ********************************************************************
// mixer.h
//
// mixer.h holds the classes used to build up a full mixer device
// from separate linear circuits and nonlinear junctions.
//
// Includes the following classes: mixer; mixer_currents
// F. Rice 7/9/98
// ********************************************************************

#ifndef MIXER_H
#define MIXER_H

#include "circuit.h"
#include "sources.h"
#include "junction.h"
#include <cmath>


// ********************************************************************
// mixer
//
// A basic device which can perform harmonic balance and small signal
// analysis of a multijunction mixer circuit.
//
// This mixer can only handle junction::Y_type nonlinear elements.
//
// The IF frequency for the small signal analysis should be set in
// device::f. The device performs the small signal analysis when asked
// to return its device parameters as an nport; the resulting sdata
// holds the small signal response of the mixer.
//
// Note that this object will never return a nonzero source vector
// in its small signal analysis result.  
//
// F. Rice 7/9/98 
//
// ********************************************************************

#include <vector>
#include <utility>
#include "newton.h"

class mixer : public data_ptr_nport
{

  // The results of the small signal mixer analysis are returned as S
  // parameters using class data_ptr_nport's get_data(), etc., functions,
  // so a mixer acts like just another circuit element.

public:

  // Constructor: it only reserves space in its internal tables for the
  // fundamental USB and LSB. Call harmonics() to include more harmonics
  // in the analysis.

  mixer();

  // Copy constructor and operator =: Since the target and source mixers will
  // use the same junction devices following these operations, you should call
  // auto_balance(mixer::Always) to ensure that the junctions will be balanced
  // appropriately. It is also recommended that you call initialize_mode(1) to
  // make harmonic balance calculations more robust.

  mixer(const mixer &);
  mixer & operator = (const mixer &);


  // Flags which indicate the readiness of the mixer to perform accurate
  // calculations. A NONZERO RETURN VALUE IS BAD; A ZERO RETURN VALUE IS GOOD.

  // a fatal error if set when attempting a mixer calculation:

  int flag_mixer_incomplete() const;   // all required elements haven't been set,
                                       // or they don't have enough ports.

  // a fatal error during small signal analysis if auto balance is inactive:

  int flag_state_invalid() const;      // at least one junction has an invalid
                                       // operating state.

  // would indicate that any small signal calculations are inaccurate:

  int flag_balance_inaccurate() const; // something major changed since the last
                                       // balance: circuit references, LO freq,
                                       // or initialize_operating_state() was done.


  // Set or read the maximum number of harmonics to be used by the mixer.

  mixer & harmonics (int);     // set maximum harmonic number (1 is LO frequency)

  int harmonics() const { return max_harmonics; }


  // Manipulate the junctions attached to the mixer. The first junction added
  // will be connected to each of the RF, IF and DC circuits' port 1; the next
  // junction to the circuits' port 2, etc. Externally available ports of the RF
  // and IF circuits are through port numbers greater than the total number of
  // junctions in the mixer. Refer to the description of port().

  mixer & add_junction( junction & );  // add another junction device.

  int junctions() const { return num_junctions; }  // junctions added so far

  mixer & void_junctions();     // remove all previously attached junctions


  // Set the Local Oscillator frequency and the linear circuit elements:

  parameter LO;                 // the public variable which holds the LO
                                // frequency for the mixer. If LO's value is
                                // currently different from the value used at the
                                // time of the last operating state calc, then
                                // flag_balance_inaccurate() will be set.

  mixer & set_LO( double f ) { LO = f; return *this; }     // set LO frequency

  mixer & set_LO( abstract_real_parameter * pf ) { LO = pf; return *this; }
                                // set LO frequency to shadow a parameter.

  mixer & set_bias( nport & );  // set the DC bias circuit. It must have the same
                                // number of ports as the number of junctions to
                                // be used.

  mixer & set_if( nport & );    // set the IF circuit. It must have more ports
                                // than the number of junctions to be used.

  mixer & set_rf( nport & );    // set the RF circuit. It must have more ports
                                // than the number of junctions to be used.
                                // Setting a new RF circuit erases any previous
                                // balance terminator settings.

  mixer & set_balance_terminator( nport &, int );  // provide a terminator for a
                                // port of the RF circuit to be used during
                                // harmonic balance. This is usually an LO power
                                // source. The int argument gives the port index
                                // on the RF circuit. The default is a device::Z0
                                // termination. Warns the user if the RF circuit
                                // hasn't been set, or the port doesn't exist.
                                // Balance terminator assignments may be cleared
                                // by setting the RF circuit using set_rf().


  // The mixer constructs a composite circuit of all linear and junction elements
  // during the small signal analysis. The following functions provide a map of
  // the ports of the individual linear elements to the resulting ports of the
  // final mixer circuit.
  // Port ordering is:
  //  (1) all open IF circuit ports
  //  (2) RF circuit at harmonic = 1: first open port at USB, then at LSB
  //  (3) all succeeding RF circuit open ports, USB then LSB at each port
  //  (4) same as (2) and (3) at harmonic = 2
  //  (5) continue for all higher harmonics

  int port( int p, int h );     // returns the mixer port index for port p of the
                                // circuit at harmonic h.  h = 0 is the IF
                                // circuit, h > 0 are upper sidebands of the RF
                                // circuit, h < 0 are lower sidebands. returns 0
                                // if the port p doesn't exist or is already
                                // connected to a nonlinear junction.

  double freq( int port );      // returns the frequency associated with the port
                                // of the mixer. If the port is associated with
                                // harmonic h, returns: fabs(device::f + h*LO)

  int size();                   // since mixer is an nport, it must calculate its
                                // size. This is the size of the small-signal
                                // object. returns 0 if the mixer is incomplete.


  // The functions to control the harmonic balance of the mixer circuit. These
  // functions control how the operating states of the junctions are set.

  mixer & initialize_operating_state();  // Just set a rough operating state for
                                // the junctions assuming the junctions behave as
                                // open circuits when calculating the input
                                // voltages. No harmonic balance iterations are
                                // performed. Will not correct
                                // flag_balance_inaccurate().

  int balance()                 // perform a harmonic balance, determining the
  { return balance_(); }        // operating states of all junctions.
                                // Returns 0 if successful, 1 if a balance could
                                // not be achieved. Will correct
                                // flag_balance_inaccurate() if successful.

  mixer & initialize_mode(int f);  // If set to 0 (default), then balance() uses
                                // the current junction operating states as the
                                // initial guess for the nonlinear routine. If
                                // set to nonzero, then balance() will first
                                // call initialize_operating_state() before
                                // executing the nonlinear routine.

  mixer & initialize_operating_state(const Matrix & V);  // Use the values in V
                                // to set the operating states of the junctions.
                                // V must have Index_C in both axes, with each
                                // row providing the state vector for
                                // call_large_signal() for its junction. The 1st
                                // junction added is addressed by row 0, etc.
                                // Uses the current LO frequency, and harmonics
                                // equal to the larger of harmonics() or
                                // V.Rmaxindex(). If V contains enough data to
                                // set all junctions, then this routine will
                                // correct flag_balance_inaccurate().

  mixer & save_operating_state(Matrix & V);  // saves the state vector of each
                                // junction into the matrix V, in a form suitable
                                // for use as an argument to
                                // initialize_operating_state().

  mixer & auto_balance(int f);  // Set auto balance mode (default 0 - Off). 
                                // If set to a value different from 0, then
                                // get_data() may attempt to balance the mixer
                                // before returning the small-signal response.
                                // See the following enumeration for valid values
                                // and their effects. If f is not 0, 1, or 2, then
                                // the auto balance mode is left unchanged.

  enum { Off   = 0,             // Turn off auto balance feature (default).
	 Always = 1,            // get_data() always rebalances the mixer. 
	 Smart  = 2             // get_data() only rebalances the mixer if
  };                            // flag_state_invalid() or flag_balance_inaccurate()


  mixer & balance_parameters(   // control parameters for the harmonic balance
			        // nonlinear solver (defaults in parentheses). The
			        // defaults should be adequate.
    int max_iterations,   // maximum number of balance iterations (100)
    double tol_1,         // max absolute error of any individual voltage (1.0e-6)
    double tol_m,         // max sum of squares of all junction voltages (1.0e-8)
    double tol_x,         // single step relative change convergence test (1.0e-7)
    double alpha          // relative convergence speed minimum value (1.0e-4)
    );


  int balance_iterations()     // the number of iterations required by the most
  { return balance_.iterations(); }  // recent harmonic balance.


  // Results of the operating state calculations. The values returned are
  // obtained by calling the junctions' I() and V() member functions.  They
  // may not be accurate if flag_state_invalid() or flag_balance_inaccurate()
  // are set.

  Vector I_junc(int m);         // the currents through the junctions at
                                // harmonic m of the LO frequency (0 is DC, 1 is
                                // the LO freq). The index into the vector
                                // selects the junction: 1 is the first junction
                                // added to the mixer, etc. m must be nonnegative.

  Vector V_junc(int m);         // like I_junc(), except return the voltages
                                // at harmonic m.


  // The normal small signal analysis results are returned by calling get_data() for
  // a mixer object. In addition, the following function is provided to aid in Y-factor
  // determinations:

  const sdata & get_term_data(); // Terminate the RF circuit using supplied balance
                                 // terminators and/or default Z0 terminations, then
                                 // perform the small signal analysis. The sdata result
                                 // will connect the open IF ports at the frequency 
                                 // given by device::f. The source vector of the result
                                 // will always be all 0's, even if there are embedded
                                 // sources in the circuits or terminators.



  // Bring in the private members and member class definitions:

#include "mixer_helper.h"

}; // mixer

// ********************************************************************
// mixer_currents
//
// mixer_currents is used to construct an equivalent nport device
// representing current sinks, one per junction in the mixer
// circuit, which can be used with an inst_circuit object (c.f.
// instrument.h) to determine the voltages or currents in the linear
// circuitry of the mixer.
//
// F. Rice 8/27/98
//
// ********************************************************************

#include "sources.h"
#include "parameter/scaled_real_parameter.h"

class mixer_currents : public data_ptr_nport
{
public:
  // the constructor requires that the number of junctions to be
  // modeled be incuded, as well as the mixer object itself:

  mixer_currents(mixer & m, int num_junctions);

  // specify the harmonic of the LO to use; the mixer_currents
  // source frequency will be set to shadow this number times the mixer
  // object's LO frequency parameter.  The default is 0 (DC bias).

  mixer_currents & set_harmonic(int h);


  // the nport interface:

  inline int size() { return n; }

private:
  mixer & mix;
  int n, h;
  std::vector <current_sink> junctions;
  scaled_real_parameter freq;
  circuit c;

  void recalc();

};

#endif /* MIXER_H */
