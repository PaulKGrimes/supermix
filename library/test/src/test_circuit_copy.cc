/*

Test circuit::circuit(circuit)

* 7/28/98: added <iostream.h> to includes, since SIScmplx.h doesn't*

*/


#include <iostream>
#include "nport.h"
#include "elements.h"
#include "sources.h"
#include "circuit.h"
#include "units.h"
#include "error.h"

using namespace std;

int main(void)
{
  device::T = 100.0 * Kelvin;
  device::f = 5.0 * GHz;

  // Create a very silly 5 GHz low pass filter
  capacitor c1;
  resistor r1;
  circuit ckt1;
  c1.parallel();
  c1.C = 0.6 * pFarad;
  r1.R.set(1.0 * Ohm);
  ckt1.connect(c1, 2, r1, 1);
  ckt1.add_port(r1, 2);
  ckt1.add_port(c1, 1);

  // Create a copy of the low pass filter.
  circuit ckt2(ckt1);
  circuit ckt3;
  ckt3 = ckt2;

  // Loop over frequency, comparing the original to the copy.
  for(double f=1.0; f<11.0; f+=1.0)
  {
    device::f = f*GHz;
    cout << "orig f=" << f << " GHz, S21=" << ckt1.S(2,1) << endl;
    cout << "copy f=" << f << " GHz, S21=" << ckt2.S(2,1) << endl;
    cout << "assi f=" << f << " GHz, S21=" << ckt3.S(2,1) << endl;
  }
}
