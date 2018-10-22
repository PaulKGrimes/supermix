// SuperMix version 1.6 C++ source file
// Copyright (c) 1999, 2001, 2004, 2009 California Institute of Technology.
// All rights reserved.
//
// mixer.cc
// 5/8/09: added an int version of abs(), called Abs().

#include "mixer.h"
#include "units.h"
#include "error.h"
#include "sources.h"
#include <cmath>

using namespace std;

// absolute value of an integer
inline int Abs(const int n) { return (n < 0)? -n : n; }

// class mixer: =======================================================

unsigned mixer::global_mixer_index = 0;

// ********************************************************************
// constructors and operator =

// the constructors have to set up the balancer and analyzer members
// to look at this particular mixer object (using *this);
// operator = must not upset this happy condition

mixer::mixer() :
  max_harmonics(1), num_junctions(0), LO_saved(0.0),
  balance_init_flag(0), auto_balance_flag(0), balance_not_ok_flag(0),
  bias_circuit(0), if_circuit(0), rf_circuit(0),
  term(0), default_term(0),
  junc(0),
  balance_(*this), ssignal_(*this), tsignal_(*this)
{
  info.source = false;      // a mixer is not a source
  LO.set_min(0.0);
  ssignal_.terminate_rf(0); // this object does a normal analysis
  tsignal_.terminate_rf(1); // this object does a terminated rf_circuit analysis
}

mixer::mixer(const mixer & m) :
  data_ptr_nport(m),
  LO(m.LO),
  max_harmonics(m.max_harmonics),
  num_junctions(m.num_junctions),
  LO_saved(m.LO_saved),
  balance_init_flag(m.balance_init_flag),
  auto_balance_flag(m.auto_balance_flag),
  balance_not_ok_flag((num_junctions != 0)),
  bias_circuit(m.bias_circuit),
  if_circuit(m.if_circuit),
  rf_circuit(m.rf_circuit),
  term(m.term), default_term(m.default_term),
  junc(m.junc),
  balance_(*this), ssignal_(*this), tsignal_(*this)
{
  info.source = false;      // a mixer is not a source
  LO.set_min(0.0);
  ssignal_.terminate_rf(0); // this object does a normal analysis
  tsignal_.terminate_rf(1); // this object does a terminated rf_circuit analysis
}

mixer & mixer::operator = (const mixer & m)
{
  // check for self assignment:
  if (& m == this) return *this;

  LO = m.LO;
  max_harmonics = m.max_harmonics,
  num_junctions = m.num_junctions,
  LO_saved = m.LO_saved;
  balance_init_flag = m.balance_init_flag;
  auto_balance_flag = m.auto_balance_flag;
  bias_circuit = m.bias_circuit;
  if_circuit = m.if_circuit;
  rf_circuit = m.rf_circuit;
  term = m.term;
  default_term = m.default_term;
  junc = m.junc;
  changed();

  return *this;
}

// ********************************************************************
// harmonics and junctions

mixer & mixer::harmonics(int N)
{
  if (N < 1) 
    error::fatal("Number of harmonics in a mixer must be a positive integer.");
  max_harmonics = N;
  int flag = balance_not_ok_flag;  // save flag
  changed();                       // this will invalidate a previous balance,
  balance_not_ok_flag = flag;      // but don't invalidate the previous balance
  return *this;
}

mixer & mixer::add_junction (junction & J)
{
  if(J.type() != junction::Y_type)
    error::fatal("Mixer can only handle Y_type junctions.");
  ++num_junctions;
  junc.push_back(&J);
  LO_saved = 0;
  changed();
  return *this;
}

mixer & mixer::void_junctions()
{
  num_junctions = 0;
  junc.resize(0);

  LO_saved = 0;
  balance_not_ok_flag = 0;  // no junctions, so no balance needed

  changed();
  return *this;
}

// ********************************************************************
// LO frequency and linear circuit elements

mixer & mixer::set_bias(nport & c)
{
  bias_circuit = &c;
  changed();
  return *this;
}

