// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// **************************************************************************
/**
 * @file real_parameter.h
 *
 * @author John Ward
 * @date November 3, 1997
 */
// **************************************************************************

#ifndef REAL_PARAMETER_H
#define REAL_PARAMETER_H

#include "abstract_real_parameter.h"
#include <iosfwd>

/**
 * Class real_parameter holds a double value to be used as a
 * parameter by the SuperMix program.  real_parameters can do
 * upper and lower bounds checking.  They can also "shadow" (or
 * reference) other parameter instances.
 *
 * This is the most commonly used type of parameter.
 */
class real_parameter : public abstract_real_parameter
{
public:

  /**
   * Create a parameter with value v, no shadowing, no max or min.
   * Also serves to cast a double into a real_parameter.
   *
   * @param v the initial value for this parameter
   */
  real_parameter(double v = 0.0);

  /**
   * Typecast an int to a parameter.
   * We need this to avoid confusion with casting int(0) to pointer.
   *
   * @param v the initial value for this parameter or value to typecast
   */
  real_parameter(int v);

//    // Create a clone of the argument
//    real_parameter(const real_parameter &);

  /**
   * Create a shadowed parameter, no max or min.
   *
   * @param p the parameter to be shadowed
   */
  real_parameter(const abstract_real_parameter *p);

  /**
   * Function to query whether this parameter shadows another parameter or
   * stores its value locally.
   *
   * @return true if this instance shadows another parameter instance
   */
  bool is_shadow() const { return shadowed != 0; }

  /**
   * Function to query whether this parameter shadows another parameter or
   * stores its value locally.
   *
   * @return true if this instance stores its value locally
   */
  bool is_local() const { return !(shadowed); }

  /**
   * Get the value of this parameter.
   *
   * @return the value of this parameter
   */
  double get() const;

  /**
   * Set the value of this parameter, checking max and min as necessary.
   * If value is outside of minimum or maximum bound, set to the bound.
   * If this parameter used to be a shadow, it won't be any more.
   *
   * @param v the new value for this parameter
   */
  real_parameter & set(double v)
  { shadowed = 0; value = limit(v); return *this; }

  /**
   * Set this parameter to shadow another parameter. Bounds checking, if
   * active, will be conducted during get().
   *
   * @param p the parameter to be shadowed
   */
  real_parameter & shadow(const abstract_real_parameter & p);

  /**
   * Set the minimum value for this parameter.
   * If the parameter is already outside of the bounds, reset it to the bound.
   *
   * @param m the new minimum
   */
  real_parameter & set_min(double m);

  /**
   * Set the maximum value for this parameter.
   * If the parameter is already outside of the bounds, reset it to the bound.
   *
   * @param m the new maximum
   */
  real_parameter & set_max(double m);

  /** Turn of the min if desired. */
  real_parameter & no_min() { use_min = false; min = 0.0; return *this; }

  /** Turn of the max if desired. */
  real_parameter & no_max() { use_max = false; max = 0.0; return *this; }

  /** @return the minimum bound, or 0.0 if bound limiting not active */
  double get_min() const { return min; }

  /** @return the maximum bound, or 0.0 if bound limiting not active */
  double get_max() const { return max; }

  /**
   * Assign this object a specific value using set(). Turns off any shadowing.
   *
   * @param v the new value for this parameter
   */
  real_parameter& operator =(const double v) { return set(v); }

  /**
   * Assign this object a specific value using set(). Turns off any shadowing.
   *
   * @param v the new value for this parameter
   */
  real_parameter& operator =(const int v) { return set(double(v)); }

  /**
   * Make this instance a clone of the argument.  If argument already
   * directly shadows this instance, do nothing.
   *
   * @param p the parameter to be cloned
   */
  real_parameter& clone(const real_parameter & p);

  /**
   * Make this instance a clone of the argument.  If argument already
   * directly shadows this instance, do nothing.
   *
   * @param p the parameter to be cloned
   */
  real_parameter& operator =(const real_parameter & p) { return clone(p); }

  /**
   * Assign this instance to shadow another abstract_real_parameter.
   *
   * @param p the parameter to be shadowed
   */
  real_parameter& operator =(const abstract_real_parameter * p);

private:
  /** Keep the value here if we aren't shadowing another parameter instance. */
  double value;

  /**
   * This points to the parameter instance we are shadowing.
   * If shadowed == 0, then we aren't using shadowing.
   */
  const abstract_real_parameter *shadowed;

  /**
   * We need a flag to avoid possible infinite self-referential loops
   * of shadowed parameters.
   */
  mutable bool get_flag ;

  /** Boolean for whether or not a minimum value has been set. */
  bool use_min;

  /** Minimum value to be used by "get" and "set" if use_min == true. */
  double min;

  /** Boolean for whether or not a maximum value has been set. */
  bool use_max;

  /** Maximum value to be used by "get" and "set" if use_max == true. */
  double max;

  /**
   * Limit a value by any active limits.
   *
   * @param x a value to be limited
   * @return x limited by any active limits
   */
  double limit(double x) const 
    {
      x = (use_min && x < min) ? min : x;
      return (use_max && x > max) ? max : x;
    }
};

/** Arithemtic assignment operators turn off shadowing. */
inline real_parameter & operator +=(real_parameter & p, double d)
{ return p.set(p.get() + d); }

/** Arithemtic assignment operators turn off shadowing. */
inline real_parameter & operator -=(real_parameter & p, double d)
{ return p.set(p.get() - d); }

/** Arithemtic assignment operators turn off shadowing. */
inline real_parameter & operator *=(real_parameter & p, double d)
{ return p.set(p.get() * d); }

/** Arithemtic assignment operators turn off shadowing. */
inline real_parameter & operator /=(real_parameter & p, double d)
{ return p.set(p.get() / d); }

/** Stream input operator. It turns off shadowing. */
std::istream & operator >>(std::istream & s, real_parameter & p);


#endif /* REAL_PARAMETER_H */
