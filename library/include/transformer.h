// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// **************************************************************
/**
 * @file transformer.h
 *
 * transformer.h defines the ideal transformer circuit elements.
 *
 * @author Jonas Zmuidzinas
 * @date October 27, 1997
 */
// **************************************************************

#ifndef TRANSFORMER_H
#define TRANSFORMER_H

#include "nport.h"
#include "parameter/complex_parameter.h"

/**
 * An ideal transformer.
 *
 * Transforms from an input impedance of Z1 = R1 + j X1
 * to an output impedance of Z2 = R2 + j X2.
 */
class transformer : public nport
{
public:
  /** input impedance (defaults shadow device::Z0) */
  complex_parameter Z1 ;

  /** output impedance (defaults shadow device::Z0) */
  complex_parameter Z2 ;

  /** The default constructor creates a 1:1 transformer. */
  transformer();

  /**
   * Transformers are 2 port devices.
   *
   * @return the number of ports
   */
  int size() { return 2; }

  /** Turn on verbose output (off by default). */
  void verbose() { is_verbose = true; }

  /** Turn off verbose output (default). */
  void quiet() { is_verbose = false; }


private:
  bool is_verbose;

  // The calculation is done here.
  void recalc();
};

#endif /* TRANSFORMER_H */
