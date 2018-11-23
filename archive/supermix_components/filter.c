// filter.c
//======================================================================
// Paul Grimes, 14th Feb, 2006
//======================================================================
// Idealised filter objects with square passbands.  Defined by band edge 
// frequencies. 2 port objects.
//
// Filter transmission is lossless within the band, and filters are
// either perfectly reflecting or perfectly absorbing out of band.
//=======================================================================
// Changes:
//=======================================================================

#include "beamsplitter.h"

filter_refl::filter_refl() : nport(2), 
    { info.source = false; }


void filter_refl::recalc_S()
{
  // set the normalizing impedance
  data.set_znorm(device::Z0);

  if (f<=freq_low || f>=freq_hi)	  // Reflective
  {
	data.S[1][1] = 1.0;
	data.S[2][2] = 1.0;
  
	data.S[1][2] = 0.0;
	data.S[2][1] = 0.0;
  }
  else // Transmitting
  {
	data.S[1][1] = 0.0;
	data.S[2][2] = 0.0;
  
	data.S[1][2] = 1.0;
	data.S[2][1] = 1.0;
  }
}

void filter_refl::recalc()
{
  recalc_S();

  // Calculate the noise correlation matrix, C.
  compute_passive_noise(data, f, Temp);
}

// The implementation of the absorbing filter
filter_abs::filter_abs() : nport(2)
    { info.source = false; }

void filter_abs::recalc_S()
{
  // set the normalizing impedance
  data.set_znorm(device::Z0);

  if (f<=freq_low || f>=freq_hi)	  // Absorbing
  {
	data.S[1][1] = 0.0;
	data.S[2][2] = 0.0;
  
	data.S[1][2] = 0.0;
	data.S[2][1] = 0.0;
  }
  else // Transmitting
  {
	data.S[1][1] = 0.0;
	data.S[2][2] = 0.0;
  
	data.S[1][2] = 1.0;
	data.S[2][1] = 1.0;
  }
}

void filter_abs::recalc()
{
  recalc_S();

  // Calculate the noise correlation matrix, C.
  compute_passive_noise(data, f, Temp);
}
