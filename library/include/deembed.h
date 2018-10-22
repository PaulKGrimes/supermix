// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ********************************************************************
/**
 * @file deembed.h
 *
 * Classes related to finding the scattering parameters for a subcircuit
 * of a known nport.
 *
 * @author John Ward
 * @date September 26, 1999
 */
// ********************************************************************
 
#ifndef DEEMBED_H 
#define DEEMBED_H

#include "nport.h"

/**
 * Deembed a 2 port circuit from a larger 2 port circuit.
 * 
 * Often real devices can't be measured directly without including
 * some simple circuitry at the input and the output to facilitate
 * connection to a test apparatus.  If the input and output circuits
 * are known, class deembed can be used to deembed the tested device.
 * 
 * For example, a spiral inductor can't be measured directly.  It might
 * be wired bonded to some sort of coplanar waveguide substrate which can
 * be probed on a network analyzer.  In this case, the measured S parameters
 * would be stored in a touchstone file.  Variable "measured" would be an
 * sdata_interp object that reads the touchstone file.  Variable
 * "input" would be a class cpw object representing the coplanar waveguide
 * at the input (port 1) of the spiral, and "output" would be a class cpw
 * object representing the coplanar waveguide at the output (port 2) of
 * the spiral.  A new deembed object created with these parameters would
 * calculate the spiral inductor without the extra input and output
 * circuits.
 * 
 * <pre>
 *      +----------------------------------------------------------+
 *      |                                                          |
 *      |                                                          |
 *      |   +-----------+     +-------------+     +------------+   |
 * o----+---|           |--o--|             |--o--|            |---+----o
 *      |   |1  input  2|     |1  deembed  2|     |1  output  2|   |
 * o----+---|           |--o--|             |--o--|            |---+----o
 *      |   +-----------+     +-------------+     +------------+   |
 *      |                                                          |
 *      |                       measured                           |
 *      +----------------------------------------------------------+
 * </pre>
 * 
 * "measured" is the entire circuit, composed of the subcircuits
 * "input", "deembed", and "output".
 * 
 * Class deembed only works if all circuits are two port devices.
 * 
 * Class deembed assumes that the device is passive at device::T to
 * calculate the noise correlation matrix.  If this is not true, then
 * the noise correlation matrix will be wrong.
 */
class deembed : public nport
{
public:

  /**
   * Create a deembed instance.
   *
   * @param input the input circuit (usually a model)
   * @param output the output circuit (usually a model)
   * @param measured the entire circuit (usually measured with a network
   * analyzer)
   */
  deembed(nport & input, nport & output, nport & measured);

  /**
   * The number of ports of the deembeded circuit.
   * Class deembed only works for 2 port devices.
   *
   * @return the number of ports
   */
  int size() { return 2; }

private:
  // Keep pointers to the input, output, and measured devices

  /** The known input subcircuit. */
  nport *in;

  /** The known output subcircuit. */
  nport *out;

  /** The known entire circuit. */
  nport *meas;

  /**
   * Calculate a transmission matrix from a scattering matrix and its
   * normalization impedance.
   *
   * @param sd contains the scattering matrix to be converted
   * @return the corresponding transmission matrix
   */
  Matrix scat_to_abcd(const sdata & sd);

  /**
   * Calculate the S and C matrices, assuming for the noise calculation
   * that the device is passive at temperature device::T.
   */
  void recalc();

  /**
   * The S matrix is calculted here.
   */
  void recalc_S();
};

#endif /* DEEMBED_H */
