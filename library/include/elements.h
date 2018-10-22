// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ********************************************************************
/**
 * @file elements.h
 *
 * Holds the most basic primitive elements used by the linear
 * circuit simulator.
 *
 * @author John Ward
 * @date September 5, 1997
 */
// ***************************************************************************

#ifndef ELEMENTS_H
#define ELEMENTS_H

#include "nport.h"
#include "parameter/complex_parameter.h"

// ***************************************************************************

/**
 * @class spimp
 *
 * A superclass for 2-port simple primitive elements consisting
 * of a Series or Parallel IMPedance.
 *
 * Elements derived from class spimp have the following characteristics:
 *    @li They are 2-port circuit objects derived from class nport.
 *    @li Their behavior is determined by a single complex Z or Y.
 *
 * In addition to the member functions common to all nports, elements
 * derived from spimp have the following additional public member functions:
 * <pre>
 *    series();    // Make the object a series impedance
 *    parallel(); // Make the object a parallel impedance
 * </pre>
 *
 * At construction, the object is set to represent a series impedance.
 *
 * Here is what is meant by series and parallel 2-ports:
 *
 * <pre>
 *           series:       parallel:
 *
 *         o--XXXX--o     o----+----o
 *                             X
 *         1        2     1    X    2
 *                             X
 *         o--------o     o----+----o
 * </pre>
 *
 * Class spimp is an abstract class.  It will never be instantiated
 * directly.
 */
class spimp : public nport
{
protected:
  /**
   * Boolean value, device is series if true, parallel if false.
   * Default value is true.
   */
  bool is_series;

  /**
   * Calculate the S matrix if the impedance Z is known.
   * Called by recalc() in subclasses of spimp.
   *
   * @param Z the impedance at the current frequency
   */
  void calcZ(complex Z);

  /**
   * Calculate the S matrix if the admittance Y is known.
   * Called by recalc() in subclasses of spimp.
   *
   * @param Y the admittance at the current frequency
   */
  void calcY(complex Y);

public:
  /**
   * Default constructor creates sdata for 2 ports.
   * Boolean is_series is set to true.
   * No spimp devices have embedded sources.
   */
  spimp() : nport(2), is_series(true) { info.source = false; }

  // Virtual destructor is necessary to ensure proper subclass destruction.
  virtual ~spimp() { }

  /**
   * All spimp devices have 2 ports.
   *
   * @return the number of ports
   */
  int size() { return 2; }

  /**
   * Call this to make device be a series impedance.
   */
  spimp & series()
  { is_series = true; return *this; }

  /**
   * Call this to make device be a parallel impedance.
   */
  spimp & parallel()
  { is_series = false; return *this; }
};

// ***************************************************************************

/**
 * @class resistor
 *
 * A series or parallel 2-port resistor, derived from class spimp.
 *
 * It has two real member parameters:
 *    @li R:     resistance
 *    @li Temp:  temperature, if different from the default temperature
 *
 * Its resistance may be set during construction as well. Example:
 * <pre>
 *    resistor r(100*Ohm); // a resistor named "r" with a value of 100 Ohms
 * </pre>
 */
class resistor : public spimp
{
public:
  /**
   * Resistance in standard units.
   */
  parameter R;

  /**
   * Temperature in standard units.
   * Shadows global temperature T by default.
   */
  parameter Temp;

  /**
   * Default constructor sets temperature to shadow device::T.
   *
   * @param r the resistance in standard units.  Defaults to 0.
   */
  explicit resistor(double r = 0.0)
    : spimp(), R(r), Temp(&T) { R.set_min(0.0); }

  /**
   * Construct a shadowed parameter resistance.
   *
   * @param r pointer to the parameter to be shadowed.
   */
  explicit resistor(const abstract_real_parameter * r)
    : spimp(), R(r), Temp(&T) { R.set_min(0.0); }

  /**
   * Set the resistance
   *
   * @param r resistance in standard units.
   */
  resistor & set(double r)
  { R = r; return *this; }

