// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ********************************************************************
/**
 * @file antenna.h
 *
 * Contains antenna and antenna-related objects for constructing
 * quasioptical receiver models.
 *
 * @author Frank Rice
 * @date 11/30/99
 */
// ********************************************************************

#ifndef ANTENNA_H
#define ANTENNA_H


#include "circuit.h"
#include "elements.h"
#include "transformer.h"

// ********************************************************************

/**
 * @class slot_antenna
 *
 * A 3-%port single slot antenna with a specified impedance.
 *
 * Port 1 is for the RF input signal. Port 2 is the
 * output %port which is out of phase with the RF signal. Port 3 is the
 * output %port which is in phase with the RF antenna input signal.
 *
 * The parameter Z should be set to the antenna impedance. If the input
 * at %port 1 is driven by a source at the normalizing impedance
 * device::Z0, then the antenna presents an impedance of Z to the rest
 * of the circuit.
 *
 * Here is an electrical diagram of the slot antenna model:
 *
 * <pre>
 * .                      -    +
 *                        o  1 o
 *                        |    |
 *                        |    |
 *                        \/\/\/  device::Z0
 *                     transformer
 *                        /\/\/\  Z
 *                        |    |
 * .              - o-----+    +-----o +
 *                 2                  3
 *                + o----------------o -
 * </pre>
 */

class slot_antenna : public nport
{
public:
  /** The antenna impedance.  Default is device::Z0. */
  complex_parameter Z;

  /**
   * The constructor sets Z to shadow the argument p.
   * @param p a real impedance
   */
  slot_antenna(const abstract_real_parameter & p = device::Z0)
    : nport(3), Z(&p)
    { info.noise = info.active = info.source = false; }

  /**
   * The constructor sets Z to shadow the argument p.
   * @param p a complex impedance
   */
  slot_antenna(const abstract_complex_parameter & p)
    : nport(3), Z(&p)
    { info.noise = info.active = info.source = false; }

private:
  // The calculation is done here.
  virtual void recalc();

};


// ********************************************************************

/**
 * @class twin_slot_antenna
 *
 * A 5-%port which represents the coupling of symmetric-excitation RF
 * into a pair of slot antennas. The %port assignments are:
 *
 *   Port 1:   RF input signal
 *   Port 2:   Antenna 1 out-of-phase output
 *   Port 3:   Antenna 1 in-phase output
 *   Port 4:   Antenna 2 out-of-phase output
 *   Port 5:   Antenna 2 in-phase output
 *
 * A twin_slot_antenna object holds a pointer to a 2-%port which provides
 * the antenna impedance and coupling. If the pointer is assigned to
 * point to a 1-%port object, then it is assumed that the coupling between
 * the antennas is to be ignored and the two antennas have identical
 * behaviors as provided by the 1-%port response. The RF source should
 * have an impedance of device::Z0 in order for the antenna impedances to
 * be correctly presented to the circuit.
 *
 * As currently implemented, the 2-%port antenna object provided to
 * twin_slot_antenna should be reciprocal and symmetric, so S11 == S22 and
 * S12 == S21. In any event, twin_slot_antenna will only use S11 and S12
 * from the supplied nport. 
 *
 * The twin_slot_antenna object has a passive thermal noise which
 * represents antisymmetric-mode excitation by a device::Z0 resistor
 * at a default temperature of device::T. 
 *
 * Here is an electrical diagram of the twin-slot antenna model:
 *
 * <pre>
 *                         +--------+                                   
 *                         |        |----o -
 *                         |       2|
 *                         |        |----o +
 *                  + o----|        |              Antenna              
 *               RF        |1       |                 1                 
 * .                - o----|        |----o +
 *                         |       3|
 *                         |        |----o -
 *                         |        |                                   
 *                         |        |                                   
 *              antisymm.  |        |                                   
 *              mode input |        |
 *                         |        |----o -
 *              +-/\/\/\/--|       4|
 *              |          |        |----o +
 *              +----------|        |              Antenna              
 *                         |        |                 2                 
 *                         |        |----o +
 *                         |       5|
 *                         |        |----o -
 *                         +--------+                                   
 * </pre>
 */

class twin_slot_antenna : public nport
{
public:

  /** Points to a 1- or 2-%port giving antenna response. */
  nport *pA;

  /** Temperature of antisymmetric thermal source. device::T is default. */
  parameter Temp;

  /** Default constructor. */
  twin_slot_antenna();

  /**
   * Constructor to set the antenna response nport.
   * @param r a 1- or 2-%port %device giving the antenna response.
   */
  twin_slot_antenna(nport & r);

  /**
   * Set the antenna response nport.
   * @param r a 1- or 2-%port %device giving the antenna response.
   */
  twin_slot_antenna & set(nport & r);

  /**
   * Set the temperature.
   * @param t the temperature
   */
  twin_slot_antenna & set_T(double t)
  { Temp = t; return *this; }

  /**
   * Set the temperature to shadow a parameter.
   * @param t the temperature parameter to be shadowed.
   */
  twin_slot_antenna & set_T(abstract_real_parameter * t)
  { Temp = t; return *this; }

  /**
   * Size is 0 if nport is not assigned and 5 otherwise.
   * @return the number of %ports
   */
  int size() { return (pA) ? 5 : 0; }

  // is an "active" noise source if its temperature is not the global T
  const nport::data_info & get_data_info() 
  { info.active = (Temp != device::T); return info; }

private:
  void recalc()   { recalc_S(); data.passive_noise(f, Temp); }
  void recalc_S();

  /** Component needed to calculate the antenna response. */
  simple_nport a;

  /** Component needed to calculate the antenna response. */
  series_tee t1,t2;

  /** Component needed to calculate the antenna response. */
  circuit c;
  
};

// ********************************************************************

/**
 * @class power_divider
 *
 * A 3-%port device to divide an incoming signal into 2 equal,
 * in-phase power signals.  It is used to feed a single signal
 * into two slot_antenna objects.
 *
 * This device is not noiseless; it effectively has an antisymmetric
 * exitation %port terminated at the normalizing impedance device::Z0,
 * which is a source of thermal noise. Its temperature (Temp) should be
 * set to the effective temperature of the surrounding dewar.
 */

class power_divider : public nport
{
public:
  /**
   * Temperature in standard units.
   * Shadows global temperature T by default.
   */
  parameter Temp;

  /** Constructor calculates the scattering matrix. */
  power_divider() : nport(3), Temp(&device::T)
  {
    info.source = false;
    const double s = 1.0/RmsToPeak;
    data.S[1][2] = data.S[2][1] = s;
    data.S[1][3] = data.S[3][1] = s;
  }

  /**
   * Set the temperature.
   * @param the new temperature in standard units.
   */
  power_divider & set_T(double t)
  { Temp = t; return *this; }

  /**
   * Set the temperature to shadow a parameter.
   * @param the new temperature in standard units.
   */
  power_divider & set_T(abstract_real_parameter * t)
  { Temp = t; return *this; }

  // is an "active" noise source if its temperature is not the global T
  const nport::data_info & get_data_info() 
  { info.active = (Temp != device::T); return info; }

private:
  // recalc()'s needn't do anything to the S matrix.
  void recalc()   { data.passive_noise(f, Temp); }
  void recalc_S() { }
};

#endif /* ANTENNA_H */
