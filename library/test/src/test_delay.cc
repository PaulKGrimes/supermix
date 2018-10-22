// test_delay.cc
//
// Test time_delay class.
//
// John Ward
// September 14, 1998
//

#include "supermix.h"
 
int main(void) {
  Complex::rel_tolerance = 1.0e-14;  // get rid of tiny imaginary parts

  time_delay d;

  complex::out_degree();
  cout << setprecision(2) << setw(10) << fixed;

  // Set the global temperature.
  device::T = 4.2 * Kelvin;
  device::f = 10.0 * GHz;

  // Calculate S and C matrices.
  sdata sd;
  sd = d.get_data();

  // Display the results.
  cout << "time_delay S matrix:" << endl;
  sd.S.show();
  cout << endl << "time_delay C matrix:" << endl;
  sd.C.show();

  cout << "Increase time delay and watch phase step in 45 degree increments:"
       << endl;
  d.time = 12.5 * Pico * Second;
  sd = d.get_data();
  sd.S.show();
  cout << endl;

  d.time = 25. * Pico * Second;
  sd = d.get_data();
  sd.S.show();
  cout << endl;

  d.time = 37.5 * Pico * Second;
  sd = d.get_data();
  sd.S.show();
  cout << endl;

  d.time = 50. * Pico * Second;
  sd = d.get_data();
  sd.S.show();
  cout << endl;

  d.time = 62.5 * Pico * Second;
  sd = d.get_data();
  sd.S.show();
  cout << endl;

  d.time = 75. * Pico * Second;
  sd = d.get_data();
  sd.S.show();
  cout << endl;

  d.time = 87.5 * Pico * Second;
  sd = d.get_data();
  sd.S.show();
  cout << endl;

  d.time = 100. * Pico * Second;
  sd = d.get_data();
  sd.S.show();
  cout << endl;

  d.time = 112.5 * Pico * Second;
  sd = d.get_data();
  sd.S.show();
  cout << endl;

  cout << "Increase frequency for phase of 90 degrees:"
       << endl;
  d.time = 12.5 * Pico * Second;
  device::f = 20. * GHz;
  sd = d.get_data();
  sd.S.show();
  cout << endl;

  cout << "Test operator= (expect 90 degrees)"
       << endl;
  time_delay d2 = d;
  sd = d2.get_data();
  sd.S.show();
  cout << endl;
  sd.C.show();
  cout << endl;

  cout << "Test copy constructor (expect 90 degrees)"
       << endl;
  time_delay d3(d2);
  sd = d3.get_data();
  sd.S.show();
  cout << endl;
  sd.C.show();
  cout << endl;
}
