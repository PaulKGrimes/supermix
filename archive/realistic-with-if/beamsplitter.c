// beamsplitter.cc
//======================================================================
// Paul Grimes, 3rd March, 2003
//======================================================================
// A 3 port object representing a beamsplitter, including noise due to
// reflection and transmission losses when the beamsplitter is not at
// device::T.
//
// Ports are laid out as follows:
//      Port 1, straight through input, antennuated by transmission loss
//      Port 2, reflected input, antennuated by reflection loss
//      Port 3, output
//
// Beamsplitter does not operate in 4 port mode (with port 4 output being
// port 1 reflected + port 2 transmitted) at the moment, as this requires
// careful termination of port 4, and we have no need for this object at
// the moment.
//
// Also includes a 2 port beamsplitter for use with hot/cold load
// simulations using the LO temperature as the temperature of the load,
// and simulation of dielectric windows.
//
//=======================================================================
// Changes:
//=======================================================================

#include "beamsplitter.h"
#include <math.h>

double pi = 3.14159265;

beamsplitter::beamsplitter(int p) : nport(3), polarisation(p)
    { info.source = false; }


double beamsplitter::reflect()
{
  // Calculate r
  double r_top, r_bot;

  if (polarisation == 1) // We are in parallel mode
  {
    r_top = epsilon*cos(angle) - pow( (epsilon-pow(sin(angle), 2.0)), 0.5);
    r_bot = epsilon*cos(angle) + pow( (epsilon-pow(sin(angle), 2.0)), 0.5);
  }
  else
  {
    r_top = cos(angle) - pow( (epsilon - pow(sin(angle), 2.0)), 0.5);
    r_bot = cos(angle) + pow( (epsilon - pow(sin(angle), 2.0)), 0.5);
  }
  double r = r_top/r_bot;

  // Calculate F
  double F = 4*r*r/((1-r*r)*(1-r*r));

  // Calculate round trip phase shift
  double phase = 4*pi*(thickness/1.0e6)*((f*1.0e9/GHz)/299795458.)*pow((epsilon - pow(sin(angle),2.0)), 0.5);

  // Calculate R
  double Rsquared_top = F*pow(sin(phase/2.0), 2.0);

  double Rsquared = Rsquared_top/(1. + Rsquared_top);

  double R = pow(Rsquared, 0.5); // The AMPLITUDE reflection

  return R;
}

double beamsplitter::transmit()
{
  double r_top, r_bot;
  // Calculate r
  if (polarisation == 1) // We are in parallel mode
  {
    r_top = epsilon*cos(angle) - pow( (epsilon-pow(sin(angle), 2.0)), 0.5);
    r_bot = epsilon*cos(angle) + pow( (epsilon-pow(sin(angle), 2.0)), 0.5);
  }
  else
  {
    r_top = cos(angle) - pow( (epsilon - pow(sin(angle), 2.0)), 0.5);
    r_bot = cos(angle) + pow( (epsilon - pow(sin(angle), 2.0)), 0.5);
  }
  double r = r_top/r_bot;

  // Calculate F
  double F = 4*r*r/((1-r*r)*(1-r*r));

  // Calculate round trip phase shift
  double phase = 4*pi*(thickness/1.0e6)*((f*1.0e9/GHz)/299795458.)*pow((epsilon-pow(sin(angle), 2.0)), 0.5);

  // Calculate R
  double Rsquared_top = F*pow(sin(phase/2.0), 2.0);

  double Tsquared = 1. /(1. + Rsquared_top);

  double T = pow(Tsquared, 0.5); // The lossless transmission amplitude

  double T_lossy;

  if (loss_tangent != 0) // We are including losses
  {
    double alpha = 2*pi*pow(epsilon,0.5)*tan(loss_tangent)*(f*1.0e9/GHz)/299795458.;
    double effective_thickness = (thickness/1.0e6) / cos(angle);
    T_lossy = T*exp(-effective_thickness*alpha/2.);
  }
  else
  {
    T_lossy = T;
  }

  return T_lossy; // The AMPLITUDE transmission
}


