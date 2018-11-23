/* BesselFn.cpp
*
* Written by Phichet Kittara
*
* Implementation of an object that can calculate various Bessel related quantities
*
* Used in MultiTone and NewMultitone
*/

/* Changes
*
*    14/04/04 : PKG : Removed two Borland specific includes.
*                     Added initialisation of DOUBLE_PRECISION to constructor.
*    03/05/04 : PKG : Comparison with working PK code shows no differences. 
*/

#include "BesselFn.h"
#include "math.h"


BesselFn::BesselFn()
{
    DOUBLE_PRECISION = std::numeric_limits<long double>::epsilon();
}


BesselFn::~BesselFn()
{
}


//---------------------------------------------------------------------//
/* Jnx finds Jn(x) using the recurrence relation 
 * proposed in Ambramowitz & Stegun (1964)
 * J(n-1)(x) = (2n/x)*Jn(x) - J(n+1)(x)
 * and normalised using J0(x) + 2J1(x) + 2J2(x) +...+2Jn(x) = 1;
 */
long double BesselFn::Jnx(int n,long double x)
{
  // beware of those zeros.
  if(n==0 && x==0.0)
    return 1.0;
  if(n!=0 && x==0.0)
    return 0.0;

  // get bessel order
  int abs_n = abs(n);

  // array of recurrence; 
  // starting from 2*(order required) + an offset of 50 
  // i.e nMax~50 for small n and nMax = 2*n for large n
  int nMax = 50 + 2*abs_n;
  std::vector<long double> J;
  J.resize(nMax);

  for(int i=0;i<nMax;i++)
    J[i] = 0.0;

  // start the recurrence with small values
  J[nMax-1] = 0.0;
  J[nMax-2] = 1.0e-30;

  for(int i=nMax-3;i>=0;i--) {
    J[i] = 2*(i+1)/x*J[i+1] - J[i+2];
    if(J[i] > 1.0e10) {
      for(int l=i;l<nMax;l++)
        J[l] /= 1.0e15;
    }
  }

  // normalising
  long double NormalisedFactor = J[0];
  for(int j=2;j<nMax;j+=2)
    NormalisedFactor += 2*J[j];

  for(int i=0;i<nMax;i++)
    J[i] /= NormalisedFactor;

  if(n>=0)
    return J[n];
  else
    return pow(-1,(abs_n%2)+2)*J[abs_n];
}

//---------------------------------------------------------------------//
/* ArrayJnx returns array of Jn(x) for n=0 to n=n_last
 * proposed in Ambramowitz & Stegun (1964)
 * J(n-1)(x) = (2n/x)*Jn(x) - J(n+1)(x)
 * and normalised using J0(x) + 2J1(x) + 2J2(x) +...+2Jn(x) = 1;
 */
bool BesselFn::setArrayJnx(unsigned int n_last, long double x, std::vector<long double> &J)
{

  if(n_last <= 0)
    return false;
    
  // init
  J.clear();
  J.resize(n_last+1,0.0); // order 0th to n_last

  // beware of those zeros.
  if( fabs(x) < DOUBLE_PRECISION) {
    J[0] = 1.0;
    return true;
  }

  // array of recurrence; 
  // starting from 2*(order required) + an offset of 50 
  // i.e nMax~50 for small n and nMax = 2*n for large n
  int nMax = 50 + 2*n_last;
  J.resize(nMax);

  for(int i=0;i<nMax;i++)
    J[i] = 0.0;

  // start the recurrence with small values
  J[nMax-1] = 0.0;
  J[nMax-2] = 1.0e-30;

  for(int i=nMax-3;i>=0;i--) {
    J[i] = 2*(i+1)/x*J[i+1] - J[i+2];
    if(J[i] > 1.0e10) {
      for(int l=i;l<nMax;l++)
        J[l] /= 1.0e15;
    }
  }

  // normalising
  long double NormalisedFactor = J[0];
  for(int j=2;j<nMax;j+=2)
    NormalisedFactor += 2*J[j];

  for(int i=0;i<nMax;i++)
    J[i] /= NormalisedFactor;

  J.resize(n_last+1);
  return true;
}

//---------------------------------------------------------------------//
/* IntegratedJnx finds Jn(x) by integrating 
 * cos(x*sin(phi) - n*phi) over phi from 0 to pi/2
 * Very time consuming
 */
long double BesselFn::IntegratedJnx(int n,long double x)
{
    int NN = 1000;
    long double dphi = M_PI/NN;
    long double phi = dphi/2.0;
    long double sum = 0.0;

    for(int i=0; i<NN; i++) {
        sum += cos( x*sin(phi) - n*phi )*dphi;
        phi += dphi;

    }
    return sum/M_PI;
}
