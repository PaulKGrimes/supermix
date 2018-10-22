// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ********************************************************************
/**
 * @file device.h
 *
 * Defines class device, the superclass of all %device classes.
 *
 * @author John Ward
 */
// ********************************************************************

#ifndef DEVICE_H
#define DEVICE_H

#include "global.h"
#include "state_tag.h"
#include "parameter.h"

/**
 * @class device
 *
 * The superclass for all device classes.  The main duties of
 * class device are to:
 *
 * (1) hold the global state variables:
 *    @li device::f (the current frequency)
 *    @li device::T (the default temperature)
 *    @li device::Z0 (the normalization impedance)
 *
 * (2) set a unique id tag for each device created.
 *
 * class device is an abstract class.  It will never be
 * instantiated directly.
 */

class device
{
public:
  /**
   * Global temperature.  The default is set in nport.cc.
   *
   * Controls the temperature used for device calculations.
   * It must be set to the desired temp. It is a parameter, so
   * it may be set to shadow another parameter variable. Many
   * devices have their own temperature variable which will
   * override device::T if desired.
   */
  static parameter T;

  /**
   * Global frequency.  The default is set in nport.cc.
   *
   * Controls the frequency used for all device calculations.
   * It must be set to the desired freq. It is a parameter, so
   * it may be set to shadow another parameter variable, or may
   * in turn be shadowed by another parameter.
   */
  static parameter f;

  /**
   * Global normalization impedance.  The default is set in nport.cc.
   *
   * Sets the normalizing impedance used for S matrix calcs. It
   * is a parameter, but should not be set to shadow another. It
   * may be useful to have other parameters shadow it, however.
   *
   * ALL RESPONSE CALCULATIONS PERFORMED BY CIRCUIT ELEMENTS
   * (DERIVED FROM CLASS nport) MUST USE THE VALUE OF device::Z0
   */
  static parameter Z0;

private:
  /**
   * Counter of total number of devices created.  Used to set device::id.
   */
  static unsigned long devcount;

protected:
  /**
   * Keep track of the last operating state where this device was calculated.
   */
  state_tag last_state;

public:
  /** A unique serial number or id tag for this device instance. */
  const unsigned long id;

  /** Constructor assigns a unique id by using devcount. */
  device() : id(++devcount) { };

  /** Copy constructor assigns a unique id by using devcount. */
  device(const device &) : id(++devcount) { };

  /** Can't use default assigment operator, since id is const. */
  device & operator =(const device &) {return *this;}

  // Virtual destructor is necessary to ensure proper subclass destruction.
  virtual ~device() { }
};

#endif /* DEVICE_H */
