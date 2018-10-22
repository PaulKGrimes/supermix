/*

Test class circuit by creating a variety of different circuits.

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

  resistor r1;
  r1.series();
  r1.R = 40.0 * Ohm;

  resistor r2;
  r2.series();
  r2.R = 40.0 * Ohm;

  branch b1;
  branch b2;

  circuit ckt1;
  circuit ckt2;
  sdata sd;

  // Connect two 40 Ohm series resistors in parallel.
  ckt1.connect(b1, 3, r1, 1);
  ckt1.connect(b1, 2, r2, 1);
  ckt1.connect(r1, 2, b2, 1);
  ckt1.connect(r2, 2, b2, 2);
  ckt1.add_port(b1, 1);
  ckt1.add_port(b2, 3);

  cout << "About to get data." << endl;
  sd = ckt1.get_data();
  cout << "Two 40 Ohm series resistors in parallel S matrix: " << endl;
  sd.S.show();
  cout << "Two 40 Ohm series resistors in parallel C matrix: " << endl;
  (sd.C / Kelvin).show();

  // Do it again, this time in a different order.
  ckt2.connect(b1, 2, r1, 1);
  ckt2.connect(b1, 3, r2, 1);
  ckt2.connect(b2, 1, r1, 2);
  ckt2.connect(b2, 2, r2, 2);
  ckt2.add_port(b1, 1);
  ckt2.add_port(b2, 3);

  cout << "About to get data." << endl;
  sd = ckt2.get_data();
  cout << "Two 40 Ohm series resistors in parallel S matrix: " << endl;
  sd.S.show();
  cout << "Two 40 Ohm series resistors in parallel C matrix: " << endl;
  (sd.C / Kelvin).show();

  // Create a silly 5 GHz low pass filter
  capacitor c1;
  c1.parallel();
  c1.C = 0.6 * pFarad;
  r1.R = 1.0 * Ohm;
  ckt1 = circuit();
  ckt1.connect(c1, 2, r1, 1);
  ckt1.add_port(r1, 2);
  ckt1.add_port(c1, 1);

  // Loop over frequency.
  for(double f=1.0; f<11.0; f+=1.0)
  {
    device::f = f*GHz;
    cout << "f=" << f << " GHz, S21=" << ckt1.S(2,1) << endl;
  }

  // Try something that will give a block diagonal S matrix
  cout << "\nTry creating a block diagonal matrix." << endl;
  r1.R = 20.0 * Ohm;
  r2.R = 30.0 * Ohm;
  ckt1 = circuit();
  ckt1.connect(b1, 3, r1, 1);
  ckt1.connect(b2, 3, r2, 1);
  ckt1.add_port(b2, 1);
  ckt1.add_port(b2, 2);
  ckt1.add_port(r2, 2);
  ckt1.add_port(b1, 1);
  ckt1.add_port(b1, 2);
  ckt1.add_port(r1, 2);
  sd = ckt1.get_data();
  cout << "Block diagonal S matrix: " << endl;
  sd.S.show();
  cout << "Block diagonal C matrix: " << endl;
  (sd.C / Kelvin).show();
}
