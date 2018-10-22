// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// supcond.cc

#include <cmath>
#include "units.h"
#include "supcond.h"
#include "integrate.h"

using namespace std;


// Some variables we need to pass to integrand functions:
//
//     Omega - reduced frequency, h*freq/e*DELTA(T)   (h=Planck's const; e=
//                                                     electron charge)
//     tau   - reduced temperature, k*T/e*DELTA(T)    (k = Boltzmann's const)

static struct { double tau; double Omega; } lparms;

// Integrand function declarations. static so we don't pollute the global
// namespace.
static double sig11_supcond(double);
static double sig12_supcond(double);
static double sig21_supcond(double);
static double sig22_supcond(double);

// additional function declarations:
inline double fermi_supcond(double, double);
inline double g_supcond(double, double);
static double gap_supcond(double);


// ***********************************************************************
// supcond():

complex supcond(double freq, double T, double Vgap, double Tc)
{
  if(Vgap < 0. || Tc < 0. || T > Tc)
    // check for unreasonable or nonsuperconducting conditions
    // and return normal-state conductivity if they exist:
    return(complex(1.0));

  double DELTA  = 0.5*Vgap;           // gap parameter
  double dratio = gap_supcond(T/Tc);  // reduced gap at T: DELTA(T)/DELTA(0)

  // Normalize frequency and temperature to gap energy:
  lparms.Omega = (freq/VoltToFreq)/(DELTA*dratio);
  lparms.tau   = BoltzK*T/(hPlanck*VoltToFreq*DELTA*dratio);
  // lparms.tau = 0.086170837*T/(DELTA*dratio)

  double S1, S2;                 // real and imaginary parts of conductivity
  static integrator<double> Int; // static so it only gets constructed once

  // Calculate real part:
  
  double S11, S12;

  S11 = Int.sqrtlower()(sig11_supcond, 1.0, 2.0)
    + Int.exp()(sig11_supcond, 2.0, 1000);
  S11 *= 2.0/lparms.Omega;

  if(lparms.Omega <= 2.) {
    S12 = 0. ;
  }
  else {
    S12 = Int.sqrtlower()(sig12_supcond, 1.0, lparms.Omega/2.0)
      +  Int.sqrtupper()(sig12_supcond, lparms.Omega/2.0, lparms.Omega-1.0);
    S12 /= lparms.Omega;
  }

  // Note following "-" sign on S12. This is correct.
  // Whitaker et al. (1988), eqn. 14a, has the incorrect sign.
  // Kautz (1978), eq. 2.9, has the correct sign.

  S1 = S11-S12 ;

  // Calculate imaginary part:

  if(lparms.Omega <= 2.) {
    if(lparms.Omega <= 1.) {
      S2 = Int.sqrtlower()(sig21_supcond, 1.0-lparms.Omega, 1.0-lparms.Omega/2)
	+ Int.sqrtupper()(sig21_supcond, 1.0-lparms.Omega/2, 1.0);
    }
    else {
      S2 = Int.sqrtupper()(sig22_supcond, 0, lparms.Omega-1.0)
	+ Int.sqrtlower()(sig21_supcond, 0, 1.0-lparms.Omega/2.0)
	+ Int.sqrtupper()(sig21_supcond, 1.0-lparms.Omega/2.0, 1.0);
    }
  }
  else {
    S2 = Int.sqrtupper()(sig22_supcond, 0, 1.0)
      + Int.sqrtupper()(sig21_supcond, 0, 1.0) ;
  }
  S2 /= lparms.Omega ;

  return(complex(S1, -S2));   // note minus sign on imaginary part
                              // this corresponds to exp(+ j omega t)
} // supcond()


// ***********************************************************************
// the integrand functions declared previously:

static double sig11_supcond(double x)
{
  double xO = x+lparms.Omega;  // we'll use this a couple of times...

  double result = fermi_supcond(x,lparms.tau) - fermi_supcond(xO,lparms.tau);
  result *= g_supcond(x,lparms.Omega);
  result /= sqrt(x*x - 1) * sqrt(xO*xO - 1);
  return result;
}


