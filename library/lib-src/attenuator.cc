// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// attenuator.cc

#include "attenuator.h"
#include <cmath>

using namespace std;

attenuator::attenuator(double a) : nport(2), dB(a), Temp(&T)
{ info.source = false; }

attenuator::attenuator(const abstract_real_parameter * a)
 : nport(2), dB(a), Temp(&T)
{ info.source = false; }

void attenuator::recalc_S()
{
  // set the normalizing impedance
  data.set_znorm(device::Z0);

  // Perfect input matches.
  data.S[1][1] = 0.0;
  data.S[2][2] = 0.0;

  // Attenuated transmission.
  double atten = pow(10, -0.05 * dB);
  data.S[1][2] = atten;
  data.S[2][1] = atten;

  // Assume source vector, B, is already 0.
}

void attenuator::recalc()
{
  recalc_S();

  // Calculate the noise correlation matrix, C.
  compute_passive_noise(data, f, Temp);
}
