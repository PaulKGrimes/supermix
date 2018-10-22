// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// sisdevice.cc

#include "sisdevice.h"
#include "error.h"
#include "units.h"
#include <cmath>   // for double tanh(), fabs()

using namespace std;

static const double deps = 1.0e-6;  // normalized voltage close to 0.0

// --------------------------------------------------------------------
// The constructor sets up things so that large_signal() will throw an
// error if all set-up is incomplete.

sis_basic_device::sis_basic_device()
  : Vn(0), Rn(0), Cap(0), piv(0), iv_data_ok(0)
{ }


// --------------------------------------------------------------------
// The large-signal analysis routine calculates a vector of currents
// from the supplied vector of voltages.  Only currents with frequencies
// <= max_harmonics * fLO will be returned.  Calculates the convolution
// constants Ck, then convolves them using the SIS IV curve to get the
// currents. Many of sis_basic_device's private variables are set by this
// function.

const Vector & sis_basic_device::large_signal(
		      const Vector & V,       // the vector of input voltages
		      double fLO,             // the LO frequency
		      int max_harmonics )     // the max number of harmonics
{
  if (piv == 0)
    error::fatal("Uninitialized sis iv curve reference.");

  if (Vn <= 0 || Rn <= 0 || Cap < 0)  // Cap == 0 is o.k.
    error::fatal("Bad or uninitialized sis junction parameters.");

  if (fLO <= 0.0 || max_harmonics < 0)
    error::fatal("Invalid argument in call to junction::large_signal().");

  // If we get here, everything is ready to do the calculation:

  Voltages = V;
  LO_freq = fLO;
  Vn_ = Vn; Rn_ = Rn; Cap_ = Cap;    // record parameter values
  C.calc(fLO, V);                    // now the ckdata, C.Ck, are calculated 

  double V0 = V.read(0).real/Vn_;              // the normalized bias voltage
  double VLO = fLO / (Vn_ * VoltToFreq);       // normalized LO photon voltage
  int limit = C.Ck.maxindex();                 // loop limit, used below

  // Fill the table of -j/Rn * ivcurve::I(V0 + n*VLO) and derivatives
  I_VLO.reallocate(limit,Index_S);
  I_VLO_prime.reallocate(limit,Index_S);
  { // limit scope of next lines 
    complex current, current_p;
    /*register*/ complex I_norm(0.0,-1/Rn_);     // = -J/Rn
    for(/*register*/ int k = -limit; k <= limit; ++k) {
      piv->Iprime(V0 + k*VLO, current, current_p);
      I_VLO[k] = current * I_norm;
      I_VLO_prime[k] = current_p * I_norm;
    }  // release register variable
  }

  // Convolve the Ck to get the harmonic currents:
  // See FR notebook, pg 104 for formulas
  Currents.reallocate(max_harmonics+1,Index_C).fill(0.0);
  for(/*register*/ int k = -limit; k <= limit; ++k) {
    /*register*/ complex f = C.Ck[k]; complex Ik = I_VLO[k];
    Currents[0].real += norm(f) * Ik.real;  // C[k]*conj(C[k])*im(I(V0+kVLO))/Rn
    f *= Ik;   // now f = -j*C[k]*I(V0 + k VLO)/Rn
    for(/*register*/ int p = 1; p <= max_harmonics; ++p)
      // I = I(p) + conj(I(-p)) , for I with nonnegative harmonic indexing:
      Currents[p] += f * conj(C.Ck.read(k+p)) + conj(f) * C.Ck.read(k-p);
  }

  // finally convert normalized currents to real RMS (already included 1/Rn above)
  Currents *= Vn_/RmsToPeak;
  Currents[0] *= RmsToPeak;  // don't RMS correct DC current

  // Capacitor effects (FR notebook, pg 106):
  complex Xlo(0.0, 2*Pi*fLO*Cap_); // imaginary admittance at LO freq
  limit = (Currents.maxindex() < V.maxindex()) ? 
    Currents.maxindex() : V.maxindex();
  for(/*register*/ int k = 1; k <= limit; ++k)
    // current thru capacitor at DC vanishes
     Currents[k] += k * Xlo * V[k]; // capacitor current

  IF_freq = 0.0;  // this tells small_signal() and noise() to rebuild I_pVIF, I_mVIF
  iv_data_ok = 1;
  return Currents;
} // large_signal()


