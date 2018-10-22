// ********************************************************************
// test_sis
//
// Test the code in sisdevice.cc
//
// usage:
// test_sis <idc_file> <ikk_file> <bias_voltage>
//   <lo_voltage> <lo_freq> <if_freq> <harmonics>
//
// where all values are peak, normalized to Vgap and Rn
//
// returns: the large signal operating state, and the small signal
// admittance and noise correlation matrices
//
// F. Rice 7/16/98
//
// ********************************************************************

#include "supermix.h"
#include <iostream>
#include <cstdlib> // for atof(), atoi()
#include <string>
#include <vector>

using namespace std;

int main(int argc, char **argv) 
{
  if(argc != 8) {
    cout << "Usage:" << endl
	 << "test_sis <idc_file> <ikk_file>"
	 << " <bias_voltage> <lo_voltage> <lo_freq> <if_freq> <harmonics>" << endl
	 << "where all values are peak, normalized to Vgap and Rn" << endl;
    return 1;
  }
  cout << endl;

  vector<string> booleans(2);
  booleans[0] = "False";  booleans[1] = "True";

  ivcurve iv(argv[1],argv[2]);

  device::T = 4*Kelvin;
  double Vgap = 3.0 * mVolt;
  double Rn = 10 * Ohm;
  double Cap = 0.0;  // no junction capacitance

  double Fgap = VoltToFreq * Vgap;

  sis_basic_device sis;
  sis.set_iv(iv);
  sis.Vn = Vgap;
  sis.Rn = Rn;
  sis.Cap = Cap;

  Vector V(2,Index_C);
  V[0] = atof(argv[3]) * Vgap;               // bias voltage
  V[1] = atof(argv[4]) * Vgap/RmsToPeak;     // LO voltage

  double Flo = atof(argv[5]) * Fgap;
  device::f = atof(argv[6]) * Fgap;

  int maxh = atoi(argv[7]);

  // display new junction status
  cout << "Created a new sis junction. Status values:" << endl;
  cout << "\tcall_large_signal: \t " << booleans[sis.call_large_signal()] << endl;
  cout << "\tVn (millivolts): \t " << sis.Vn/mVolt << endl;
  cout << "\tRn (ohms): \t\t " << sis.Rn/Ohm << endl;
  cout << "\tCap (picofarads): \t " << sis.Cap/pFarad << endl;
  cout << "\tsize of V(): \t\t " << sis.V().size << endl;
  cout << "\tsize of I(): \t\t " << sis.I().size << endl << endl;

  // now set the operating state and redisplay status
  sis.large_signal(V,Flo,maxh);
  cout << "Just set operating state. Status values:" << endl;
  cout << "\tcall_large_signal: \t " << booleans[sis.call_large_signal()] << endl;
  cout << "\tsize of V(): \t\t " << sis.V().size << endl;
  cout << "\tsize of I(): \t\t " << sis.I().size << endl << endl;

  cout << "The Operating Voltages (millivolt RMS):";
  (sis.V()/mVolt).show();
  cout << "\nThe Operating Currents (milliamp RMS):";
  (sis.I()/mAmp).show();
  cout << "\nThe Ck's:";
  sis.Ck().show();
  cout<< endl;

  // the small signal results
  ydata_ptr py(&sis.small_signal(device::f, maxh), &sis.noise(device::f, device::T, maxh));
  cout << "The Ymn (1/Ohm):";
  (*(py.pY)*Ohm).show();
  cout << "\nThe Hmn (milliamp^2):";
  (*(py.pC)/(mAmp*mAmp)).show();


}
