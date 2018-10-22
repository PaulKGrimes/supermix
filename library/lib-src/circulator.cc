// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// circulator.cc

#include "circulator.h"

circulator::circulator() : nport(3)
{
  // Perfect input matches.
  data.S[1][1] = 0.0;
  data.S[2][2] = 0.0;
  data.S[3][3] = 0.0;

  // No reverse transmission.
  data.S[1][2] = 0.0;
  data.S[2][3] = 0.0;
  data.S[3][1] = 0.0;

  // Perfect forward transmission: 1->2, 2->3, 3->1.
  data.S[2][1] = 1.0;
  data.S[3][2] = 1.0;
  data.S[1][3] = 1.0;

  // Assume C matrix and B vector are already 0.
}
