// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ************************************************************************
/**
 * @file ampdata.h
 *
 * Contains class ampdata, a subclass of sdata, which provides
 * functions related to amplifier noise matching and stability.
 *
 * @author John Ward
 * @date November 28, 1998
 */
// ************************************************************************

#ifndef AMPDATA_H
#define AMPDATA_H

#include "sdata.h"

/**
 * A subclass of sdata with extra functionality specific to amplifiers.
 *
 * Class ampdata must have its size = 2.
 * It is created by copying an sdata instance.
 */
class ampdata : public sdata
{
public:

  /** Construct a new ampdata instance. */
  ampdata() : sdata(2) { }

  /** Construct ampdata from an sdata object, checking that size()=2. */
  ampdata(const sdata &sd);

  /** @return the optimum noise match. */
  complex gamma_opt();

  /** @return the minimum noise temperature in standard units. */
  double t_min();

  /** @return the minimum noise figure in standard units. */
  double F_min();

  /**
   * Return the noise resistance normalized to internal z_norm.
   * This normalizing resistance can be retrieved using sdata::get_znorm(). 
   *
   * @return the noise resistance normalized to internal z_norm
   */
  double Rn();

  /** @return the noise measure in standard units. */
  double noise_measure();

  /** @return the determinant of the scattering matrix. */
  complex delta();

  /** @return k, which must be > 1 for unconditional stability */
  double k();

  /**
   * Noise stability circle center.
   * The noise stability circle is the circle for |gamma| = 1.
   *
   * @param p specifies the input or output port.
   * @return the center of the stability circle.
   */
  complex center(int p);

  /**
   * Noise stability circle radius.
   * The noise stability circle is the circle for |gamma| = 1.
   *
   * @param p specifies the input or output port.
   * @return radius of stability circle.
   */
  double radius(int p);

  /** @return true if amp is unconditionally stable at this frequency. */
  int unconditionally_stable() { return k()>1.0 && norm(delta())<1.0; }
};

#endif /* AMPDATA_H */
