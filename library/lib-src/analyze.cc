// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// analyze.cc

#include "mixer.h"
#include "units.h"
#include "error.h"
#include "Amath.h"
#include <cmath>

using namespace std;

// ********************************************************************
// analyzer public functions:

mixer::analyzer::analyzer(mixer & m) :
  result(0),
  mix(m),
  must_rebuild(1),
  terminate_flag(0),
  linear_p(0),
  linear_m(0),
  junctions(0)
{ }


mixer::analyzer & mixer::analyzer::terminate_rf(int f)
{
  terminate_flag = (f != 0);
  changed();
  return *this;
}


const sdata & mixer::analyzer::operator()()
{
  rebuild(); fill_data(); calc_T(); calc_X(); calc_Y();

  // loop over all elements in the result sdata
  int h1, h2, n1, n2, m, h;

  for (h1 = h_low; h1 <= h_high; ++h1) {
    int n1_high = n_high(h1);  // could be different for h1 == 0

    for (h2 = h_low; h2 <= h_high; ++h2) {
      int n2_high = n_high(h2);

      for (n1 = n_low; n1 <= n1_high; ++n1) {
	for (n2 = n_low; n2 <= n2_high; ++n2) {

	  // avoid the following index dereferences inside the inner loops
	  complex & Sr = S(n1,n2,h1,h2);
	  complex & Cr = C(n1,n2,h1,h2);
	  complex * X_n1_h1 = Xrow(n1,h1);
	  complex * X_n2_h2 = Xrow(n2,h2);
	  complex * Y_n1_h1 = Yrow(n1,h1);
	  complex * Y_n2_h2 = Yrow(n2,h2);
	  
	  Sr = (h1==h2)? S11(n1,n2,h2) : 0.0;
	  Cr = (h1==h2)? C11(n1,n2,h2) : 0.0;

	  for (m = m_low; m <= m_high; ++m) {
	    complex Ytemp = Y_n1_h1[Ycol(m,h2)];  // Y(n1,m,h1,h2)

	    Sr += Ytemp*S21(m,n2,h2);  // that's it for the S matrix!

	    Cr += Ytemp*C21(m,n2,h2) + conj(Y_n2_h2[Ycol(m,h1)]*C21(m,n1,h1));
	    for (h = h_low; h <= h_high; ++h) {

	      //Commented out; using Adot instead:
	      //register complex sum(0.0);
	      //register int i;
	      //for (i = h_low; i <= h_high; ++i)
	      //  sum += Cj(m,h,i)*conj(X_n2_h2[Xcol(m,i)]);
	      //Cr += X_n1_h1[Xcol(m,h)]*sum;
	      //sum = 0.0;
	      //for (i = m_low; i <= m_high; ++i)
	      //  sum += C22(m,i,h)*conj(Y_n2_h2[Ycol(i,h)]);
	      //Cr += Y_n1_h1[Ycol(m,h)]*sum;

	      Cr += X_n1_h1[Xcol(m,h)]*
		Adot(&(X_n2_h2[Xcol(m,h_low)]), &Cj(m,h,h_low), h_size);
	      Cr += Y_n1_h1[Ycol(m,h)]*
		Adot(&(Y_n2_h2[Ycol(m_low,h)]), &C22(m,m_low,h), m_high);

	    }// h
	  }  // m
	}    // n2
      }      // n1
    }        // h2
  }          // h1

  return result;
 
} // operator()


// ********************************************************************
// rebuild() and fill_data():

