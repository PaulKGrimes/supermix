// test_circuit_block
// Build block-diagonal devices using circuit and test them
// tests the new circuit and connection code added 7/3/98.

#include "supermix.h"

int main()
{
  cout << endl;

  // use these voltage_source's as 1-port resistors:
  voltage_source r1; r1.R = 100*Ohm;
  voltage_source r2; r2.R = 25*Ohm;

  sdata s1 = r1.get_data();
  sdata s2 = r2.get_data();

  cout << "r1 = 100 Ohms:" << endl;
  cout << "S matrix:" << endl;
  s1.S.show();
  cout << "Z matrix:" << endl;
  zdata(s1).Z.show();

  cout << "r2 = 25 Ohms:" << endl;
  cout << "S matrix:" << endl;
  s2.S.show();
  cout << "Z matrix:" << endl;
  zdata(s2).Z.show();

  // a "circuit" which is a single 1-port:
  circuit c;
  c.add_port(r1,1);

  cout << "c = single 100 Ohms" << endl;
  sdata sc = c.get_data();
  cout << "S matrix:" << endl;
  sc.S.show();
  cout << "Z matrix:" << endl;
  zdata(sc).Z.show();

  // now two unconnected 1-ports:
  c = circuit();
  c.add_port(r1,1);
  c.add_port(r2,1);

  cout << "c = block diagonal 100 & 25 Ohms" << endl;
  sc = c.get_data();
  cout << "S matrix:" << endl;
  sc.S.show();
  cout << "Z matrix:" << endl;
  zdata(sc).Z.show();



}
