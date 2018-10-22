// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// **************************************************************************
/**
 * @file fet.h
 *
 * Modeling elements for field effect transistors.
 *
 * For prebuilt models of commercially available transistors
 * ready to be used in circuits, see the file hemt.h.
 *
 * @author John Ward
 * @date August 19, 1998
 */
// **************************************************************************

#ifndef FET_H
#define FET_H

#include "nport.h"
#include "circuit.h"
#include "elements.h"

/**
 * Create an ideal transconductance, primarily useful for modeling FET's.
 */
class transconductance: public nport
{
public:
  /**
   * Transconductance (usually given in mSiemen)
   */
  parameter G;

  /**
   * Fet transit time (usually given in Pico*Second)
   * Applied by multiplying exp(-I*2*Pi*f*Tau) by transconductance G.
   * Note that this is the sign convention used by MMICAD, which may
   * not be the same as that used by pcircuit.
   */
  parameter Tau;

  /**
   * The constructor sets G and Tau to 0.
   */
  transconductance() : nport(2), G(0.0), Tau(0.0)
    { info.source = false; }  // Transconductances are sourceless.

  /**
   * Transconductances are 2-port devices.
   *
   * @return the number of ports
   */
  int size() { return 2; }
  

private:
  // The calculation is done here.
  void recalc();
};

/**
 * Creates a field effect transistor using the Merian
 * Pospieszalski noise model.
 *
 * Note that this fet class is useless until the parameters are
 * set.  For some pre-built transistors which are ready to use,
 * see the file hemt.h.
 *
 * Fets are modelled as 2-ports in a common-source config.
 * Here're the port assignments:
 *
 * <pre>
 *                 d
 *            g  |----------o 2
 *    1 o--------|
 *               |---.
 *                 s |
 *                   |
 *      o------------+------o
 *
 * </pre>
 */
class fet: public data_ptr_nport
{
public:
  // Circuit elements in the model.  At this time, circuit elements
  // are public so that users can set their values for a particular fet.
  // Perhaps there is a fancier way to do it, but this method has the
  // advantage of being simple, and it works.
  inductor Lg;
  inductor Ld;
  inductor Ls;
  resistor Rg;
  resistor Rd;
  resistor Rs;
  resistor Rgs;
  resistor Rds;
  capacitor Cpg;
  capacitor Cpd;
  capacitor Cgs;
  capacitor Cds;
  capacitor Cgd;
  transconductance Gm;

  /**
   * The constructor assembles the circuit and holds it in the private
   * member variable fetckt.  It does not set the parameters for the
   * primitive elements, which must be done before the fet can be used.
   */
  fet();

  /**
   * Copy constructor.
   *
   * @param f the fet to be copied
   */
  fet(const fet & f);

  /**
   * Assignment operator.
   *
   * @param f the fet to be copied
   */
  fet & operator=(const fet & f);

  /**
   * FET's are 2-port devices.
   *
   * @return the number of ports
   */
  int size() { return 2; }

protected:
  /**
   * The circuit which holds the built fet.
   */
  circuit fetckt;

private:
  // Some connector elements used to build the circuit.
  branch b1, b2;
  series_tee st3, st4, st5, st6;

  /**
   * Helper function to assemble fetckt.
   * Used by constructor and copy constructor.
   */
  void construct();

  /**
   * Helper function for copying 
   */
  void fet_copy(const fet & f);

  // The calculation is done here.
  virtual void recalc()     { data_ptr = &fetckt.get_data(); }
  virtual void recalc_S()   { data_ptr = &fetckt.get_data_S(); }
};

#endif /* FET_H */
