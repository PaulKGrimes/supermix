// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// scaled_real_parameter.cc

#include "parameter/scaled_real_parameter.h"
#include "global.h"
#include "error.h"


// default constructor
scaled_real_parameter::scaled_real_parameter() :
  shadowed(0),
  scale(0.),
  get_flag(0)
{ }

scaled_real_parameter::scaled_real_parameter(const double s, abstract_real_parameter & p) :
  shadowed(&p),
  scale(s),
  get_flag(0)
{ }

double scaled_real_parameter::get() const
{
  if(shadowed==0)
  {
    error::warning(
      "Accessing an uninitialized scaled_real_parameter, returning zero.");
    return 0.0;
  }

  if(get_flag)     // is flag on already !!?
  {
    get_flag = 0;  // reset it for next time
    error::warning(
       "Terminating infinite real_parameter shadowing loop, returning zero.");
    return 0.;
  }

  get_flag = 1;

  double v = scale * shadowed->get();

  get_flag = 0;

  return v;
}

void scaled_real_parameter::set(const double s, abstract_real_parameter & p)
{
  shadowed = &p;
  scale = s;
}
