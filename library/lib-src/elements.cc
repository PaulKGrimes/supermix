// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// elements.cc

#include "elements.h"
#include "units.h"
#include "error.h"

using namespace std;

void spimp::calcZ(complex Z)
{
  data.set_znorm(device::Z0);   // added 12/29/97

  if(is_series) {
    if (Z != 0.0) {
      complex zo = 2 * device::Z0;  // common coefficient      
      data.S[1][1] = data.S[2][2] = Z/(zo + Z);
      data.S[2][1] = data.S[1][2] = zo/(zo + Z);
    }
    else /* Z == 0.0 */ {
      data.S[1][1] = data.S[2][2] = 0.0;
      data.S[2][1] = data.S[1][2] = 1.0;
    }
  }
  else /* parallel */ {
    if (Z != 0.0) {
      complex z = 2 * Z;  // common coefficient
      data.S[1][1] = data.S[2][2] = -device::Z0/(z + device::Z0);
      data.S[2][1] = data.S[1][2] = z/(z + device::Z0);
    }
    else /* Z == 0.0 */ {
      data.S[1][1] = data.S[2][2] = -1.0;
      data.S[2][1] = data.S[1][2] = 0.0;
    }
  }
}

void spimp::calcY(complex Y)
{
  data.set_znorm(device::Z0);

  if(is_series) {
    if (Y != 0.0) {
      complex zoy = 2 * device::Z0 * Y;  // common coefficient      
      data.S[1][1] = data.S[2][2] = 1/(1 + zoy);
      data.S[2][1] = data.S[1][2] = zoy/(1 + zoy);
    }
    else /* Y == 0.0 */ {
      data.S[1][1] = data.S[2][2] = 1.0;
      data.S[2][1] = data.S[1][2] = 0.0;
    }
  }
  else /* parallel */ {
    if (Y != 0.0) {
      complex zoy = device::Z0 * Y;  // common coefficient      
      data.S[1][1] = data.S[2][2] = -zoy/(2 + zoy);
      data.S[2][1] = data.S[1][2] = 2/(2 + zoy);
    }
    else /* Y == 0.0 */ {
      data.S[1][1] = data.S[2][2] = 0.0;
      data.S[2][1] = data.S[1][2] = 1.0;
    }
  }
}

// **************************************************************

branch::branch(int b) : nport(b), branches(b)
{ 
  if(b < 2)
    error::fatal("Invalid number of branches in branch constructor.");
  info.noise = info.active = info.source = false;
  calc();
}

branch & branch::set_branches(int b)
{
  if(b < 2)
    error::fatal("Branches must have at least two ports.");
  branches = b;
  calc();
  return *this;
}

void branch::calc()
{
  data.set_znorm(0.0);   // since S is independent of device::Z0
  data.resize(branches);

  double diagonal = (2.0 - branches) / branches;
  double off_diagonal = 2.0 / branches;

  data.S.fill(off_diagonal);

  for(int i=1; i<=branches; i++)
    data.S[i][i] = diagonal;

  // Assume that data.C is already a zero matrix.
}

// **************************************************************

series_tee::series_tee() : nport(3)
{
  data.set_znorm(0.0);   // since S is independent of device::Z0

  data.S[1][1] = 1;
  data.S[2][2] = 1;
  data.S[3][3] = 1;

  data.S[2][1] = -2;
  data.S[1][2] = -2;

  data.S[3][1] = 2;
  data.S[3][2] = 2;
  data.S[1][3] = 2;
  data.S[2][3] = 2;

  data.S /= 3.0;
  info.noise = info.active = info.source = false;
}

// **************************************************************

void zterm::recalc()
{
  recalc_S();
  data.passive_noise(f, Temp);
}

void zterm::recalc_S()
{
  data.set_znorm(device::Z0);
  data.S[1][1] = (Z == 0.0) ? -1.0 : (Z - double(device::Z0))/(Z + double(device::Z0));
}

// **************************************************************

void yterm::recalc()
{
  recalc_S();
  data.passive_noise(f, Temp);
}

void yterm::recalc_S()
{
  data.set_znorm(device::Z0);
  complex y = double(device::Z0) * Y;
  data.S[1][1] = (y == 0.0) ? 1.0 : (1 - y)/(1 + y);
}

