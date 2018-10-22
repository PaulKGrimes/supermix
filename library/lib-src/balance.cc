// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// balance.cc

#include "mixer.h"
#include "units.h"
#include "error.h"
#include "sources.h"
#include <cmath>

using namespace std;

// ********************************************************************
// balancer public functions:

mixer::balancer::balancer(mixer & m) :
  mix(m),
  must_rebuild(1),
  ival(0,Index_C),
  iter(0),
  pY(0),
  linear(0)
{ max_iter = 100; }


// operator(): saves the current junction operating states, then attempts
// to perform a harmonic balance. If it succeeds, it returns 0 and clears
// mixer::balance_not_ok_flag; if it fails, it restores the saved states
// and returns 1, also setting mixer::balance_not_ok_flag.

int mixer::balancer::operator()()
{
  // need a complete mixer to do this
  if (mix.flag_mixer_incomplete())
    error::fatal("Must correctly add all elements before\
 calling mixer::balance().");

  if((mix.num_junctions != 0) && (mix.LO <= 0))
    error::fatal("Must have a positive LO frequency during\
 mixer::balance().");

  // if there are no junctions, no work need be done
  if (mix.num_junctions == 0) {
    mix.balance_not_ok_flag = 0;
    mix.LO_saved = mix.LO;
    return 0;
  }

  Matrix Vsave; mix.save_operating_state(Vsave);  // in case of failure

  if (mix.balance_init_flag) i_state();

  init();
  solve();

  if (no_solution()&& !mix.balance_init_flag) {
    // solver failed, try again from a basic operating state:
    i_state();
    init();
    solve();
  }
  if (no_solution()) {
    // balance still fails, out of ideas. Restore original state
    mix.initialize_operating_state(Vsave);
    mix.balance_not_ok_flag = 1;
    return 1;
  }

  // balance worked if we get here.
  mix.balance_not_ok_flag = 0;
  return 0;
}


void mixer::balancer::i_state()
{
  // if there are no junctions, no work need be done
  if (mix.num_junctions == 0) return;

  // save away LO frequency used for operating state calculations
  mix.LO_saved = mix.LO;

  // rebuild data structures if needed and then fetch linear data
  rebuild();
  fill_data();
  
  // Build an array of state voltages (rows:junctions; columns:harmonics).
  // we call the global solve() (in matmath.h), not newton::solve():
  Matrix Vs(mix.num_junctions, mix.max_harmonics + 1, Index_1, Index_C);
  Matrix I = identity_matrix(mix.num_junctions, Index_1);
  
  Vs.fillcol(0, ::solve((I - linear[0].S), linear[0].B));  // DC voltages

  for(int n = 1; n <= mix.max_harmonics; ++n)
    Vs.fillcol(n, ::solve((I - linear[n].S), linear[n].B));

  // Finally call each junction's large_signal() code:
  for(int n = 0; n < mix.num_junctions; ++n)
    mix.junc[n]->large_signal(row(n+1,Vs), mix.LO_saved, mix.max_harmonics);
}


mixer::balancer & mixer::balancer::parameters
(int m, double tf, double tm, double tx, double a)
{
  max_iter = m;    // maximum number of iterations
  f_tol = tf;      // single voltage balance error tolerance
  F_tol = tm;      // variance of all voltages from balance tolerance 
  dx_tol = tx;     // change in a single voltage step tolerance
  rate_factor = a; // if variance of all voltages changes by less than
                   //   this, assume at a local minimum of the error function

  return *this;
}


// ********************************************************************
// balancer private helper functions (matrix element access):

// index(): given junction n and harmonic m, find the index of this
// entry in a representation vector like xlast

inline int mixer::balancer::index(int n, int m)
{
  return rep.junc_inc * n + rep.harm_inc * m;
}


// to_rep(): enter the harmonic state data for junction n, contained in
// state, into the representation vector V at the correct locations

