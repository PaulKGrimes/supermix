// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// **************************************************************************
/**
 * @file function_real_parameter.h
 *
 * @author John Ward
 * @date November 3, 1997
 */
// **************************************************************************

#ifndef FUNCTION_REAL_PARAMETER_H
#define FUNCTION_REAL_PARAMETER_H

#include "abstract_real_parameter.h"
#include "real_parameter.h"

/**
 * function_parameters calculate themselves from other
 * parameters.  They hold a function pointer and an array
 * of real_parameters to pass to the function.
 */
class function_real_parameter : public abstract_real_parameter
{
private:
  /** This is a function to calculate the parameter. */
  double (* function)(real_parameter *);
 
  /**
   * Hold an array of parameters to pass to the function.
   * They must be real_parameter instances!
   */
  real_parameter *function_pars;

public:
  function_real_parameter();

  // Use default copy constructor

  /**
   * @param func a function to be used to calculate this real_parameter
   * @param pars an array of parameters to be passed to func
   */
  function_real_parameter(double (* func)(real_parameter *), real_parameter *pars);

  /**
   * Set the function and array of parameters to be used to calculate this
   * parameter.
   *
   * @param func a function to be used to calculate this real_parameter
   * @param pars an array of parameters to be passed to func
   */
  void set(double (* func)(real_parameter *), real_parameter *pars);

  /**
   * Get the value of this parameter.
   *
   * @return the value of this parameter
   */
  double get() const;
};

#endif /* FUNCTION_REAL_PARAMETER_H */
