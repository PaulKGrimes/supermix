// test_circulator.cc
//
// Test circulator class.
//
// John Ward
// September 1, 1998
//

#include "supermix.h"
 
int main(void) {
  circulator c;

  // Set the global temperature.
  device::T = 4.2 * Kelvin;
  device::f = 10.0 * GHz;

  // Calculate S and C matrices.
  sdata sd;
  sd = c.get_data();

  // Display the results.
  cout << "Circulator S matrix:" << endl;
  sd.S.show();
  cout << endl << "Circulator C matrix:" << endl;
  sd.C.show();

  cout << "Try copy constructor." << endl;
  circulator c2(c);
  sd = c2.get_data();
  sd.S.show();
  cout << endl;
  sd.C.show();
  cout << endl;

  cout << "Try operator=" << endl;
  circulator c3;
  c3 = c2;
  sd = c3.get_data();
  sd.S.show();
  cout << endl;
  sd.C.show();
  cout << endl;
}
