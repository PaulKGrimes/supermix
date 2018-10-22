// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// sdata.cc

#include "units.h"
#include "error.h"
#include "nport.h"
#include "Amath.h"
#include <cmath>

using namespace std;

//
// passive_noise_temp()
//

double passive_noise_temp(double freq, double Temp)
{
  if(Temp < 0.0)
  {
    error::warning("Can't compute passive noise for negative temperature.");
    Temp = -Temp;
  }

  if(freq < 0.0)
  {
    error::warning("Can't compute passive noise for negative frequency.");
    freq = -freq;
  }

  if(Temp < Tiny) Temp = Tiny;
  double h_f_over_2_kB = 0.5 * hPlanck * freq / BoltzK;
 
  // in the limit freq -> 0, d is simply the temperature:
  return (freq == 0.0) ? Temp : h_f_over_2_kB / tanh(h_f_over_2_kB / Temp);
}  


//
// sdata member functions
//

sdata::sdata(int s, v_index_mode t) :
  z_norm(0.0), S(s,t), C(s,t), B(s,t)  // if s < 0, size 0 objects are created (cf table.cc)
{
  if(s<0) error::fatal("Cannot create sdata for negative number of ports.");
}

sdata::sdata(const sdata &sd) :
  z_norm(sd.z_norm), S(sd.S), C(sd.C), B(sd.B)
{ }

sdata::sdata(const sdata &sd, double z0) :
  z_norm(sd.z_norm), S(sd.S), C(sd.C), B(sd.B)
{ change_norm(z0); }

sdata::sdata(const zdata &zd, double z0) :
  z_norm(z0), S(0), C(0), B(0)
{
  S.resize(zd.Z); C.resize(zd.Z); B.resize(zd.Vs);

  if(z0 <= 0.) {
    error::warning(
    "Zero or negative normalization impedance passed to sdata constructor !");
    z_norm = z0 = device::Z0 ;   // just use default normalization
  }
  if(S.Lmaxindex() != S.Rmaxindex()) {
    error::fatal(
    "sdata constructor: zdata::Z matrix must be square.");
  }

  // use special fast matrix math subroutines
  Matrix Temp; Temp.resize(zd.Z);
  IplusM(Temp, z_norm, zd.Z);  // Temp = Z + z_norm*I

  IplusM(S, -z_norm, zd.Z);    // S = Z - z_norm*I (temporarily)
  S = solve(Temp, S);          // S = (Z + z_norm*I)^(-1) (Z - z_norm*I)

  B = solve(Temp, zd.Vs);
  B *= sqrt(z_norm);           // B = sqrt(z_norm) (Z + z_norm*I)^(-1) Vs

  IminusM(Temp, 1, S);         // Temp = I - S
  MAMdagger(C, Temp, zd.C);    // C = (I - S) * Cz * dagger(I - S)
  C *= 1.0/(4*z_norm*BoltzK);
}

sdata::sdata(const ydata &yd, double z0) :
  z_norm(z0), S(0), C(0), B(0)
{
  S.resize(yd.Y); C.resize(yd.Y); B.resize(yd.Is);

  if(z0 <= 0.) {
    error::warning(
    "Zero or negative normalization impedance passed to sdata constructor !");
    z_norm = device::Z0 ;
  }
  if(S.Lmaxindex() != S.Rmaxindex()) {
    error::fatal(
    "sdata constructor: ydata::Y matrix must be square.");
  }

  // use special fast matrix math subroutines
  Matrix Temp; Temp.resize(yd.Y);
  IplusM(Temp, 1/z_norm, yd.Y);  // Temp = I/z_norm + Y

  IminusM(S, 1/z_norm, yd.Y);    // S = I/z_norm - Y (temporarily)
  S = solve(Temp, S);            // S = (I/z_norm + Y)^(-1) (I/z_norm - Y)

  B = solve(Temp, yd.Is);
  B *= -1.0/sqrt(z_norm);        // B = -sqrt(z_norm) (I + z_norm Y)^(-1) Is

  IplusM(Temp, 1, S);            // Temp = I + S
  MAMdagger(C, Temp, yd.C);      // C = (I + S) * Cy * dagger(I + S)
  C *= z_norm/(4*BoltzK);
}

