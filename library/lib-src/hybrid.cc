// SuperMix version 1.5 C++ source file
// Copyright (c) 1999, 2001, 2004, 2007 California Institute of Technology.
// All rights reserved.
//
// hybrid.cc

#include "hybrid.h"
#include <cmath>

using namespace std;

hybrid90::hybrid90() : nport(4)
{
  double norm = -1.0 / sqrt(2.0);
  
  // Perfect input matches.
  data.S[1][1] = 0.0;
  data.S[2][2] = 0.0;
  data.S[3][3] = 0.0;
  data.S[4][4] = 0.0;

  data.S[1][4] = 0.0;
  data.S[4][1] = 0.0;
  data.S[2][3] = 0.0;
  data.S[3][2] = 0.0;

  data.S[3][1] = norm;
  data.S[1][3] = norm;
  data.S[2][4] = norm;
  data.S[4][2] = norm;

  data.S[1][2] = I*norm;
  data.S[2][1] = I*norm;
  data.S[3][4] = I*norm;
  data.S[4][3] = I*norm;

  // Ideal hybrid is lossless, so noise correlation matrix, C, is 0.
  // Assume source vector, B, is already 0.
  info.noise = info.active = info.source = false;
}

hybrid180::hybrid180() : nport(4)
{
  double norm = 1 / sqrt(2.0);
  
  // Perfect input matches.
  data.S[1][1] = 0.0;
  data.S[2][2] = 0.0;
  data.S[3][3] = 0.0;
  data.S[4][4] = 0.0;

  data.S[1][4] = 0.0;
  data.S[4][1] = 0.0;
  data.S[2][3] = 0.0;
  data.S[3][2] = 0.0;

  data.S[2][1] = norm;
  data.S[1][2] = norm;
  data.S[3][1] = norm;
  data.S[1][3] = norm;
  data.S[4][3] = norm;
  data.S[3][4] = norm;

  data.S[2][4] = -norm;
  data.S[4][2] = -norm;

  // Ideal hybrid is lossless, so noise correlation matrix, C, is 0.
  // Assume source vector, B, is already 0.
  info.noise = info.active = info.source = false;
}
