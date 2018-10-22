/*

Test class alias.

* 7/28/98: added <iostream.h> to includes

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
  sdata sd;

  // Connect two 40 Ohm series resistors in parallel.
  ckt1.connect(b1, 3, r1, 1);
  ckt1.connect(b1, 2, r2, 1);
  ckt1.connect(r1, 2, b2, 1);
  ckt1.connect(r2, 2, b2, 2);
  ckt1.add_port(b1, 1);
  ckt1.add_port(b2, 3);

  sd = ckt1.get_data();
  cout << "Two 40 Ohm series resistors in parallel S matrix: " << endl;
  sd.S.show();
  cout << "Two 40 Ohm series resistors in parallel C matrix: " << endl;
  (sd.C / Kelvin).show();

  // Create an alias of the circuit ckt1.
  alias ckt2(ckt1);

  sd = ckt2.get_data();
  cout << "Alias of two 40 Ohm series resistors in parallel S matrix: " << endl;
  sd.S.show();
  cout << "Alias of two 40 Ohm series resistors in parallel C matrix: " << endl;
  (sd.C / Kelvin).show();

  // Test alias operator=
  alias ckt3;
  ckt3 = ckt1;

  sd = ckt3.get_data();
  cout << "Alias of two 40 Ohm series resistors in parallel S matrix: " << endl;
  sd.S.show();
  cout << "Alias of two 40 Ohm series resistors in parallel C matrix: " << endl;
  (sd.C / Kelvin).show();

  // Change the original, and verify that the alias also changed.
  cout << "\nChanging original." << endl;
  r1.R = 20.0 * Ohm;

  sd = ckt1.get_data();
  cout << "Two series resistors in parallel S matrix: " << endl;
  sd.S.show();
  cout << "Two series resistors in parallel C matrix: " << endl;
  (sd.C / Kelvin).show();

  sd = ckt2.get_data();
  cout << "Alias of two series resistors in parallel S matrix: " << endl;
  sd.S.show();
  cout << "Alias of two series resistors in parallel C matrix: " << endl;
  (sd.C / Kelvin).show();

  sd = ckt3.get_data();
  cout << "Alias of two series resistors in parallel S matrix: " << endl;
  sd.S.show();
  cout << "Alias of two series resistors in parallel C matrix: " << endl;
  (sd.C / Kelvin).show();

}
