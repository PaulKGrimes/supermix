// test_hybrid.cc
//
// Test hybrid coupler classes.
//
// John Ward
// September 1, 1998
//

#include "supermix.h"
 
int main(void) {
  Complex::rel_tolerance = 1.0e-14;  // get rid of tiny imaginary parts

  // Set the global temperature.
  device::T = 4.2 * Kelvin;
  device::f = 10.0 * GHz;

  // Create a 90 degree hybrid coupler.
  hybrid90 h1;

  // Calculate S and C matrices.
  sdata sd;
  sd = h1.get_data();

  // Display the results.
  cout << "90 degree hybrid coupler S matrix:" << endl;
  sd.S.show();
  cout << endl << "90 degree hybrid C matrix:" << endl;
  sd.C.show();

  // Create a 180 degree hybrid coupler.
  hybrid180 h2;

  // Calculate S and C matrices.
  sd = h2.get_data();

  // Display the results.
  cout << endl << "180 degree hybrid coupler S matrix:" << endl;
  sd.S.show();
  cout << endl << "180 degree hybrid C matrix:" << endl;
  sd.C.show();
}
