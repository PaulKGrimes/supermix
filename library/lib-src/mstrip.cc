// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// **************************************************************************
// mstrip.cc
// contains the definitions of microstrip::mstrip1(), etc.

#include <cmath>
#include "units.h"
#include "trlines.h"
using namespace std;

//    J. Zmuidzinas 11/87, A. Vayonakis & J.Z. 4/04, F. Rice 6/04
//
//    Based on equations given by:
//      Hammerstadt & Jensen (HJ), IEEE MTT-S Int. Microw. Symp. Dig., 1980.
//      Kirschning & Jansen  (KJ), Elecronics Ltrs. 18, 1982.
//      Yassin & Withington  (YW), J. Phys. D: Appl. Phys. 28, 1995.



// ========================================================================
// mstrip1():
// Calculations which depend solely on the thicknesses of the dielectric
// substrate and top strip. Sets members of m1:
// ------------------------------------------------------------------------
void microstrip::mstrip1()
{
  // inputs:
  const double & t1 = i_param.t1;  // thickness of top strip
  const double & h  = i_param.h;   // thickness of substrate


  // outputs:
  const double & t = m1.tnorm = t1/h; // relative thickness of top strip

  // The other outputs will be used in the calculations of the geometric
  // factors g1, g2strip, and g2ground involving the current distributions
  // in the two conductors. These calculations are due to YW and are 
  // completed in mstrip3(). We calculate these now, only if thicknesses
  // change, because they involve trancendental library function calls.

  // p and its square root, sp, are used by YW (see mstrip3()).
  const double temp1 = 1. + t;
  m1.p = 2.*temp1*(temp1 + sqrt(temp1*temp1-1.)) - 1.;
  m1.sp = sqrt(m1.p);

  // L1 and L2 are used to get YW's ra and rb (see mstrip3()).
  const double temp2 = 4./(m1.p - 1.);
  const double temp3 = atanh(1./m1.sp);
  m1.L1 = log(temp2*m1.p) - (m1.p + 1.0)*temp3/m1.sp - 1.;
  m1.L2 = 0.5*(m1.p + 1.)*(1.+ log(temp2)) - m1.sp*2.*temp3;
}


// ========================================================================
// mstrip2():
// Calculations which depend solely on the dielectric epsilons. Sets
// members of m2:
// ------------------------------------------------------------------------
void microstrip::mstrip2()
{
  // inputs: i_param.esub and i_param.eup

  // outputs:
  // m2.erel: the relative eps of substrate wrt superstrate. This value is
  // used often throughout the microstrip calculations.
  const double & e = m2.erel = i_param.esub/i_param.eup;

  // We calculate the following here, only if epsilons change, because they
  // are time-consuming calculations.

  // These are used by HJ calculations of ur and ee1 in mstrip3():
  m2.cosh = 0.5 + 0.5/cosh(sqrt(e-1.));          // used to get ur
  m2.b    = 0.564 * pow((e-0.9)/(e+3.), 0.053);  // used to get ee0

  // These are used by KJ calculations to get epeff in mstrip4():
  m2.p2  = 0.33622 * (1.- exp(-0.03442*e));
  m2.p4  = 1.0 + 2.751*(1.- exp(-pow(e/15.916, 8)));
}


