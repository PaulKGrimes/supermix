// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// port.cc

#include "port.h"
#include "error.h"

port::port()
{
  id = 0;
  index = 0;
}
 
port::port(unsigned long a, int b)
{
  id = a;
  index = b;
}
 
int operator ==(const port & a, const port & b)
{
  return (a.id == b.id) && (a.index == b.index);
}
 
int operator >(const port & a, const port & b)
{
  if(a == b) return 0;
  if(a.id > b.id) return 1;
  return (a.index > b.index);
}
 
portArray::portArray()
{
  length =0;
  data = 0;
}

portArray::portArray(int l)
{
  length = l;
  data = new port [length];
}

portArray::portArray(const portArray & a)
{
  length = a.length;
  data = new port [length];
  for(int i = 0; i<length; i++)
    data[i] = a.data[i];
}

portArray::~portArray()
{
  delete [] data;
}

int portArray::len()
{
  return length;
}

void portArray::set(int index, port value)
{
  if((index<1) || (index>length))
    error::warning("Index out of bounds for portArray set.");
  else
    data[index-1] = value;
}

void portArray::zero()
{
  for(int i=1; i<=length; i++)
      set(i, port());
}

port portArray::get(int index) const
{
  if((index<1) || (index>length))
  {
    error::warning("Index out of bounds for portArray get.");
    return port();
  }
  else
    return data[index-1];
}

portArray& portArray::operator=(const portArray& a)
{
  int i;

  // Beware of self assignment: a = a
  if(this != &a) {

    // Delete the current portArray
    delete [] data;

    // Copy the old portArray over to the new one
    length = a.length;
    data = new port [length];
    for(i = 0; i<length; i++)
      data[i] = a.data[i];
  }
  return *this;
}
