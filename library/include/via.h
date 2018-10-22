// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// **************************************************************
/**
 * @file via.h
 *
 * This header file contains classes relating to via holes.
 *
 * @author John Ward
 * @date January 16, 2000
 */
// **************************************************************

#ifndef VIA_H
#define VIA_H

#include "parameter.h"

/**
 * A parameter that calculates the inductance of a via hole.
 *
 * Uses a semi-empirical formula from "Modeling Via Hole Grounds in
 * Microstrip," Goldfarb and Pucel, IEEE Microwave and Guided Wave
 * Letters, Vol. 1, No. 6, June 1991.
 * 
 * Parameters:
 *	@li D is the hole diameter
 *	@li H is the hole height
 *
 * Useful ranges:
 *	@li Dielectric constant 2.2 < epsilon < 20
 *	@li 0.2 < D/H < 1.5
 *	@li 100 um < H < 635 um
 *
 */
class L_via : public abstract_real_parameter
{
public:
  /** Hole diameter in standard units. */
  parameter D;

  /** Hole height in standard units. */
  parameter H;

  /**
   * The default constructor creates a via hole of height=0 and diameter=0.
   */
  L_via() : abstract_real_parameter(), D(0.0), H(0.0), coeff(muVacuum/(2.0*Pi))
  { }

  /**
   * Return the value of the inductance.
   *
   * A warning message will be displayed if H or D is outside its useful
   * range.
   */
  double get() const
  {
    if (D/H < 0.2)
      error::warning("Via hole D/H ratio should be > 0.2 in class L_via");

    if (D/H > 1.5)
      error::warning("Via hole D/H ratio should be < 1.5  in class L_via");

    if (H < 100.*Micro*Meter)
      error::warning("Via hole height should be > 100 microns in class L_via");

    if (H > 635.*Micro*Meter)
      error::warning("Via hole height should be < 635 microns in class L_via");

    double r = 0.5*D;
    double hypot = sqrt(r*r + H*H);
    return coeff * (H * log((H + hypot)/r) + 1.5 * (r - hypot));
  }

private:
  /**
   * The permeability of free space over 2 pi.  Set at construction.
   */
  double coeff;
};

#endif /* VIA_H */