  /**
   * Set the resistance to shadow a parameter
   *
   * @param r parameter of resistance in standard units.
   */
  resistor & set(abstract_real_parameter * r)
  { R = r; return *this; }

  /**
   * Set the temperature
   *
   * @param t temperature in standard units.
   */
  resistor & set_T(double t)
  { Temp = t; return *this; }

  /**
   * Set the temperature to shadow a parameter
   *
   * @param t parameter of temperature in standard units.
   */
  resistor & set_T(abstract_real_parameter * t)
  { Temp = t; return *this; }

  /**
   * Is an "active" noise source if its temperature is not the global T.
   */
  const nport::data_info & get_data_info() 
  { info.active = (Temp != device::T); return info; }

private:
  // The calculation is done here.
  void recalc()   { calcZ(Complex(R)); data.passive_noise(f, Temp); } 
  void recalc_S() { calcZ(Complex(R)); }
};

// ***************************************************************************

/**
 * @class capacitor
 *
 * A series or parallel 2-port capacitor, derived from class spimp.
 *
 * It has one real member parameter:
 *    @li C:     capacitance
 *
 * Its capacitance may be set during construction as well. Example:
 * <pre>
 *    capacitor c(200*Pico*Farad); // a capacitor named "c"
 * </pre>
 */
class capacitor : public spimp
{
public:
  /**
   * Capacitance in standard units
   */
  parameter C;

  /**
   * Constructor sets the capacitance to 0 by default.
   */
  explicit capacitor(double c = 0.0)
    : spimp(), C(c) { C.set_min(0.0); info.noise = info.active = false; }

  /**
   * Construct a shadowed parameter capacitance.
   *
   * @param c pointer to the parameter to be shadowed
   */
  explicit capacitor(const abstract_real_parameter * c)
    : spimp(), C(c) { C.set_min(0.0); info.noise = info.active = false; }

  /**
   * Set the capacitance.
   *
   * @param c capacitance in standard units.
   */
  capacitor & set(double c)
  { C = c; return *this; }

  /**
   * Set the capacitance to shadow a parameter
   *
   * @param c parameter of capacitance in standard units.
   */
  capacitor & set(abstract_real_parameter * c)
  { C = c; return *this; }

private:
  // The calculation is done here.
  void recalc() { calcY((2 * Pi * f * C) * I); }
};

// ***************************************************************************

/**
 * @class inductor
 *
 * A series or parallel 2-port inductor, derived from class spimp.
 *
 * It has one real member parameter:
 *    @li L:     inductance
 *
 * Its inductance may be set during construction as well. Example:
 * <pre>
 *    inductor m(2*Nano*Henry); // an inductor named "m"
 * </pre>
 */
class inductor : public spimp
{
public:
  /**
   * Inductance in standard units
   */
  parameter L;

  /**
   * Constructor sets the inductance to 0 by default.
   */
  explicit inductor(double i = 0.0)
    : spimp(), L(i) { L.set_min(0.0); info.noise = info.active = false; }

  /**
   * Construct a shadowed parameter inductance.
   *
   * @param i pointer to the parameter to be shadowed.
   */
  explicit inductor(const abstract_real_parameter * i)
    : spimp(), L(i) { L.set_min(0.0); info.noise = info.active = false; }

  /**
   * Set the inductance.
   *
   * @param i inductance in standard units.
   */
  inductor & set(double i)
  { L = i; return *this; }

  /**
   * Set the inductance to shadow a parameter.
   *
   * @param i parameter of inductance in standard units.
   */
  inductor & set(abstract_real_parameter * i)
  { L = i; return *this; }

private:
  // The calculation is done here.
  void recalc() { calcZ((2 * Pi * f * L) * I); }
};


