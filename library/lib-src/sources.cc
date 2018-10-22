// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// sources.cc

#include "sources.h"
#include <cmath>
#include "error.h"

using namespace std;


// generator:

generator::generator() :
  nport(1), 
  R(&device::Z0), Temp(&device::T), 
  source_f(0.0), source_width(0.0), source_power(0.0), source_phase(0.0)
{
  R.set_min(0.0);
  source_f.set_min(0.0);
  source_width.set_min(0.0);
  source_power.set_min(0.0);
}

const nport::data_info & generator::get_data_info()
{ 
  info.active = (Temp != device::T && R != 0.0);
  info.source = source_power != 0.0;
  return info;
}

void generator::recalc_S()
{
  // Set normalization impedance. added 12/29/97
  data.set_znorm(Z0);

  // Calculate S11
  data.S[1][1] = (R - Z0)/(R + Z0);

  // Convert the source power to an amplitude wave and put in B1
  if(fabs(f - source_f) <= (source_width/2.0))
    data.B[1] = polar(2.0*sqrt(Z0*R*source_power)/(Z0+R), source_phase);
  else
    data.B[1] = 0.0;
}

void generator::recalc()
{
  recalc_S();

  // Calculate C11
  compute_passive_noise(data, f, Temp);
}


// voltage_source:

voltage_source::voltage_source() :
  nport(1),
  R(0.0), Temp(&T),
  source_f(0.0), source_width(0.0), source_voltage(0.0), source_phase(0.0)
{
  R.set_min(0.0);
  source_f.set_min(0.0);
  source_width.set_min(0.0);
}

const nport::data_info & voltage_source::get_data_info()
{ 
  info.active = (Temp != device::T && R != 0.0);
  info.source = source_voltage != 0.0;
  return info;
}

void voltage_source::recalc_S()
{
  // Set normalization impedance:
  data.set_znorm(Z0);

  // Calculate S11
  data.S[1][1] = (R - Z0)/(R + Z0);

  // Convert the source voltage to an amplitude wave and put in B1
  if(fabs(f - source_f) <= (source_width/2.0))
    data.B[1] = polar(sqrt(Z0)*source_voltage/(Z0+R), source_phase);
  else
    data.B[1] = 0.0;
}

void voltage_source::recalc()
{
  recalc_S();

  // Calculate C11
  compute_passive_noise(data, f, Temp);
}


// current_sink:

current_sink::current_sink() :
  nport(1),
  Y(0.0), Temp(&T),
  source_f(0.0), source_width(0.0), sink_current(0.0), sink_phase(0.0)
{
  Y.set_min(0.0);
  source_f.set_min(0.0);
  source_width.set_min(0.0);
}

const nport::data_info & current_sink::get_data_info()
{ 
  info.active = (Temp != device::T && Y != 0.0);
  info.source = sink_current != 0.0;
  return info;
}

void current_sink::recalc_S()
{
  // Set normalization impedance:
  data.set_znorm(Z0);

  // Calculate S11
  data.S[1][1] = (1 - Y*Z0)/(1 + Y*Z0);

  // Calculate C11
  compute_passive_noise(data, f, Temp);

  // Convert the sink current to an amplitude wave and put in B1
  if(fabs(f - source_f) <= (source_width/2.0))
    data.B[1] = - polar(sqrt(Z0)*sink_current/(1 + Y*Z0), sink_phase);
  else
    data.B[1] = 0.0;
}

void current_sink::recalc()
{
  recalc_S();

  // Calculate C11
  compute_passive_noise(data, f, Temp);
}
