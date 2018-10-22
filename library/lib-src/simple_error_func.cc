// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// simple_error_func.cc

#include "simple_error_func.h"
#include "error.h"
#include <iostream>

using namespace std;

abstract_real_parameter * error_func_parameters::vary(double min, double init, 
                                                  double max, double scale)
{
  if(min > max)
    error::fatal("min > max in error_func_parameters::vary() argument list");

  if(scale <= 0.0)
    error::fatal("Units argument <= 0 in error_func_parameters::vary()");

  // scale the min, init, and max values
  min  *= scale;
  init *= scale;
  max  *= scale;

  // increment the count and save the values in their associated vectors
  num_parms++;
  minimum.push_back(min);
  initial.push_back(init);  // this value isn't limited by min, max
  maximum.push_back(max);
  units.push_back(scale);

  // create and set the associated variable to a limited initial value
  init = limit(min, init, max);
  parms.push_back(init);    // the actual value is limited by min,max
  current.push_back(error_func_parameters::vary_parameter(init));

  // Return an abstract_real_parameter pointer to the variable
  // Note that the following funny syntax is needed.
  // The * dereferences the STL reverse_iterator rbegin(), which
  // "points" to the last element of the list.
  // The & produces a pointer to an abstract_real_parameter object.
  return(&(*current.rbegin())) ;   
}

real_vector error_func_parameters::get_parms_user()
{
  real_vector pv(size(), Index_1);

  for(int i = 1; i <= size(); i++)
    pv[i] = parms[i]/units[i];

  return(pv) ;
}

void error_func_parameters::set_parms(const real_vector & pv)
{
  // check if vector length is ok  
  int imin = pv.minindex() ;
  int imax = pv.maxindex() ;
  if(size() != imax-imin+1) {
    error::warning(
    "error_func_parameters::set_parms: length of real_vector != number of parameters") ;
  }
  
  // and store away the values (note that we will take incorrect length)
  int i = imin ;
  int j = 1;      // index into minimum, maximum, and parms vectors (Index_1)
  std::list<vary_parameter>::iterator ip ;
  for(ip=current.begin(); ip!= current.end() && i <= imax; ip++, i++, j++)
  {
    double v = limit(minimum[j], pv[i], maximum[j]);
    parms[j] = v;
    ip->set(v);
  }

  return ;
}