/**
 * @class series_RLC
 *
 * A two port series RLC device, derived from class spimp.
 *
 * The impedance of a series_RLC is that of a series resistor,
 * capacitor, and inductor.  The order of the components is
 * mathematically irrelevant, since the impedance is the sum of the
 * individual impedances, and the scattering and noise correlation
 * matrices are functions only of the impedance.
 *
 * The resulting impedance of the series RLC combination is connected
 * either in series with or in parallel with the two ports, as
 * described for class spimp.
 *
 * series_RLC has four real member parameters:
 *    @li R:     resistance
 *    @li L:     inductance
 *    @li C:     capacitance
 *    @li Temp:  temperature, if different from the default temperature
 *
 * The RLC values may be set during construction as well. Example:
 * <pre>
 *    series_RLC a(.01*Ohm, .2*Nano*Henry, 1*Pico*Farad);
 * </pre>
 *
 * This device may be useful if you need a simple but more accurate
 * model of some real lumped circuit element.
 */
class series_RLC: public spimp
{
public:
  /** Resistance in standard units */
  parameter R;

  /** Inductance in standard units */
  parameter L;

  /** Capacitance in standard units */
  parameter C;

  /**
   * Temperature in standard units.
   * Shadows global temperature T by default.
   */
  parameter Temp;

  /**
   * Default constructor sets temperature to shadow device::T.
   *
   * @param r the resistance in standard units.  Defaults to 0.
   * @param l the inductance in standard units.  Defaults to 0.
   * @param c the capacitance in standard units.  Defaults to 0.
   */
  explicit series_RLC(double r = 0.0, double l = 0.0, double c = 0.0)
    : spimp(), R(r), L(l), C(c), Temp(&T)
      { R.set_min(0.0); L.set_min(0.0); C.set_min(0.0);
        info.noise = info.active = false; }

  /**
   * Set the resistance.
   *
   * @param r the resistance in standard units.
   */
  series_RLC & set_R(double r)
  { R = r; return *this; }

  /**
   * Set the resistance to shadow a parameter.
   *
   * @param r parameter of resistance in standard units.
   */
  series_RLC & set_R(abstract_real_parameter * r)
  { R = r; return *this; }

  /**
   * Set the inductance.
   *
   * @param l the inductance in standard units.
   */
  series_RLC & set_L(double l)
  { L = l; return *this; }

  /**
   * Set the inductance to shadow a parameter.
   *
   * @param l parameter of inductance in standard units.
   */
  series_RLC & set_L(abstract_real_parameter * l)
  { L = l; return *this; }

  /**
   * Set the capacitance.
   *
   * @param c the capacitance in standard units.
   */
  series_RLC & set_C(double c)
  { C = c; return *this; }

  /**
   * Set the capacitance to shadow a parameter.
   *
   * @param c parameter of capacitance in standard units.
   */
  series_RLC & set_C(abstract_real_parameter * c)
  { C = c; return *this; }

  /**
   * Set the temperature.
   *
   * @param t temperature in standard units.
   */
  series_RLC & set_T(double t)
  { Temp = t; return *this; }

  /**
   * Set the temperature to shadow a parameter.
   *
   * @param t parameter of temperature in standard units.
   */
  series_RLC & set_T(abstract_real_parameter * t)
  { Temp = t; return *this; }

  /**
   * Is an "active" noise source if its temperature is not the global T.
   */
  const nport::data_info & get_data_info() 
  { info.active = (Temp != device::T); return info; }

private:
  // The calculation is done here.
  void recalc_S()
  {
    if (C==0.0)
      calcY(0.0);   // Use admittance calculation since impedance is infinite.
    else
    {
      double omega = 2*Pi*f;
      calcZ(Complex(R, omega*L - 1/(omega*C)));
    }
  }

  void recalc() { recalc_S(); data.passive_noise(f, Temp); }
};

// ***************************************************************************

