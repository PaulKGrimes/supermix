// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// real_parameter.cc

#include "parameter/real_parameter.h"
#include "global.h"
#include "error.h"
#include <iostream>

using namespace std;


// default constructor
real_parameter::real_parameter(double v) :
  value(v),
  shadowed(0),
  get_flag(false),
  use_min(false), min(0.0),
  use_max(false), max(0.0)
{ }

real_parameter::real_parameter(int v) :
  value(double(v)),
  shadowed(0),
  get_flag(false),
  use_min(false), min(0.0),
  use_max(false), max(0.0)
{ }

// shadow another parameter
real_parameter::real_parameter(const abstract_real_parameter *p) :
  value(0.0),
  shadowed(p),
  get_flag(false),
  use_min(false), min(0.0),
  use_max(false), max(0.0)
{
  if (p == 0) {
    error::warning("real_parameter constructor: " 
		   "Null pointer passed to constructor; not shadowing.");
  }
}

double real_parameter::get() const
{
  if (is_local()) return value;

  else {

    // if shadow loop brings us back to this object, we quit indirecting
    if(get_flag) {
      get_flag = false;  // reset it for next time
      error::warning("real_parameter::get(): Terminating infinite" 
		     " shadowing loop, returning zero.");
      return 0.0;
    }

    // use indirection through the shadowing pointer
    get_flag = true;
    double v = shadowed->get();
    get_flag = false;

    return limit(v);
  }
}

real_parameter & real_parameter::shadow(const abstract_real_parameter &p)
{
  // do nothing in case argument is this object
  if (&p == this) {
    error::warning("real_parameter::shadow(): a real_parameter must not shadow itself.");
  }
  else {
    shadowed = &p;
  }
  return *this;
}

real_parameter & real_parameter::set_min(double m)
{
  if(use_max && m >= max) {
    error::warning("real_parameter::set_min(): minimum should be"
		   " less than real_parameter maximum.");
    m = max;
  }

  use_min = true;
  min = m;
  if(is_local() && value < min) value = min;
  return *this;
}

real_parameter & real_parameter::set_max(double m)
{
  if(use_min && m <= min) {
    error::warning("real_parameter::set_max(): maximum should be"
		   " greater than real_parameter minimum.");
    m = min;
  }

  use_max = true;
  max = m;
  if(is_local() && value > max) value = max;
  return *this;
}

real_parameter& real_parameter::operator=(const abstract_real_parameter *p)
{
  if(p == 0) {
    error::warning("real_parameter assignment: Cannot set real_parameter"
		   " to shadow a null pointer !") ;
    return *this;
  }
  else return shadow(*p);
}

real_parameter& real_parameter::clone(const real_parameter & p)
{
  // do nothing in case argument is this object or directly shadows this object
  if (&p == this || p.shadowed == this) return *this;

  // make an identical twin of the argument
  value = p.value;
  shadowed = p.shadowed;
  use_min = p.use_min;
  min = p.min;
  use_max = p.use_max;
  max = p.max;

  return *this;
}

istream & operator >>(istream & in_file, real_parameter & p)
{
  double temp;
  in_file >> temp;
  p.set(temp);
  return in_file;
}
