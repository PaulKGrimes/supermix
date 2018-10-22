// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// **************************************************************************
/**
 * @file sdata.h
 *
 * @author Jonas Zmuidzinas
 * @author John Ward
 * @author Frank Rice
 * @date November 5, 1997
 */
// **************************************************************************

#ifndef SDATA_H
#define SDATA_H

#include "global.h"
#include "device.h"

class sdata ;
class zdata ;
class ydata ;
class ydata_ptr ;

// **************************************************************************

/**
 * @class sdata
 *
 * sdata holds linear circuit data, including scattering
 * matrix, noise correlation matrix, and source vector.
 */
class sdata
{
protected:
  /** The characteristic impedance that was used. */
  double z_norm;

public:
  /** The scattering matrix. */
  Matrix S;

  /** The noise correlation matrix. */
  Matrix C;

  /** The source vector, in square root of power. */
  Vector B;

  /** @return the number of ports and thus size of the matrices */
  int size() const
  { return S.Lmaxindex(); }

  v_index_mode mode() const
  { return S.Lmode; }

  // convert sdata values to "engineering" forms:

  /**
   * Get a scattering parameter in decibels.
   *
   * @param out the output port (row in the S matrix)
   * @param in the input port (column in the S matrix)
   * @return S parameter in dB for in -> out
   */
  double SdB(int out, int in) const;

  /**
   * Get the noise temperature in standard units.
   *
   * @param out the output port (row in the S matrix)
   * @param in the input port (column in the S matrix)
   * @return the noise temperature referred to the input
   */
  double tn(int out, int in) const;

  /**
   * Get the noise figure in decibels.
   *
   * @param out the output port
   * @param in the input port
   * @return the noise figure referred to the input in decibels
   */
  double NF(int out, int in) const;

  /**
   * Construct data for a device with a known number of ports and index mode.
   *
   * @param n the number of ports, defaults to 0
   */
  explicit sdata(int n = 0, v_index_mode = Index_1);

  /** Copy constructor */
  sdata(const sdata & sd) ;

  /**
   * Copy constructor, converting data to a new normalization impedance.
   *
   * @param sd data to be copied
   * @param z0 the new normalization impedance, z0 > 0
   */
  sdata(const sdata & sd, double z0) ;

  // "conversion" constructors - to sdata from zdata, ydata,  or ydata_ptr

  /**
   * Convert data from the impedance representation to the wave representation.
   */
  sdata(const zdata & zd, double z0 = double(device::Z0)) ;

  /**
   * Convert data from the admittance representation to the wave
   * representation.
   */
  sdata(const ydata & yd, double z0 = double(device::Z0)) ;

  /**
   * Convert data from the admittance representation to the wave
   * representation.
   */
  sdata(const ydata_ptr &, double z0 = double(device::Z0));

  /**
   * Change the size of an existing object.
   */
  sdata & resize(int);

  /**
   * Change the indexing of an existing object.
   */
  sdata & reindex(v_index_mode m = Index_1)
  { S.reindex(m,m); C.reindex(m,m); B.reindex(m); return *this; }

  /**
   * Get the normalization impedance.
   *
   * @return the normalization impedance
   */
  double get_znorm() const { return z_norm; }

  /**
   * Set the normalization impedance.
   * Must have z>=0 (z==0 is special; see change_norm)
   *
   * @param z the new normalization impedance.
   */
  sdata & set_znorm(double z) ;

  /**
   * Change the normalization impedance.
   * If old z_norm == 0, then do nothing.
   *
   * @param new_z0 the new normalization impedance.
   */
  sdata & change_norm(double new_z0 = double(device::Z0)) ; 

  /**
   * Calculate the noise matrix for a passive element using the        
   * scattering matrix, temperature, and frequency.               
   *
   * The noise correlation matrix C is calculated as
   *
   * @f$ \mathcal{C} = {h \nu \over 2} \, \coth \left( {h \nu \over {2 k_B T}}
   * \right) \, (\mathcal{I} - \mathcal{S S^\dagger}) @f$
   */
  sdata & passive_noise(double freq, double temp) ;
};

// **************************************************************************

/**
 * @class zdata
 *
 * zdata holds linear circuit data in impedance form:
 * impedance matrix, voltage source vector, and voltage noise
 * correlation matrix.
 */
class zdata
{
public:
  /** The impedance matrix. */
  Matrix Z;

  /** The voltage noise correlation matrix. */
  Matrix C; 

  /** The voltage source vector. */
  Vector Vs;

  /** @return the number of ports and thus size of the matrices */
  int size() const
  { return Z.Lmaxindex(); }

  v_index_mode mode() const
  { return Z.Lmode; }

  /**
   * Construct data for a device with a known number of ports and index mode.
   *
   * @param n the number of ports, defaults to 0
   */
  explicit zdata(int n = 0, v_index_mode = Index_1);

