/*

Test transconductance and fet

*/

#include "supermix.h"
 
int main(void) {
  Complex::rel_tolerance = 1.0e-14;  // get rid of tiny imaginary parts

  // Set global frequency and temperature
  device::f = 5.0 * GHz;
  device::T = 12.0 * Kelvin;

  // Test ideal transconductance
  cout << "Test a transconductance." << endl;
  transconductance tc;
  tc.G = 50.0*mSiemens;
  sdata sd;
  sd = tc.get_data();
  cout << endl;
  sd.S.show();
  cout << endl;
  (sd.C / Kelvin).show();

  // Test out the copy function
  cout << "Make a copy with the copy constructor." << endl;
  transconductance *tc1;
  tc1 = new transconductance;
  tc1->G = 50.0*mSiemens;
  transconductance tc2(*tc1);
  delete tc1;
  sd = tc2.get_data();
  cout << endl;
  sd.S.show();
  cout << endl;
  (sd.C / Kelvin).show();
 
  // Test operator =
  cout << "Test the assignment operator." << endl;
  tc1 = new transconductance;
  tc1->G = 50.0*mSiemens;
  transconductance tc3 = *tc1;
  delete tc1;
  sd = tc3.get_data();
  cout << endl;
  sd.S.show();
  cout << endl;
  (sd.C / Kelvin).show();

  // Create a field-effect transistor (fet)
  fet f;
  f.Gm.G = 73.1621 * mSiemens;
  f.Gm.Tau = 0.7 * Pico * Second;
  f.Cgs.C = 0.18305 * pFarad;
  f.Rds.R = 166.21 * Ohm;
  f.Cgd.C = 0.0242041 * pFarad;
  f.Cds.C = 0.0230908 * pFarad;
  f.Rgs.R = 1.47226 * Ohm;
  f.Rg.R = 0.382871 * Ohm;
  f.Rs.R = 1.90601 * Ohm;
  f.Rd.R = 5.0 * Ohm;
  f.Lg.L = 0.0676668 * nHenry;
  f.Ls.L = 0.0810802 * nHenry;
  f.Ld.L = 0.103801 * nHenry;
  f.Cpg.C = 0.00548683 * pFarad;
  f.Cpd.C = 0.0300508 * pFarad;
  f.Rd.Temp = 500.0 * Kelvin;  // 4150 at room temperature.

  // View complex numbers in polar form.
  complex::out_degree();

  // Look at S and C matrices for FET in polar form.
  cout << "Look at S and C matrices of a FET." << endl;
  sd = f.get_data();
  cout << endl;
  sd.S.show();
  cout << endl;
  (sd.C / Kelvin).show();

  // Look at gain and noise from 1 to 10 GHz.
  cout << endl << " Freq      S21(MAG ANG)         Gain(dB)     Tn(K)       NF" << endl;
  for(double freq = 1.0; freq <=10.0; freq += 1.0)
  {
    device::f = freq * GHz;
    sd = f.get_data();
    cout << fixed << setw(6) << setprecision(3)
         << freq
         << setprecision(5) << showpoint
         << setw(22) << sd.S.get(2, 1)
         << setw(12) << sd.SdB(2,1)
         << setw(12) << sd.tn(2,1)/Kelvin
         << setw(12) << sd.NF(2,1) << endl;
  }

  cout << endl;
  cout << endl;
  cout << "! FREQ       --- S11 ---       --- S21 ---       --- S12 ---       --- S22 ---" << endl;
  cout << "!            MAG     ANG       MAG     ANG       MAG     ANG       MAG     ANG" << endl;
  cout << endl;
  cout << fixed << right;
  complex_table::out_separator(" ");
  for(double freq = 1.0; freq <=10.0; freq += 0.5)
  {
    device::f = freq * GHz;
    sd = f.get_data();
    sd.S = transpose(sd.S);
    cout << fixed << setw(7) << setprecision(3) << freq << "  "
         << general << setprecision(6) << showpoint
         << sd.S
         << endl;
  }

  // Show this original fet.
  cout << "\nThe original fet:" << endl;
  sd = f.get_data();
  sd.S.show();
  cout << sd.tn(2, 1)/Kelvin << endl;;

  // Test fet copy constructor.
  cout << "\nTest fet copy constructor (compare to previous line):" << endl;
  fet f2(f);
  sd = f2.get_data();
  sd.S.show();
  cout << sd.tn(2, 1)/Kelvin << " Kelvin" << endl;;
  
  // Test assignment operator.
  cout << "\nTest fet assignment operator (should be the same as above):"
       << endl;
  fet f3;
  f3 = f;
  sd = f3.get_data();
  sd.S.show();
  cout << sd.tn(2, 1)/Kelvin << " Kelvin" << endl;;

  // Try to make it dump core.
  cout << "\nMake sure this doesn't cause core dumps." << endl;
  fet *f4;
  f4 = new fet();
  fet f5(*f4);
//  f5 = *f4;
  delete f4;
  sd = f5.get_data();
  sd.S.show();
  cout << sd.tn(2, 1)/Kelvin << " Kelvin" << endl;;
}