sdata::sdata(const ydata_ptr &yp, double z0) :
  z_norm(z0), S(0), C(0), B(0)
{
  if(z0 <= 0.) {
    error::warning(
    "Zero or negative normalization impedance passed to sdata constructor !");
    z_norm = device::Z0 ;
  }
  if(yp.pY == 0) {
    error::warning(
    "Initializing sdata object from an uninitialized ydata_ptr object !");
    return;
  }

  S.resize(*(yp.pY)); C.resize(*(yp.pY)); B.reallocate(yp.pY->Rsize, yp.pY->Rmode);
  if(S.Lmaxindex() != S.Rmaxindex()) {
    error::fatal(
    "sdata constructor: ydata_ptr::pY must point to a square matrix.");
  }

  Matrix Temp; Temp.resize(S);
  IplusM(Temp, 1/z_norm, (*yp.pY));  // Temp = I/z_norm + Y

  IminusM(S, 1/z_norm, (*yp.pY));    // S = I/z_norm - Y (temporarily)
  S = solve(Temp, S);                // S = (I/z_norm + Y)^(-1) (I/z_norm - Y)

  if(yp.pIs) {
    B = solve(Temp, (*yp.pIs));
    B *= -1.0/sqrt(z_norm);          // B = -sqrt(z_norm) (I + z_norm Y)^(-1) Is
  }

  if(yp.pC) {
    IplusM(Temp, 1, S);              // Temp = I + S
    MAMdagger(C, Temp, (*yp.pC));    // C = Temp * C * dagger(Temp)
    C *= z_norm/(4*BoltzK);
  }
}

sdata & sdata::resize(int n)
{
  if(n < 0) {
    error::warning(
    "Cannot resize sdata to a size < 0. Using size = 0 !") ;
    n = 0;
  }
  S.resize(n); C.resize(n); B.resize(n);
  return *this;
}

double sdata::SdB(int i, int j) const
{
  return 10.0 * log10(norm(S.read(i, j)));
}

double sdata::tn(int i, int j) const
{
  return abs(C.read(i, i)) / norm(S.read(i, j));
}

double sdata::NF(int i, int j) const
{
  return 10.0 * log10(1.0 + tn(i, j)/(290.0*Kelvin));
}

sdata & sdata::set_znorm(double z)
{
  if(z < 0.) {
    error::warning(
    "Cannot set sdata to a normalization impedance < 0. Using absolute value !") ;
    z = -z;
  }

  z_norm = z;
  return *this;
}

sdata & sdata::change_norm(double new_z0)
{
  if(new_z0 <= 0.) {
    error::warning(
    "Cannot change_norm sdata to a normalization impedance <= 0. !") ;
    return *this;
  }
  if((new_z0 == z_norm)||(z_norm == 0.0))
    return *this;   // no action required

  // if we get here, then new_z0 > 0 and new_z0 != z_norm != 0
  if(S.Lmaxindex() != S.Rmaxindex()) {
    error::fatal(
    "sdata::change_norm(): S matrix must be square.");
  }

  double rho = sqrt(new_z0/z_norm) ;
  z_norm = new_z0 ;
  double Sigma = 0.5*(1./rho + rho) ;
  double Delta = 0.5*(1./rho - rho) ;

  Matrix I = identity_matrix(S) ;
  Matrix Factor = Sigma*I - Delta*S ;
  B = Factor * B ;
  C = Factor * C * dagger(Factor) ;
  Factor += (2*Delta) * S ;    // now Factor == Sigma*I + Delta*S
  S = solve(Factor, Delta*I + Sigma*S);
  return *this;
}


sdata & sdata::passive_noise(double freq, double Temp)
{
  if(Temp < 0.0)
  {
    error::warning("Can't compute passive noise for negative temperature.");
    Temp = -Temp;
  }

  if(freq < 0.0)
  {
    error::warning("Can't compute passive noise for negative frequency.");
    freq = -freq;
  }

  if(Temp < Tiny) Temp = Tiny;
  double h_f_over_2_kB = 0.5 * hPlanck * freq / BoltzK;
 
  // in the limit freq -> 0, d is simply the temperature:
  double d = (freq == 0.0) ? Temp : h_f_over_2_kB / tanh(h_f_over_2_kB / Temp);
  // C = d*(I - S*dagger(S));  this is calculated in the code that follows:

  // size the C matrix and set some index limits
  int min = S.Lminindex(), max = S.Lmaxindex();
  if ((min != S.Rminindex())||(max != S.Rmaxindex()))
    // then S isn't square
    error::fatal("S matrix isn't square in sdata::passive_noise.");
  C.resize(S);
  int len = max - min + 1; len = (len < 0) ? 0 : len;   // length of a row of S
  complex *ai, *aj;
  double neg_d = -d;
  for (int i = min; i <= max; ++i) {
    ai = & S[i][min];
    C[i][i] = d*(complex(1) - Adot(ai,ai,len));
    for (int j = i + 1; j <= max; ++j) {
      aj = & S[j][min];
      C[j][i] = conj( C[i][j] = neg_d * Adot(aj,ai,len) );
    }
  }

  return *this;
}

//*************************************************************
// zdata member functions
//

zdata::zdata(int s, v_index_mode t) :
  Z(s,t), C(s,t), Vs(s,t)    // vector and table classes handle s < 0 gracefully
{
  if(s<0) error::fatal("Cannot create zdata for negative number of ports.");
}

zdata::zdata(const zdata &zd) :
  Z(zd.Z), C(zd.C), Vs(zd.Vs)
{ }

