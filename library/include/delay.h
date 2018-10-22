// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ********************************************************************
/**
 * @file delay.h
 *
 * This header file contains classes relating to time delays
 * or phase shifts.
 *
 * @author John Ward
 * @date September 13, 1998
 */
// ********************************************************************

#ifndef DELAY_H
#define DELAY_H

#include "nport.h"

/**
 * An ideal, noiseless time delay.
 */
class time_delay: public nport
{
public:
  /** Length of delay in standard units of time. */
  parameter time;

  /**
   * The constructor sets the delay to 0 by default.
   *
   * @param t length of delay in standard units.
   */
  explicit time_delay(double t = 0.0) : nport(2), time(t)
  { info.source = info.noise = info.active = false; }

  /**
   * This device has 2 ports.
   *
   * @return the number of ports
   */
  int size() { return 2; }

private:
  void recalc();
};

#endif /* DELAY_H */
