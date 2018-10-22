// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// ampdata.cc

#include "ampdata.h"
#include "error.h"

using namespace std;

// Construct ampdata from an sdata object, checking that size()=2.
ampdata::ampdata(const sdata &sd) : sdata(sd)
{
  if(sd.size() != 2)
  {
    error::warning(
      "ampdata cannot be constructed from sdata whose size != 2.");
  }
}

// Return the optimum noise match.
// Equations may be found in Scott Wedge's Ph.D. thesis (Caltech) page 27.
complex ampdata::gamma_opt()
{
  if(size() != 2)
  {
    error::warning("ampdata size != 2 in ampdata::gamma_opt().");
    return complex();
  }

  complex d;  // denominator of eta;
  double  n;  // numerator of eta;

  d = real(C[2][2])*S[1][1] - C[1][2]*S[2][1];
  n = real(C[2][2])*(1+norm(S[1][1])) + real(C[1][1])*norm(S[2][1]);
  n -= 2.0 * real(C[1][2]*S[2][1]*conj(S[1][1]));

  double nn = n*n;
  double dd = norm(d);

  if(nn*1e36 < dd) 
    return I*zarg(d);
  else if(nn > dd*1e18)
    return conj(d)/n;
  else if(d == 0.0)
    return complex(0.0);
  else
    return 0.5*n/d*(1.0 - sqrt(complex(1.0 - 4.0*dd/nn)));

//    complex eta;
//    eta = real(C[2][2]);
//    eta += real(C[1][1]) * norm(S[2][1]);
//    eta += real(C[2][2]) * norm(S[1][1]);
//    eta -= C[2][1]*S[1][1]*conj(S[2][1]);
//    eta -= C[1][2]*S[2][1]*conj(S[1][1]);
//    eta /= real(C[2][2])*S[1][1] - C[1][2]*S[2][1];

//    return 0.5*eta*(1.0-sqrt(Complex(1-(4.0/norm(eta)))));

}

// Return the minimum noise temperature.
// Equations may be found in Scott Wedge's Ph.D. thesis (Caltech) page 27.
double ampdata::t_min()
{
  if(size() != 2)
  {
    error::warning("ampdata size != 2 in ampdata::t_min().");
    return 0.0;
  }

  double g = norm(gamma_opt());
  double tmin;

  tmin = real(C[1][1])*norm(S[2][1]) + real(C[2][2])*norm(S[1][1]);
  tmin -= 2.0 * real(C[2][1]*S[1][1]*conj(S[2][1]));
  tmin *= -g;
  tmin += real(C[2][2]);
  tmin /= norm(S[2][1]) * (1.0 + g);

  return tmin;

//    complex tmin;

//    tmin = C[1][1] * norm(S[2][1]);
//    tmin += C[2][2] * norm(S[1][1]);
//    tmin -= C[2][1]*S[1][1]*conj(S[2][1]);
//    tmin -= C[1][2]*S[2][1]*conj(S[1][1]);
//    tmin *= g;
//    tmin *= -1.0;
//    tmin += C[2][2];
//    tmin /= norm(S[2][1]) * (1.0 + g);

//    return real(tmin);
}

// Return the minimum noise figure.
double ampdata::F_min()
{
  if(size() != 2)
  {
    error::warning("ampdata size != 2 in ampdata::F_min().");
    return 0.0;
  }

  return 10.0 * log10(1.0 + t_min()/(290.0*Kelvin));
}

// Return the noise measure.
// Equations may be found in Scott Wedge's Ph.D. thesis (Caltech) page 28.
double ampdata::noise_measure()
{
  if(size() != 2)
  {
    error::warning("ampdata size != 2 in ampdata::noise_measure().");
    return 0.0;
  }

  return real(C[2][2]) / (norm(S[2][1]) + norm(S[2][2]) - 1.0);
}

// Return the noise resistance normalized to z_norm.
// Equations may be found in Scott Wedge's Ph.D. thesis (Caltech) page 27.
double ampdata::Rn()
{
  if(size() != 2)
  {
    error::warning("ampdata size != 2 in ampdata::Rn().");
    return 0.0;
  }

  double r = C[1][1].real - 2*(C[2][1]*(1+S[1][1])/S[2][1]).real 
    + C[2][2].real*norm((1+S[1][1])/S[2][1]);
  r /= 4.0 * 290.0 * Kelvin;
  return r;
}

// Return the determinant of the scattering matrix.
complex ampdata::delta()
{
  if(size() != 2)
  {
    error::warning("ampdata size != 2 in ampdata::delta().");
    return complex();
  }
  return S[1][1]*S[2][2] - S[1][2]*S[2][1];
}

// Return k, which must be > 1 for unconditional stability
double ampdata::k()
{
  if(size() != 2)
  {
    error::warning("ampdata size != 2 in ampdata::k().");
    return 0.0;
  }
  return
  (1.0-norm(S[1][1])-norm(S[2][2])+norm(delta()))/(2.0*abs(S[1][2]*S[2][1]));
}

// Return center of the stability circle.
// Parameter specifies the input or output port.
complex ampdata::center(int p)
{
  if(size() != 2)
  {
    error::warning("ampdata size != 2 in ampdata::center(int).");
    return complex();
  }
  if(p < 1 || p > 2)
  {
    error::warning("port parameter must be 1 or 2 in ampdata::center(int).");
    return complex();
  }

  int q = 3 - p;	// The other port
  complex d = delta();
  return zconj(S[p][p] - d*zconj(S[q][q]))/(norm(S[p][p])-norm(d));
}

// Return radius of stability circle (circle for |gamma| = 1)
// Parameter specifies the input or output port.
double ampdata::radius(int p)
{
  if(size() != 2)
  {
    error::warning("ampdata size != 2 in ampdata::radius(int).");
    return  0.0;
  }
  if(p < 1 || p > 2)
  {
    error::warning("port parameter must be 1 or 2 in ampdata::radius(int).");
    return 0.0;
  }
  int q = 3 - p;	// The other port

  return zabs(S[p][q]*S[q][p]/(norm(S[p][p])-norm(delta())));
}

