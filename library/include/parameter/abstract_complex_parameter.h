// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// **************************************************************************
/**
 * @file abstract_complex_parameter.h
 *
 * @author John Ward
 * @author Jonas Zmuidzinas
 *
 * @date November 3, 1997
 */
// **************************************************************************

#ifndef ABSTRACT_COMPLEX_PARAMETER_H
#define ABSTRACT_COMPLEX_PARAMETER_H

#include "SIScmplx.h"

/**
 * This is a abstract class to define the interface that all
 * complex parameters must implement.
 */
class abstract_complex_parameter
{
public:
  /**
   * Get the value of this parameter.
   *
   * @return the value of this parameter
   */
  virtual Complex get() const = 0;

  /**
   * Allow automatic typecasting from "abstract_complex_parameter" to
   * "complex"
   */
  operator complex() const {return get();}

  // virtual functions demand a virtual destructor:
  virtual ~abstract_complex_parameter() { }
};

#endif /* ABSTRACT_COMPLEX_PARAMETER_H */