zdata::zdata(const sdata &sd) :
  Z(0), C(0), Vs(0)
{
  double z0 = sd.get_znorm() ;
  if(z0 < 0.) {
    error::warning(
    "Normalization impedance < 0. in sdata to zdata constructor!");
  }
  else {
    if (z0 == 0.0) z0 = device::Z0;  // z0 == 0 means impedance doesn't matter
    Matrix I = identity_matrix(sd.size(),sd.mode()) ;
    Matrix Temp = inverse(I - sd.S) ;
    if (Temp.maxindex() < sd.size()) {
      // then (I - sd.S) is singular;
      error::warning(
      "Impedance representation does not exist in sdata to zdata constructor!");
    }
    else {
      Z = z0*(Temp * (I + sd.S)) ;
      C = (4.0 * BoltzK *z0)*(Temp * sd.C * dagger(Temp)) ;
      Vs = (2.0 * sqrt(z0))*(Temp * sd.B) ;
    }
  }
}

zdata::zdata(const ydata &yd) :
  Z(0), C(0), Vs(0)
{
  Z = inverse(yd.Y) ;
  if (Z.maxindex() < yd.size()) {
    // then yd.Y is singular;
    error::warning(
    "Impedance representation does not exist in ydata to zdata constructor!");
  }
  else {
    C = Z * yd.C * dagger(Z) ;
    Vs = -Z * yd.Is ;
  }
}

zdata & zdata::passive_noise(double freq, double Temp)
{
  if(Temp < 0.0)
  {
    error::warning("Can't compute passive noise for negative temperature.");
    Temp = -Temp;
  }

  if(freq < 0.0)
  {
    error::warning("Can't compute passive noise for negative frequency.");
    freq = -freq;
  }

  if(Temp < Tiny) Temp = Tiny;
  double h_f_over_2 = 0.5 * hPlanck * freq ;
  double h_f_over_2_kB = h_f_over_2 / BoltzK;
 
  // in the limit freq -> 0, d is simply the temperature:
  double d = (freq == 0.0) ? BoltzK*Temp : 
                             h_f_over_2 / tanh(h_f_over_2_kB / Temp);

  C = (2.0 * d) * (Z + dagger(Z)) ;
  return *this;
}


//*************************************************************
// ydata member functions
//

ydata::ydata(int s, v_index_mode t) :
  Y(s,t), C(s,t), Is(s,t)
{
  if(s<0) error::fatal("Cannot create ydata for negative number of ports.");
}

ydata::ydata(const ydata &yd) :
  Y(yd.Y), C(yd.C), Is(yd.Is)
{ }

ydata::ydata(const sdata &sd) :
  Y(0), C(0), Is(0)
{
  double z0 = sd.get_znorm() ;
  if(z0 < 0.) {
    error::warning(
    "Normalization impedance < 0. in sdata to ydata constructor!");
  }
  else {
    if (z0 == 0.0) z0 = device::Z0;  // z0 == 0 means impedance doesn't matter
    Matrix I = identity_matrix(sd.size(),sd.mode()) ;
    Matrix Temp = inverse(I + sd.S) ;
    if (Temp.maxindex() < sd.size()) {
      // then (I + sd.S) is singular;
      error::warning(
      "Admittance representation does not exist in sdata to ydata constructor!");
    }
    else {
      Y = (1.0 / z0)*(Temp * (I - sd.S)) ;
      C = (4.0 *BoltzK / z0)*(Temp * sd.C * dagger(Temp)) ;
      Is = (-2.0 / sqrt(z0))*(Temp * sd.B) ;
    }
  }
}

ydata::ydata(const zdata &zd) :
  Y(0), C(0), Is(0)
{
  Y = inverse(zd.Z) ;
  if (Y.maxindex() < zd.size()) {
    // then Y is singular;
    error::warning(
    "Admittance representation does not exist in zdata to ydata constructor!");
  }
  else {
    C = Y * zd.C * dagger(Y) ;
    Is = -Y * zd.Vs ;
  }
}

ydata & ydata::passive_noise(double freq, double Temp)
{
  if(Temp < 0.0)
  {
    error::warning("Can't compute passive noise for negative temperature.");
    Temp = -Temp;
  }

  if(freq < 0.0)
  {
    error::warning("Can't compute passive noise for negative frequency.");
    freq = -freq;
  }

  if(Temp < Tiny) Temp = Tiny;
  double h_f_over_2 = 0.5 * hPlanck * freq ;
  double h_f_over_2_kB = h_f_over_2 / BoltzK;
 
  // in the limit freq -> 0, d is simply the temperature:
  double d = (freq == 0.0) ? BoltzK*Temp : 
                             h_f_over_2 / tanh(h_f_over_2_kB / Temp);

  C = (2.0 * d) * (Y + dagger(Y));
  return *this;
}
