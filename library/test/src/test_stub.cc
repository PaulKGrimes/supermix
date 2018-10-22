// test_stub.cc
//
// Test radial stub class.
//
// John Ward
// September 11, 1998
//
// 9/22/00: fixed Nb Tc. Now Tc = 9.2 Kelvin.

#include "supermix.h"
 
int main(void) {
  // Set the global temperature.
  device::T = 4.2 * Kelvin;
  device::f = 600.0 * GHz;

  // Define niobium wiring.
  super_film nb;
  nb.Vgap = 2.9*mVolt;
  nb.Tc = 9.2*Kelvin;
  nb.rho_normal = 5.*Micro*Ohm*Centi*Meter;
  nb.Thick = 3000.*Angstrom;

  // Define NbTiN ground plane.
  super_film nbtin;
  nbtin.Vgap = 5.0*mVolt;
  nbtin.Tc = 15.75*Kelvin;
  nbtin.rho_normal = 30.*Micro*Ohm*Centi*Meter;
  nbtin.Thick = 3000.*Angstrom;

  // Define vacuum dielectric.
  const_diel vacuum;
  vacuum.eps = 1.0;
  vacuum.tand = 0.0;

  // Define SiO dielectric.
  const_diel sio;
  sio.eps = 5.6;        // dielectric constant
  sio.tand = 0.0;       // loss tangent

  radial_stub r1;

  r1.substrate(sio);
  r1.superstrate(vacuum);
  r1.top_strip(nb);
  r1.ground_plane(nbtin);

  r1.radius = 36 * Micron;
  r1.angle = 90. * Degree;
  r1.width = 5.8 * Micron;
  r1.length = 3.3 * Micron;
  r1.sub_thick = 4500. * Angstrom;

  sdata sd;
  sd = r1.get_data();

  cout << "Test 90 degree radial stub." << endl;
  cout << "Total area of this stub is 1028.61 micron^2." << endl;
  cout << sd.S;
  cout << endl;
  cout << endl;
  cout << sd.C;
  cout << endl;
  cout << endl;

  cout << "Test operator equals" << endl;
  radial_stub r2;
  r2 = r1;
  sd = r2.get_data();

  cout << sd.S;
  cout << endl;
  cout << endl;
  cout << sd.C;
  cout << endl;
  cout << endl;

  cout << "Test copy constructor" << endl;
  radial_stub r3(r2);
  sd = r3.get_data();

  cout << sd.S;
  cout << endl;
  cout << endl;
  cout << sd.C;
  cout << endl;
  cout << endl;

  cout << "Test 180 degree stub." << endl;
  cout << "Total area of this stub is 2054.89 micron^2." << endl;
  r3.angle = 180. * Degree;
  sd = r3.get_data();

  cout << sd.S;
  cout << endl;
  cout << endl;
  cout << sd.C;
  cout << endl;
  cout << endl;

  cout << "At low frequency should be more like a straight through." << endl;
  device::f = 5.0 * GHz;
  sd = r3.get_data();

  cout << sd.S;
  cout << endl;
  cout << endl;
  cout << sd.C;
  cout << endl;
  cout << endl;
}
