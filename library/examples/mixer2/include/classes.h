// classes.h
// classes and objects which aid in the creation of SIS receiver models
//
// this file is used by specs.h and build_mixer.inc to specify and construct
// a twinslot receiver model.

#include "supermix.h"

// ==========================================================================
// OBJECTS HOLDING THE CHARACTERISTICS OF THE SUPERCONDUCTING FILMS

struct sc_material { parameter Vgap, Tc, rho_normal; }
  nb    = { 2.9*mVolt,  9.20*Kelvin,  5.0*Micro*Ohm*Centi*Meter },
  nbtin = { 5.0*mVolt, 15.75*Kelvin, 30.0*Micro*Ohm*Centi*Meter };


// ==========================================================================
// OBJECTS HOLDING THE CHARACTERISTICS OF THE DIELECTRICS

struct diel_material { parameter eps, tand; }
  air = { 1.0, 0.0 },
  sio = { 5.6, 0.0 };


// ==========================================================================
// THE WIDTHS, LENGTHS, AND DIELECTRIC THICKNESSES OF THE MICROSTRIPS

struct wlt { parameter width, length, thickness; };


// ==========================================================================
// CLASSES WHICH CAN CALCULATE THE PRODUCT OR RATIO OF TWO SUPPLIED PARAMETERS
// (These will be used to get Rn and Capacitance from Area, RnA, and C/A)

struct product_calc : public abstract_real_parameter
{
  const abstract_real_parameter & p1;
  const abstract_real_parameter & p2;

  product_calc(const abstract_real_parameter & a,
	       const abstract_real_parameter & b)
    : p1(a), p2(b) { }

  double get() const { return p1*p2; }
};

struct ratio_calc : public abstract_real_parameter
{
  const abstract_real_parameter & p1;
  const abstract_real_parameter & p2;

  ratio_calc(const abstract_real_parameter & a,
	     const abstract_real_parameter & b)
    : p1(a), p2(b) { }

  double get() const { return p1/p2; }
};

