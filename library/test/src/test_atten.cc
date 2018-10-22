// test_atten.cc
//
// Test attenuator class.
//
// John Ward
// September 1, 1998
//

#include "supermix.h"
 
int main(void) {
  attenuator at(3.0);

  // Set the global temperature.
  device::T = 4.2 * Kelvin;
  device::f = 10.0 * GHz;

  sdata sd;
  sd = at.get_data();

  cout << "Gain: " << sd.SdB(2,1) << endl;
  cout << "Gain: " << sd.SdB(1,2) << endl;
  cout << "Tn: " << sd.tn(2,1)/Kelvin << " K" << endl;

  cout << "Setting at.Temp=0.0" << endl;
  at.Temp = 0.0;
  cout << "device::T = " << device::T << endl;
  sd = at.get_data();
  cout << "Gain: " << sd.SdB(2,1) << endl;
  cout << "Gain: " << sd.SdB(1,2) << endl;
  cout << "Tn: " << sd.tn(2,1)/Kelvin << " K" << endl;

  cout << "Setting at.Temp=300.0" << endl;
  at.Temp = 300.0;
  sd = at.get_data();
  cout << "Gain: " << sd.SdB(2,1) << endl;
  cout << "Gain: " << sd.SdB(1,2) << endl;
  cout << "Tn: " << sd.tn(2,1)/Kelvin << " K" << endl;

  cout << "Setting at.dB=10.0" << endl;
  at.dB = 10.0;
  sd = at.get_data();
  cout << "Gain: " << sd.SdB(2,1) << endl;
  cout << "Gain: " << sd.SdB(1,2) << endl;
  cout << "Tn: " << sd.tn(2,1)/Kelvin << " K" << endl;

  cout << "Shadow global temperature, set it to 100 K" << endl;
  at.Temp = &device::T;
  device::T = 100.0;
  sd = at.get_data();
  cout << "Gain: " << sd.SdB(2,1) << endl;
  cout << "Gain: " << sd.SdB(1,2) << endl;
  cout << "Tn: " << sd.tn(2,1)/Kelvin << " K" << endl;

  cout << "Test copy constructor:" << endl;
  attenuator a2(at);
  sd = a2.get_data();
  cout << "Gain: " << sd.SdB(2,1) << endl;
  cout << "Gain: " << sd.SdB(1,2) << endl;
  cout << "Tn: " << sd.tn(2,1)/Kelvin << " K" << endl;

  cout << "Test operator=" << endl;
  attenuator a3;
  a3 = a2;
  sd = a3.get_data();
  cout << "Gain: " << sd.SdB(2,1) << endl;
  cout << "Gain: " << sd.SdB(1,2) << endl;
  cout << "Tn: " << sd.tn(2,1)/Kelvin << " K" << endl;

}
