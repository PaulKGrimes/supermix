// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// function_real_parameter.cc

#include "parameter/function_real_parameter.h"
#include "error.h"


// default constructor
function_real_parameter::function_real_parameter() :
  function(0),
  function_pars(0)
{ }

function_real_parameter::function_real_parameter(double (* func)(real_parameter *), real_parameter *pars) :
  function(func),
  function_pars(pars)
{ }

double function_real_parameter::get() const
{
  if(function==0)
  {
    error::warning(
      "Accessing an uninitialized function_real_parameter, returning zero.");
    return 0.0;
  }

  return function(function_pars);
}

void function_real_parameter::set(double (* func)(real_parameter *), real_parameter *pars)
{
  function = func;
  function_pars = pars;
}
