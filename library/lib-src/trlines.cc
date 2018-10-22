// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// **************************************************************************
// trlines.cc

#include <cmath>
#include "global.h"
#include "units.h"
#include "SIScmplx.h"
#include "surfaceZ.h"
#include "parameter.h"
#include "nport.h"
#include "mstrip.h"
#include "error.h"
#include "trlines.h"


// ************************************************************************
//
// class trl_base
//
// ************************************************************************

// calc calculates the scattering matrix
void trl_base::calc(complex b, complex z, double length, double znorm)
{
  // Note: propagation factor is taken to be exp(-b*length)
  // so b.real > 0 ensures attenuation of wave
  //
  // the form of the scattering matrix elements used requires only one
  // complex exponential evaluation:
  //
  //       (z*z - 1)*(1 - exp(-k)*exp(-k))           4*z*exp(-k)
  // s11 = -------------------------------  ;  s12 = -----------
  //                 denom                              denom
  //
  // denom = (z+1)*(z+1) - (z-1)*(z-1)*exp(-k)*exp(-k)
  //
  // k = b*length;    z = z/znorm
  //
  // NOTE: if length == 0.0, then s11 = 0 and s12 = 1 will be returned

  complex s11(0.0), s12(1.0);   // the values if length == 0

  if(length != 0.0) {
    z /= znorm;                // normalize to standard impedance
    b *= -length; b = exp(b);   // now b = exp(-k)
    s12 = 4*z*b;                //   s12 = 4*z*exp(-k)
    b *= b;                     // now b = exp(-2*k)
    s11 = z + 1;                // use s11 as a temporary for now
    z -= 1;
    complex denom = s11*s11 - z*z*b;
    s12 /= denom;
    s11 *= z*(1-b)/denom;
  }

  // fill in S matrix
  data.set_znorm(znorm);        // added 12/29/97
  data.S[1][1] = s11;
  data.S[2][2] = s11;
  data.S[1][2] = s12;
  data.S[2][1] = s12;
}

double trl_base::wavelength(complex prop_constant)
{
   return(2.*Pi/imag(prop_constant));
}

double trl_base::percent_loss(complex prop_constant)
{
   double alpha = real(prop_constant);
   double wvlen = wavelength(prop_constant);
   return( 100.*(1.-exp(-2.*alpha*wvlen)) );
}

// ************************************************************************
//
// class microstrip:
//
// ************************************************************************

// Default constructor
microstrip::microstrip()
  : trl_base(), length(0.), width(0.), sub_thick(0.), Temp(&device::T),
    sub(0), super(0), top(0), ground(0)
{ }