// --------------------------------------------------------------------
// The small-signal analysis routine calculates a symmetrical harmonic
// admittance matrix using several private variables set by the large
// signal analysis; it also may fill IF_freq, I_pVIF, and I_mVIF, for
// later use by noise(); conversely, it will use values set by noise().
// large_signal() must be called at least once before calls to this
// routine, but then many calls may be made to this routine as long as
// the balance remains valid.

const Matrix & sis_basic_device::small_signal(
		      double fIF,             // the IF frequency
		      int max_harmonics )     // the max number of harmonics
{
  if (fabs(Vn - Vn_) > deps*mVolt ||
      fabs(Rn - Rn_) > deps*Ohm ||
      fabs(Cap - Cap_) > deps*fFarad)
    error::fatal("SIS junction parameters changed since last large_signal() call.");

  if (fIF < 0.0 || max_harmonics < 0 || fIF >= LO_freq)
    error::fatal("Invalid argument in call to junction::small_signal().");

  // If we get here, everything is ready to do the calculation:

  double Vif = fIF/(Vn_ * VoltToFreq);       // normalized IF photon voltage 
  double VLO = LO_freq/(Vn_ * VoltToFreq);   // normalized LO photon voltage
  int limit = C.Ck.maxindex()+max_harmonics; // required range of I(V) data

  // fill in the ivcurve lookup values, if necessary, and calculate Vif:
  if (IF_freq == 0.0 || IF_freq != fIF || limit > I_pVIF.maxindex()) {
    IF_freq = fIF;
    
    // Fill the tables of -j/Rn * ivcurve::I(V0 + n*VLO +/- Vif)
    I_pVIF.reallocate(limit, Index_S);
    I_mVIF.reallocate(limit, Index_S);
    /*register*/ double V0 = Voltages[0].real/Vn_; // the normalized bias voltage
    complex I_norm(0.0,-1/Rn_);                     // -J/Rn
    /*register*/ double Vl = VLO;                       // keep value of VLO in a register
    if (Vif != 0.0) {
      /*register*/ double Vi = Vif;
      for(/*register*/ int k = -limit; k <= limit; ++k) {
	I_pVIF[k] = I_norm * (*piv)((V0 + k*Vl) + Vi);
	I_mVIF[k] = I_norm * (*piv)((V0 + k*Vl) - Vi);
      }
    }
    else { // (Vif == 0.0)
      for(/*register*/ int k = -limit; k <= limit; ++k) {
	I_pVIF[k] = I_norm * (*piv)(V0 + k*Vl);
	I_mVIF[k] = I_pVIF[k];
      }
    }
  }

  // Convolve the Ck and the ivcurve to get the Ymn:
  // Formulas used are in pp following pg 102 in FR notebook. Note that the local
  // I(V) lookup tables already include a factor -j/Rn. This affects the conj(I),
  // since -j*conj(I) = conj(j*I) = -conj(-j*I); so all conj(I) terms change sign.
 
  Y.reallocate(max_harmonics,max_harmonics,Index_S,Index_S).fill(0.0);
  limit = C.Ck.maxindex();  // reusing the variable defined above

  if (fIF != 0.0) {
    // Normal small signal analysis condition
    double Vi2 = 1/(2*Vif);  // used inside the loops
    for(/*register*/ int k = -limit; k <= limit; ++k) {
      complex Ck = C.Ck[k];            // C(k)
      complex Ik = I_VLO[k];           // -j/Rn I(V0 + kVLO)
      complex Ikmif = I_mVIF[k];       // -j/Rn I(V0 + kVLO - Vif)
      complex Ikpif = conj(I_pVIF[k]); // conj(-j/Rn I(V0 + kVLO + Vif))
      Y[0][0] += (norm(Ck)*Vi2)*(Ik - Ikmif + Ikpif - conj(Ik));

      complex Co;  // will be C(k)C(k+m-n)* in loops below
      for(/*register*/ int n = 1; n <= max_harmonics; ++n) {
	/*register*/ int kpn = k+n, kmn = k-n;
	/*register*/ double fp = 1/(2*(Vif+n*VLO)), fm = 1/(2*(Vif-n*VLO));
	complex Ikpn = Ik - I_mVIF[kpn]; // will be used in loop over m
	complex Ikmn = Ik - I_mVIF[kmn];
	if (kpn <= limit) {  // so C(k+n) is nonzero
	  Co = Ck * conj(C.Ck[kpn]);
	  complex Itemp =  conj(I_VLO[kpn]);
	  Y[0][-n] += (fm*Co)*(Ikpn + Ikpif - Itemp);
	  Y[n][0] += (Vi2*Co)*(Ik - Ikmif + conj(I_pVIF[kpn]) - Itemp); 
	}
	if (kmn >= -limit) { // so C(k-n) is nonzero
	  Co = Ck * conj(C.Ck[kmn]);
	  complex Itemp = conj(I_VLO[kmn]);
	  Y[0][n] += (fp*Co)*(Ikmn + Ikpif - Itemp);
	  Y[-n][0] += (Vi2*Co)*(Ik - Ikmif + conj(I_pVIF[kmn]) - Itemp);
	}

	for(/*register*/ int m = 1; m <= max_harmonics; ++m) {
	  /*register*/ int kpm = k+m, kmm = k-m;
	  complex Ikpm = conj(I_pVIF[kpm]);
	  complex Ikmm = conj(I_pVIF[kmm]);
	  /*register*/ int kk = k+m-n;  // +m,+n
	  if((kk <= limit)&&(kk >= -limit)) { //C(k+m-n) nonzero
	    Co = Ck * conj(C.Ck[kk]);
	    Y[m][n] += (fp*Co)*(Ikmn + Ikpm - conj(I_VLO[kk]));
	  }
	  kk = k+m+n;  // +m,-n
	  if((kk <= limit)&&(kk >= -limit)) { //C(k+m+n) nonzero
	    Co = Ck * conj(C.Ck[kk]);
	    Y[m][-n] += (fm*Co)*(Ikpn + Ikpm - conj(I_VLO[kk]));
	  }
	  kk = k-m-n;  // -m,+n
	  if((kk <= limit)&&(kk >= -limit)) { //C(k-m-n) nonzero
	    Co = Ck * conj(C.Ck[kk]);
	    Y[-m][n] += (fp*Co)*(Ikmn + Ikmm - conj(I_VLO[kk]));
	  }
	  kk = k-m+n;  // -m,-n
	  if((kk <= limit)&&(kk >= -limit)) { //C(k-m+n) nonzero
	    Co = Ck * conj(C.Ck[kk]);
	    Y[-m][-n] += (fm*Co)*(Ikpn + Ikmm - conj(I_VLO[kk]));
	  }

    }}} // for m,n,k loops
  }

  else {
    // fIF = 0; we need to look at derivatives of the I(V) curve (notes, pp 110-113)
    for(/*register*/ int k = -limit; k <= limit; ++k) {
      complex Ck = C.Ck[k];               // C(k)
      complex Ik = I_VLO[k];              // -j/Rn I(V0 + kVLO)
      complex Ikprime = I_VLO_prime[k];   // -j/Rn I'(V0 + kVLO)
      Y[0][0] += norm(Ck) * Ikprime.real; // |Ck|^2 * Idc'

      complex Co;  // will be C(k)C(k+m-n)* in loops below
      for(/*register*/ int n = 1; n <= max_harmonics; ++n) {
	/*register*/ int kpn = k+n, kmn = k-n;
	/*register*/ double f = 1/(2*n*VLO);
	complex Ikpn = Ik - I_mVIF[kpn]; // will be used in loop over m
	complex Ikmn = Ik - I_mVIF[kmn];
	if(kpn <= limit) {  // C(k+n) is nonzero
	  Co = Ck*conj(C.Ck[kpn]);
	  Y[n][0] += (0.5/RmsToPeak)*Co*(Ikprime + conj(I_VLO_prime[kpn]));
	  Y[0][-n] -= Co*(2*f*RmsToPeak*Ikpn.real);  // note -= vice +=
	}

	for(/*register*/ int m = 1; m <= max_harmonics; ++m) {
	  complex Ikpm = conj(I_pVIF[k+m]);
	  /*register*/ int kk = kmn+m; // k+m-n
	  if((kk <= limit)&&(kk >= -limit)) { //C(k+m-n) nonzero
	    Co = Ck * conj(C.Ck[kk]);
	    Y[m][n] += (f*Co)*(Ikmn + Ikpm - conj(I_VLO[kk]));
	  }
	  kk = kpn+m;  // +m,-n
	  if((kk <= limit)&&(kk >= -limit)) { //C(k+m+n) nonzero
	    Co = Ck * conj(C.Ck[kk]);
	    Y[m][-n] -= (f*Co)*(Ikpn + Ikpm - conj(I_VLO[kk]));
	  }
    }}} // for n,m,k loops

    // now fill in the other elements
    for(/*register*/ int i = 0; i <= max_harmonics; ++i)
      for(/*register*/ int j = 1; j <= max_harmonics; ++j) {
	Y[-j][-i] = conj(Y[j][i]);
	Y[-i][j] = conj(Y[i][-j]);
      }
  } // else

  // Capacitor effects (FR notebook, pg 106):
  { // limit scope of the following constant
    /*register*/ const double c = 2*Pi*Cap;
    for(/*register*/ int k = -max_harmonics; k <= max_harmonics; ++k)
      Y[k][k].imaginary += c * (fIF + k*LO_freq); // only diagonal terms
  }

  return Y;
} // small_signal()