static double sig12_supcond(double x)
{
  x *= -1;
  double xO = x+lparms.Omega;  // we'll use this a couple of times...

  double result = 1 - 2.0*fermi_supcond(xO,lparms.tau);
  result *= g_supcond(x,lparms.Omega) ;
  result /= sqrt(x*x - 1) * sqrt(xO*xO - 1) ;
  return result;
}


static double sig21_supcond(double x)
{
  double xO = x+lparms.Omega;  // we'll use this a couple of times...

  double result = 1 - 2.0*fermi_supcond(xO,lparms.tau);
  result *= g_supcond(x,lparms.Omega) ;
  result /= sqrt(1 - x*x) * sqrt(xO*xO - 1) ;
  return result;
}


static double sig22_supcond(double x)
{
  x *= -1;
  double xO = x+lparms.Omega;  // we'll use this a couple of times...

  double result = 1 - 2.0*fermi_supcond(xO,lparms.tau);
  result *= g_supcond(x,lparms.Omega) ;
  result /= sqrt(1 - x*x) * sqrt(xO*xO - 1) ;
  return result;
}


// ***********************************************************************
// additional functions:

inline double fermi_supcond(double x, double tau)
{
  double y = exp(-x/tau);
  return y / (1.0 + y) ;
}

inline double g_supcond(double x, double Omega)
{ return 1.0 + x*(x + Omega); }

// -----------------------------------------------------------------------
// Table of reduced energy gap vs. T/Tc, from Muhlschlegel (1959).
// Table starts at x = T/Tc = 0.18, and goes in steps of dx = 0.02

static double ratio[42] = {
           1.0,    0.9999, 0.9997, 0.9994, 0.9989, 
           0.9982, 0.9971, 0.9957, 0.9938, 0.9915,
           0.9885, 0.985,  0.9809, 0.976,  0.9704,
           0.9641, 0.9569, 0.9488, 0.9399, 0.9299,
           0.919,  0.907,  0.8939, 0.8796, 0.864, 
           0.8471, 0.8288, 0.8089, 0.7874, 0.764, 
           0.7386, 0.711,  0.681,  0.648,  0.6117, 
           0.5715, 0.5263, 0.4749, 0.4148, 0.3416, 
           0.2436, 0.0
      } ;


// -----------------------------------------------------------------------
// uses ratio[] to calculate superconducting energy gap as a function of temp:

static double gap_supcond(double x)
{
  // the argument x = T/Tc; check if x is physical
  if(x < 0. || x >= 1.) return(0.0);  // return gap of zero

  // if T/Tc < 0.04, reduced energy gap is 1.0, to better than 1 part in 10^22
  if(x < 0.04) return(1.0);  // return gap of 1.0

  // if T/Tc <= xmatch, we use an analytic expression to extend the gap table;
  //   ( which uses k Tc ~= Delta(0)/1.764; c.f. Tinkham )

  static const double xmatch = 0.32;  // xmatch must be a multiple of 0.02, >= 0.18
  static const double ymatch = exp(-sqrt(3.562*xmatch)*exp(-1.764/xmatch));
  if(x <= xmatch) return(exp(-sqrt(3.562*x)*exp(-1.764/x)));  // 3.562==2*Pi/1.764

  // if we get here, we interpolate between data points to calculate gap, taking
  // sqrt(1 - T/Tc) dependence near the gap into account.

  int index = static_cast<int>(floor((x-0.18)/0.02));  // index into ratio[]
  double xl = index*0.02 + 0.18,
         xu = xl + 0.02,
         yl = ((xl > xmatch) ? ratio[index] : ymatch)/sqrt(1.-xl),
         yu = (index < 40) ? ratio[index+1]/sqrt(1.-xu) : 1.74;
              // Behavior near Tc - see Tinkham, eqn. 2-54.

  return sqrt(1 - x) * (yl + (yu-yl)*(x-xl)/(xu-xl));  // linear interp into ratio[]
}