  /** Copy constructor */
  zdata(const zdata & zd) ;

  // "conversion" constructors - to zdata from sdata or ydata

  /**
   * Convert data from the wave representation to the impedance representation.
   */
  zdata(const sdata & sd) ;

  /**
   * Convert data from the admittance representation to the impedance
   * representation.
   */
  zdata(const ydata & yd) ;

  // assignment operator: just use default operator = for now
  // zdata & operator=(const zdata &zd) ;

  /**
   * Calculate the noise matrix for a passive element using the
   * impedance matrix, temperature, and frequency.
   */
  zdata & passive_noise(double freq, double temp) ;
};

// **************************************************************************

/**
 *
 * @class ydata
 *
 * ydata holds linear circuit data in admittance form:
 * admittance matrix, voltage source vector, and current noise
 * correlation matrix.
 */
class ydata
{
public:
  /** The admittance matrix. */
  Matrix Y;
 
  /** The current noise correlation matrix. */
  Matrix C;

  /** The current source vector. */
  Vector Is;

  /** @return the number of ports and thus size of the matrices */
  int size() const
  { return Y.Lmaxindex(); }

  v_index_mode mode() const
  { return Y.Lmode; }

  /**
   * Construct data for a device with a known number of ports and index mode.
   *
   * @param n the number of ports, defaults to 0
   */
  explicit ydata(int n = 0, v_index_mode = Index_1);

  /** Copy constructor */
  ydata(const ydata & yd) ;

  // "conversion" constructors - to ydata from sdata or zdata

  /**
   * Convert data from the wave representation to the admittance representation.
   */
  ydata(const sdata & sd) ;

  /**
   * Convert data from the impedance representation to the admittance
   * representation.
   */
  ydata(const zdata & zd) ;

  // assignment operator: just use default operator = for now
  // ydata & operator=(const ydata & yd) ;

  /**
   * Calculate the noise matrix for a passive element using the
   * admittance matrix, temperature, and frequency.
   */
  ydata & passive_noise(double freq, double temp) ;
};

// **************************************************************************

/**
 * @class ydata_ptr
 *
 * ydata_ptr is like ydata, except it only holds pointers to
 * admittance matrix, voltage source vector, and current noise
 * correlation matrix held elsewhere.
 *
 * Be careful when you access that the pointers aren't NULL.
 * The main purpose for this object is to generate an sdata
 * object from admittance data stored in individual matrices
 * using this object as a wrapper: see the constructor and
 * operator =() in the sdata class declaration.
 */
class ydata_ptr
{
public:
  /** The admittance matrix */
  const Matrix * pY;

  /** The current noise correlation matrix */
  const Matrix * pC;

  /** The current source vector */
  const Vector * pIs;

  /** @return the number of ports and thus size of the matrices */
  int size() const
  { return (pY) ? pY -> Lmaxindex() : 0; }

  v_index_mode mode() const
  { return (pY) ? pY -> Lmode : Index_1; }

  // Constructor:
  explicit ydata_ptr
  (const Matrix * Y = 0, const Matrix * C = 0, const Vector * Is = 0)
    : pY(Y), pC(C), pIs(Is) { }

};

// **************************************************************************
//
// function compute_passive_noise
//

/**
 * Calculate the noise matrix for a passive element from the scattering
 * matrix, temperature, and frequency.
 *
 * @param d the known scattering matrix and noise matrix to be calculated
 * @param freq the frequency
 * @param Temp the temperature of the passive device
 */
inline void compute_passive_noise(sdata & d, double freq, double Temp)
{ d.passive_noise(freq, Temp); }

/**
 * Calculate the noise matrix for a passive element from the admittance
 * matrix, temperature, and frequency.
 *
 * @param d the known admittance matrix and noise matrix to be calculated
 * @param freq the frequency
 * @param Temp the temperature of the passive device
 */
inline void compute_passive_noise(ydata & d, double freq, double Temp)
{ d.passive_noise(freq, Temp); }

/**
 * Calculate the noise matrix for a passive element from the impedance
 * matrix, temperature, and frequency.
 *
 * @param d the known impedance matrix and noise matrix to be calculated
 * @param freq the frequency
 * @param Temp the temperature of the passive device
 */
inline void compute_passive_noise(zdata & d, double freq, double Temp)
{ d.passive_noise(freq, Temp); }

// **************************************************************************

/**
 * Calculate the effective passive noise temperature, including
 * quantum noise, as a fuction of physical temperature and frequency.
 *
 * @param freq the frequency, defaults to device::f
 * @param Temp the temperature, defaults to device::T
 * @return the noise temperature in standard units
 */
double passive_noise_temp(double freq = device::f, double Temp = device::T);

#endif /* SDATA_H */

