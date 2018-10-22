// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// **************************************************************************
/**
 * @file scaled_real_parameter.h
 *
 * @author John Ward
 * @date November 3, 1997
 */
// **************************************************************************

#ifndef SCALED_REAL_PARAMETER_H
#define SCALED_REAL_PARAMETER_H

#include "abstract_real_parameter.h"

/**
 * A scaled parameter is a fixed factor times another
 * parameter.
 */
class scaled_real_parameter : public abstract_real_parameter
{
private:
  /** The parameter we are a fixed factor times. */
  abstract_real_parameter *shadowed;

  /** The scaling factor. */
  double scale;

  /**
   * A flag to avoid possible infinite self-referential loops
   * of shadowed parameters
   */
  mutable int get_flag ;

public:
  scaled_real_parameter();

  // Use default copy constructor

  /**
   * @param s the scaling factor to be multiplied by p
   * @param p the parameter to be shadowed and scaled
   */
  scaled_real_parameter(const double s, abstract_real_parameter & p);

  /**
   * Set up shadowing and the scaling factor.
   *
   * @param s the scaling factor to be multiplied by p
   * @param p the parameter to be shadowed and scaled
   */
  void set(const double s, abstract_real_parameter & p);

  /**
   * Get the value of this parameter.
   *
   * @return the value of this parameter
   */
  double get() const;
};

#endif /* SCALED_REAL_PARAMETER_H */