void microstrip::update(double freq, double T)
{
  // Make sure we have valid pointers
  if(sub == 0)
    error::fatal("Substrate dielectric not defined for microstrip line !");
  if(super == 0)
    error::fatal("Superstrate dielectric not defined for microstrip line !");
  if(ground == 0)
    error::fatal("Ground plane film not defined for microstrip line !");
  if(top == 0)
    error::fatal("Top strip film not defined for microstrip line !");
  if(length.get() < 0.)
    error::fatal("Microstrip length is negative !");
  if(width.get() <= 0.)
    error::fatal("Microstrip width is zero or negative !");
  if(sub_thick.get() <= 0.)
    error::fatal("Microstrip substrate thickness is zero or negative !");

  // These booleans are used to determine which of the mstrip() fcns need to
  // be called.
  bool c1, c2, c3, c4, c5;
  c1 = c2 = c3 = c4 = c5 = false;

  // set up i_param members and the above booleans...
  Complex ctemp; double dtemp;
  
  // Check if a call to mstrip1() is needed...
  dtemp =  top->thickness();
  if(dtemp != i_param.t1) {
    i_param.t1 = dtemp;
    c1 = c3 = true;
  }

  dtemp = sub_thick.get();
  if(dtemp != i_param.h) {
    i_param.h = dtemp;
    c1 = c3 = true;
  }

  // Check if a call to mstrip2() is needed...
  ctemp =  sub->epsilon(freq, T);
  if (ctemp != i_param.Esubc) {
    if(imag(ctemp) > 0. || real(ctemp) < 1.0)
      error::fatal("Microstrip Substrate has unphysical"
		   " complex dielectric constant !");
    i_param.Esubc = ctemp;
    i_param.esub  = real(ctemp) ;
    i_param.tdsub = -imag(ctemp)/i_param.esub;
    c2 = c3 =true;
  }
  ctemp =  super->epsilon(freq, T);
  if (ctemp != i_param.Eupc) {
    if(imag(ctemp) > 0. || real(ctemp) < 1.0)
      error::fatal("Microstrip Superstrate has unphysical"
		   " complex dielectric constant !");
    i_param.Eupc = ctemp;
    i_param.eup  = real(ctemp) ;
    i_param.tdup = -imag(ctemp)/i_param.eup;
    c2 = c3 = true;
  }

  // Check if a call to mstrip3() is needed...
  dtemp = width.get();
  if(dtemp != i_param.w) {
    i_param.w = dtemp;
    c3 = true;
  }

  // Check if a call to mstrip4() is needed...
  if(freq != i_param.freq) {
    i_param.freq = freq;
    c4 = true;
  }
    
  // Check if a call to mstrip5() is needed...
  ctemp = top->Zsurf(freq, T);
  if (ctemp != i_param.Ztop) {
    if(real(ctemp) < 0.)
    error::fatal("Microstrip Top Conductor has"
		 " real(surface Z) < 0 !");
    i_param.Ztop = ctemp;
    c5 = true;
  }
  ctemp = (ground == top)? i_param.Ztop : ground->Zsurf(freq, T);
  if (ctemp != i_param.Zground) {
    if(real(ctemp) < 0.)
    error::fatal("Microstrip Ground Conductor has"
		 " real(surface Z) < 0 !");
    i_param.Zground = ctemp;
    c5 = true;
  }

  // call mstrip() functions
  if (c1)        mstrip1();
  if (c2)        mstrip2();
  if (c3)      { mstrip3(); mstrip4(); mstrip5(); }
  else if (c4) { mstrip4(); mstrip5(); }
  else if (c5)   mstrip5();
}


// ************************************************************************
//
// class cpw
//
// ************************************************************************

double cpw::cefr(double k)
{
  double power = 1;

  if(k < 0.7071067812)
  {
    k = sqrt(1 - k*k);
    power = -1;
  }

  double sqrt_k = sqrt(k);
  double rtval = log(2. * (1.+sqrt_k) / (1 - sqrt_k)) / Pi;	// Natural log.

  if (power == 1) return rtval;

  return 1.0 / rtval;
}


// constructor
cpw::cpw()
  : trl_base(), length(0.), width(0.), space(0.), sub_thick(0.), Temp(&device::T),
    sub(0), top(0), bottom(0)
{ }

void cpw::update(double freq, double /* T */ )  // lossless, so T isn't needed yet
{
  // Make sure we have valid pointers
   if(sub == 0)
     error::fatal("Substrate dielectric not defined for cpw line !");
   if(top == 0)
     error::fatal("Top strip film not defined for cpw line !");
   if(length.get() < 0.)
     error::fatal("cpw length is negative !");
   if(width.get() <= 0.)
     error::fatal("cpw width is zero or negative !");
   if(space.get() <= 0.)
     error::fatal("cpw space is zero or negative !");
   if(sub_thick.get() <= 0.)
     error::fatal("cpw substrate thickness is zero or negative !");

  // The following variables are used to make the math simpler.
  // See "Microwave Solid State Circuit Design" by Bahl and Bhartia, page 24.
  double a, b, k, k1;

  a = width /2.;
  b = space + a;
  k = a / b;

  double epsilon_effective;

  if(bottom == 0)  // No ground plane under substrate
  {
    k1 = sinh(0.5 * Pi * a / sub_thick) / sinh(0.5 * Pi * b / sub_thick);

    epsilon_effective = 1. + (0.5 * (sub->epsilon(freq, Temp).real - 1.)
                * cefr(k1) / cefr(k));
    zchar = 30. * Pi / (sqrt(epsilon_effective) * cefr(k));
  }
  else  // There is a ground plane under substrate
  {
    k1 = tanh(0.5 * Pi * a / sub_thick) / tanh(0.5 * Pi * b / sub_thick);

    epsilon_effective = (1. + sub->epsilon(freq, Temp).real * cefr(k1)/cefr(k))
                    / (1. + cefr(k1) / cefr(k));

    zchar = 60. * Pi / (sqrt(epsilon_effective) * (cefr(k) + cefr(k1)));
  }

  //
  // Convert to equivalent series and shunt elements
  //
  double wavelen = cLight/(freq*sqrt(epsilon_effective));
  double kimag = fabs(2.*Pi/wavelen);  // make sure we know signs

  beta = complex(0., kimag);   // lossless
  Zs = beta*zchar;            // guaranteed that Re(Zs) > 0
  Yp = beta/zchar;            // guaranteed that Re(Yp) > 0

/* This is the lossy microstrip code... needs to be checked for cpw
// WHAT IS ALPHA?  (attenuation factor)
// WHAT IS g2?  (geometrical factor for loss calculations)

   double kreal = fabs(alpha);

   if(kreal < MSTRIP_H_VERYSMALL*kimag)
      kreal = MSTRIP_H_VERYSMALL*kimag; // always keep tiny amount of loss
   // 
   // Propagation factor is defined as exp(-beta*length)
   // This is consistent with exp(+j omega t) time dependence
   //
   beta = complex(kreal, kimag);       // uncorrected propagation const
   Zs = beta*zchar;                    // guaranteed that Re(Zs) > 0
   Yp = beta/zchar;                    // guaranteed that Re(Yp) > 0

   //
   // Calculate corrections due to superconductor

   complex Zsurf = top->Zsurf(freq, T);
   Zs += g2*Zsurf;           // corrected series impedance per length

   // Note: Re(Zsurf) > 0 should be true since metal films are passive
   // This implies Re(Zs) > 0 still true
   // So corrected propagation constant and characteristic impedance
   // should still satisfy Re(beta) > 0 and Re(zchar) > 0

   beta = sqrt(Zs*Yp);
   if(real(beta) < 0.)
     beta *= -1.;   // choose other branch to make wave attenuate

   zchar = sqrt(Zs/Yp);
   if(real(zchar) < 0.)
     zchar *= -1.;   // choose other branch
*/
}

