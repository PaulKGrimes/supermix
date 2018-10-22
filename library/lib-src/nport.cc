// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// nport.cc

#include "nport.h"
#include "units.h"
#include "error.h"

using namespace std;

unsigned long state_tag::current_state = 1;
unsigned long device::devcount = 0;

parameter device::T  = 300 * Kelvin;
parameter device::Z0 = 50 * Ohm;
parameter device::f  = 0.0;

// **************************************************************

port nport::get_port(int index)
{
  if((index > 0) && (index <=size()))
    return port(id, index);

  // Subscript out of range, return default port.
  error::warning("Port index out of range in nport::get_port(int).");
  return port();
}

int nport::get_port(port p)
{
  if((p.id == id) && (p.index > 0) && (p.index <=size()))
  {
    return p.index;
  }

  // Subscript out of range, return default port.
  error::warning("Port out of range in nport::get_port.");
  return 0;
}

complex nport::S(int i, int j)
{
  // Don't make any assumptions about how this device recalculates itself.
  // This way, we can avoid having to overload this function.
  const sdata *dptr = &get_data();

  // Check range on (i,j) and warn if invalid:
  if((i<1)||(j<1)||(i>size())||(j>size()))
    error::warning("Port number out of range for call to S(i,j).");

  return dptr->S.read(i,j);
}

complex nport::C(int i, int j)
{
  // Don't make any assumptions about how this device recalculates itself.
  // This way, we can avoid having to overload this function.
  const sdata *dptr = &get_data();

  // Check range on (i,j) and warn if invalid:
  if((i<1)||(j<1)||(i>size())||(j>size()))
    error::warning("Port number out of range for call to C(i,j).");

  return dptr->C.read(i,j);
}

complex nport::B(int i)
{
  // Don't make any assumptions about how this device recalculates itself.
  // This way, we can avoid having to overload this function.
  const sdata *dptr = &get_data();

  // Check range on (i) and warn if invalid:
  if((i<1)||(i>size()))
    error::warning("Port number out of range for call to B(i).");

  return dptr->B.read(i);
}

// **************************************************************

alias & alias::operator=(nport & n)
{
  // Beware of self assignment: n = n
  if(this != &n)
  {
    original = &n;
    data_ptr = &data;
  }
  return *this;
}
