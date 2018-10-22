// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// antenna.cc

#include "antenna.h"
#include "error.h"

using namespace std;

void slot_antenna::recalc()
{
  const complex s = (Z - device::Z0)/(Z + device::Z0);
  const complex c = 1.0/(3.0 - s);
  const double  x = 2*sqrt(1.0 - norm(s));

  data.S[1][1] = (1.0 - 3.0*conj(s))*c;
  data.S[2][2] = data.S[3][3] = (1.0 + s)*c;
  data.S[1][3] = data.S[3][1] = x*c;
  data.S[1][2] = data.S[2][1] = - data.S[1][3];
  data.S[2][3] = data.S[3][2] = 2.0*(1.0 - s)*c;
}


twin_slot_antenna::twin_slot_antenna()
  : nport(5), pA(0), Temp(&device::T), a(3)
{
  info.source = a.set_info().source = false;

  // build circuit:
  c.add_port(a,1);      // port 1: RF in
  c.connect(a,2,t1,1);
  c.connect(a,3,t2,1);
  c.add_port(t1,2);     // port 2: ant 1 - output
  c.add_port(t1,3);     // port 3: ant 1 + output
  c.add_port(t2,2);     // port 4: ant 2 - output
  c.add_port(t2,3);     // port 5: ant 2 + output
}


twin_slot_antenna::twin_slot_antenna(nport & s)
  : nport(5), pA(0), Temp(&device::T), a(3)
{
  set(s);
  info.source = a.set_info().source = false;
  

  // build circuit:
  c.add_port(a,1);      // port 1: RF in
  c.connect(a,2,t1,1);
  c.connect(a,3,t2,1);
  c.add_port(t1,2);     // port 2: ant 1 - output
  c.add_port(t1,3);     // port 3: ant 1 + output
  c.add_port(t2,2);     // port 4: ant 2 - output
  c.add_port(t2,3);     // port 5: ant 2 + output
}


twin_slot_antenna & twin_slot_antenna::set(nport & s)
{
  if (s.size() != 1 && s.size() != 2)
    error::warning("twin_slot_antenna: can only set to an nport with size 1 or 2");
  else
    pA = &s;
  return *this;
}


void twin_slot_antenna::recalc_S()
{
  // fill in data for device a
  const Matrix & S = pA->get_data_S().S;
  a().S[2][2] = a().S[3][3] = S[1][1];
  a().S[2][3] = a().S[3][2] = S.read(1,2);  // will be 0.0 if size is 1
  a().S[1][1] = -conj(a().S[2][2]+a().S[2][3]);
  a().S[1][2] = a().S[1][3] = a().S[2][1] = a().S[3][1] =
    sqrt(0.5 - 0.5*norm(a().S[1][1]));

  // calc circuit S matrix only.
  data = c.get_data_S();
}
