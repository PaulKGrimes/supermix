/*

Get an idea how fast the program is.
You should probably compile this with -fast or -O3.

This test program connects 2 2-ports and 2 3-ports 1000 times.

*/

#include "nport.h"
#include "elements.h"
#include "sources.h"
#include "circuit.h"
#include "units.h"
#include "error.h"

#include <iostream>

using namespace std;

int main(void)
{
  device::T = 100.0 * Kelvin;
  device::f = 5.0 * GHz;

  sdata sd;

  resistor r1;
  r1.series();
  r1.R.set(40.0 * Ohm);

  resistor r2;
  r2.series();
  r2.R.set(40.0 * Ohm);

  branch b1;
  branch b2;

  circuit ckt1;

  // Connect two 40 Ohm series resistors in parallel.
  ckt1.connect(b1, 3, r1, 1);
  ckt1.connect(b1, 2, r2, 1);
  ckt1.connect(r1, 2, b2, 1);
  ckt1.connect(r2, 2, b2, 2);
  ckt1.add_port(b1, 1);
  ckt1.add_port(b2, 3);

  // Loop over frequency.
  for(double f=1.0; f<11.0; f+=.001)
  {
    device::f = f*GHz;
    sd = ckt1.get_data();
  }
}