void mixer::analyzer::rebuild()
{
  if(!must_rebuild) return; // not necessary to rebuild

  must_rebuild = 0;

  int i, s;  // used to hold indexes or sizes

  // the index limits:
  int nj = mix.num_junctions;  // will need this often
  h_high = mix.max_harmonics;
  h_low = -h_high;
  h_size = 2*mix.max_harmonics + 1;
  n_low = nj + 1;  // first external port index
  // *** CHANGING m_low WILL REQUIRE CHANGES TO NEARLY ALL HELPER FUNCTIONS!!! ***
  m_low = 1;       // starting junction indexing from 1 vice 0
  m_high = nj;

  // size the STL vectors:
  linear_p.resize(h_high + 1);      // linear_p[0] will hold IF sdata
  linear_m.resize(h_high + 1);      // linear_m[0] will be unused
  junctions.resize(m_high + m_low); // junctions[m_low] will be lowest used

  // size the sdata elements of the STL vectors
  s = (terminate_flag)? nj : mix.rf_circuit->size();
  for (i = 1; i <= h_high; ++i) {
    // the RF sdatas
    linear_p[i].resize(s);
    linear_m[i].resize(s);
  }
  linear_p[0].resize(mix.if_circuit->size()); // the IF sdata
  for (i = m_low; i <= m_high; ++i) {
    // the junction sdatas
    if (junctions[i].mode() != Index_S) {
      junctions[i].S.reallocate(h_high,h_high,Index_S,Index_S);
      junctions[i].C.reallocate(h_high,h_high,Index_S,Index_S);
      junctions[i].B.reallocate(h_high,Index_S);
    }
    else {
      junctions[i].resize(h_high);
    }
  }

  // if necessary, rebuild the terminated rf circuit:
  if(terminate_flag) {
    int n;
    int num_ports = mix.rf_circuit->size() - nj;
    temp = circuit();  // clear out old construction
    for (n = 0; n < num_ports; ++n) {
      // connect terminators to external RF ports
      int p = nj + n + 1;
      temp.connect(*(mix.rf_circuit), p, 
		   ((mix.term[p-1])? *(mix.term[p-1]) : mix.default_term[n]), 1);
    }
    for (n = 1; n <= nj; ++n)
      temp.add_port(*(mix.rf_circuit), n); // ports to connect to junctions
  }

  // size the result sdata, and T_, X_, Y_
  s = (terminate_flag) ?
    mix.if_circuit->size() - nj :  // only IF ports are open
    mix.size();                    // all external ports are open
  result.resize(s);
  result.S.maximize();
  result.C.maximize();
  result.B.fillall(0.0).maximize(); // ensures source vector is all 0's.
  T_.reallocate(nj*h_size, nj*h_size, Index_1, Index_1).maximize();
  int max_ports;
  if (terminate_flag)
    max_ports = s;
  else
    max_ports = (mix.rf_circuit->size() > mix.if_circuit->size()) ?
      mix.rf_circuit->size() - nj : mix.if_circuit->size() - nj;
  X_.reallocate(max_ports*h_size, nj*h_size, Index_C, Index_1).maximize();
  Y_.reallocate(max_ports*h_size, nj*h_size, Index_C, Index_1).maximize();

} // rebuild()   


void mixer::analyzer::fill_data()
{
  parameter IF_saved = device::f;
  double LO = mix.LO;
  const sdata * ps;

  // the IF circuit:
  ps = & mix.if_circuit->get_data();
  linear_p[0].S.copy(ps->S);
  linear_p[0].C.copy(ps->C);

  // the RF circuit:
  nport * rf = (terminate_flag) ? & temp : mix.rf_circuit;
  double fp = LO + IF_saved;  // harmonic = 1  (USB freq)
  double fm = LO - IF_saved;  // harmonic = -1 (LSB freq)
  for (int h = 1; h <= h_high; ++h, fp += LO, fm += LO) {

    // the + harmonics:
    device::f = fp;
    ps = & rf->get_data();
    linear_p[h].S.copy(ps->S);
    linear_p[h].C.copy(ps->C);

    // the - harmonics:
    device::f = fm;
    ps = & rf->get_data();
    linear_m[h].S.copy(ps->S).apply(conj);
    linear_m[h].C.copy(ps->C).apply(conj);
    // negative frequencies require conjugates

  }
  device::f = IF_saved;

  // the small_signal responses of the junctions:
  for (int m = m_low; m <= m_high; ++m) {
    junction * pj = mix.junc[m-m_low];
    junctions[m] = ydata_ptr(& pj->small_signal(IF_saved, h_high),
			     & pj->noise(IF_saved, device::T, h_high));
  }

}


// ********************************************************************
// calc_T(), calc_X(), calc_Y():

