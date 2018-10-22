// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ************************************************************************** 
/**
 * @file complex_parameter.h
 *
 * @author John Ward
 * @date November 3, 1997
 */
// **************************************************************************

#ifndef COMPLEX_PARAMETER_H
#define COMPLEX_PARAMETER_H

#include "abstract_complex_parameter.h"
#include "abstract_real_parameter.h"
#include <iosfwd>

/**
 * Class complex_parameter holds a complex value to be used as
 * a parameter by the SuperMix program.
 */
class complex_parameter : public abstract_complex_parameter
{
public:

  /**
   * Create a parameter with value v, no shadowing.
   * Also serves to cast a complex into a complex_parameter.
   *
   * @param v the initial value for this parameter, defaults to 0
   */
  complex_parameter(Complex v = Complex(0.0));

  /**
   * Typecast an int to a complex_parameter.
   * We need this to avoid confusion with casting int(0) to pointer.
   *
   * @param v the initial value for this parameter
   */
  complex_parameter(int v);

  /**
   * Typecast a double to a complex_parameter.
   * We need this to avoid casting a double argument to int.
   *
   * @param v the initial value for this parameter or value to typecast
   */
  complex_parameter(double v);

//    // Create a clone of the argument
//    complex_parameter(const complex_parameter &) ;

  /**
   * Create a shadowed parameter.
   *
   * @param p the parameter to be shadowed
   */
  complex_parameter(const abstract_complex_parameter *p);

  /**
   * Create a shadowed parameter.  Resulting values will be real.
   *
   * @param p the parameter to be shadowed
   */
  complex_parameter(const abstract_real_parameter *p);

  enum {CARTESIAN = 1, POLAR = 2};

  /**
   * Create a complex_parameter which will shadow two real_parameters.
   *
   * @param p1 real part (CARTESIAN) or magnitude (POLAR)
   * @param p2 imag part (CARTESIAN) or radians (POLAR)
   * @param imode mode to use to interpret p1 and p2
   */
  complex_parameter(const abstract_real_parameter & p1,
		    const abstract_real_parameter & p2,
		    int imode = CARTESIAN) ;  

  /**
   * Function to query whether this parameter shadows another parameter
   * (or parameters) or stores its value locally.
   *
   * @return true if this instance shadows another parameter or parameters
   */
  inline bool is_shadow() const ;

  /**
   * Function to query whether this parameter shadows another parameter
   * (or parameters) or stores its value locally.
   *
   * @return true if this instance stores its value locally
   */
  inline bool is_local() const ;

  /**
   * Returns false if not shadowing two real_parameters.
   *
   * @return true if mode is CARTESIAN
   */
  bool is_cartesian() const { return mode == CARTESIAN; }

  /**
   * Returns false if not shadowing two real_parameters.
   *
   * @return true if mode is POLAR
   */
  bool is_polar() const { return mode == POLAR; }

  /**
   * Get the value of this parameter.
   *
   * @return the value of this parameter
   */
  Complex get() const;

  /**
   * Set the value of this parameter.
   * If this parameter used to be a shadow, it won't be any more.
   *
   * @param v the new value for this parameter
   */
  inline complex_parameter & set(Complex v);

  /**
   * Make this instance a clone of the argument (as default oper= would).
   *
   * @param p the parameter to be cloned
   */
  complex_parameter & clone(const complex_parameter & p);

  /**
   * Set this parameter to shadow another parameter
   *
   * @param p the parameter to be shadowed
   */
  complex_parameter & shadow(const abstract_complex_parameter & p);

  /**
   * Shadow a single real_parameter.  Resulting values will be real.
   *
   * @param p the parameter to be shadowed
   */
  complex_parameter & shadow(const abstract_real_parameter & p);

  /**
   * Shadow two real_parameters.
   *
   * @param p1 real part (CARTESIAN) or magnitude (POLAR)
   * @param p2 imag part (CARTESIAN) or radians (POLAR)
   * @param imode mode to use to interpret p1 and p2
   */
  complex_parameter & shadow(const abstract_real_parameter & p1,
			     const abstract_real_parameter & p2,
			     int imode = CARTESIAN);

  /**
   * Assign this object a specific value using set().
   *
   * @param v the new value for this parameter
   */
  complex_parameter & operator =(Complex v) { return set(v); }

  /**
   * Assign this object a specific value using set().
   *
   * @param v the new value for this parameter
   */
  complex_parameter & operator =(double v) { return set(v); }

  /**
   * Assign this object a specific value using set().
   *
   * @param v the new value for this parameter
   */
  complex_parameter & operator =(int v) { return set(Complex(v)); }

  /**
   * Make this instance a clone of the argument.
   *
   * @param p the parameter to be cloned
   */
  complex_parameter & operator =(const complex_parameter & p)
  { return clone(p); }

  /**
   * Assign this instance to shadow another abstract_complex_parameter.
   *
   * @param p the parameter to be shadowed
   */
  complex_parameter & operator =(const abstract_complex_parameter * p);

  /**
   * Assign this instance to shadow an abstract_real_parameter.
   *
   * @param p the parameter to be shadowed
   */
  complex_parameter & operator =(const abstract_real_parameter * p);

private:
  /** Keep the value here if we aren't shadowing another parameter instance. */
  Complex value;

  // used with CARTESIAN and POLAR
  enum {VALUE = 0, C_SHADOW = 3, R_SHADOW = 4};

  /** This tells the mode. A nonzero value implies shadowing. */
  int mode ;

  union { 
    /** A pointer to a parameter instance we may be shadowing. */
    const abstract_complex_parameter *cp;
    /** A pointer to a parameter instance we may be shadowing. */
    const abstract_real_parameter *rp1;
  };

  /** A pointer to a parameter instance we may be shadowing. */
  const abstract_real_parameter *rp2;

  /**
   * We need a flag to avoid possible infinite self-referential loops
   * of shadowed parameters.
   */
  mutable bool get_flag ;
};

// inline member function definitions

inline bool complex_parameter::is_shadow() const
{ return mode != VALUE; }

inline bool complex_parameter::is_local() const
{ return mode == VALUE; }

inline complex_parameter & complex_parameter::set(Complex v)
{ mode = VALUE; value = v; return *this; }

/** Arithemtic assignment operators turn off shadowing. */
inline complex_parameter & operator +=(complex_parameter & p, Complex d)
{ return p.set(p.get() + d); }

/** Arithemtic assignment operators turn off shadowing. */
inline complex_parameter & operator -=(complex_parameter & p, Complex d)
{ return p.set(p.get() - d); }

/** Arithemtic assignment operators turn off shadowing. */
inline complex_parameter & operator *=(complex_parameter & p, Complex d)
{ return p.set(p.get() * d); }

/** Arithemtic assignment operators turn off shadowing. */
inline complex_parameter & operator /=(complex_parameter & p, Complex d)
{ return p.set(p.get() / d); }

/** Stream input operator. It turns off shadowing. */
std::istream & operator >>(std::istream &, complex_parameter&);

#endif /* COMPLEX_PARAMETER_H */
