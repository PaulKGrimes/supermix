// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ********************************************************************
/**
 * @file attenuator.h
 *
 * A standard, run-of-the-mill resistive attenuator.
 *
 * @author John Ward
 * @date August 31, 1998
 */
// ********************************************************************


#ifndef ATTENUATOR_H
#define ATTENUATOR_H

#include "nport.h"

/**
 * An ideal attenuator including Johnson noise.
 */
class attenuator: public nport
{
public:
  /**
   * Attenuation in dB; impedance is given by device::Z0
   */
  parameter dB;

  /**
   * Temperature in standard units.
   * Shadows global temperature T by default.
   */
  parameter Temp;

  /**
   * Default constructor creates a 3 dB attenuator.
   */
  attenuator(double a = 3.0);

  /**
   * Construct a shadowed parameter attenuator.
   *
   * @param a attenuation in dBs
   */
  attenuator(const abstract_real_parameter * a);

  /**
   * Attenuators are 2 port devices.
   *
   * @return the number of ports
   */
  int size() { return 2; }

  /**
   * Attenuators are "active" noise sources if not at device::T
   */
  const nport::data_info & get_data_info()
    { info.active = (Temp != device::T && dB != 0.0); return info; }

private:
  // The calculation is done here.
  void recalc();
  void recalc_S();
};

#endif /* ATTENUATOR_H */
