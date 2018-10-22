// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// *************************************************************************
/**
 * @file parameter.h
 *
 * Includes real_parameter.h and typedefs for parameter.
 *
 * @author John Ward
 * @date November 3, 1997
 *
 * Class parameter holds a double value to be used as a
 * parameter by the SuperMix program.  Parameters can do upper
 * and lower bounds checking.  They can also "shadow" (or
 * reference) other parameter instances.
 *
 * Most overloaded math and relational operators treat
 * parameters as doubles.
 *
 * There are many different types of parameters.  Look in
 * the <code>include/parameter/</code> subdirectory for more information.
 *
 * @see abstract_real_parameter
 * @see abstract_complex_parameter
 * @see real_parameter
 * @see complex_parameter
 * @see scaled_real_parameter
 * @see function_real_parameter
 */
// ************************************************************************

#ifndef PARAMETER_H
#define PARAMETER_H

#include "parameter/real_parameter.h"

/**
 * The most commonly used parameter is the real_parameter.
 * A typedef is added for convenience and backward compatibility. 
 */
typedef real_parameter parameter;

#endif /* PARAMETER_H */
