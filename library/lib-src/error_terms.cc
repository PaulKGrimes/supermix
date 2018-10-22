// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// error_terms.cc

#include "error_terms.h"
#include "error.h"
#include <iostream>

// Need ampdata for amp_k and amp_mag_delta
#include "ampdata.h"

// Need logarithms.
#include <cmath>

using namespace std;


double gain_dB::get(state_tag tag)
{
  double retval = 0. ;

  // get a reference to scattering matrix in first nport object
  const Matrix & Scat = (np->get_data(tag)).S ;
    
  if(in_port < Scat.Rminindex() || in_port > Scat.Rmaxindex()) {
    error::warning("Input port index out of range in gain_dB::get()");
  }
  if(out_port < Scat.Lminindex() || out_port > Scat.Lmaxindex()) {
     error::warning("Output port index out of range in gain_dB::get()");
  }
    
  double gain = abs(Scat.read(out_port, in_port)) ;
  if(gain > 0.)
    gain = 20.*log10(gain) ;  // convert to dB
  else
    gain = -1.e20 ;           // just a huge negative number
  retval = checkval(gain) ;   // checkval looks at mode flag

  return(retval) ;
}


double s_mag::get(state_tag tag)
{
  double retval = 0. ;

  // get a reference to scattering matrix in first nport object
  const Matrix & Scat = (np->get_data(tag)).S ;
    
  if(in_port < Scat.Rminindex() || in_port > Scat.Rmaxindex()) {
    error::warning("Input port index out of range in s_mag::get()");
  }
  if(out_port < Scat.Lminindex() || out_port > Scat.Lmaxindex()) {
    error::warning("Output port index out of range in s_mag::get()");
  }
    
  double sm = abs(Scat.read(out_port, in_port)) ;
  retval = checkval(sm) ;   // checkval looks at mode flag

  return(retval) ;
}


double input_tn::get(state_tag tag)
{
  double retval = 0. ;

  // get a reference to scattering matrix in first nport object
  const Matrix & Scat = (np->get_data(tag)).S ;
  // get a reference to noise matrix in first nport object
  const Matrix & CNoise = (np->get_data(tag)).C ;
    
  if(in_port < Scat.Rminindex() || in_port > Scat.Rmaxindex()) {
    error::warning("Input port index out of range in sm::get()");
  }
  if(out_port < Scat.Lminindex() || out_port > Scat.Lmaxindex()) {
    error::warning("Output port index out of range in sm::get()");
  }
    
  double sm = abs(Scat.read(out_port, in_port)) ;
  double cm = abs(CNoise.read(out_port, out_port)) ;    
  double tn = cm/(sm*sm) ;

  retval = checkval(tn) ;   // checkval looks at mode flag

  return(retval) ;
}


// Error functions for amplifier stability

double amp_k::get(state_tag tag)
{
  ampdata sd(np->get_data(tag));

  return (checkval(sd.k()));
}


double amp_mag_delta::get(state_tag tag)
{
  ampdata sd(np->get_data(tag));

  return (10.0 * checkval(zabs(sd.delta())));
}


// an error function term for matching S matrices of two circuits

double two_match::get(state_tag tag)
{
  double retval = 0. ;

  // get a reference to scattering matrices in two nport objects
  const Matrix & Scat0 = (np1->get_data(tag)).S ;
  const Matrix & Scat1 = (np2->get_data(tag)).S ;
  Matrix diff = Scat0 - Scat1 ;  // subtract the matrices
  // add up squares of complex differences
  for(int i = diff.Lminindex(); i<= diff.Lmaxindex(); i++)
    for(int j = diff.Rminindex(); j<= diff.Rmaxindex(); j++)
  retval += zmagsq(diff.read(i,j)) ;

  return(retval) ;
}


void fts_match::reset()
{
  // We must call scaled_match_error_term::zero() in reset()
  zero();

  // Store initial LO power so that we can reset it before exit.
  // We use a parameter in case the LO power parameter shadows another
  parameter old_LO_power(*LO_power);

  // Calculate the dark current...
  *LO_power = 0.0;
  mix->balance();

  // Sum all junction DC currents into the variable dark_current
  currents = mix->I_junc(0);  // vector of DC bias currents
  int min = currents.minindex();
  int max = currents.maxindex();
  dark_current = 0.0;
  for(int i=min; i<=max; i++) dark_current += currents[i].real;

  // Make sure we leave the mixer the way we found it.
  *LO_power = old_LO_power;
}

double fts_match::get_b(state_tag)
{
  // Store initial LO power so that we can reset it before exit.
  // We use a parameter in case the LO power parameter shadows another
  parameter old_LO_power(*LO_power);

  // Calculate the pumped current...
  *LO_power = pumped_power;
  mix->balance();

  // Sum all junction DC currents into the variable response
  currents = mix->I_junc(0);  // vector of DC bias currents
  int min = currents.minindex();
  int max = currents.maxindex();
  double response = 0.0;
  for(int i=min; i<=max; i++) response += currents[i].real;

  // Make sure we leave the mixer the way we found it.
  *LO_power = old_LO_power;

  // response holds the pumped current; subtract off dark current
  response -= dark_current;

  // If the measured FTS data includes a freq correction factor,
  // adjust response to include it as well.
  if (f) response *= *LO_freq;

  return response;
}


double iv_match::get_b()
{
  mix->balance();

  // Sum all junction DC currents into the variable c
  currents = mix->I_junc(0);  // vector of DC bias currents
  int min = currents.minindex();
  int max = currents.maxindex();
  double c = 0.0;
  for(int i=min; i<=max; i++) c += currents[i].real;

  return c;
}

double iv_match::get(state_tag)
{
  double i1 = get_a();
  double i2 = get_b();
  return (i1-i2)*(i1-i2);
}