void beamsplitter::recalc_S()
{
  // set the normalizing impedance
  data.set_znorm(device::Z0);

  // Perfect input matches, and no input coupling.
  data.S[1][1] = 0.0;
  data.S[2][2] = 0.0;
  data.S[3][3] = 0.0;
  data.S[1][2] = 0.0;
  data.S[2][1] = 0.0;

  // Attenuated transmission.
  double transmission = transmit();
  data.S[1][3] = transmission;
  data.S[3][1] = transmission;

  // Attenuated reflection.
  double reflection = reflect();
  data.S[2][3] = reflection;
  data.S[3][2] = reflection;

  // Assume source vector, B, is already 0.
}

void beamsplitter::recalc()
{
  recalc_S();

  // Calculate the noise correlation matrix, C.
  compute_passive_noise(data, f, Temp);
}

// The implementation of the reduced beamsplitter
beamsplitter_red::beamsplitter_red(int p) : nport(2), polarisation(p)
    { info.source = false; }

double beamsplitter_red::reflect()
{
  // Calculate r
  double r_top, r_bot;

  if (polarisation == 1) // We are in parallel mode
  {
    r_top = epsilon*cos(angle) - pow( (epsilon-pow(sin(angle), 2.0)), 0.5);
    r_bot = epsilon*cos(angle) + pow( (epsilon-pow(sin(angle), 2.0)), 0.5);
  }
  else
  {
    r_top = cos(angle) - pow( (epsilon - pow(sin(angle), 2.0)), 0.5);
    r_bot = cos(angle) + pow( (epsilon - pow(sin(angle), 2.0)), 0.5);
  }
  double r = r_top/r_bot;

  // Calculate F
  double F = 4*r*r/((1-r*r)*(1-r*r));

  // Calculate round trip phase shift
  double phase = 4*pi*(thickness/1.0e6)*((f*1.0e9/GHz)/299795458.)*pow((epsilon - pow(sin(angle),2.0)), 0.5);

  // Calculate R
  double Rsquared_top = F*pow(sin(phase/2.0), 2.0);

  double Rsquared = Rsquared_top/(1. + Rsquared_top);

  double R = pow(Rsquared, 0.5); // The AMPLITUDE reflection

  return R;
}

double beamsplitter_red::transmit()
{
  double r_top, r_bot;
  // Calculate r
  if (polarisation == 1) // We are in parallel mode
  {
    r_top = epsilon*cos(angle) - pow( (epsilon-pow(sin(angle), 2.0)), 0.5);
    r_bot = epsilon*cos(angle) + pow( (epsilon-pow(sin(angle), 2.0)), 0.5);
  }
  else
  {
    r_top = cos(angle) - pow( (epsilon - pow(sin(angle), 2.0)), 0.5);
    r_bot = cos(angle) + pow( (epsilon - pow(sin(angle), 2.0)), 0.5);
  }
  double r = r_top/r_bot;

  // Calculate F
  double F = 4*r*r/((1-r*r)*(1-r*r));

  // Calculate round trip phase shift
  double phase = 4*pi*(thickness/1.0e6)*((f*1.0e9/GHz)/299795458.)*pow((epsilon-pow(sin(angle), 2.0)), 0.5);

  // Calculate R
  double Rsquared_top = F*pow(sin(phase/2.0), 2.0);

  double Tsquared = 1. /(1. + Rsquared_top);

  double T = pow(Tsquared, 0.5);

  double T_lossy;
  if (loss_tangent != 0) // We are including losses
  {
    double alpha = 2*pi*pow(epsilon,0.5)*tan(loss_tangent)*(f*1.0e9/GHz)/299795458.;
    double effective_thickness = (thickness/1.0e6) / cos(angle);
    T_lossy = T*exp(-effective_thickness*alpha/2.);
  }
  else
  {
    T_lossy = T;
  }

  return T_lossy; // The AMPLITUDE transmission
}


void beamsplitter_red::recalc_S()
{
  // set the normalizing impedance
  data.set_znorm(device::Z0);

  // Perfect input matches, and no input coupling.
  double reflection = reflect();
  data.S[1][1] = reflection;
  data.S[2][2] = reflection;

  // Attenuated transmission.
  double transmission = transmit();
  data.S[1][2] = transmission;
  data.S[2][1] = transmission;

  // Assume source vector, B, is already 0.
}

void beamsplitter_red::recalc()
{
  recalc_S();

  // Calculate the noise correlation matrix, C.
  compute_passive_noise(data, f, Temp);
}