// ========================================================================
// mstrip3():
// Calculations which are affected by any geometry or epsilon changes,
// including the microstrip width. Call after calling mstrip1() or
// mstrip2(). Uses i_param, m1, and m2.  Sets members of m3 and m4:
// ------------------------------------------------------------------------
void microstrip::mstrip3()
{
  // the scaled microstrip width, u = w/h, is used extensively:
  const double & u = m3.u = i_param.w/i_param.h;
  const double & e = m2.erel;
  const double & t = m1.tnorm;

  // These are used by KJ calculations to get epeff in mstrip4():
  m3.p1a = 0.27488 - 0.065683*exp(-8.7513*u);
  m3.p3a = 0.0363*exp(-4.6*u);

  // the lossless effective dielectric constant (m3.ee0) is one of the
  // major calculations performed by mstrip3(); it will be relative to
  // the value of the superstrate dielectric constant i_param.eup:
  double & ee = m3.ee0;

  // HJ:
  // --
  // Calculates the lossless effective dielectric constant (ee) in the
  // zero frequency limit. First we need two "corrected" values of u: u1
  // and ur...
  const double E4   = 4. * 2.71828182845905;    // 4*exp(1.)
  const double coth = 1./tanh(sqrt(6.517*u));
  const double d1   = (t/Pi) * log(1.+ E4/(t*coth*coth));
  const double dr   = m2.cosh * d1;
  const double u1   = u + d1;
  const double ur   = u + dr;   // ur == u1 if e == 1
  const double ur2  = ur*ur;

  // Now we calculate ee using ur as the effective width...
  double a = 1.+ log((ur2 + 1.0/(2704.))/(ur2 + 0.432/ur2))/49.
               + log(1.+(ur*ur2)/5929.741)/18.7;

  ee = 0.5*( (e+1.) + (e-1.)*pow((1.+ 10./ur),(-a*m2.b)) );

  // The lossless char impedance Z0(u) due to HJ is given by:
  //   Z0(u) = 2*Pi*eta0*log(f(u)/u + sqrt(1+4/(u*u))), where:
  //    f(u) = 6 + (2*Pi-6)*exp(-pow(30.666/u , 0.7528))
  // We need z == Z0(u1)/Z0(ur) further correct ee...
  double f  = 6. + (2.*Pi - 6.)*exp(-pow(30.666/u1 , 0.7528));
  double z  = log(f/u1 + sqrt(1.+ 4./(u1*u1)));
         f  = 6. + (2.*Pi - 6.)*exp(-pow(30.666/ur , 0.7528));
         z /= log(f/ur + sqrt(1.+ 4./ur2));
  ee *= z*z;
	
  // YW:
  // --
  // Compute the correct geometric factors for calculation of Z0 (m3.g1)
  // and conductor loss contributions (m4.g2ground, m4.g2strip).

  // First we complete the calculation of ra started in mstrip1()...
  const double  &  p = m1.p;
  const double  & sp = m1.sp;
  const double logra = m1.L1 - 0.5*Pi*u;
  const double    ra = exp(logra);  // if u >> 1, ra could be VERY small.

  // next we need rb, again using results from mstrip1()...
  const double q = 0.5*Pi*u*sp + m1.L2; 
  const double D = (p >= q) ? p : q; //D = max(p,q)
  double rb = q + 0.5*(p + 1.)*log(D);
  if(u < 5.0) {
    double fun1 = sqrt((rb - p)/(rb - 1.));
    double fun2 = atanh(fun1/sp);
    double fun3 = sqrt((rb - 1.)*(rb - p));
    fun1  = atanh(fun1);
    rb += (p + 1.)*fun1 - 2.*sp*fun2 + 0.5*Pi*sp*u - fun3; 
  }

  // now we can finish the calculation of g1...
  // g1 is simply L/Permeability, where L is inductance/length. It is
  // also effectivePermittivity/C, where C is capacitance/length. The
  // L here is "external inductance," due to fields external to the
  // conductors.
  m3.g1 = 0.5*Pi/(log(2*rb)-logra);

  // Finally, compute g2strip, g2ground. The g2's are 1/"effective
  // width" of the two conductors, in the sense that the contribution
  // to the series impedance/length of the transmission line due to a
  // real conductor would be g2*Zsurface. 
  double Ra = (1. - ra)*(p - ra);
  double Is1 = log((2.*p - (p + 1.)*ra + 2.*sqrt(p*Ra))/(p - 1.))-logra;
  double Rb = (rb - 1.)*(rb - p);
  double Is2 = -log(((p + 1.)*rb - 2.*p - 2.*sqrt(p*Rb))/rb/(p - 1.));
  double Rbp = (rb + 1.)*(rb + p);
  double Ig1 = -log(((p + 1.)*rb + 2.*p + 2.*sqrt(p*Rbp))/rb/(p - 1.));
  double Rap = (ra + 1.)*(ra + p);
  double Ig2 = log(((p + 1.)*ra + 2.*p + 2.*sqrt(p*Rap))/(p - 1.))-logra;
  double denom = (m3.g1/i_param.h)/
                 ((u < 2)? (log(rb)-logra) : (log(2.*rb)-logra));

  m4.g2strip  = (Pi + Is1 + Is2)*denom;
  m4.g2ground = (Ig1 + Ig2)*denom;
}


// ========================================================================
// mstrip4():
// Calculations which are directly affected by the frequency in addition to
// the other parameters. Call after calling mstrip3(). Sets remaining
// members of m4:
// ------------------------------------------------------------------------
void microstrip::mstrip4()
{
  // KJ:
  // --
  // Frequency dispersion correction to m3.ee0 gives the lossless effective
  // dielectric constant due to the geometry and dielectrics used.
  // We finish the calculations started in mstrip2() and mstrip3()...
  double fn = i_param.freq*i_param.h/(GHz*Milli*Meter);
  double p1 = m3.p1a + m3.u*(0.6315 + 0.525*pow(1+0.0157*fn, -20.));
  double p3 = m3.p3a*(1.- exp(-pow(fn/38.7, 4.97)));
  double pf = p1*m2.p2*pow((0.1844 + p3*m2.p4)*fn, 1.5763);
  m4.epeff = (m3.ee0 + pf*m2.erel)/(1. + pf);


  // Now we calculate the loss tangent contribution due to dielectric
  // losses. We weight the individual dielectric loss contributions by
  // comparing the epsilons to epsilon effective for the microstrip...
  double q = (m4.epeff - 1.)/(m2.erel - 1.);
  m4.tandel = 0.5 * ((1.- q)*i_param.tdup + q*m2.erel*i_param.tdsub)/m4.epeff;

  // Now fix up epeff; we calculated it relative to eup
  m4.epeff *= i_param.eup ;

  // The lossless characteristic impedance comes from m4.epeff and m3.g1.
  m4.Z0 = ZVacuum * m3.g1 / sqrt(m4.epeff);
}