/**
 * @class parallel_RLC
 *
 * A two port parallel RLC device, derived from class spimp.
 *
 * The impedance of a parallel_RLC is that of a parallel resistor,
 * capacitor, and inductor.  The order of the components is
 * mathematically irrelevant, since the admittance is the sum of the
 * individual admittances, and the scattering and noise correlation
 * matrices are functions only of the admittance.
 *
 * The resulting admittance of the parallel RLC combination is connected
 * either in series with or in parallel with the two ports, as
 * described for class spimp.
 *
 * parallel_RLC has four real member parameters:
 *    @li R:     resistance
 *    @li L:     inductance
 *    @li C:     capacitance
 *    @li Temp:  temperature, if different from the default temperature
 *
 * The RLC values may be set during construction as well. Example:
 * <pre>
 *    parallel_RLC a(.01*Ohm, .2*Nano*Henry, 1*Pico*Farad);
 * </pre>
 *
 * This device may be useful if you need a simple but more accurate
 * model of some real lumped circuit element.
 */
class parallel_RLC : public spimp
{
public:
  /** Resistance in standard units. */
  parameter R;

  /** Inductance in standard units. */
  parameter L;

  /** Capacitance in standard units. */
  parameter C;

  /**
   * Temperature in standard units.
   * Shadows global temperature T by default.
   */
  parameter Temp;

  /**
   * Default constructor sets temperature to shadow device::T.
   *
   * @param r the resistance in standard units.  Defaults to 0.
   * @param l the inductance in standard units.  Defaults to 0.
   * @param c the capacitance in standard units.  Defaults to 0.
   */
  explicit parallel_RLC(double r = 0.0, double l = 0.0, double c = 0.0)
    : spimp(), R(r), L(l), C(c), Temp(&T)
      { R.set_min(0.0); L.set_min(0.0); C.set_min(0.0);
        info.noise = info.active = false; }

  /**
   * Set the resistance.
   *
   * @param r the resistance in standard units.
   */
  parallel_RLC & set_R(double r)
  { R = r; return *this; }

  /**
   * Set the resistance to shadow a parameter.
   *
   * @param r parameter of resistance in standard units.
   */
  parallel_RLC & set_R(abstract_real_parameter * r)
  { R = r; return *this; }

  /**
   * Set the inductance.
   *
   * @param l the inductance in standard units.
   */
  parallel_RLC & set_L(double l)
  { L = l; return *this; }

  /**
   * Set the inductance to shadow a parameter.
   *
   * @param l parameter of inductance in standard units.
   */
  parallel_RLC & set_L(abstract_real_parameter * l)
  { L = l; return *this; }

  /**
   * Set the capacitance.
   *
   * @param c the capacitance in standard units.
   */
  parallel_RLC & set_C(double c)
  { C = c; return *this; }

  /**
   * Set the capacitance to shadow a parameter.
   *
   * @param c parameter of capacitance in standard units.
   */
  parallel_RLC & set_C(abstract_real_parameter * c)
  { C = c; return *this; }

  /**
   * Set the temperature.
   *
   * @param t temperature in standard units.
   */
  parallel_RLC & set_T(double t)
  { Temp = t; return *this; }

  /**
   * Set the temperature to shadow a parameter.
   *
   * @param t parameter of temperature in standard units.
   */
  parallel_RLC & set_T(abstract_real_parameter * t)
  { Temp = t; return *this; }

  /**
   * Is an "active" noise source if its temperature is not the global T.
   */
  const nport::data_info & get_data_info() 
  { info.active = (Temp != device::T); return info; }

private:
  // The calculation is done here.
  void recalc_S()
  {
    if (R==0.0 || L==0.0)    // Short circuit.
      calcZ(0.0);
    else
    {
      double omega = 2*Pi*f;
      // calcZ( R*L / (L + I*(R*omega*L*C - R/omega)));
      calcY(Complex(1/R, omega*C - 1/(omega*L)));
    }
  }

  void recalc() { recalc_S(); data.passive_noise(f, Temp); }
};

// ***************************************************************************

