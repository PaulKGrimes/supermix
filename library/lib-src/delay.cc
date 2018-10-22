// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// delay.cc

#include "delay.h"

void time_delay::recalc()
{
  // Perfect input and output matches.
  // Assume matrix elements and data.znorm are already 0.

  // Time delayed transmission elements.
  data.S[1][2] = exp(I * (-2. * Pi * f * time));
  data.S[2][1] = data.S[1][2];

  // Assume noiseless.  C matrix should already be 0.

  // Source vector should already be 0.
}