void mixer::balancer::to_rep
(real_vector & V, const Vector & state, int n)
{
  /*register*/ int real_index = index(n, 0);
  /*register*/ int imag_index = real_index + rep.imag_inc;
  for (/*register*/ int m = 0; m <= mix.max_harmonics; ++m) {
    complex v = state.read(m);
    V[real_index] = v.real;
    V[imag_index] = v.imaginary;
    real_index += rep.harm_inc;
    imag_index += rep.harm_inc;
  }
}


// fm_rep(): construct the harmonic state data for junction n into
// state, by reading the representation vector V at the correct locations

void mixer::balancer::fm_rep
(Vector & state, const real_vector & V, int n)
{
  /*register*/ int real_index = index(n, 0);
  /*register*/ int imag_index = real_index + rep.imag_inc;
  for (/*register*/ int m = 0; m <= mix.max_harmonics; ++m) {
    state.get(m) = complex(V[real_index], V[imag_index]);
    real_index += rep.harm_inc;
    imag_index += rep.harm_inc;
  }
}


// B(): fetch the source vector element for junction n at harmonic m
// from the array of linear device sdatas

inline complex mixer::balancer::B(int n, int m)
{ return linear[m].B.read(n+1); }


// S(): fetch the S matrix element S(n1,n2) connecting junctions n1
// and n2 at harmonic m from the array of linear device sdatas

inline complex mixer::balancer::S(int n1, int n2, int m)
{ return linear[m].S.read(n1+1, n2+1); }


// Y(): fetch the Y matrix element Y(m1,m2) connecting harmonicss m1
// and m2 at junction n from the array of junction Y(m1,m2) pointers

inline complex mixer::balancer::Y(int n, int m1, int m2)
{ return pY[n]->read(m1, m2); }


// SV(): calculate the (n,m)th element of the product S*V, ie: at harmonic
// m, calculate Sum( S(n,n')*V(n') ), where the S matrix comes from the
// array of linear sdatas, and the vector V is a representation vector
// such as xlast; only those elements of V associated with harmonic m
// are used; n and n' are junction indexes

complex mixer::balancer::SV(int n, int m, const real_vector & V)
{
  complex result = 0;
  complex * S = linear[m].S[n+1];

  int index_r = index(0,m), index_i = index_r + rep.imag_inc;
  for (/*register*/ int i = 1; i <= mix.num_junctions; ++i) {
    result += S[i]*complex(V[index_r], V[index_i]);
    index_r += rep.junc_inc; index_i += rep.junc_inc;
  }

  return result;
}


// ********************************************************************
// helper functions limited to this source file:


// Kronecker Delta function:
static inline int Delta(int i, int j)
{ return (i == j); }

// various linear combinations of Ynm and Yn-m, scaled by Z0
static inline complex YpY(int n, int m, const Matrix & Y)
{ return device::Z0 * (Y[n][m] + Y[n][-m]); }

static inline complex YmY(int n, int m, const Matrix & Y)
{ return device::Z0 * (Y[n][m] - Y[n][-m]); }

// scaled Yom
static inline complex Yom(int m, const Matrix & Y)
{ return device::Z0 * Y[0][m]; }


// ********************************************************************
// init(): performs the following tasks in preparation for calling
//         the nonlinear solver routine:
// 
// 1. establishes the data representation used in xlast and fills
//    the members of rep
//
// 2. declares and builds the RF balance circuit by terminating RF ports
//    not connected to junctions using balance terminators or default Z0
//    terminations, which it also creates
//
// 3. Properly size the balancer data members used by the nonlinear solver,
//    using the representation established in 1.
//
// 4. performs a get_data() on the bias circuit, then get_data() on the
//    circuit created in 2 at each LO harmonic frequency. The sdata values
//    are copied into the mixer linear device data arrays. The sdata::B
//    vectors are converted to voltages by appropriate scaling
//
// 5. Fill xlast with the initial junction operating state data
//
// 6. Set nonlinear solver control variable values
//
// Uses rebuild() for tasks 1, 2, 3; fill_data() for task 4. 

