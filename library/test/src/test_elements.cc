#include "elements.h"
#include <iostream>

using namespace std;

int main(void)
{
  Complex::rel_tolerance = 1.0e-14;  // get rid of tiny imaginary parts

  series_RLC a, b(20*Ohm, 0, Micro * Farad);
  parallel_RLC e, d(20*Ohm, Micro*Henry, Femto * Farad);
  resistor r(20. * Ohm);
  capacitor c(10. * Pico * Farad);
  inductor l(3. * Nano * Henry);

  device::f = 6.0 * GHz;
  device::T = 300. * Kelvin;

  cout << b.S(2,1) << " should be the same as " << r.S(2,1) << endl;
  cout << b.S(1,1) << " should be the same as " << r.S(1,1) << endl;

  b.set_R(0.0);
  b.set_C(10. * Pico * Farad);
  b.set_L(0.0);

  cout << b.S(2,1) << " should be the same as " << c.S(2,1) << endl;
  cout << b.S(1,1) << " should be the same as " << c.S(1,1) << endl;

  a.L = 3. * Nano * Henry;
  a.C = 10. * Micro * Farad;

  cout << a.S(2,1) << " should be the same as " << l.S(2,1) << endl;
  cout << a.S(1,1) << " should be the same as " << l.S(1,1) << endl;
  
  cout << d.S(2,1) << " should be the same as " << r.S(2,1) << endl;
  cout << d.S(1,1) << " should be the same as " << r.S(1,1) << endl;

  r.R = 0.0 * Ohm;
  d.L = 0.0;

  cout << d.S(2,1) << " should be the same as " << r.S(2,1) << endl;
  cout << d.S(1,1) << " should be the same as " << r.S(1,1) << endl;

  d.R = 50000. * Ohm;
  d.L = 10. * Micro * Henry;
  d.C = 10. * Pico * Farad;

  c.C = 10. * Pico * Farad;

  cout << d.S(2,1) << " should be the same as " << c.S(2,1) << endl;
  cout << d.S(1,1) << " should be the same as " << c.S(1,1) << endl;

  d.R = 73. * Ohm;
  d.L = 10. * Micro * Henry;
  d.C = 1.0 * Femto * Farad;

  r.R = 73. * Ohm;

  cout << "The following two noise correlation matrices should be nearly identical:" << endl;

  d.get_data().C.show();
  r.get_data().C.show();

  cout << "\nThe following two scattering matrices should be nearly identical:" << endl;

  d.get_data().S.show();
  r.get_data().S.show();

  d.set_T(10. * Kelvin);
  r.set_T(10. * Kelvin);

  cout << "The following two noise correlation matrices should be nearly identical:" << endl;

  d.get_data().C.show();
  r.get_data().C.show();


  b.R = 73. * Ohm;
  b.L = 0. * Micro * Henry;
  b.C = Milli * Farad;

  r.R = 73. * Ohm;
  r.set_T(&device::T);

  cout << "The following two noise correlation matrices should be nearly identical:" << endl;

  b.get_data().C.show();
  r.get_data().C.show();

  cout << "\nThe following two scattering matrices should be nearly identical:" << endl;

  b.get_data().S.show();
  r.get_data().S.show();

  b.set_T(10. * Kelvin);
  r.set_T(10. * Kelvin);

  cout << "The following two noise correlation matrices should be nearly identical:" << endl;

  b.get_data().C.show();
  r.get_data().C.show();

}
