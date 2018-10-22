// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ********************************************************************
/**
 * @file units.h
 *
 * units definitions for I/O conversions
 *
 * @author Frank Rice
 * @date July 31, 1997
 *
 * @see global.h
 *
 * <pre>
 *         UNITS AND CONSTANTS INCLUDED HERE AND IN global.h
 *
 *            (* master unit for standard normalization)
 *   (Standard normalization requires all wave amplitudes to be RMS.)
 *
 * Powers of Ten (to multiply any unit value):
 *    Kilo     Mega    Giga    Tera
 *    Centi    Milli   Micro   Nano   Pico   Femto 
 *
 * Voltage:                Current:               Impedance:
 *    Volt                    Amp                  * Ohm
 *  * mVolt (millivolt)       mAmp (milliamp)
 *
 * Power:                  Frequency:             Time:
 *    Watt                    Hertz                  Second
 *    uWatt (microwatt)     * GHz  (gigahertz)       nSec (nanosecond)
 *                            MHz  (megahertz)
 *
 * Length:                 Temperature:           Energy:
 *    Meter                 * Kelvin                 Joule
 *  * Micron                                         eV (electron volt)
 *    Angstrom
 *
 * Conductance:
 *    Mho
 *    Siemens
 *    mSiemens
 *
 * Capacitance and Inductance:
 *    Farad                   fFarad (femtofarad)    nHenry (nanohenry)
 *    Henry                   pFarad (picofarad)
 *
 *
 * Physical Constants:
 *    Numerical values for physical constants are taken from the "1986
 *    CODATA Recommended Values" from NIST. Ref:
 *       http://physics.nist.gov/PhysRefData/codata86/codata86.html
 *
 *    cLight      : Speed of light in vacuum
 *                  ex: wavelength = cLight / freq;
 *
 *    muVacuum    : The Permeability of free space
 *
 *    epsVacuum   : The Permittivity of free space
 *
 *    ZVacuum     : The Impedance of free space = muVacuum * cLight
 *
 *    VoltToFreq  : (e/h) convert voltage to photon frequency
 *                  ex: freq = Vgap * VoltToFreq;
 *
 *    BoltzK      : Boltzmann's Constant: convert temp to total
 *                  noise power (uWatt)
 *                  ex: noisepower = BoltzK * noisetemp * bandwidth;
 *
 *    hPlanck     : Planck's Constant
 *
 *    eCharge     : absolute value of the Electron charge
 *
 *  Please note that each of the above constants has been assigned a
 *  value independently directly from the CODATA tables.  Consequently,
 *  even though VoltToFreq is theoretically equal to eCharge/hPlanck,
 *  it is also the case that:
 *
 *   VoltToFreq * hPlanck / eCharge != 1.00000...
 *
 *  because VoltToFreq was not derived by taking the ratio (e/h), but
 *  was determined experimentally.  If it is important to your application
 *  that such expressions evaluate to 1 exactly, pick two constants and
 *  use a derived value for the third.
 *
 * ********************************************************************
 *
 *           HOW TO USE THE UNITS AND CONVERSION CONSTANTS
 *
 * The include file "global.h" specifies that all internal values for
 * physical parameters and variables be normalized so that typically
 * encountered values in SIS heterodyne dectectors will be of order 1.
 * This include file provides some useful constants and instructions to
 * help perform this conversion between internal values and real
 * physical parameters.  In addition, it should make it more clear how
 * the Master Units constants in "global.h" are to be used.
 *
 * 
 *---------------------------------------------------------------------
 *             The Meaning of the global.h Master Units
 *
 * The Master Units constant definitions in "global.h" can be simply
 * interpreted as the values which shall be stored internally to
 * represent unit amounts of the physical quantities named by the
 * constants.  For example:
 *
 * In global.h:
 *   const double  GHz = 0.01;   // gigahertz
 * implies that the stored value for 1 GHz would be 0.01:
 *   freq = 0.01;                // freq <==> 1 GigaHertz
 * Similarly, 214.868 GHz would be:
 *   freq = 2.14868;             // freq <==> 214.868 GigaHertz
 *
 * To use the GHz constant to perform the normalization:
 *   freq = 214.868*GHz;         // ==> 214.868 * 0.01 ==> 2.14868
 *
 * and you needn't recall what the actual normalization value is.
 *
 *
 *---------------------------------------------------------------------
 *         This Header File and Derived Conversion Constants
 *
 * All derived conversion constants in this file are calculated using
 * the Master Units in "global.h", which is why that file must be
 * included before this one.  By defining the constants this way,
 * changes to the normalization can be accomplished by changing the
 * values in "global.h" alone, leaving this include file unaffected.
 *
 * The currently defined Master Units from "global.h" are:
 *
 *   mVolt  : millivolt         Ohm    : ohm
 *   GHz    : gigaHertz         Micron : millionth of a meter
 *   Kelvin : degrees K
 *
 * These units form a "basis" for specifying any physical quantity
 * used in electrodynamics, since they include Charge, Mass, Length,
 * Time, and Temperature.
 *
 * The derived constants in this file can be used as described above
 * for more convenient normalizations.  Examples:
 *
 * Store a normalized volt:
 *   v = 1*Volt;  (or)   v = 1000*mVolt;  // mVolt = 1.0 ==> v = 1000.0
 *
 * Store a normalized 0.1 nanosecond:
 *   t = 0.1*nSec;  // GHz = 0.01 ==> t = 10.0, since t = 10 periods
 *                  // of a 100 GHz signal.
 *
 *
 *---------------------------------------------------------------------
 *              Changing Normalizations for Calculations
 *
 * Example:
 *
 * Assume you require an impedance to be normalized to some special
 * value for a calculation.  The special impedance value in this case
 * is, say: 50 Ohms; you must renormalize a stored impedance value to
 * its ratio with this special value.
 *
 *   const double z0 = 50*Ohm;   // Required normalization impedance,
 *                               // properly normalized itself, using the
 *                               // standard normalization constants
 *      . . .(other code). . .
 *   z /= z0;                    // Adjust z to required normalization
 *      . . .(perform required calculations). . .
 *   z *= z0;                    // Return z to standard normalization
 *
 * Note when to divide and when to multiply:
 *   Divide to convert from standard normalization to special.
 *   Multiply to convert from special to standard normalization.
 *
 * Another Note:
 *   This only works if the special normalization constant is represented
 *   in standard normalized form, as in the case above.  Simply saying:
 *      z0 = 50;   // Ohms implied
 *   would be a serious error.
 * 
 *
 *---------------------------------------------------------------------
 *             I/O Conversion and Real, Physical Values
 *
 * Conversion to and from the standard internal normalizations to real
 * physical quantities is accomplished by essentially the same process
 * as described above for special normalization.
 * 
 * Examples:
 *
 * User inputs an SIS junction gap voltage in volts:
 *    double v;
 *    cout << "Enter Gap Voltage (Volts): ";
 *    cin >> v;
 *    v *= Volt;  // normalize to internal form
 *
 * Output a complex impedance in ohms:
 *    complex z; z = <whatever>;  // a normalized impedance
 *    cout << "Complex Impedance (Ohms): " << z / Ohm << "\n";
 *
 * A useful mnemonic is to read multiplication and division by the 
 * units conversion factors as:
 *
 *   Muliplication:     vn = v  * Volts;   //  "v IN volts"
 *   Divison:            v = vn / Volts;   // "vn TO volts"
 *    
 * ********************************************************************
 */

