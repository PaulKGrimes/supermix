// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// complex_parameter.cc

#include "parameter/complex_parameter.h"
#include "error.h"
#include <iostream>

using namespace std;

// constructors

complex_parameter::complex_parameter(Complex v) :
  value(v),
  mode(VALUE),
  cp(0),
  rp2(0),
  get_flag(false)
{ }

complex_parameter::complex_parameter(int v) :
  value(Complex(double(v))),
  mode(VALUE),
  cp(0),
  rp2(0),
  get_flag(false)
{ }

complex_parameter::complex_parameter(double v) :
  value(Complex(v)),
  mode(VALUE),
  cp(0),
  rp2(0),
  get_flag(false)
{ }

complex_parameter::complex_parameter(const abstract_complex_parameter *p) :
  value(Complex(0.0)), 
  mode(C_SHADOW),
  cp(p),
  rp2(0),
  get_flag(false)
{
  if (p == 0) {
    error::warning("complex_parameter constructor: " 
		   "Null pointer passed to constructor; not shadowing.");
    mode = VALUE ;
  }
}

complex_parameter::complex_parameter(const abstract_real_parameter *p) :
  value(Complex(0.0)), 
  mode(R_SHADOW),
  rp1(p),
  rp2(0),
  get_flag(false)
{
  if (p == 0) {
    error::warning("complex_parameter constructor: " 
		   "Null pointer passed to constructor; not shadowing.");
    mode = VALUE ;
  }
}

complex_parameter::complex_parameter(const abstract_real_parameter &p1,
				     const abstract_real_parameter &p2,
				     int imode) :
  value(Complex(0.0)), 
  mode(imode),
  rp1(&p1),
  rp2(&p2),
  get_flag(false)
{ 
  if(mode != CARTESIAN && mode != POLAR) {
    error::warning("complex_parameter constructor: " 
		   "Improper mode chosen - setting to CARTESIAN !") ;
    mode = CARTESIAN ;
  }
}


// other member functions

Complex complex_parameter::get() const
{
  switch(mode) {

  default:
  case VALUE:
    return value;

  case R_SHADOW:
    return Complex(rp1->get());

  case CARTESIAN:
    return Complex(rp1->get(), rp2->get());

  case POLAR:
    return polar(rp1->get(), rp2->get());
    
  case C_SHADOW:
    // if shadow loop brings us back to this object, we quit indirecting
    if(get_flag) {
      get_flag = false;  // reset it for next time
      error::warning("complex_parameter::get(): Terminating infinite" 
		     " shadowing loop, returning zero.");
      return Complex(0.0);
    }

    get_flag = true;
    Complex v = cp->get();
    get_flag = false;
    return v;
  }
}

complex_parameter & complex_parameter::shadow(const abstract_complex_parameter &p)
{
  // do nothing in case argument is this object
  if (&p == this) {
    error::warning("complex_parameter::shadow(): a complex_parameter must not shadow itself.");
  }
  else {
    mode = C_SHADOW;
    cp = &p;
  }
  return *this;
}

complex_parameter & complex_parameter::shadow(const abstract_real_parameter &p)
{
  mode = R_SHADOW;
  rp1 = &p;
  return *this;
}

complex_parameter & complex_parameter::shadow(const abstract_real_parameter &p1,
					      const abstract_real_parameter &p2,
					      int imode)
{
  mode = imode;
  if(mode != CARTESIAN && mode != POLAR) {
    error::warning("complex_parameter::shadow(): " 
		   "Improper mode chosen - setting to CARTESIAN !") ;
    mode = CARTESIAN ;
  }
  rp1 = &p1;
  rp2 = &p2;
  return *this;
}

complex_parameter& complex_parameter::operator=(const abstract_complex_parameter *p)
{
  if(p == 0) {
    error::warning("complex_parameter assignment: Cannot set complex_parameter"
		   " to shadow a null pointer !") ;
    return *this;
  }
  else return shadow(*p);
}

complex_parameter& complex_parameter::operator=(const abstract_real_parameter *p)
{
  if(p == 0) {
    error::warning("complex_parameter assignment: Cannot set complex_parameter"
		   " to shadow a null pointer !") ;
    return *this;
  }
  else return shadow(*p);
}

complex_parameter & complex_parameter::clone(const complex_parameter &p)
{
  // do nothing in case argument is this object or directly shadows this object
  if (&p == this || (p.mode == C_SHADOW && p.cp == this)) return *this;

  // make an identical twin of the argument
  value = p.value;
  mode = p.mode;
  cp = p.cp;
  rp2 = p.rp2;

  return *this;
}


// other operations

istream & operator >>(istream & in_file, complex_parameter & cp)
{
  Complex tmp1;
  double tx, ty ;
  in_file >> tx;
  in_file >> ty;
  tmp1 = Complex(tx, ty) ;
  cp.set(tmp1);
  return in_file;
}