// ************************************************************************
//
// class r_waveguide
//
// ************************************************************************

void r_waveguide::update(double freq, double T)
{
  // Make sure we have valid parameters
  if(length.get() < 0.)
    error::fatal("r_waveguide length is negative !");
  if(a.get() <= 0.)
    error::fatal("r_waveguide \"a\" dimension is zero or negative !");
  if(b.get() <= 0.)
    error::fatal("r_waveguide \"b\" dimension is zero or negative !");

  // the material characteristics:
  complex eps   = (fill_) ? fill_->epsilon(freq,T) : 1.0;    // vacuum is default
  double  Rs    = (wall_) ? wall_->Zsurf(freq,T).real : 0.0; // perfect is default
  double  n     = sqrt(eps.real);                            // index of refraction
  
  // wave propagation characteristics:
  double  Zfree = ZVacuum/n;                // dielectric wave impedance
  double  k   = 2*Pi*freq*n/cLight;         // dielectric wave number
  double  kc  = Pi/a;                       // cutoff wave number
  bool imaginary_beta = (k <= kc);          // are we at or below cutoff?
  double beta = (imaginary_beta) ?          // wavenumber in guide
    sqrt(kc*kc - k*k) : sqrt(k*k - kc*kc);  // gamma = alpha + I*beta

  // handle above cutoff case first:
  if(!imaginary_beta) {
    // these loss calculations are only accurate if small compared to beta
    double alpha_c = (wall_) ?
      Rs*(2*b*Pi*Pi/(a*a*a) + k*k)/(b*k*Zfree*beta) : 0.0;
    double alpha_d = (fill_) ?
      -k*k*eps.imaginary/(2*eps.real*beta) : 0.0; // eps.imaginary <= 0

    gamma = complex(alpha_c+alpha_d, beta);
    zwave = (k*Zfree/beta)*complex(1.0, (alpha_d - alpha_c*k/beta)/beta);
  }
  else {
    // don't include loss calulations below cutoff; too hard to figure out
    gamma = beta;
    zwave = complex(0.0, -k*Zfree/beta);
  }
}

double r_waveguide::fc(double /*freq*/, double T)
{
  if ( a <= 0.0)
    error::fatal("r_waveguide \"a\" dimension is zero or negative !");

  double f = cLight/(2.0*a);
  if(fill_) f /= sqrt((fill_->epsilon(f,T)).real);
  return f;
}

const nport::data_info & r_waveguide::get_data_info()
{
  info.noise  = (fill_ || wall_); // a real material gives noise
  info.active = (info.noise && Temp != device::T);
  return info;
}
