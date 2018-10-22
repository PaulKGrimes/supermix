// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// **************************************************************************
/**
 * @file abstract_real_parameter.h
 *
 * @author John Ward
 * @date November 3, 1997
 */
// **************************************************************************

#ifndef ABSTRACT_REAL_PARAMETER_H
#define ABSTRACT_REAL_PARAMETER_H

#include "SIScmplx.h"

/**
 * This is a abstract class to define the interface that all
 * real parameters must implement.
 *
 * real_parameter, scaled_real_parameter, and
 * function_real_parameter are all subclasses of this class.
 */
class abstract_real_parameter
{
public:
  /**
   * Get the value of this parameter.
   *
   * @return the value of this parameter
   */
  virtual double get() const = 0;

  /** Allow automatic typecasting from abstract_real_parameter. */
  operator double() const {return get();}

  // virtual functions demand a virtual destructor:
  virtual ~abstract_real_parameter() { }
};

#endif /* ABSTRACT_REAL_PARAMETER_H */
