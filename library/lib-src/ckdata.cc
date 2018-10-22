// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// ckdata.cc

#include "junction.h"
#include "units.h"
#include <cmath>

using namespace std;

// ZEROTOL is the accuracy of the calculations in ckdata

#ifndef ZEROTOL   /* should be defined in global.h */
#define ZEROTOL (1.0e-6)
#endif


// The private variable bessel_values is used as follows:
// bessel_values[0] will hold bessel function values calculated by
// bessels().
// bessel_values[1] will hold the derivatives of the bessel fcns.
// Its size is set by find_max_bessel_n()

// find_max_bessel_n() will calculate the proper size of bessel_values so
// that all functions with values bigger than ZEROTOL are returned.
// The argument x must be nonnegative; returns: bessel_values.Rmaxindex()

int ckdata::find_max_bessel_n(const double x)
{
  int nmax;  // the maximum order of |Jn(x)| > ZEROTOL
  if ( x < ZEROTOL ) {       // for x near zero, J1(x) ~= x/2.0
    nmax = 0;
  }
  else if ( x < 2*sqrt(ZEROTOL) ) { // in this range, J2(x)~=x*x/8.0
    nmax = 1;
  }
  // the following formulas need modification if ZEROTOL != 1.0e-6
  else if ( x < 10.0 ) {    // a quadratic fit in this range
    nmax = int( 2.165 + 4.678 * sqrt(x) + 0.5786 * x );
  }
  else {                     // for |x| > 10, a linear fit suffices
    nmax = int( 10.0 + 1.25 * x );
  }

  // now reallocate bessel_values if necessary, and return
  if (nmax > bessel_values.Rmaxindex(nmax))
    bessel_values.resize(1,nmax);  // left index range should be {0,1}
  return bessel_values.Rmaxindex();
}


// bessel() fills bessel_values with Jn(x) and Jn'(x).
// The argument must be nonnegative; returns: bessel_values.Rmaxindex()
//
// Uses the recursion formulas:
//   Jj-1(x) = (2 j/x)*Jj(x) - Jj+1(x)
//   Jj'(x)  = (Jj-1 - Jj+1) / 2
//
// And the Normalization identity:
//   1 = Jo(x) + 2*( J2(x) + J4(x) + J6(x) + ...)
//
// Loosely based on "Numerical Recipes" backwards recursion algorithm.
// See FR notebook, pg 80 for info re derivative calculation

int ckdata::bessel(const double x)
{
  int n,m;             // Jn is max returned order, Jm starts the recursion
  double *J, *Jprime;  // will alias the rows in bessel_values

  // size n, m, and bessel_values (using find_max_bessel_n())
  n = find_max_bessel_n(x); // Jn, Jn' will be the highest in answers
  m = 2 * int(1 + (sqrt( double(n) ) + n)/2);  // an even value for m
  J = bessel_values[0];      // could change with each call to
  Jprime = bessel_values[1]; // find_max_bessel_n(), because of resize()

  // handle x == 0.0
  if ( x == 0.0 ) {
    J[0] = 1.0; Jprime[0] = 0.0;   // only Jo and Jo' will be returned
    return n;
  }

  // x != 0 from here on. Prepare for the backwards recursion loop
  double Jj, Jjm, Jjp;  // terms in the recursion: Jj(x), Jj-1(x), Jj+1(x)

  double sum = 0.0;        // the summation used for normalization
  int sum_flag = FALSE;    // a boolean toggle for including terms in sum

  const double Limit = 1.0e100;  // don't let unnormalized results get
  const double Scale = 1.0e-100; // too big (unlikely). Scale = 1/Limit

  double mult = 2.0 / x;        // x != 0.0
  
  // the backwards recursion loop
  int j;
  for ( j = m, Jj = 1.0, Jjp = 0.0; j > 0; --j ) { // remember, m is even

    Jjm = j*mult*Jj - Jjp; // the recursive calculation

    // Save results for Jj and Jj':
    if ( j <= n ) {   
      J[j] = Jj;
      Jprime[j] = (Jjm - Jjp) / 2.0;
    }

    // Add the results for j-1 even into normalization sum:
    if ( sum_flag == TRUE )
      sum += Jjm;
    sum_flag = ! sum_flag; // for next trip thru loop

    // Rescale values if results are getting too big:
    if ( fabs(Jjm) > Limit ) {
      Jjm *= Scale; Jj *= Scale; sum *= Scale;
      bessel_values *= Scale;
    }

    // step the terms down one
    Jjp = Jj; Jj = Jjm;   

  } // end main recursion loop

  // now j = 0; and Jj = Jo(ax), Jjp = J1(ax) (unnormallized)
  J[0] = Jj;
  Jprime[0] = -Jjp;  // Jo'(x) = - J1(x)

  // normalize the answer
  sum = 2.0*sum - Jj;      // now sum = normalization formula
  bessel_values /= sum;

  // return the max order calculated
  return n;

} // bessel()
  

// Amj_values will hold the complex Amj coefficients derived from bessel
// function calculations and the phases of the large-signal harmonic
// voltages.  It is sized and filled by fillA().  Only values for nonnegative
// m are in this vector; use Amj(-m) = (-1)^m * conj(Amj(m)) for m < 0.
// See FR notebook, pg 34 for formulas used.
//
// fillA calculates the set of complex Amj = Am(Vj) using:
//   Am(Vj) = Jm(alpha_j) * exp(-I*m*phase(Vj))
//
// Its argument is the peak voltage Vj divided by the photon voltage for
// harmonic j, ie: a complex version of the pumping factor alpha at harmonic j. 
// It returns Amj_values.maxindex(). 