/**
 * @class branch
 *
 * A connector used to connect several ports of other nport objects in
 * parallel.
 *
 * The default branch has 3 ports. The constructor can take a single
 * int argument to set the number of ports in the branch to some other
 * value:
 * <pre>
 *    branch b;     // default, 3-port branch named "b"
 *    branch c(4);  // this branch has 4 ports
 * </pre>
 *
 * To change the number of ports of the branch once you have declared
 * it, use a member function or assignment as follows:
 * <pre>
 *    c.set_branches(2);  // now c has only 2 ports
 *    b = branch(5);      // now branch b has 5 ports
 * </pre>
 *
 * To find out how many ports a branch has, use either of the following
 * two member functions (each returns an int):
 * <pre>
 *    size();
 *    get_branches();
 * </pre>
 */
class branch : public nport
{
public:
  /**
   * Default constructor creates a 3-port branch.
   *
   * @param b the number of branches (ports).
   */
  explicit branch(int b = 3);

  /** @return the number of ports */
  int size() { return branches; }

  /**
   * Wrapper function to change the number of branches.
   *
   * @param b the new number of branches (ports).
   */
  branch & set_branches(int);

  /** @return the number of branches (ports). */
  int get_branches() const { return branches; }

private:
  /** The number of branches in this device.  Default value is 3. */
  int branches;

  /**
   * The calculation is done here. It is called whenever the number
   * of branches is changed.
   */
  void calc();

  /** recalc() doesn't need to do anything. */
  void recalc() { }
};

// ***************************************************************************

/**
 * @class series_tee
 *
 * A 3-port connector used to connect ports in series.
 *
 * The connection scheme and port assignments are as follows:
 *
 * <pre>
 *                o--------------- 
 *              1                 |
 *                o------         |
 *                       |         -----o
 *                       |                3
 *                       |         -----o
 *                o------         |
 *              2                 |
 *                o--------------- 
 * </pre>
 *
 * When using series_tee, be aware of the arrangement of the various
 * ports; the three ports are NOT equivalent!
 */
class series_tee : public nport
{
public:
  /** Default constructor creates a 3-port series_tee. */
  series_tee();

  /**
   * All series_tee devices have 3 ports.
   *
   * @return the number of ports
   */
  int size() { return 3; }

private:
  /**
   * Since the device is independent of device::f, device::T, and device::Z0,
   * series_tee's are only calculated at construction.
   */
  void recalc() { }
};

// ***************************************************************************

/**
 * @class zterm
 *
 * A one-port terminator whose impedance is controlled by a complex
 * parameter (default 0).
 *
 * The type name "short_term" is a synonym for "zterm" (using a
 * typedef), since the default value for a zterm's impedance is 0.
 *
 * zterm has one complex parameter:
 *    @li Z:     impedance
 *
 * and one real parameter:
 *    @li Temp:  temperature, if different from the default temperature
 *
 * Its impedance may be set during construction (the default value is
 * 0). Example:
 * <pre>
 *     zterm t( (100 + I*10)*Ohm) ;  // a terminator named "t" with a complex
 *                                  // impedance of (100 + I*10)*Ohm
 * </pre>
 *
 * Its impedance may be set to shadow another complex parameter type
 * as well at construction:
 * <pre>
 *    complex_parameter Imped;
 *    zterm t(&Imped);              // now t's impedance will shadow Imped
 * </pre>
 */
class zterm : public nport
{
public:
  /** Impedance in standard units (default is 0) */
  complex_parameter Z;

  /** Temperature in standard units (shadows device::T by default) */
  parameter Temp;

  /**
   * The default constructor sets the impedance to 0 by default.
   *
   * @param z the impedance of the terminator
   */
  explicit zterm(complex z = 0.0) : nport(1), Z(z), Temp(&T)
  { info.source = false; }

  /**
   * Construct a shadowed parameter impedance.
   *
   * @param z pointer to the parameter to be shadowed.
   */
  explicit zterm(abstract_complex_parameter * z) : nport(1), Z(z), Temp(&T)
  { info.source = false; }

