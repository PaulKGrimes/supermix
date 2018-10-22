// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// fet.cc

#include "fet.h"
#include <cmath>
#include "error.h"

using namespace std;

void transconductance::recalc()
{
  // Set normalization impedance. added 12/29/97
  data.set_znorm(Z0);

  // Calculate S matrix
  data.S[1][1] = 1.0;
  data.S[2][2] = 1.0;
  data.S[1][2] = 0.0;
  data.S[2][1] = -2.0*G*Z0*exp(-I*2*Pi*f*Tau);

  // The C matrix is all zeros, i.e. the transconductance is noiseless.

  // Leave source vector at its default value, all zeroes.
}

fet::fet() : data_ptr_nport()
{
  construct();
}

fet::fet(const fet & f) : data_ptr_nport()
{
  construct();
  fet_copy(f);
}

fet & fet::operator=(const fet & f)
{
  // Beware of self assignment: f = f
  if(this != &f)
  {
    fet_copy(f);
  }
  return *this;
}

void fet::construct()
{
  // fets are active noise devices, but are sourceless:
  info.source = false;

  // build the circuitry for the model:
  Lg.series();
  Rg.series();
  Cpg.parallel();

  Cgs.parallel();
  Rgs.series();

  Cgd.series();
  Rds.parallel();
  Cds.parallel();

  Rs.parallel();
  Ls.parallel();

  Cpd.parallel();
  Rd.series();
  Ld.series();

  fetckt.add_port(Lg, 1);
  fetckt.connect(Lg, 2, Rg, 1);
  fetckt.connect(Rg, 2, Cpg, 1);

  fetckt.connect(b1, 2, Rgs, 1);
  fetckt.connect(Rgs, 2, Cgs, 1);
  fetckt.connect(Cgs, 2, Gm, 1);
  fetckt.connect(Gm, 2, b2, 2);
  fetckt.connect(b1, 1, Cgd, 1);
  fetckt.connect(b2, 1, Cgd, 2);

  fetckt.connect(b2, 3, Rds, 1);
  fetckt.connect(Rds, 2, Cds, 1);

  fetckt.connect(st3, 1, b1, 3);
  fetckt.connect(st4, 1, Cds, 2);
  fetckt.connect(st3, 2, Rs, 1);
  fetckt.connect(st4, 2, Rs, 2);

  fetckt.connect(st5, 1, st3, 3);
  fetckt.connect(st6, 1, st4, 3);
  fetckt.connect(st5, 2, Ls, 1);
  fetckt.connect(st6, 2, Ls, 2);

  fetckt.connect(Cpg, 2, st5, 3);

  fetckt.connect(st6, 3, Cpd, 1);
  fetckt.connect(Cpd, 2, Rd, 1);
  fetckt.connect(Rd, 2, Ld, 1);
  fetckt.add_port(Ld, 2);
}

void fet::fet_copy(const fet & f)
{
  Gm = f.Gm;
  Cgs = f.Cgs;
  Cds = f.Cds;
  Rds = f.Rds;
  Cgd = f.Cgd;
  Rgs = f.Rgs;
  Rg = f.Rg;
  Rs = f.Rs;
  Rd = f.Rd;
  Lg = f.Lg;
  Ls = f.Ls;
  Ld = f.Ld;
  Cpg = f.Cpg;
  Cpd = f.Cpd;
}