void mixer::analyzer::calc_T()
{
  /*register*/ int m2, m1, h2, h1;

  for (h1 = h_low; h1 <= h_high; ++h1) {
    for (h2 = h_low; h2 <= h_high; ++h2) {
      /*register*/ int d = Delta(h1,h2);
      for (m1 = m_low; m1 <= m_high; ++m1) {
	/*register*/ complex sj = Sj(m1,h1,h2);
	for (m2 = m_low; m2 <= m_high; ++m2) {
	  T(m1,m2,h1,h2) = Delta(m1,m2)*d - sj*S22(m1,m2,h2);
  }}}}
  T_ = inverse(T_);
}


void mixer::analyzer::calc_X()
  // must execute calc_T() before calling calc_X()
{
  /*register*/ int m, m2, h2;
  int n, h1;
  const int nj = m_high - m_low + 1;  // number of junctions

  for (h1 = h_low; h1 <= h_high; ++h1) {
    int nh = n_high(h1); 
      for (n = n_low; n <= nh; ++n) {
	/*register*/ complex * s12 = & S12(n,m_low,h1);
	for (h2 = h_low; h2 <= h_high; ++h2) {
	  for (m2 = m_low; m2 <= m_high; ++m2) {
	    /*register*/ complex * t = & T(m_low,m2,h1,h2);
	    /*register*/ complex * x = & X(n,m2,h1,h2);
	    (*x) = 0.0;
	    for (m = 0; m < nj; ++m)
	      (*x) += s12[m]*t[m];
  }}}}
}


void mixer::analyzer::calc_Y()
  // must execute calc_X() before calling calc_Y()
{
  /*register*/ int h, m, h2;
  int n, h1;

  for (h1 = h_low; h1 <= h_high; ++h1) {
    int nh = n_high(h1); 
      for (n = n_low; n <= nh; ++n) {
	/*register*/ complex * xr = Xrow(n,h1);
	/*register*/ complex * yr = Yrow(n,h1);
	for (h2 = h_low; h2 <= h_high; ++h2) {
	  for (m = m_low; m <= m_high; ++m) {
	    /*register*/ complex * y = & yr[Ycol(m,h2)];
	    (*y) = 0.0;
	    for (h = h_low; h <= h_high; ++h)
	      (*y) += xr[Xcol(m,h)]*Sj(m,h,h2);
  }}}}
}


// ********************************************************************
// other analyzer private helper functions (matrix element access):
// MANY OF THESE FUNCTIONS REQUIRE THAT m_low == 1

int mixer::analyzer::n_high(int h)
{
  if (h == 0)
    return mix.if_circuit->size();

  return (terminate_flag)? 0 : mix.rf_circuit->size();
}


// the following functions are used to access the linear element sdatas:

inline complex & mixer::analyzer::S11(int n1, int n2, int h)
{ return (h < 0) ? linear_m[-h].S[n1][n2] : linear_p[h].S[n1][n2]; }


inline complex & mixer::analyzer::S12(int n, int m, int h)
  // requires m_low == 1; if m_low == 0, use m+1 vice m below:
{ return (h < 0) ? linear_m[-h].S[n][m] : linear_p[h].S[n][m]; }


inline complex & mixer::analyzer::S21(int m, int n, int h)
  // requires m_low == 1; if m_low == 0, use m+1 vice m below:
{ return (h < 0) ? linear_m[-h].S[m][n] : linear_p[h].S[m][n]; }


inline complex & mixer::analyzer::S22(int m1, int m2, int h)
  // requires m_low == 1; if m_low == 0, use m+1 vice m below:
{ return (h < 0) ? linear_m[-h].S[m1][m2] : linear_p[h].S[m1][m2]; }


inline complex & mixer::analyzer::C11(int n1, int n2, int h)
{ return (h < 0) ? linear_m[-h].C[n1][n2] : linear_p[h].C[n1][n2]; }


inline complex & mixer::analyzer::C12(int n, int m, int h)
  // requires m_low == 1; if m_low == 0, use m+1 vice m below:
{ return (h < 0) ? linear_m[-h].C[n][m] : linear_p[h].C[n][m]; }


