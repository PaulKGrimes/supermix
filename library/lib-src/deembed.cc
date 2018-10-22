// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// deembed.cc

#include "deembed.h"
#include "error.h"
#include "matmath.h"

deembed::deembed(nport & input, nport & output, nport & measured) : nport(2),
               in(&input), out(&output), meas(&measured)
{
  if(in->size() !=2 || out->size() !=2 || meas->size() !=2)
    error::fatal("All circuits must be 2 ports in deembed constructor");

  info.noise = info.active = info.source = false;
}

Matrix deembed::scat_to_abcd(const sdata & sd)
{
  Matrix s = sd.S;
  double z = sd.get_znorm();
  Matrix t(2);

  t[1][1] = ((1+s[1][1])*(1-s[2][2]) + s[1][2]*s[2][1])/(2 * s[2][1]);
  t[1][2] = ((1+s[1][1])*(1+s[2][2]) - s[1][2]*s[2][1]) * z / (2 * s[2][1]);
  t[2][1] = ((1-s[1][1])*(1-s[2][2]) - s[1][2]*s[2][1]) / (2 * z * s[2][1]);
  t[2][2] = ((1-s[1][1])*(1+s[2][2]) + s[1][2]*s[2][1])/(2 * s[2][1]);

  return t;
}

void deembed::recalc()
{
  recalc_S();
  data.passive_noise(f, T);
}

void deembed::recalc_S()
{
  Matrix dut(2);
  Matrix i_inv = inverse(scat_to_abcd(in->get_data()));
  Matrix o_inv = inverse(scat_to_abcd(out->get_data()));

  if(i_inv.Lsize != 2 || i_inv.Rsize != 2)
    error::warning("Inversion of input matrix in deembed failed");

  if(o_inv.Lsize != 2 || o_inv.Rsize != 2)
    error::warning("Inversion of output matrix in deembed failed");

  dut = i_inv * scat_to_abcd(meas->get_data()) * o_inv;

  complex A = dut[1][1];
  complex B = dut[1][2];
  complex C = dut[2][1];
  complex D = dut[2][2];

  data.set_znorm(Z0);
  double z = Z0;

  data.S[1][1] = (A + B/z - C*z - D)
               / (A + B/z + C*z + D);

  data.S[1][2] = 2.0 * (A*D - B*C) / (A + B/z + C*z + D);

  data.S[2][1] = 2.0 / (A + B/z + C*z + D);

  data.S[2][2] = (-A + B/z - C*z + D)
               / (A + B/z + C*z + D);
}
 