#ifndef UNITS_H
#define UNITS_H

#include "global.h"

const double  Kilo   = 1000;
const double  Mega   = Kilo * Kilo;
const double  Giga   = Kilo * Mega;
const double  Tera   = Kilo * Giga;
const double  Centi  = 0.01;
const double  Milli  = 0.001;
const double  Micro  = Milli * Milli;
const double  Nano   = Milli * Micro;
const double  Pico   = Milli * Nano;
const double  Femto  = Milli * Pico;

const double  Volt   = Kilo * mVolt;

const double  mAmp   = mVolt / Ohm;
const double  Amp    = Kilo * mAmp;

const double  Mho    = 1 / Ohm;
const double  Siemens  = Mho;
const double  mSiemens = Milli * Siemens;

const double  uWatt  = mAmp * mVolt;
const double  Watt   = Mega * uWatt;

const double  Hertz  = Nano * GHz;
const double  MHz    = Milli * GHz;

const double  nSec   = 1/GHz;  // so 1 nS is stored internally as (1/GHz)
const double  Second = 1/Hertz; 

const double  Joule  = Watt * Second;

const double  Meter  = Mega * Micron;
const double  Angstrom = (1.0e-10) * Meter;

const double  Farad  = Second / Ohm;
const double  fFarad = Femto * Farad;
const double  pFarad = Pico * Farad;
const double  Henry  = Second * Ohm;
const double  nHenry = Nano * Henry;


// Physical Constants, in terms of the Master Units:

/**
 * To convert from normalized freq to normalized lambda:
 * <pre>
 *   lamba = cLight / freq;     // If freq normalized, so is lambda
 * </pre>
 */
const double cLight = 299792.458 * Kilo*Meter / Second;

const double muVacuum = 4 * Pi * (1.0e-7) * Ohm /(Meter/Second);

const double epsVacuum = 1/(muVacuum * cLight * cLight);

const double ZVacuum = muVacuum * cLight;

/**
 * To convert a gap voltage to a photon freq, using freq = (e/h)*Vgap:
 * <pre>
 *   freq = VoltToFreq * Vgap;  // If Vgap normalized, so is freq
 * </pre>
 */
const double VoltToFreq = 241.798836 * GHz / mVolt;

/**
 * Boltzmann's Constant normalized to standard units.
 *
 * To convert a noise temperature to spectral power density:
 * <pre>
 *   noise = BoltzK * temperature; // noise/Hertz in normalized units
 * </pre>
 *
 * To convert a noise and IF bandwidth to noise temperature:
 * <pre>
 *   noisetemp = (noisepwr / IFbandwidth) / BoltzK;
 * </pre>
 */
const double BoltzK = (1.380658e-23) * Watt / (Hertz * Kelvin);

/** Planck's constant normalized to standard units. */
const double hPlanck = (6.6260755e-34) * Joule * Second;

/** Electron charge: amp * second = coulomb. */
const double eCharge = (1.60217733e-19) * Amp * Second;

#endif /* UNITS_H */
