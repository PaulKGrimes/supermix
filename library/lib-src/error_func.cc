// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// error_func.cc

#include "error_func.h"
#include "error.h"
#include <iostream>

using namespace std;

void error_func::add_term(double weight, error_term & et, sweeper & swp)
{
  // make an entry in the master terms vector:
  terms.push_back(error_func::weighted_term(weight, et));
  // put the index of this term in the associated sweeper's terms vector:
  (swp_map[&swp]).sweeper_terms.push_back(terms.size() - 1) ;
}

void error_func::add_term(double weight, error_term & et)
{
  // make an entry in the master terms vector:
  terms.push_back(error_func::weighted_term(weight, et));
  // put the index of this term in the sweeperless_terms vector:
  sweeperless_terms.push_back(terms.size() - 1);
}

real_vector error_func::get_func_breakdown() const
{
  real_vector result(terms.size());
  int i;
  term_index j;
  for(i = result.minindex(), j = 0; i <= result.maxindex(); i++, j++)
    result[i] = terms[j].result;
  return result;
}

double error_func::func_value()
{
  double retval = 0. ;

  // Loop over the terms that don't have sweepers.
  state_tag tag = state_tag::get_tag();
  for(term_index_list_index i = 0; i < sweeperless_terms.size(); i++)
    retval += terms[sweeperless_terms[i]].reset().get(tag).result;

  // loop over all sweepers
  std::map<sweeper *, error_func::sweeper_info>::iterator is ;
  for(is=swp_map.begin(); is != swp_map.end(); is++) {

    // get a reference to this sweeper and its sweeper_info
    sweeper &       swp     = *(*is).first ;
    sweeper_info &  errors  =  (*is).second;

    // First reset the sweeper, so parameters are initialized
    swp.reset();

    // Now reset the error terms for this sweeper.
    errors.reset_terms(terms);

    // now loop over this sweeper's parameter range
    for(; !swp.finished(); swp++) {
      tag = state_tag::get_tag();
      errors.calc_terms(tag, terms) ;
    }

    // average the values of the terms
    retval += errors.get_error(swp.npoints(), terms);
  }
  return retval ;
}


void error_func::sweeper_info::reset_terms(error_func::term_list & terms)
{
  // Loop through the error terms, calling their reset methods.
  for(error_func::term_index_list_index i = 0; i < sweeper_terms.size(); i++)
    terms[sweeper_terms[i]].reset();
}

void error_func::sweeper_info::calc_terms(state_tag t, error_func::term_list & terms)
{
  // Loop through the error terms, calling their get methods.
  for(error_func::term_index_list_index i = 0; i < sweeper_terms.size(); i++)
    terms[sweeper_terms[i]].get(t);
}

double error_func::sweeper_info::get_error(int n, error_func::term_list & terms)
{
  double retval = 0. ;
  // Loop through the terms, dividing their results by n and adding them up
  for(error_func::term_index_list_index i = 0; i < sweeper_terms.size(); i++) {
    terms[sweeper_terms[i]].result /= n;
    retval += terms[sweeper_terms[i]].result;
  }
  return(retval) ;
}


double error_term_mode::checkval(double x)
{
  // If we are not optimizing for FLAT, we want to compute the error
  // based on the target value.
  if (flag != FLAT)
    x -= target_val;

  double new_error, return_value;  // used by some cases below

  switch(flag) {

    // the easy ones first...

  default:
    error::warning("Unknown mode setting in error_term_mode::checkval");
    return 0.;

  case MATCH:  return x*x;
  case ABOVE:  return (x < 0.) ? x*x : 0.;
  case BELOW:  return (x > 0.) ? x*x : 0.;


    // now the ones that need a memory of past values:

  case FLAT: {
    // Update the sum, sum of the squares, and counter.
    sum_xx += x*x;
    sum_x += x;
    n++;

    // Find the new value of the variance from the mean
    new_error = sum_xx - sum_x*sum_x/n;
  
    // We will only return the increase of the total error function,
    // since the caller is keeping a running sum.
    return_value = new_error - old_error;
  
    // Update old_error so that it will be correct for the next iteration.
    // Notice that we increment old_error rather than replace by new_error.
    // That way our old_error matches the caller's running sum, and maybe we
    // can minimize truncation errors that way. 
    old_error += return_value;
    return return_value;
  }

    // the following cases leave the switch without returning from the function:
  case M_ABOVE:
    new_error = (x < 0.) ? x*x : 0.;
    break;
  case M_BELOW:
    new_error = (x > 0.) ? x*x : 0.;
    break;
  case M_MATCH:
    new_error = x*x;
    break;
  } // switch

  // The modes M_ABOVE, M_BELOW, and M_MATCH get here,
  // with properly set value for new_error:

  n++;  // calls counter
    
  if (new_error > old_error) {
    // Oops, a new maximum error has been found.

    // The value returned by all calls should be the maximum error found;
    // since earlier calls returned values that were too small, we correct
    // by including the correction for all those previous calls as well. 
    return_value = n*new_error - (n-1)*old_error;

    // Update old_error so that it will be correct for the next iteration.
    old_error = new_error;
    return return_value;
  }
  else
    // Nothing has changed; the old maximum is still the maximum so far.
    return old_error;
}


double scaled_match_error_term::get(state_tag tag)
{
  // Get the values.
  double a = get_a(tag);
  double b = get_b(tag);

  // Error = Sum[(a - k*b)*(a - k*b)],
  //   with k chosen to minimize the error. The value of the scale
  //   factor k is therefore:
  // k = Sum[a*b]/Sum[b*b].
  //   with this value for k, the formula for the error is:
  // Error = Sum[a*a] - Sum[a*b]*Sum[a*b]/Sum[b*b]
  //       = N*<a*a>*(1 - (<a*b>*<a*b>)/(<a*a>*<b*b>))
  //   Where <x> indicates the mean value of the expression x. Note
  //   that since error_func divides the error by N, the number of
  //   points in the sweep, the error term contibution will be
  //   independent of the number of points in the sweeper, although
  //   it WILL depend on the mean value of the square of a, the
  //   reference data. By dividing out this mean square value, we
  //   can get an error term which is independent of the actual
  //   overall scale of the reference data as well. Therefore,
  //   we actually use the formula:
  // Error = N*(1 - (Sum[a*b]*Sum[a*b])/(Sum[a*a]Sum[b*b]))
  //   once error_func divides this value by N, we see that we
  //   get an error contribution that must be between 0 and 1.

  // We save the error we calculate. When we get the next get()
  // call, we calculate a new error and return the difference
  // between it and the error we saved. That way if the caller
  // sums the return values, his running sum will reflect the
  // true error (except for inaccuracies due to roundoff errors).

  // Update the sums.
  sum_aa += a*a;
  sum_bb += b*b;
  sum_ab += a*b;
  n++;

  // Find the new value of the error function. We check for pathological
  // cases.
  double new_error;
  if (sum_aa == 0.0)
    new_error = 0.0;
  else if (sum_bb == 0.0)
    new_error = 1.0;
  else {
    new_error = 1.0 - sum_ab*sum_ab/(sum_aa*sum_bb);
    if (new_error < 0.0) new_error = 0.0;
  }
  new_error *= n;  // compensate for error_func averaging


  // We will only return the change in the total error function,
  // since the caller is keeping a running sum.
  double change = new_error - old_error;

  // Update old_error so that it will be correct for the next iteration.
  // Notice that we increment by change rather than replace by new_error.
  // That way our old_error matches the caller's running sum, and maybe we
  // can minimize truncation errors that way. 
  old_error += change;

  // Return the change in the error function.
  return change;
}