mixer & mixer::set_if(nport & c)
{
  if_circuit = &c;
  changed();
  return *this;
}

mixer & mixer::set_rf(nport & c)
{
  rf_circuit = &c;
  term.resize(0); term.resize(c.size());  // clear out previous terminators
  default_term.resize(c.size());
  changed();
  return *this;
}

mixer & mixer::set_balance_terminator(nport & c, int p)
{
  if ((rf_circuit == 0) || (rf_circuit->size() < p) || (p < 1))
    error::warning("Ignoring attempt to set a terminator to an invalid port\
 in mixer.");
  else if (c.size() != 1)
    error::fatal("Mixer balance terminators must be 1-ports.");
  else
    term[p-1] = &c;

  changed();
  return *this;
}

int mixer::port(int p, int h)
{
  // port ordering is:
  //  (1) all open IF circuit ports
  //  (2) RF circuit at harmonic = 1: first open port at USB, then at LSB
  //  (3) all succeeding RF circuit open ports, USB then LSB at each port
  //  (4) same as (2) and (3) at harmonic = 2
  //  (5) continue for all higher harmonics

  p -= num_junctions;  // don't count the ports connected to junctions
  if((p <= 0)||(Abs(h) > max_harmonics))
     return 0;  // invalid argument

  nport *c = if_circuit;
  if (c == 0)
    return 0;  // can't do anything if no circuit
  int ports = c->size() - num_junctions;  // open IF ports

  if (h == 0)
    return (ports < p) ? 0 : p;       // check for valid IF port index
  // |h| > 0
  c = rf_circuit;
  if (c == 0)
    return 0;  // now we can't do anything if no RF circuit
  int out = ports + 1;                // skip over IF ports
  ports = c->size() - num_junctions;  // open RF ports per harmonic
  if (p > ports) return 0;            // invalid index to an RF port
  out += 2 * ports * ( Abs(h) - 1 );  // skip lower harmonic port indexes
  out += 2 * ( p - 1 );               // skip to open port p
  if (h < 0) ++out;                   // LSB comes after USB

  return out;
}

double mixer::freq(int p)
{
  if(LO == 0.0 || p < 1 || rf_circuit == 0 || if_circuit == 0)
    return 0;  // can't get a frequency

  int IF_size = if_circuit->size() - num_junctions;
  int RF_size = rf_circuit->size() - num_junctions;

  if(p <= IF_size)
    return device::f;  // an IF port
  else
    p -= IF_size;

  double freq = device::f; 
  freq *= (p == (p>>1)<<1)? -1.0 : 1.0;  // freq is a lower sideband if p is even
  freq += LO * (1 + (p-1)/(2*RF_size));  // an integer divide to get harmonic number

  return freq;
}

int mixer::size()
{
  // mixer must be complete to get a nonzero size
  if (flag_mixer_incomplete())
    return 0;
  else
    return if_circuit->size() - num_junctions
      + 2*max_harmonics*(rf_circuit->size() - num_junctions);
}


// ********************************************************************
// status and mode flags

int mixer::flag_mixer_incomplete() const
{
  // this function examines the circuit elements for completeness and
  // consistency. The balance and recalc routines check this
  // function return value before proceding.

  if ((if_circuit == 0)||(rf_circuit == 0))
    return 1;  // essential linear circuits not assigned

  if ((bias_circuit == 0)&&(num_junctions != 0))
    return 1;  // junctions exist but no bias circuit

  if (((bias_circuit != 0)&&(bias_circuit->size() < 1))||
      (if_circuit->size() < 1)||
      (rf_circuit->size() < 1))
    return 1;  // circuits must know their sizes

  // note: only if_circuit must have an open port after attaching junctions
  if ((num_junctions != 0)&&
      ((bias_circuit->size() != num_junctions)||
       (if_circuit->size()   <= num_junctions)||
       (rf_circuit->size()   <  num_junctions)))
    return 1;  // circuits don't have enough ports

  return 0;    // all checks passed
}