  /**
   * Set the impedance.
   *
   * @param z impedance in standard units.
   */
  zterm & set(complex z)
  { Z = z; return *this; }

  /**
   * Set the impedance to shadow a parameter.
   *
   * @param z parameter of impedance in standard units.
   */
  zterm & set(abstract_complex_parameter * z)
  { Z = z; return *this; }

  /**
   * Set the temperature.
   *
   * @param t temperature in standard units.
   */
  zterm & set_T(double t)
  { Temp = t; return *this; }

  /**
   * Set the temperature to shadow a parameter.
   *
   * @param t parameter of temperature in standard units.
   */
  zterm & set_T(abstract_real_parameter * t)
  { Temp = t; return *this; }

  /**
   * Is an "active" noise source if its temperature is not the global T.
   */
  const nport::data_info & get_data_info() 
  { info.active = (Temp != device::T && Z != Complex(0.0)); return info; }

private:
  // The calculation is done here.
  void recalc();
  void recalc_S();
};

/**
 * Shorted terminator (0 impedance)
 */
typedef zterm short_term;

// ***************************************************************************

/**
 * @class yterm
 *
 * A one-port terminator whose admittance is controlled by a complex
 * parameter (default 0).
 *
 * The type name "open_term" is a synonym for "yterm" (using a
 * typedef), since the default value for a yterm's admittance is 0.
 *
 * yterm has one complex parameter:
 *    @li Y:     admittance
 *
 * and one real parameter:
 *    @li Temp:  temperature, if different from the default temperature
 *
 * Its admittance may be set during construction (the default value is
 * 0). Example:
 * <pre>
 *    yterm t( (100 + I*10)*Mho) ;  // a terminator named "t" with a complex
 *                                  // admittance of (100 + I*10)*Mho
 * </pre>
 *
 * Its admittance may be set to shadow another complex parameter type
 * as well at construction:
 * <pre>
 *    complex_parameter Admitt;
 *    yterm t(&Admitt);              // now t's admittance will shadow Admitt
 * </pre>
 */
class yterm : public nport
{
public:
  /** Admittance in standard units (default is 0). */
  complex_parameter Y;

  /** Temperature in standard units (shadows device::T by default) */
  parameter Temp;

  /**
   * The default constructor sets the admittance to 0 by default.
   *
   * @param y the admittance of the terminator
   */
  explicit yterm(complex y = 0.0) : nport(1), Y(y), Temp(&T)
  { info.source = false; }

  /**
   * Construct a shadowed parameter admittance.
   *
   * @param z pointer to the parameter to be shadowed.
   */
  explicit yterm(abstract_complex_parameter * y) : nport(1), Y(y), Temp(&T)
  { info.source = false; }

  /**
   * Set the admittance.
   *
   * @param y admittance in standard units.
   */
  yterm & set(complex y)
  { Y = y; return *this; }

  /**
   * Set the admittance to shadow a parameter
   *
   * @param y parameter of admittance in standard units.
   */
  yterm & set(abstract_complex_parameter * y)
  { Y = y; return *this; }

  /**
   * Set the temperature.
   *
   * @param t temperature in standard units.
   */
  yterm & set_T(double t)
  { Temp = t; return *this; }

  /**
   * Set the temperature to shadow a parameter.
   *
   * @param t parameter of temperature in standard units.
   */
  yterm & set_T(abstract_real_parameter * t)
  { Temp = t; return *this; }

  /**
   * Is an "active" noise source if its temperature is not the global T.
   */
  const nport::data_info & get_data_info() 
  { info.active = (Temp != device::T && Y != Complex(0.0)); return info; }

private:
  // The calculation is done here.
  void recalc();
  void recalc_S();
};

/**
 * Open circuit terminator (0 admittance).
 */
typedef yterm open_term;

#endif /* ELEMENTS_H */
