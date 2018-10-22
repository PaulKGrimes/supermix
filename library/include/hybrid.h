// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ***************************************************************************
/**
 * @file hybrid.h
 *
 * Classes for 90 and 180 degree hybrid couplers.
 *
 * @author John Ward
 * @date August 31, 1998
 */
// ***************************************************************************

#ifndef HYBRID_H
#define HYBRID_H

#include "nport.h"

/**
 * An ideal, noiseless 90 degree hybrid.
 */
class hybrid90: public nport
{
public:
  /**
   * The default constructor creates the scattering matrix.
   */
  hybrid90();

  /**
   * Hybrid couplers are 4 port devices.
   *
   * @return the number of ports
   */
  int size() { return 4; }

private:
  /**
   * Since the device is independent of f, T, and Z0, hybrids
   * are only calculated at construction.
   */
  void recalc() { };
};

/**
 * An ideal, noiseless 180 degree hybrid.
 */
class hybrid180: public nport
{
public:
  /**
   * The default constructor creates the scattering matrix.
   */
  hybrid180();

  /**
   * Hybrid couplers are 4 port devices.
   *
   * @return the number of ports
   */
  int size() { return 4; }

private:
  /**
   * Since the device is independent of f, T, and Z0, hybrids
   * are only calculated at construction.
   */
  void recalc() { };
};

#endif /* HYBRID_H */