int mixer::flag_state_invalid() const
{
  // this function polls all asigned junctions for a nonzero
  // junction::call_large_signal()

  int out = 0;  // hold the output value
  for (unsigned i = 0; i < junc.size(); ++i)
    out = out || junc[i]->call_large_signal();
  return out;
}

int mixer::flag_balance_inaccurate() const
{
  return (balance_not_ok_flag || (num_junctions != 0 && LO_saved != LO));
}

mixer & mixer::initialize_mode(int f)
{
  balance_init_flag = f;
  return *this;
}

mixer & mixer::auto_balance(int f)
{
  if ((f >= 0) && (f <= 2)) auto_balance_flag = f;
  return *this;
}


// ********************************************************************
// operating state calculations

mixer & mixer::save_operating_state(Matrix & V)
{
  int maxindex = max_harmonics;
  for(int n = 0; n < num_junctions; ++n) {
    int m = junc[n]->V().maxindex(); if (m < 0) m = 0;
    if (m > maxindex) maxindex = m;
  }
    
  V.reallocate(num_junctions, maxindex+1, Index_C, Index_C).maximize();

  for(int n = 0; n < num_junctions; ++n)
    V.fillrow(n, junc[n]->V());
  
  return *this;
}

mixer & mixer::initialize_operating_state(const Matrix & V)
{
  if((num_junctions != 0) && (LO <= 0))
    error::fatal("Must have a positive LO frequency during\
 mixer::initialize_operating_state().");

  if((V.Lmode != Index_C)||(V.Rmode != Index_C))
    error::fatal("Matrix argument to mixer::initialize_operating_state() must\
 be Index_C.");

  int max = num_junctions;
  if (V.Lsize < max) {
    error::warning("Argument to mixer::initialize_operating_state() too small\
 to set all junctions.");
    max = V.Lsize;
  }
  else if (V.Lsize > max) {
    error::warning("Argument to mixer::initialize_operating_state() has too\
 many rows - extra ignored.");
  }

  int h = V.Rmaxindex(); if (h < 0) h = 0;
  h = (h > max_harmonics)? h : max_harmonics;
  for(int n = 0; n < max; ++n)
    junc[n]->large_signal(row(n,V), LO, h);

  if (max > 0) LO_saved = LO;
  // assume the state came from a previous full balance:
  if (max == num_junctions) balance_not_ok_flag = 0;
  return *this;
}


mixer & mixer::initialize_operating_state()
{
  // need a complete mixer to do this, and positive LO freq
  if (flag_mixer_incomplete())
    error::fatal("Must correctly add all elements\
 before calling mixer::initialize_operating_state().");

  if((num_junctions != 0) && (LO <= 0))
    error::fatal("Must have a positive LO frequency during\
 mixer::initialize_operating_state().");

  // call the routine to perform the initialization:
  balance_.i_state();

  // this wasn't a full balance, so:
  balance_not_ok_flag = 1;
  return *this;
}


void mixer::auto_state()  // a private function
{
  switch (auto_balance_flag) {

  default:
  case mixer::Off: {
    // no balance attempt
    if (flag_state_invalid())
      error::fatal("Invalid junction operating state during mixer small\
 signal analysis.");
    break;
  }

  case mixer::Always: {
    if ( balance() )  // balance attempt fails
      error::fatal("Autobalance attempt failed during mixer small signal\
 analysis.");
    break;
  }

  case mixer::Smart: {
    if ((flag_state_invalid()||flag_balance_inaccurate()) // trigger balance
	&& balance() )  // and balance attempt fails
      error::fatal("Autobalance attempt failed during mixer small signal\
 analysis.");
  }

  } // switch
      
  // we only get here if the operating states are valid
}


mixer & mixer::balance_parameters(int m, double t_1, double t_m, double t_x, double a)
{ balance_.parameters(m,t_1,t_m,t_x,a); return *this; }


// ********************************************************************
// return operating state data