// --------------------------------------------------------------------
// The routine that returns the symmetrical harmonic noise correlation
// matrix using several private variables set by the large signal
// analysis; it also may fill IF_freq, I_pVIF, and I_mVIF, for later
// use by small_signal(); conversely, it will use values set by that
// routine, to speed up the calculation for calls to both routines.
// large_signal() must be called at least once before calls to this
// routine, but then many calls may be made to this routine as long as
// the balance remains valid.

const Matrix & sis_basic_device::noise(
	       double fIF,                    // the IF frequency
	       double T,                      // the temperature
	       int max_harmonics )            // the max number of harmonics
{
  if (fabs(Vn - Vn_) > deps*mVolt ||
      fabs(Rn - Rn_) > deps*Ohm ||
      fabs(Cap - Cap_) > deps*fFarad)
    error::fatal("SIS junction parameters changed since last large_signal() call.");

  if (fIF < 0.0 || max_harmonics < 0 || fIF >= LO_freq || T <= 0.0)
    error::fatal("Invalid argument in call to junction::noise().");

  // If we get here, everything is ready to do the calculation:

  double Vif = fIF/(Vn_ * VoltToFreq);       // normalized IF photon voltage 
  double VLO = LO_freq/(Vn_ * VoltToFreq);   // normalized LO photon voltage
  double V0 = Voltages[0].real/Vn_;     // the normalized bias voltage
  int limit = C.Ck.maxindex()+max_harmonics; // required range of I(V) data

  // fill in the ivcurve lookup values, if necessary, and calculate Vif:
  if (IF_freq == 0.0 || IF_freq != fIF || limit > I_pVIF.maxindex()) {
    IF_freq = fIF;

    // Fill the tables of -j/Rn * ivcurve::I(V0 + n*VLO +/- Vif)
    I_pVIF.reallocate(limit, Index_S);
    I_mVIF.reallocate(limit, Index_S);
    /*register*/ double Vo = V0;                 // save in a register
    complex I_norm(0.0,-1/Rn_);              // -J/Rn
    /*register*/ double Vl = VLO;                // another register copy
    if (Vif != 0.0) {
      /*register*/ double Vi = Vif;
      for(/*register*/ int k = -limit; k <= limit; ++k) {
	I_pVIF[k] = I_norm * (*piv)((V0 + k*Vl) + Vi);
	I_mVIF[k] = I_norm * (*piv)((V0 + k*Vl) - Vi);
      }
    }
    else { // (Vif == 0.0)
      for(/*register*/ int k = -limit; k <= limit; ++k) {
	I_pVIF[k] = I_norm * (*piv)(Vo + k*Vl);
	I_mVIF[k] = I_pVIF[k];
      }
    }
  }

  // a table of coth()'s times Idc's:
  // the formulas are in FR notebook, pp 104-105

  real_vector cothp(limit,Index_S);
  real_vector cothm(limit,Index_S);
  { // this block limits the scope of the following variables
    /*register*/ double f1 = eCharge*Vn_;     // the result normalization
    /*register*/ double f2 = (2*BoltzK*T)/f1; // normalize coth() arg, even if T == 0
    for(/*register*/ int k = -limit; k <= limit; ++k) {
      // note that because of the I_norm factor, I_pVIF[k].real == Idc[k]/Rn, etc.
      // we will handle the case where the coth argument is 0 using Idc'(V)
      double arg, arg_f2;
      complex current, current_p;   // hold results of ivcurve::Iprime()

      arg = (V0 + k*VLO) + Vif;
      if(fabs(arg) < deps) arg = 0.0 ;  // very close to 0.0

      if(f2 == 0.0 || fabs((arg_f2 = arg/f2)) > 24.0) {
	// then |tanh(arg/f2)| == 1.0 to 20 decimal places
	cothp[k] = ((arg < 0.0)? -1.0 : 1.0)*f1*I_pVIF[k].real;
      }
      else if(arg != 0.0) {
	cothp[k] = f1/tanh(arg_f2)*I_pVIF[k].real;
      }
      else {
	// lim(arg->0) = (f1*f2)*Idc'(0)/Rn
	piv->Iprime(0,current,current_p);
	cothp[k] = (2*BoltzK*T/Rn_)*current_p.imaginary;
      }

      arg = (V0 + k*VLO) - Vif;
      if(fabs(arg) < deps) arg = 0.0 ;

      if(f2 == 0.0 || fabs((arg_f2 = arg/f2)) > 24.0) {
	// then |tanh(arg/f2)| == 1.0 to 20 decimal places
	cothm[k] = ((arg < 0.0)? -1.0 : 1.0)*f1*I_mVIF[k].real;
      }
      else if(arg != 0.0) {
	cothm[k] = f1/tanh(arg_f2)*I_mVIF[k].real;
      }
      else {
	// lim(arg->0) = (f1*f2)*Idc'(0)/Rn
	piv->Iprime(0,current,current_p);
	cothm[k] = (2*BoltzK*T/Rn_)*current_p.imaginary;
      }
    }
  }
  
  // Convolve the Ck and the coth's to get the Hmn:
  // See FR notebook, pp 104-105 for formulas

  H.reallocate(max_harmonics,max_harmonics,Index_S,Index_S).fill(0.0);
  limit = C.Ck.maxindex();
  for(int n = -max_harmonics; n <= max_harmonics; ++n)
    for(/*register*/ int m = -max_harmonics; m <= max_harmonics; ++m) {
      /*register*/ complex *pHmn = & H[m][n];
      for(/*register*/ int k = -limit; k <= limit; ++k)
	*pHmn += 
	  (C.Ck[k] * conj(C.Ck.read(k+m-n))) *
	   (cothp[k+m] + cothm[k-n]);
    }

  return H;
} // noise()


// --------------------------------------------------------------------

int sis_basic_device::call_large_signal() const
{
  // if parameters have changed since the last call to large_signal,
  // then state data is assumed bad.
  return (!iv_data_ok ||
	  fabs(Vn - Vn_) > deps*mVolt ||
	  fabs(Rn - Rn_) > deps*Ohm ||
	  fabs(Cap - Cap_) > deps*fFarad);
}
