// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ********************************************************************
/**
 * @file circulator.h
 *
 * @author John Ward
 * @date August 31, 1998
 */
// ********************************************************************

#ifndef CIRCULATOR_H
#define CIRCULATOR_H

#include "nport.h"

/**
 * An ideal, noiseless 3-port circulator.
 *
 * <pre>
 *     1 -> 2
 *     2 -> 3
 *     3 -> 1
 * </pre>
 */
class circulator: public nport
{
public:
  /**
   * The default constructor creates the scattering matrix.
   */
  circulator();

  /**
   * This device has 3 ports.
   *
   * @return the number of ports
   */
  int size() { return 3; }

private:
  /**
   * Since the device is independent of f, T, and Z0, circulators
   * are only calculated at construction.
   */
  void recalc() { };
};

#endif /* CIRCULATOR_H */