inline complex & mixer::analyzer::C21(int m, int n, int h)
  // requires m_low == 1; if m_low == 0, use m+1 vice m below:
{ return (h < 0) ? linear_m[-h].C[m][n] : linear_p[h].C[m][n]; }


inline complex & mixer::analyzer::C22(int m1, int m2, int h)
  // requires m_low == 1; if m_low == 0, use m+1 vice m below:
{ return (h < 0) ? linear_m[-h].C[m1][m2] : linear_p[h].C[m1][m2]; }


// the following functions are used to access the junction sdatas

inline complex & mixer::analyzer::Sj(int m, int h1, int h2)
{ return junctions[m].S[h1][h2]; } 

inline complex & mixer::analyzer::Cj(int m, int h1, int h2)
{ return junctions[m].C[h1][h2]; } 


// the following functions are used to access the result sdata
// note that the index mapping is determined by mixer::port()

inline complex & mixer::analyzer::S(int n1, int n2, int h1, int h2)
{ return result.S[mix.port(n1,h1)][mix.port(n2,h2)]; }


inline complex & mixer::analyzer::C(int n1, int n2, int h1, int h2)
{ return result.C[mix.port(n1,h1)][mix.port(n2,h2)]; }


// the following functions are used to access the T_, X_, Y_ matrices
// THE FOLLOWING FUNCTIONS WILL REQUIRE CHANGES IF THE INDEX MODES OF
// T_, X_, Y_, OR THE VALUE OF m_low ARE CHANGED IN rebuild().


inline complex & mixer::analyzer::T(int m1, int m2, int h1, int h2)
  // T_ indexing is grouped as follows:
  // Lindex: m2,h2; Rindex: m1,h1  (ie, m2,h2 select the row of T_)
  // Lindex and Rindex:
  //   Grouped by h, then ordered by m within each h
  //   with Index_1 and m_low == 1, lowest element has (m,h) == (1,h_low)
  // So elements with constant m2,h1,h2 and varying m1 are contiguous
{ return T_[(h2+h_high)*m_high + m2][(h1+h_high)*m_high + m1]; }


inline complex & mixer::analyzer::X(int n, int m, int h1, int h2)
  // X_ indexing is grouped as follows:
  // Lindex: n,h1; Rindex: m,h2  (ie, n,h1 select the row of X_)
  // Lindex:
  //   Grouped by n, then ordered by h1 within each n
  //   with Index_C, lowest element has (n,h1) == (n_low,h_low)
  // Rindex:
  //   Grouped by m, then ordered by h2 within each m
  //   with Index_1, and m_low == 1, lowest element has (m,h2) == (1,h_low)
  // So elements with constant n,m,h1 and varying h2 are contiguous
{ return X_[(n-n_low)*h_size + h1+h_high][m*h_size + h2-h_high];  }


inline complex & mixer::analyzer::Y(int n, int m, int h1, int h2)
  // Y_ indexing is grouped as follows:
  // Lindex: n,h1; Rindex: m,h2  (ie, n,h1 select the row of Y_)
  // Lindex:
  //   Grouped by n, then ordered by h1 within each n
  //   with Index_C, lowest element has (n,h1) == (n_low,h_low)
  // Rindex:
  //   Grouped by h2, then ordered by m within each h2
  //   with Index_1 and m_low == 1, lowest element has (m,h2) == (1,h_low)
  // So elements with constant n,h1,h2 and varying m are contiguous
{ return Y_[(n-n_low)*h_size + h1+h_high][(h2+h_high)*m_high + m]; }


// A use of the following functions is as follows:
// X(n,m,h1,h2) ==> Xrow(n,h1)[Xcol(m,h2)]

inline complex * mixer::analyzer::Xrow(int n, int h)
{ return X_[(n-n_low)*h_size + h+h_high]; }


inline int mixer::analyzer::Xcol(int m, int h)
{ return m*h_size + h-h_high; }


inline complex * mixer::analyzer::Yrow(int n, int h)
{ return Y_[(n-n_low)*h_size + h+h_high]; }


inline int mixer::analyzer::Ycol(int m, int h)
{ return (h+h_high)*m_high + m; }
