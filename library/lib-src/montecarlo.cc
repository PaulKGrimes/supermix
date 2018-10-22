// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// montecarlo.cc

#include <cstdlib>
#include <ctime>
#include "error.h"
#include "montecarlo.h"
#include <iostream>

using namespace std;

montecarlo::montecarlo(abstract_error_func & aef) :
  minimizer(aef), q_(aef), p_(aef), m(0), n(100), display_f(false),
  display_r(0.0), t1(1000.), t2(100.)
{ srand48(time(0)); verbose(); }

montecarlo::montecarlo(abstract_error_func & aef, minimizer & method) :
  minimizer(aef), q_(aef), p_(aef), m(&method), n(100), display_f(false),
  display_r(0.0), t1(1000.), t2(100.)
{ srand48(time(0)); verbose(); }

void montecarlo::randomize()
{
  static real_vector x, y;     // only allocate once
  x = erf.get_min_parms();     // x gets sized here
  y = erf.get_max_parms();     // y gets sized here

  for (int i = x.minindex(); i <= x.maxindex(); ++i) {
    x[i] += drand48()*(y[i] - x[i]);
  }

  erf.set_parms(x);
}

double montecarlo::minimize()
{
  // set up local minimizer
  if(target_on) {
    q_.set_target(target);
    if(m) m->set_target(target);
    else  p_.set_target(target);
  }
  else {
    q_.no_target();
    if(m) m->no_target();
    else  p_.no_target();
  }
  if(is_very_verbose) {
    q_.verbose();
    if(m) m->verbose();
    else  p_.verbose();
  }
  else {
    q_.quiet();
    if(m) m->quiet();
    else  p_.quiet();
  }

  // the first iteration will use the provided initial point:
  erf.set_parms(erf.get_initial_parms());
  best_e = (m) ? m->minimize() : p_.minimize();
  best_x = erf.get_parms();
  if(is_verbose)
    error::stream() << 1 << " : " << best_e << " : " 
		    << erf.get_parms_user() << endl;

  // check for early termination
  if(stop(1)) return best_e;

  // the rest of the iterations:
  for( unsigned i = 1; i < n; ++i) {
    randomize();
    double e = erf();
    if(e < t1*best_e && e >= t2*best_e) e = q_.minimize();
    if(e < t2*best_e) e = (m) ? m->minimize() : p_.minimize();
    if(e < best_e) {
      best_e = e;
      best_x = erf.get_parms();
      if(is_verbose)
	error::stream() << i+1 << " : " << best_e << " : " 
			<< erf.get_parms_user() << endl;
    }
    // some other tests in case of verbose output:
    else if(display_f && is_verbose) 
      error::stream() << i+1 << " : " << e << " : " 
		      << erf.get_parms_user() << endl;
    else if(display_r > 0.0 && is_verbose) {
      if((best_e > 0.0 && e <= (1+display_r)*best_e)||
	 (best_e < 0.0 && e <= (1-display_r)*best_e))
	error::stream() << i+1 << " : " << e << " : " 
			<< erf.get_parms_user() << endl;
    }

    // check for early termination
    if(stop(i+1)) break;
  }

  // set error function parameters to the best seen and
  // recalculate error fuction one last time to make its
  // internal state consistent with the return value.
  erf.set_parms(best_x);
  return erf();
}
