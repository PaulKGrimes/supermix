// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
/**************************************************************\
* radial_stub.h                                                *
*                                                              *
* John Ward, September 11, 1998                                *
\**************************************************************/

#ifndef RADIALSTUB_H
#define RADIALSTUB_H

#include "circuit.h"
#include "trlines.h"

/**************************************************************\
*                                                              *
* class radial_stub                                            *
*                                                              *
* The current algorithm breaks the stub into a fixed number    *
* of annular slices, simulating them as microstrips.           *
*                                                              *
* The implementation of radial_stub could be changed to a      *
* different algorithm without affecting the public interface.  *
*                                                              *
* The angle should be between 0 and 180 degrees.               *
*                                                              *
* The program will throw a fatal error if the radial stub      *
* isn't larger than the transmission line feeding it.          *
*                                                              *
\**************************************************************/
class radial_stub: public data_ptr_nport
{
public:
  // Radius of stub in standard units
  parameter radius;

  // Opening angle of stub in standard units. 
  parameter angle;

  // Length of input line in standard units
  parameter length ;

  // Width of input line in standard units
  parameter width;

  // Substrate thickness
  parameter sub_thick ;

  // Default constructor.
  radial_stub();

  // Copy constructor
  radial_stub(const radial_stub & rs);

  // Assignment operator
  radial_stub & operator=(const radial_stub &r) ;

  // Functions to set various pointers
  radial_stub & substrate(dielectric & d);   /* substrate dielectric */
  radial_stub & superstrate(dielectric & d); /* superstrate dielectric */
  radial_stub & top_strip(surfimp & s);       /* top strip surface impedance */
  radial_stub & ground_plane(surfimp & s);   /* ground plane surface impedance */

  // Radial stubs are 2 port devices.
  int size() { return 2; }
  
  // Virtual destructor is necessary to ensure proper subclass destruction.
  virtual ~radial_stub() { }

private:
  // Keep the circuit here.
  circuit rs;

  // Number of sections used to simulate the stub.
  int nsections;

  // Microstrip sections used to build the stub.
  microstrip sec[16];

  // Pointers to dielectric objects
  dielectric *sub;
  dielectric *super;

  // pointers to surface impedance objects
  surfimp *top;
  surfimp *ground;

  // The calculation is done here.
  void recalc(bool noise);
  void recalc()   { recalc(true); }
  void recalc_S() { recalc(false); }
};

#endif /* RADIALSTUB_H */