void mixer::balancer::init()
{
  int n;  // a loop index used variously below

  // if necessary, rebuild data structures:
  rebuild();

  // Calculate the linear circuit sdata's:
  fill_data();

  // save away LO frequency used for operating state calculations
  mix.LO_saved = mix.LO;

  // now build xlast from the current junction state vectors
  for (n = 0; n < mix.num_junctions; ++n) {
    to_rep(xlast, mix.junc[n]->V(), n);
  }

  // now set the maximum allowable step size and reset iteration count
  maxstep = 10;
  iter = 0;

} // mixer::balancer::init()


// ********************************************************************
// rebuild() and fill_data():

// rebuild(): performs the following data allocation tasks, if
//            must_rebuild flag is set, then resets that flag.
// 
// 1. establishes the data representation used in xlast and fills
//    the members of rep
//
// 2. declares and builds the RF balance circuit by terminating RF ports
//    not connected to junctions using balance terminators or default Z0
//    terminations, which it also creates
//
// 3. Properly size the balancer data members used by the nonlinear solver,
//    using the representation established in 1.

void mixer::balancer::rebuild()
{
  if (must_rebuild) {
    int n;
    
    // establish the internal vector representation scheme
    rep.length   = 2 * mix.num_junctions * (mix.max_harmonics + 1);
    rep.harm_inc = 2 * mix.num_junctions;
    rep.junc_inc = 2;
    rep.imag_inc = 1;
    
    // properly size the balancer data members:
    xlast.reallocate(rep.length,Index_C);
    fval.reallocate(rep.length,Index_C);
    ival.reallocate(rep.length, Index_C);
    Jacobian.reallocate(rep.length, rep.length, Index_C, Index_C);
    pY.resize(mix.num_junctions);
    linear.resize(mix.max_harmonics + 1);
    int num_ports = mix.rf_circuit->size() - mix.num_junctions;
    //    default_term.resize(num_ports);  // our Z0 terminations

    // Build the RF balance circuit into temp, using terminators.
    temp = circuit();  // clear out old construction
    for (n = 0; n < num_ports; ++n) {
      int p = mix.num_junctions + n + 1;
      temp.connect(*(mix.rf_circuit), p, 
		   ((mix.term[p-1])? *(mix.term[p-1]) : mix.default_term[n]), 1);
    }
    for (n = 1; n <= mix.num_junctions; ++n)
      temp.add_port(*(mix.rf_circuit), n);

    must_rebuild = 0;
  } // if(must_rebuild)
}


// fill_data():
//    performs a get_data_S() on the bias circuit, then get_data_S() on the
//    circuit created in 2 at each LO harmonic frequency. The sdata values
//    are copied into the mixer linear device data arrays. The sdata::B
//    vectors are converted to voltages by appropriate scaling

void mixer::balancer::fill_data()
{
  parameter IF_saved = device::f;
  double LO = mix.LO;
  double B_factor = 2 * sqrt(device::Z0);  // convert sdata::B units to voltages

  const sdata * ps;   // will point to various get_data() results

  device::f = 0;      // DC bias

  ps = & mix.bias_circuit->get_data_S();  // don't need noise data
  linear[0].S = ps->S;
  linear[0].B = ps->B;
  linear[0].B *= B_factor;

  device::f = LO;
  for (int n = 1; n <= mix.max_harmonics; ++n, device::f += LO ) {
    ps = & temp.get_data_S();  // holds harmonic n sdata
    linear[n].S = ps->S;
    linear[n].B = ps->B;
    linear[n].B *= B_factor;
  }
  device::f = IF_saved;
}


// ********************************************************************
// calc(): called by the nonlinear solver during each iteration, it
// uses the junction voltages in xlast to set the junction operating
// states, then calls junction::large_signal() and small_siganl(). It
// uses the results of these calls to fill fval and Jacobean.

