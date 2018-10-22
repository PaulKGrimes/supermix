// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ***************************************************************************
/**
 * @file hemt.h
 *
 * Contains models for several commercially available low-noise
 * transistors.  These models are ready to be used without
 * setting any parameters.
 *
 * @author John Ward
 * @date August 19, 1998
 */
// ***************************************************************************

#ifndef HEMT_H
#define HEMT_H

#include "fet.h"

/**
 * @class fhx13x
 *
 * Creates a Fujitsu FHX13X low-noise HEMT.  Uses parameters
 * determined by John Ward and Jacob Kooi by fitting Marian
 * Pospiezalski's model to Fujitsu's room temperature data
 * sheet.
 *
 * The drain temperature for transistors below 5 K is fixed at
 * 500 K.  The room-temperature drain temperature was
 * determined from the fit to the data sheet.  Avove 5 K, the
 * drain temperature is interpolated based on the 5 K and 297 K
 * values.
 */
class fhx13x: public fet
{
public:
  // Constructor
  fhx13x();

private:
  /** Minimum drain temperature */
  double td_min;

  /** Drain temperature for 297 Kelvin transistor */
  double td_297k;

  /**
   * Assuming drain temperature rises linearly with ambient temperature,
   * the slope of that rise from td_min at 5K.
   */
  double slope;

  // The calculation is done here.
  void recalc();
};

/**
 * @class fhr02x
 *
 * Creates a Fujitsu FHR02X low-noise HEMT.  Uses parameters
 * determined by John Ward and Jacob Kooi by fitting Marian
 * Pospiezalski's model to Fujitsu's room temperature data
 * sheet.
 *
 * The drain temperature for transistors below 5 K is fixed at
 * 500 K.  The room-temperature drain temperature was
 * determined from the fit to the data sheet.  Avove 5 K, the
 * drain temperature is interpolated based on the 5 K and 297 K
 * values.
 */
class fhr02x: public fet
{
public:
  // Constructor
  fhr02x();

private:
  /** Minimum drain temperature */
  double td_min;

  /** Drain temperature for 297 Kelvin transistor */
  double td_297k;

  /**
   * Assuming drain temperature rises linearly with ambient temperature,
   * the slope of that rise from td_min at 5K.
   */
  double slope;

  // The calculation is done here.
  void recalc();
};

/**
 * @class kukje
 *
 * Creates a Kukje KH1032-C00 low-noise HEMT.
 *
 * Model data was obtained from Dr. Sandy Weinreb at UMass.
 *
 * The drain temperature for transistors below 5 K is fixed at
 * 500 K.  Above 5 K, drain temperature is interpolated based
 * 5 K and 297 K values.
 */
class kukje: public fet
{
public:
  // Constructor
  kukje();

private:
  /** Minimum drain temperature */
  double td_min;

  /** Drain temperature for 297 Kelvin transistor */
  double td_297k;

  /**
   * Assuming drain temperature rises linearly with ambient temperature,
   * the slope of that rise from td_min at 5K.
   */
  double slope;

  // The calculation is done here.
  void recalc();
};

/**
 * @class jpltrw160
 *
 * Creates a TRW 160 x 0.1 micron chip trasistor.
 * VD = 0.40 V ; ID = 5.0 mA ; VG = -0.04 V
 * Model was obtained from Daniel Gallego.
 *
 * Model includes effects of 0.7 mil gold bond wires each approximately
 * 250 - 300 microns long as follows:  1 on the gate, 1 on the drain,
 * and 4 on the source.
 */
class jpltrw160: public fet
{
public:
  // Constructor
  jpltrw160();

  /**
   * Calling this option will cause model of bare, unbonded transistor
   * to be used.  By default, wire bonds are included (see comment, above.)
   */
  void remove_bond_wires();

private:
  /** Minimum drain temperature */
  double td_min;

  /** Drain temperature for 297 Kelvin transistor */
  double td_297k;

  /**
   * Assuming drain temperature rises linearly with ambient temperature,
   * the slope of that rise from td_min at 5K.
   */
  double slope;

  // The calculation is done here.
  void recalc();
};

/**
 * @class jpltrw300
 *
 * Creates a TRW 300 x 0.1 micron chip trasistor.
 * VD = 0.50 V ; ID = 5.0 mA ; VG = -0.431 V
 * Model was obtained from Daniel Gallego.
 *
 * Model includes effects of 0.7 mil gold bond wires each approximately
 * 250 - 300 microns long as follows:  1 on the gate, 1 on the drain,
 * and 4 on the source.
 */
class jpltrw300: public fet
{
public:
  // Constructor
  jpltrw300();

  /**
   * Calling this option will cause model of bare, unbonded transistor
   * to be used.  By default, wire bonds are included (see comment, above.)
   */
  void remove_bond_wires();

private:
  /** Minimum drain temperature */
  double td_min;

  /** Drain temperature for 297 Kelvin transistor */
  double td_297k;

  /**
   * Assuming drain temperature rises linearly with ambient temperature,
   * the slope of that rise from td_min at 5K.
   */
  double slope;

  // The calculation is done here.
  void recalc();
};

#endif /* HEMT_H */