// ========================================================================
// mstrip5():
// Final calculations of zchar, beta, Zs, and Yp using results in m4 and
// the surface impedances of the conductors. Call after mstrip4() or if
// a surface impedance changes.
// ------------------------------------------------------------------------
void microstrip::mstrip5()
{
  // ========================================================================
  // Now what we have so far in structure m4:
  //
  //  Z0:       Characteristic impedance for ideal, lossless case.
  //  epeff:    Effective dielectric constant for ideal, lossless case. 
  //  tandel:   Microstrip loss tangent for lossy dielectrics but ideal
  //            conductors.
  //  g2strip,
  //  g2ground: Geometric factors for current distributions in conductors
  //
  // We modify these results to include the effects of real conductors
  // (including superconductors).
  //
  // m4.tandel is the dielectric contribution to the loss tangent == de.
  // There will be a conductor contribution == dg. For a hint of what needs
  // to be done, note that the complex propagation factor beta and
  // characteristic impedance Zc (see below) will have the forms (for
  // small de and dg):
  //
  //    beta  ~= j*k*(1 - j*(de+dg))
  //      Zc  ~=  Z0*(1 + j*(de-dg))
  //
  // The equivalent series impedance (Zs) and parallel admittance (Yp) per
  // length are related to beta and Zc as follows:
  //
  //      Zs  == beta*Zc           beta  == sqrt(Zs*Yp)
  //      Yp  == beta/Zc             Zc  == sqrt(Zs/Yp)
  //
  // The appropriate branches of sqrt() must be chosen so the above
  // expressions obtain. In the case of our Complex::sqrt() function, the
  // returned real part is >= 0 and the returned imaginary part has the
  // same sign as the argument imaginary part, along with sqrt(-1) == j.
  //
  // For small de and dg, Zs and Yp would be (note that the real parts of
  // both Yp and Zs are nonnegative for real, passive elements):
  //
  //      Zs  ~= j*(k*Z0)*(1 - 2*j*dg) = (k*Z0)*(2*dg + j)
  //      Yp  ~= j*(k/Z0)*(1 - 2*j*de) = (k/Z0)*(2*de + j)
  //
  // We include the effects of using real conductors as follows:
  //   (1) Use results in m4 to get beta and Zc for ideal conductor case.
  //   (2) Get initial Zs and Yp from these values of beta and Zc.
  //   (3) Modify Zs by adding the nonzero surface impedances of the
  //       conductors weighted by g2strip and g2ground. Assume Yp is not
  //       affected.
  //   (4) Use Yp and the new Zs to calculate new values for beta and Zc.
  // ------------------------------------------------------------------------
  
  // The complex propagation factor beta is defined so that the complex
  // wave amplitude varies as exp(-beta*length), which  is consistent with
  // the exp(+j omega t) time dependence of the phase. With this definition,
  // beta = j*k*(1 - j*tan(delta)). Initially k comes from m4.epeff, and
  // tan(delta) = de = m4.tandel, so initial beta = j*k*(1 - j*de).
  const double    k = 2.*Pi*i_param.freq*sqrt(m4.epeff)/cLight;
  const double & de = m4.tandel;

  // The initial characteristic impedance Zc requires an imaginary part > 0 if
  // the dielectric is lossy. m4.Z0 is the lossless characteristic impedance
  // of the line. Complex Zc == Zo/(1 - j*de) == Zo*(1 + j*de)/(1 + de*de)
  const double & Zo = m4.Z0;

  // Calculate equivalent series and shunt elements. Since we've assumed
  // ideal conductors and only dielectric loss so far, we must have
  // Zs pure imaginary, and Yp with a nonnegative real part.
  // With Zs == beta*Zc and Yp == beta/Zc, we get:
  Zs = Complex(0., k*Zo);
  Yp = (k/Zo)*Complex(2*de, 1.- de*de);


  // Calculate additional contribution to Zs due to conductor surface
  // impedances:
  Zs += m4.g2strip*i_param.Ztop + m4.g2ground*i_param.Zground;

  // Note: Re(Zsurf) >= 0 since metal films are passive, so this
  // implies  Re(Zs) >= 0 also.

  beta = sqrt(Zs*Yp);
  // Since sqrt() always returns real part > 0, and since Zs and Yp both
  // have imaginary parts >= 0, beta will have both real and imaginary
  // parts > 0, as required.

  zchar = sqrt(Zs/Yp);
  // The real part of zchar will be > 0. The sign of the imaginary part
  // depends on whether dielectric or conductor losses dominate.
}
