// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// sdata_interp.cc

#include "sdata_interp.h"

using namespace std;

S_interp & S_interp::add_S(double f, const Matrix & S, double Zn)
{
  if((S.Lmode != Index_1) && (S.Rmode != Index_1) && (S.Lsize != N) && (S.Rsize != N)) {
    error::warning("S_interp::add_S(): improperly sized S matrix argument");
    return *this;
  }

  if(Znorm_ <= 0.0) Znorm_ = device::Z0;
  if(Zn == Znorm_ || Zn <= 0.0)
    s.add(f,S);
  else
    s.add(f,S_renormalize(S,Znorm_,Zn));
  return *this;
}


S_interp & S_interp::add_S(double f, const sdata & Sd)
{
  add_S(f, Sd.S, Sd.get_znorm());
  return *this;
}


S_interp & S_interp::add_SC(double f, const sdata & Sd)
{
  if(Sd.size() != N || Sd.mode() != Index_1) {
    error::warning("S_interp::add_SC(): incompatible sdata argument");
    return *this;
  }

  if(Sd.get_znorm() == Znorm_ || Sd.get_znorm() == 0) {
    // no renormalization required
    s.add(f, Sd.S);
    c.add(f, Sd.C);
  }
  else {
    // must renormalize sdata first
    sdata temp(Sd, Znorm_);
    s.add(f, temp.S);
    c.add(f, temp.C);
  }
    
  noise_ = true;
  return *this; 
}


S_interp & S_interp::add_SC(double f, const Matrix & S, const Matrix & C, double Zn)
{
  if((S.Lmode != Index_1) && (S.Rmode != Index_1) && (S.Lsize != N) && (S.Rsize != N)) {
    error::warning("S_interp::add_SC(): improperly sized S matrix argument");
    return *this;
  }
  if((C.Lmode != Index_1) && (C.Rmode != Index_1) && (C.Lsize != N) && (C.Rsize != N)) {
    error::warning("S_interp::add_SC(): improperly sized C matrix argument");
    return *this;
  }

  if(Znorm_ <= 0.0) Znorm_ = device::Z0;
  if(Zn == Znorm_ || Zn <= 0.0) {
    // renormalization not needed - just add the matrices to the interpolators
    s.add(f,S);
    c.add(f,C);
  }
  else {
    // need to renormalize first; use an sdata to accomplish this
    sdata temp(N);
    temp.set_znorm(Zn);
    temp.S = S;
    temp.C = C;
    temp.change_norm(Znorm_);
    s.add(f,temp.S);
    c.add(f,temp.C);
  }    

  return *this;
}


bool S_interp::touchstone(const char * name, double f_scale)
{
  touchstone_read d;
  if(!d.open(name, N, f_scale)) return false;

  if(Znorm_ <= 0.0) Znorm_ = device::Z0;
  double f;
  Matrix S;

  // read in the S matrix data and rebuild the S interpolator
  while(d.Svalue(f,S)) add_S(f,S);
  s.build();

  // if a 2-port, there may also be noise data
  if(N == 2 && d.has_noise()) {
    noise_ = true;

    // here's where we fetch and convert the noise data:
    Matrix C(2);
    touchstone_read::noise N;
    double To = 290.0*Kelvin;
    double Zo = device::Z0;
    while(d.get_noise(f,N)) {
      S = s(f);
      // renormalize S to device::Z0 if necessary
      if (Znorm_ != Zo) S = S_renormalize(S,Zo,Znorm_);

      double Tmin = To * (pow(10.0, N.Fmin/10.0) - 1.0); // noise figure to T
      double t = (4.0*To*N.Reff/Zo)/norm(1 + N.Gopt);  // an intermediate calc

      C[1][1] = Tmin*(norm(S[1][1]) - 1) + t*norm(1 - S[1][1]*N.Gopt);
      C[2][2] = norm(S[2][1])*(Tmin + t*norm(N.Gopt));
      C[1][2] = zconj(S[2][1])*(S[1][1]*(Tmin + t*norm(N.Gopt)) - t*zconj(N.Gopt));
      C[2][1] = zconj(C[1][2]);

      // if necessary renormalize C to Znorm_
      if (Znorm_ != Zo) {
	Matrix F = identity_matrix(2,Index_1);
	double rho = sqrt(Znorm_ / Zo) ;
	double Sigma = 0.5*(1./rho + rho) ;
	double Delta = 0.5*(1./rho - rho) ;
	F = Sigma*F - Delta*S;
	C = F*C*dagger(F);
      }

      // put result into the interpolation
      c.add(f,C);

    } // while()
    c.build();

  } // if

  return d.good() && s.ready() && (!noise_ || c.ready());
}


// ********************************************************************

sdata_interp::sdata_interp(int ports, const abstract_real_parameter & f)
  : nport(ports), pf(&f), S(ports)
{ 
  if(ports <= 0) error::fatal("sdata_interp: must be constructed with ports > 0");
  info.source = false;
}


bool sdata_interp::copy(const S_interp & s)
{
  if (s.ports() != size()) {
    error::warning("sdata_interp::copy(): argument has incorrect number of ports.");
    return false;
  }
  else {
    S = s;
    return true;
  }
}


void sdata_interp::recalc_S()
{
  if (!ready()) {
    error::warning("sdata_interp::recalc(): interpolator is empty or not ready");
    return;
  }

  // perform the interpolation and put in nport's sdata object
  if(S.fill(data,*pf)) {
    // data.C has been filled; adjust normalization and return
    data.change_norm(device::Z0);
  }
  else {
    // only data.S has been filled; data.C has stale values
    if(data.get_znorm() != device::Z0) {
      data.S = S_renormalize(data.S, device::Z0, data.get_znorm());
      data.set_znorm(device::Z0);
    }
  }
}

void sdata_interp::recalc()
{
  // recalc_S() also interpolates C matrix if available
  recalc_S();

  // if recalc_S didn't generate a C matrix, then generate a passive one
  if(!S.has_noise()) data.passive_noise(device::f, device::T);
}


// ********************************************************************

void S_to_sdata(sdata & SD, const Matrix & S, double Zn)
{
  SD.S = S;
  SD.set_znorm(Zn);
  SD.passive_noise(device::f, device::T);
  SD.B = 0.0;
}


Matrix S_renormalize(const Matrix & S, double Znew, double Zold)
{
  if(Znew == Zold)
    return S;
  else {
    // assumes S is square with indexing mode Index_1
    int N = S.Lmaxindex();
    double r = (Zold - Znew)/(Zold + Znew);
    Matrix i = identity_matrix(N);
    return solve((i + r*S), (r*i + S));
  }
}