Vector mixer::I_junc(int m)
{
  Vector result(num_junctions, Index_1);
  if (m < 0) {
    error::warning("Passed a negative harmonic index to mixer::I_junc().");
    return result;
  }
  for(int n = 1; n <= num_junctions; ++n)
    result[n] = junc[n-1]->I().read(int(m));
  return result;
}

Vector mixer::V_junc(int m)
{
  Vector result(num_junctions, Index_1);
  if (m < 0) {
    error::warning("Passed a negative harmonic index to mixer::V_junc().");
    return result;
  }
  for(int n = 1; n <= num_junctions; ++n)
    result[n] = junc[n-1]->V().read(int(m));
  return result;
}

// ********************************************************************
// small signal response calculations

void mixer::recalc()
{
  // if the LO frequency is not set, or the mixer is incompletely specified,
  // or device::f < 0, or >= LO frequency fatal errors result:
  if (flag_mixer_incomplete())
    error::fatal("Must correctly add all elements before\
 using mixer.");
  if((num_junctions != 0) && (LO <= 0))
    error::fatal("Must have a positive LO frequency before\
 using mixer.");
  if (device::f < 0)
    error::fatal("device::f must be nonnegative for mixer IF frequency.");
  if (device::f >= LO)
    error::fatal("device::f must be less than LO frequency for mixer.");

  // calling auto_state() ensures that the junctions all have valid operating
  // states before the small signal mixer analysis is performed.
  // It will not return if the states remain invalid.

  auto_state();
  unsigned local_mixer_index = ++global_mixer_index;  // the only mixer?
  data_ptr = & ssignal_();  // call the analyzer
  if (local_mixer_index != global_mixer_index)
    // uh-oh, a mixer in the linear circuits
    error::fatal("Can't analyze a circuit with more than one mixer.");

}


const sdata & mixer::get_term_data()
{
  // if the LO frequency is not set, or the mixer is incompletely specified,
  // or device::f < 0, or >= LO frequency fatal errors result:
  if (flag_mixer_incomplete())
    error::fatal("Must correctly add all elements before\
 using mixer.");
  if((num_junctions != 0) && (LO <= 0))
    error::fatal("Must have a positive LO frequency before\
 using mixer.");
  if (device::f < 0)
    error::fatal("device::f must be nonnegative for mixer IF frequency.");
  if (device::f >= LO)
    error::fatal("device::f must be less than LO frequency for mixer.");

  // calling auto_state() ensures that the junctions all have valid operating
  // states before the small signal mixer analysis is performed.
  // It will not return if the states remain invalid.

  auto_state();
  unsigned local_mixer_index = ++global_mixer_index;  // the only mixer?
  const sdata *ps = & tsignal_();  // call the terminated-circuit analyzer
  if (local_mixer_index != global_mixer_index)
    // uh-oh, a mixer in the linear circuits
    error::fatal("Can't analyze a circuit with more than one mixer.");
  return *ps;

}


// class mixer_currents: ==============================================

mixer_currents::mixer_currents(mixer & m, int num_junctions) :
  data_ptr_nport(),
  mix(m), n(num_junctions), h(0),
  junctions(n),
  freq(0.0, mix.LO)
{
  info.active = info.noise = false;  // noiseless current sources
  int i;
  for (i = 0; i < n; ++i) {
    junctions[i].source_f = &freq;
    c.add_port(junctions[i], 1);
  }
}


mixer_currents & mixer_currents::set_harmonic(int harm)
{
  if(harm < 0)
    error::fatal("Can't use a negative harmonic in mixer_currents.");
  freq.set(double(harm), mix.LO);
  return *this;
}


void mixer_currents::recalc()
{
  Vector Amps(mix.I_junc(h));
  for (int i = 0; i < n; ++i) {
    junctions[i].sink_current = abs(Amps.read(i+1));
    junctions[i].sink_phase = arg(Amps.read(i+1));
  }
  data_ptr = & c.get_data();
}