void mixer::balancer::calc()
{
  int n, m;    // common loop indices
  ++iter;      // increment iteration counter

  // fill ival and a vector of pointers to small signal admittance matrices,
  // one per junction.

  Vector V(mix.max_harmonics+1, Index_C);  // holds the voltages for a junction
  for (n = 0; n < mix.num_junctions; ++n) {
    // fill V with junction n's state voltages:
    fm_rep(V, xlast, n);
    // then use V to get state currents into ival
    to_rep(ival, 
	   mix.junc[n]->large_signal(V, mix.LO_saved, mix.max_harmonics), n);

    // next get the small signal admittance matrix
    pY[n] = & mix.junc[n]->small_signal(0, mix.max_harmonics);
  }
  //convert units of ival to voltage from current:
  ival *= device::Z0;


  // use the currents, voltages, and linear sdata objects to calculate
  // the error vector fval: f(V) = I + V + S(I - V) - B

  fval = ival + xlast;  // f(V) = I + V
  ival -= xlast;           // now use ival to hold I - V

  // loop over junctions and harmonics:
  int index_r, index_i;
  for (n = 0; n < mix.num_junctions; ++n) {
    index_r = index(n,0), index_i = index_r + rep.imag_inc;
    for (m = 0; m <= mix.max_harmonics; ++m) {
      complex sum = SV(n,m,ival) - B(n,m);  // S(I - V) - B
      fval[index_r] += sum.real;
      fval[index_i] += sum.imaginary;
      index_r += rep.harm_inc; index_i += rep.harm_inc;
    }
  }
  // finished: fval = f(V) = I + V + S(I - V) - B


  // use the admittance matrices and sdata objects to calculate Jacobian:

  int n2, m2;  // two more loop indices
  for (n = 0; n < mix.num_junctions; ++n) {
    for (n2 = 0; n2 < mix.num_junctions; ++n2) {
      int dn = Delta(n,n2);
      const Matrix & Y = *(pY[n2]);
      for (m = 0; m <= mix.max_harmonics; ++m) {
	int Lindex_r = index(n,m), Lindex_i = Lindex_r + rep.imag_inc;
	for (m2 = 0; m2 <= mix.max_harmonics; ++m2) {
	  int dm = Delta(m,m2);
	  int Rindex_r = index(n2,m2), Rindex_i = Rindex_r + rep.imag_inc;
	  complex Iprime, A;

	  if (m == 0) {
	    Iprime = Yom(m2,Y);
	    A = dn*(Iprime + dm) + S(n,n2,m)*(Iprime - dm);  // A(n,n2,0,m2)
	    Jacobian[Lindex_r][Rindex_r] = A.real;
	    Jacobian[Lindex_i][Rindex_r] = 0;
	    Jacobian[Lindex_r][Rindex_i] = -A.imaginary;
	    Jacobian[Lindex_i][Rindex_i] = 0;
	  }
	  else {
	    Iprime = YpY(m, m2, Y);
	    A = dn*(Iprime + dm) + S(n,n2,m)*(Iprime - dm);  // A(n,n2,m,m2);r
	    Jacobian[Lindex_r][Rindex_r] = A.real;
	    Jacobian[Lindex_i][Rindex_r] = A.imaginary;

	    Iprime = YmY(m, m2, Y);
	    A = dn*(Iprime + dm) + S(n,n2,m)*(Iprime - dm);  // A(n,n2,m,m2);i
	    Jacobian[Lindex_r][Rindex_i] = -A.imaginary;
	    Jacobian[Lindex_i][Rindex_i] = A.real;
	  }
	}  // m2 loop
      }    // m  loop
    }      // n2 loop
  }        // n  loop

  // fix up the singularities in Jacobian for the imaginary parts of the
  // junctions' DC currents (just putting 1's in the diagonal elements):

  index_i = index(0,0) + rep.imag_inc;
  for (n = 0; n < mix.num_junctions; ++n, index_i += rep.junc_inc)
    Jacobian[index_i][index_i] = 1;

} // mixer::balancer::calc()
