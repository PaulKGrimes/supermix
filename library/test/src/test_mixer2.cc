// test_mixer2
// Test the operation state manipulators
// 7/21/98: Changed flag_balance_invalid() to flag_state_invalid(), except in
//          literal strings (to support regression tests).

#include "supermix.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main()
{
  cout << endl;

  vector<string> booleans(2);
  booleans[0] = "False";  booleans[1] = "True";


  // create an empty mixer and test that the member functions behave correctly

  mixer m;

  cout << "Created an empty mixer. Status values:" << endl;
  cout << "\tflag_mixer_incomplete: \t\t " << booleans[m.flag_mixer_incomplete()] << endl;
  cout << "\tflag_balance_invalid: \t\t " << booleans[m.flag_state_invalid()] << endl;
  cout << "\tflag_balance_inaccurate: \t " << booleans[m.flag_balance_inaccurate()] << endl;
  cout << "\tharmonics: \t\t\t " << m.harmonics() << endl;
  cout << "\tjunctions: \t\t\t " << m.junctions() << endl;

  cout << endl;

  // create a couple of junctions and a bias circuit

  ivcurve iv("iv.dat","ikk.dat");
  parameter Rn = 10*Ohm;
  parameter Vn = 3*mVolt;
  parameter Cap = 0;                   // no capacitance for now

  sis_basic_device j1, j2;

  j1.set_iv(iv);
  j1.Rn = &Rn;
  j1.Vn = &Vn;
  j1.Cap = &Cap;

  j2.set_iv(iv);
  j2.Rn = &Rn;
  j2.Vn = &Vn;
  j2.Cap = &Cap;

  voltage_source j1_bias, j2_bias;

  j1_bias.source_voltage = Vn/2;
  j2_bias.source_voltage = Vn/3;

  circuit bias;
  bias.add_port(j1_bias, 1);
  bias.add_port(j2_bias, 1);

  // How did the circuits turn out?
  device::f = 0;
  cout << "The Bias Circuit:" << endl;
  cout << "\tThe junction gap voltage: \t" << Vn/mVolt << " millivolt" << endl;
  cout << "\tThe bias circuit: j1 at 1/2 gap voltage, j2 at 1/3" << endl;
  cout << "\nImpedance Matrix (should be 2x2, all zeroes)(ohms):";
  ((zdata(bias.get_data()).Z)/Ohm).show();
  cout << "Source Voltages (should be j1 bias, j2 bias)(millivolts):";
  ((zdata(bias.get_data()).Vs)/mVolt).show();

  cout << endl;

  // Give the mixer a local oscillator:
  double LO, IF;
  voltage_source LO_source;
  LO_source.source_f = 0.5*Vn*VoltToFreq;       // 1/2 the gap frequency
  LO_source.source_width = 1*GHz;
  LO_source.source_voltage = 0.5*Vn/RmsToPeak;  // Peak value 1/2 the gap voltage

  // New LO and IF frequencies, and harmonics:
  LO = LO_source.source_f;
  m.set_LO(LO).harmonics(4);              // 4 harmonics (LO + 3)
  device::f = IF = .01 * Vn*VoltToFreq;   // IF is .01 of the gap frequency

  // now just connect 1 junction and a single voltage bias source to the mixer:

  m.add_junction(j1).set_bias(j1_bias);

  // status now:
  cout << "Added a single junction. Status values:" << endl;
  cout << "\tflag_mixer_incomplete: \t\t " << booleans[m.flag_mixer_incomplete()] << endl;
  cout << "\tflag_balance_invalid: \t\t " << booleans[m.flag_state_invalid()] << endl;
  cout << "\tflag_balance_inaccurate: \t " << booleans[m.flag_balance_inaccurate()] << endl;
  cout << "\tharmonics: \t\t\t " << m.harmonics() << endl;
  cout << "\tjunctions: \t\t\t " << m.junctions() << endl;

  // Set the RF and IF circuits to simple straight-through 2-ports:
  branch b1; b1.set_branches(2); branch b2(b1);
  m.set_if(b1).set_rf(b2).set_balance_terminator(LO_source, 2);
  device::f = IF;
  cout << "The IF circuit S matrix at the IF frequency: " << device::f/GHz << " GHz" ;
  b1.get_data().S.show();
  cout << "The IF circuit Noise matrix at the IF frequency:";
  b1.get_data().C.show();
  device::f = LO + IF;
  cout << "The RF circuit S matrix at the 1st USB frequency: " << device::f/GHz << " GHz" ;
  b2.get_data().S.show();
  cout << "The RF circuit Noise matrix at the 1st USB frequency:";
  b2.get_data().C.show();
  device::f = IF;
  

  // set the operating state:
  m.initialize_operating_state();

  // mixer status should now say everything is ready:
  cout << "Added LO and set operating state of junction. Status values:" << endl;
  cout << "\tflag_mixer_incomplete: \t\t " << booleans[m.flag_mixer_incomplete()] << endl;
  cout << "\tflag_balance_invalid: \t\t " << booleans[m.flag_state_invalid()] << endl;
  cout << "\tflag_balance_inaccurate: \t " << booleans[m.flag_balance_inaccurate()] << endl;
  cout << "\tharmonics: \t\t\t " << m.harmonics() << endl;
  cout << "\tjunctions: \t\t\t " << m.junctions() << endl;

  // display j1's operating state:
  cout << "\nJunction Operating State:" << endl;
  cout << "voltage (millivolt RMS):";
  (j1.V()/mVolt).show();
  cout << "current (milliamp RMS):";
  (j1.I()/mAmp).show();


  // calculate mixer
  cout << "\nThe mixer small signal response Y matrix (1/Ohm), " << m.harmonics() << " harmonics";
  (ydata(m.get_data()).Y*Ohm).show();

  // finally, a 2-junction mixer:
  b1.set_branches(3);
  b2.set_branches(3);
  m.set_if(b1).set_rf(b2).set_bias(bias).set_balance_terminator(LO_source, 3).add_junction(j2);
  m.initialize_operating_state();

  cout << "\nShould be a complete, 2 junction mixer. Status values:" << endl;
  cout << "\tflag_mixer_incomplete: \t\t " << booleans[m.flag_mixer_incomplete()] << endl;
  cout << "\tflag_balance_invalid: \t\t " << booleans[m.flag_state_invalid()] << endl;
  cout << "\tflag_balance_inaccurate: \t " << booleans[m.flag_balance_inaccurate()] << endl;
  cout << "\tharmonics: \t\t\t " << m.harmonics() << endl;
  cout << "\tjunctions: \t\t\t " << m.junctions() << endl;

  // display j1's and j2's operating states:
  cout << "\nJunction j1 Operating State:" << endl;
  cout << "voltage (millivolt RMS):";
  (j1.V()/mVolt).show();
  cout << "current (milliamp RMS):";
  (j1.I()/mAmp).show();
  cout << "\nJunction j2 Operating State:" << endl;
  cout << "voltage (millivolt RMS):";
  (j2.V()/mVolt).show();
  cout << "current (milliamp RMS):";
  (j2.I()/mAmp).show();

  // calculate mixer, should be no error
  m.get_data();

  // try saving and restoring operating states:
  Matrix Save;
  m.save_operating_state(Save);
  branch b3; b3.set_branches(2);  // a bias circuit with no sources
  m.set_bias(b3).harmonics(1);
  m.set_rf(b2).initialize_operating_state();  // cleared out LO source (terminator)

  // display the zeroed operating state:
  cout << "Zeroed the operating states:\n" << endl;
  cout << "\nJunction j1 Operating State:" << endl;
  cout << "voltage (millivolt RMS):";
  (j1.V()/mVolt).show();
  cout << "current (milliamp RMS):";
  (j1.I()/mAmp).show();
  cout << "\nJunction j2 Operating State:" << endl;
  cout << "voltage (millivolt RMS):";
  (j2.V()/mVolt).show();
  cout << "current (milliamp RMS):";
  (j2.I()/mAmp).show();

  //now the restore:
  m.initialize_operating_state(Save);
  cout << "Restored the operating states:\n" << endl;
  cout << "\nJunction j1 Operating State:" << endl;
  cout << "voltage (millivolt RMS):";
  (j1.V()/mVolt).show();
  cout << "current (milliamp RMS):";
  (j1.I()/mAmp).show();
  cout << "\nJunction j2 Operating State:" << endl;
  cout << "voltage (millivolt RMS):";
  (j2.V()/mVolt).show();
  cout << "current (milliamp RMS):";
  (j2.I()/mAmp).show();


}
