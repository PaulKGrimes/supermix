// Test class ampdata
//
// Recalculate examples from "Microwave Engineering," by David Pozar, page 617
//
// Also test error cases, including invalid parameters
// and device that are not 2 ports.
//
// John Ward
// November 29, 1998
//
// Changes:
// 6/99  Added test for t_min, gamma_opt(), and Rn().
//

#include "supermix.h"

int main()
{

  // Test math against Pozar example 11.5
  cout << "Calculating results of Pozar example 11.5, page 617." << endl;
  ampdata ad;
  ad.S[1][1] = polar(0.894, -60.6*Degree);
  ad.S[2][1] = polar(3.122, 123.6*Degree);
  ad.S[1][2] = polar(0.020, 62.4*Degree);
  ad.S[2][2] = polar(0.781, -27.6*Degree);

  complex::out_degree();
  cout << setprecision(3);

  cout << "delta = " << ad.delta() << endl;
  cout << "K = " << ad.k() << endl;

  cout << "\nLoad (port 2):" << endl;
  cout << "Center = " << ad.center(2) << endl;
  cout << "Radius = " << ad.radius(2) << endl;

  cout << "\nSource (port 1):" << endl;
  cout << "Center = " << ad.center(1) << endl;
  cout << "Radius = " << ad.radius(1) << endl;

  if(ad.unconditionally_stable())
    cout << "\nUnconditionally Stable" << endl;
  else
    cout << "\nNot Unconditionally Stable" << endl;

  // Copy ampdata into an sdata.
  sdata sd = ad;

  // Now test other constructors.
  ampdata ad2(ad);
  ampdata ad3(sd);

  // Now test assignment
  ampdata ad4; ad4 = ad;
  ampdata ad5; ad5 = sd;

  cout << fixed << endl;
  cout << "Print out original, plus copies made 4 different ways." << endl;
  cout << "All five lines should be identical." << endl;
  cout << ad.delta() << "  " << ad.k() << "  " << ad.center(2)
    << "  " << ad.radius(2) << "  " << ad.center(1) << "  " << ad.radius(1)
    << "  " << ad.unconditionally_stable() << endl;
  cout << ad2.delta() << "  " << ad2.k() << "  " << ad2.center(2)
    << "  " << ad2.radius(2) << "  " << ad2.center(1) << "  " << ad2.radius(1)
    << "  " << ad2.unconditionally_stable() << endl;
  cout << ad3.delta() << "  " << ad3.k() << "  " << ad3.center(2)
    << "  " << ad3.radius(2) << "  " << ad3.center(1) << "  " << ad3.radius(1)
    << "  " << ad3.unconditionally_stable() << endl;
  cout << ad4.delta() << "  " << ad4.k() << "  " << ad4.center(2)
    << "  " << ad4.radius(2) << "  " << ad4.center(1) << "  " << ad4.radius(1)
    << "  " << ad4.unconditionally_stable() << endl;
  cout << ad5.delta() << "  " << ad5.k() << "  " << ad5.center(2)
    << "  " << ad5.radius(2) << "  " << ad5.center(1) << "  " << ad5.radius(1)
    << "  " << ad5.unconditionally_stable() << endl;

  cout << "\nNow make some mistakes, and verify warning messages." << endl;
  cout << ad.center(0) << endl;
  cout << ad.center(3) << endl;
  cout << ad.radius(0) << endl;
  cout << ad.radius(3) << endl;
  ad.resize(3);
  cout << ad.k() << endl;
  cout << ad.delta() << endl;
  cout << ad.center(1) << endl;
  cout << ad.radius(1) << endl;
  cout << ad.unconditionally_stable() << endl;
  ad.resize(1);
  cout << ad.k() << endl;
  cout << ad.delta() << endl;
  cout << ad.center(1) << endl;
  cout << ad.radius(1) << endl;
  cout << ad.unconditionally_stable() << endl;

  cout << endl << "Test gamma_opt, t_min, F_min, Rn, etc." << endl;

  fhr02x t;

  device::T = 12.0 * Kelvin;
  cout << "! Noise data" << endl;
  cout << "!               Gamma   Gamma" << endl;
  cout << "!               opt     opt" << endl;
  cout << "! Freq  Fmin    Mag     Angle   Rn" << endl;
  for(double freq = 1.; freq <= 8.1; freq+=0.5)
  {
    device::f = freq * GHz;
    ad = t.get_data();
    cout << freq << "\t" << ad.F_min()/Kelvin << "\t"
         << abs(ad.gamma_opt()) << "\t"
         << arg(ad.gamma_opt())/Degree << "\t"
         << ad.Rn() << endl;
  }
}

