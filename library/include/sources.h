// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// **************************************************************************
/**
 * @file sources.h
 *
 * This file contains circuit elements that generate signals.  In
 * other words, their source vectors sdata::B are typically not zero.
 *
 * @author John Ward
 * @date September 11, 1997
 */
// **************************************************************************

#ifndef SOURCES_H
#define SOURCES_H

#include "nport.h"

// **************************************************************************

/**
 * @class generator
 *
 * A 1-port resistor (terminator) that can be a power source.
 * The source frequency is held in source_f, source power in
 * source_power, and the frequency width of the source is held
 * in source_width. The power in source_power represents the power
 * that would be delivered to a matched load resistance R at frequency
 * source_f.
 *
 * The default is to create an ideal power source, with perfect
 * normalized impedance match R = Z0. Only a real resistance R
 * can be specified for this device; if a complex impedance is
 * required, use an ideal source and couple it to the circuit
 * through a 2-port transformer with the desired matching
 * impedance.
 *
 * If the global frequency differs from source_f by more than
 * source_width/2, then the source vector sdata::B will be set
 * to 0.
 *
 * @see voltage_source
 * @see current_sink
 */
class generator : public nport
{
public:
  /** Resistance in standard units.  Shadows device::Z0 by default. */
  parameter R;

  /**
   * Temperature in standard units.
   * Shadows global temperature T by default.
   */
  parameter Temp;

  /** Source frequency in standard units.  Default is 0. */
  parameter source_f;

  /** Source frequency width in standard units.  Default is 0. */
  parameter source_width;

  /** Source power in standard units.  Default is 0. */
  parameter source_power;

  /** Source phase in standard units (Radians).  Default is 0. */
  parameter source_phase;

  /** Constructor */
  generator();

  /**
   * Generators are 1-port devices.
   *
   * @return the number of ports
   */
  int size() { return 1; }

  // is an "active" noise source if its temperature is not the global T
  // is a wave source if source_power is nonzero
  const nport::data_info & get_data_info(); 

private:
  // The calculation is done here.
  void recalc();
  void recalc_S();
};

// **************************************************************************

/**
 * @class voltage_source
 *
 * A 1-port resistor (terminator) that can be a power source.
 * For this device, a source voltage is
 * specified in source_voltage and source_phase.
 *
 * The default is to create an ideal voltage source, with
 * R = 0.  Only a real resistance R can be specified for this
 * device; if a complex impedance is required, use an ideal
 * source and couple it to the circuit through a 2-port with
 * the desired series impedance.
 *
 * The source frequency is held in source_f, and the frequency
 * width of the source is held in source_width.
 *
 * If the global frequency differs from source_f by more than
 * source_width/2, then the source vector sdata::B will be set
 * to 0.
 *
 * @see generator
 * @see current_sink
 */
class voltage_source : public nport
{
public:
  /** Resistance in standard units.  Shadows device::Z0 by default. */
  parameter R;

  /**
   * Temperature in standard units.
   * Shadows global temperature T by default.
   */
  parameter Temp;

  /** Source frequency in standard units.  Default is 0. */
  parameter source_f;

  /** Source frequency width in standard units.  Default is 0. */
  parameter source_width;

  /** Source voltage in standard units.  Default is 0. */
  parameter source_voltage;

  /** Source phase in standard units (Radians).  Default is 0. */
  parameter source_phase;

  /** Constructor */
  voltage_source();

  /**
   * Voltage sources are 1-port devices.
   *
   * @return the number of ports
   */
  int size() { return 1; }

  // is an "active" noise source if its temperature is not the global T
  // is a wave source if source_power is nonzero
  const nport::data_info & get_data_info(); 

private:
  // The calculation is done here.
  void recalc();
  void recalc_S();
};

// **************************************************************************

/**
 * @class current_sink
 *
 * A 1-port resistor (terminator) that can be a power source.
 * Note that unlike generator, this device is a SINK
 * of current; for a SOURCE, the phase must be adjusted by
 * 180 Degrees.
 *
 * For this device, the sink current is specified in
 * sink_current and sink_phase.
 *
 * The default is to create an ideal current sink, with
 * Y = 0.  Only a real admittance Y can be specified for this
 * device; if a complex admittance is required, use an ideal
 * sink and couple it to the circuit through a 2-port with
 * the desired parallel admittance.
 *
 * The source frequency is held in source_f, and the frequency
 * width of the source is held in source_width.
 *
 * If the global frequency differs from source_f by more than
 * source_width/2, then the source vector sdata::B will be set
 * to 0.
 *
 * @see voltage_source
 * @see generator
 */
class current_sink : public nport
{
public:
  /** Admittance in standard units (1/Ohm).  Default is 0. */
  parameter Y;

  /**
   * Temperature in standard units.
   * Shadows global temperature T by default.
   */
  parameter Temp;

  /** Source frequency in standard units.  Default is 0. */
  parameter source_f;

  /** Source frequency width in standard units.  Default is 0. */
  parameter source_width;

  /** Sink current in standard units.  Default is 0. */
  parameter sink_current;

  /** Source phase in standard units (Radians).  Default is 0. */
  parameter sink_phase;

  /** Constructor */
  current_sink();

  /**
   * Generators are 1-port devices.
   *
   * @return the number of ports
   */
  int size() { return 1; }

  // is an "active" noise source if its temperature is not the global T
  // is a wave source if source_power is nonzero
  const nport::data_info & get_data_info(); 

  // The calculation is done here.
  void recalc();
  void recalc_S();
};

#endif /* SOURCES_H */