int ckdata::fillA(const Complex a)    // a is the complex version of alpha, ie: V/Vphoton
{
  double  alpha = abs(a);
  complex unit  = (alpha == 0.0) ? complex(1.0) : conj(a/alpha);  // unit = exp(-I*arg(a))
  int     max   = bessel(alpha);  // calc the bessel fcns

  // now resize Amj_values if required
  if (max > Amj_values.maxindex(max))  // then Amj_values is too small
    Amj_values.resize(max);
 
  // lastly fill in the Amj_values
  max = Amj_values.maxindex();   // merely a safety precaution ico alloc problem
  int m; Complex phase;          // phase will be exp(-I*m*arg(a))
  double *J = bessel_values[0];  // the vector of bessel functions
  for ( m = 0, phase = 1.0; m <= max; ++m, phase *= unit )
    Amj_values[m] = J[m] * phase;
  
  return max;
}


// The public variable Ck is a symmetrically-indexed vector which holds the SIS
// junction large-signal convolution constants (Ck) for +/- values of k.  The
// public variable Tol is approximately the magnitude of the smallest nonzero
// value in Ck.  It is equal to the #define'd value ZEROTOL in global.h, which
// is used by calc() to limit the depth of its calculations, saving time and
// memory.

// calc() generates the large-signal harmonic voltage convolution
// constants Ck, given a vector of voltages at each harmonic, the
// Local Oscillator frequency, and the gap voltage of the SIS junction.
// Both fLO and VGap must be > 0.0, or calc() will empty Ck.
// calc() also sets Tol to the value of ZEROTOL.
// See FR notebook, pg 65 for derivations of index limits used in calc().

// this is a helper fcn for calc(); definition follows calc()
static void convC(const int, const int, Matrix &, Vector &);

ckdata & ckdata::calc(
		      const double   fLO,  // The Large-Signal (LO) frequency
		      const Vector & V     // The Large-Signal (LO) harmonic voltages
		      )
{
  const int INITSIZE = 20;   // for temporary table alloc
  static Matrix C(2, INITSIZE, Index_C, Index_S);  // will hold intermediate results
  C.fillall(0.0);
  int curr = 0;   // will be either 0 or 1: picks a row of C
  int cmax, pmax; // like individual Rmaxindex() for current and previous C result
  int harms = V.maxindex();  // number of harmonics
  
  // initialize C[curr] for the first iteration
  C[curr][0] = 1.0;
  cmax = C.Rmaxindex(0);  // note that elements of C beyond cmax are all 0 here
  
  // loop through V values, convolving C with the previous result
  int amax;
  double scale = RmsToPeak*VoltToFreq / fLO; // convert Vj to alpha_j

  for ( int j = 1; j <= harms; ++j ) {
    // at this point in the loop, all C elements beyond cmax == C.Rmaxindex() are 0
    // this will be a loop constant
    // if Vj is zero, the Ck don't change
    if (V[j] == 0.0) continue;

    // Vj is nonzero here
    curr = !curr; pmax = cmax;  // step to other row of C
    amax = fillA(scale * V[j] / j);
    // now adjust size of C
    cmax += (j * amax);  // the new index range for nonzero Ck
    if(cmax > C.Rmaxindex(cmax))  // then the capacity of C is too small
      // grow capacity of C, plus some extra
      cmax = C.resize(1,cmax+INITSIZE).Rmaxindex(cmax);

    // now the previous row, C[!curr], is zero for all elements beyond pmax
    // and the size of C is at least cmax, so we can use raw data access for speed
    // and C.Rmaxindex() == cmax, so loop constant is still true.

    // do the convolution sum
    // this is written as a separate function to support future expandability for
    // the harmonic balance routine
    convC(j, curr, C, Amj_values);
  }
  
  // C[curr] has the results
  Ck = row(curr, C).shrink(ZEROTOL);  //this will reallocate Ck to be big enough
  Tol = ZEROTOL;
  return *this;
}


// convC() calculates the Ck(j) given the Ck(j-1) and the Amj, using the
// formula:
//
// Ck(j) = Sum(m = -oo to m = oo)( C(k-j*m)(j-1) * Amj )
//
// Since we only have Amj for m >= 0, we calculate Amj for m < 0 using:
//
// A(-m)j = ((-1)^m) * conj(Amj)
//
// See FR notebook, pg 32 for formulas used.
//
// Also, since we know the real limits for nonzero Ck(j-1) and for nonzero Amj,
// before calling convC(), C.Rmaxindex() must be set to the limit for nonzero
// Ck(j), which is limit[Ck(j-1)] + j * limit[Amj].  The values stored for Ck(j-1)
// within this new limit but beyond limit[Ck(j-1)] must be all zeroes or convC will
// give the wrong answer.  Also, you must have Amj.maxindex() == limit[Amj], or
// again the answer will be wrong.  ckdata::calc() ensures these conditions are
// met before calling convC(). 

static void convC(const int j,     // the harmonic number
		  const int curr,  // the row of C which will hold the results
		  Matrix & C,      // convC will write into the row C[curr]
		  Vector & Amj     // the vector of Amj values for harmonic j
		  )
{
  int k, kmax;         // loop index into C and its limit
  int m, amax;         // another loop index (into Amj) and limit
  int sign;            // holds +/- 1 for the A(-m)j

  kmax = C.Rmaxindex();
  amax = Amj.maxindex();
  for ( k = -kmax; k <= kmax; ++k ) {
    Complex *pCk = & C[curr][k];  // only do this indexing calculation once
    *pCk = C[!curr][k] * Amj[0];
    for ( m = 1, sign = -1; m <= amax; ++m, sign *= -1 )
      *pCk += C.read(!curr, k - j*m) * Amj[m]                 // term for +m
	   +  C.read(!curr, k + j*m) * sign * conj(Amj[m]);   // term for -m
  }
}

