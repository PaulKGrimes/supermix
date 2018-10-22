// test_hemt.cc
//
// Test HEMT models (Fujitsu FHR02X, FHX13X, and Kukje transistors)
//
#include "supermix.h"
 
int main(void) {
  // Set global temperature
  device::T = 12.0 * Kelvin;

  sdata sd;

  complex::out_degree();
  cout << fixed << right;
  complex_table::out_separator(" ");

  fhx13x f13;
  cout << endl;
  cout << "Fujitsu FHX13X at 12 K" << endl;
  cout << "! FREQ       --- S11 ---       --- S21 ---       --- S12 ---       --- S22 ---" << endl;
  cout << "!            MAG     ANG       MAG     ANG       MAG     ANG       MAG     ANG" << endl;
  cout << endl;
  for(double freq = 1.0; freq <=10.0; freq += 0.5)
  {
    device::f = freq * GHz;
    sd = f13.get_data();
    sd.S = transpose(sd.S);
    cout << fixed << setw(7) << setprecision(3) << freq << "  "
         << general << setprecision(6) << showpoint
         << sd.S
         << endl;
  }

  kukje k;
  cout << endl;
  cout << "Kukje HEMT at 12 K" << endl;
  cout << "! FREQ       --- S11 ---       --- S21 ---       --- S12 ---       --- S22 ---" << endl;
  cout << "!            MAG     ANG       MAG     ANG       MAG     ANG       MAG     ANG" << endl;
  cout << endl;
  for(double freq = 1.0; freq <=10.0; freq += 0.5)
  {
    device::f = freq * GHz;
    sd = k.get_data();
    sd.S = transpose(sd.S);
    cout << fixed << setw(7) << setprecision(3) << freq << "  "
         << general << setprecision(6) << showpoint
         << sd.S
         << endl;
  }

  fhr02x f02;
  cout << endl;
  cout << "Fujitsu FHR02X at 12 K" << endl;
  cout << "! FREQ       --- S11 ---       --- S21 ---       --- S12 ---       --- S22 ---" << endl;
  cout << "!            MAG     ANG       MAG     ANG       MAG     ANG       MAG     ANG" << endl;
  cout << endl;
  for(double freq = 1.0; freq <=10.0; freq += 0.5)
  {
    device::f = freq * GHz;
    sd = f02.get_data();
    sd.S = transpose(sd.S);
    cout << fixed << setw(7) << setprecision(3) << freq << "  "
         << general << setprecision(6) << showpoint
         << sd.S
         << endl;
  }

  cout << endl << "FHR02X noise with 50 Ohm input and output:" << endl;
  cout << "  FREQ      4K        77K       300K" << endl;

  for(double freq = 1.0; freq <=10.0; freq += 0.5)
  {
    cout << fixed << setw(7) << setprecision(3) << freq << "  "
         << general << setprecision(6) << showpoint;
    device::f = freq * GHz;
    device::T = 4. * Kelvin;
    cout << setprecision(6) << setw(10)
         << f02.get_data().tn(2,1)/Kelvin;
    device::T = 77. * Kelvin;
    cout << setw(10)
         << f02.get_data().tn(2,1)/Kelvin;
    device::T = 300. * Kelvin;
    cout << setw(10)
         << f02.get_data().tn(2,1)/Kelvin;
    cout << endl;
  }
  
  kukje *k1;
  k1 = new kukje();
  kukje k2(*k1);
  kukje k3;
  k3 = *k1;

  cout << "\nTest a kukje fet." << endl;
  sd = k1->get_data();
  sd.S.show();
  cout << sd.tn(2, 1)/Kelvin << " Kelvin" << endl;

  // Delete the original so that we know copies aren't using it.
  delete k1;

  cout << "\nNow compare it to a copy of itself made with the copy constructor."
       << endl;
  sd = k2.get_data();
  sd.S.show();
  cout << sd.tn(2, 1)/Kelvin << " Kelvin" << endl;

  cout << "\nNow compare it to an assigment operator." << endl;
  sd = k3.get_data();
  sd.S.show();
  cout << sd.tn(2, 1)/Kelvin << " Kelvin" << endl;
}
