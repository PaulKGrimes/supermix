// ********************************************************************
// test_pumpedsis
//
// Test the large-signal output of sis_basic_device
//
// usage:
// test_pumpedsis <idc_file> <ikk_file> <bias_voltage> <lo_voltage> <lo_freq> <harmonic>
//
// where all values are peak, normalized to Vgap and Rn
//
// returns: <i_magnitude> <i_phase>
//
// 11/10/98: Minor change to support new vector component accessing
// 7/15/98: Changed to fit new sis_basic_device requirements
//
// F. Rice 7/1/98
//
// ********************************************************************

#include "sisdevice.h"
#include "units.h"
#include <iostream>
#include <cstdlib> // for atof(), atoi()

using namespace std;

int main(int argc, char **argv) 
{
  if(argc < 7) {
    cout << "Usage:" << endl
	 << "test_pumpedsis <idc_file> <ikk_file>"
	 << " <bias_voltage> <lo_voltage> <lo_freq> <harmonic>" << endl
	 << "where all values are peak, normalized to Vgap and Rn" << endl;
    cout << "\nReturns: <i_magnitude> <i_phase>" << endl;
    return 1;
  }

  ivcurve iv(argv[1],argv[2]);

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
  V[1] = atof(argv[4]) * Vgap/RmsToPeak;   // LO voltage

  double Flo = atof(argv[5]) * Fgap;

  int maxh = atoi(argv[6]);

  Vector Iout(sis.large_signal(V,Flo,maxh));

  complex current = Iout.get(maxh);
  current *= (Rn/Vgap); // normalize

  cout << abs(current) << " " << arg(current) << endl;

  // device state:
  // (V/Vgap).show();
  // (sis.V() / Vgap).show(); (sis.I() * (Rn/Vgap)).show();
  // sis.Ck().show();
}
