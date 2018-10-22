// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// transformer.cc

#include "transformer.h"
#include "global.h"
#include "error.h"
#include <cmath>

using namespace std;

//
// Default constructor
//
transformer::transformer() : 
  nport(2), Z1(&device::Z0), Z2(&device::Z0), is_verbose(false)
{ info.noise = info.active = info.source = false; }

void transformer::recalc()
{
  double znorm = device::Z0 ;
  Complex zz1 = Z1 ;
  Complex zz2 = Z2 ;
  double rr1 = real(zz1)/znorm ;
  double xx1 = imag(zz1)/znorm ;
  if(rr1 < Tiny) {
    if(is_verbose)
      error::warning(
      "Very small or negative input resistance for transformer") ;
    rr1 = Tiny;
  }

  double rr2 = real(zz2)/znorm ;
  double xx2 = imag(zz2)/znorm ;
  if(rr2 < Tiny) {
    if(is_verbose)
      error::warning(
      "Very small or negative output resistance for transformer") ;
    rr2 = Tiny;
  }

  double sr = sqrt(rr1/rr2) ;
  double zeta = 0.5*(sr - 1./sr) ;
  double psi = 0.5*(xx2*sr + xx1/sr) ;
  complex denom = sqrt(1.+zeta*zeta) + I*psi;

  complex S11 = (zeta + I*psi)/denom ;
  complex S22 = (-zeta + I*psi)/denom ;
  complex S12 = 1./denom ;  // = S21 from reciprocity

  data.set_znorm(znorm);  // which is Z0.  added 12/29/97.
  data.S[1][1] = S11;
  data.S[2][2] = S22;
  data.S[1][2] = S12;
  data.S[2][1] = S12;

  // Assume that data.C is already a zero matrix.
}
