// test_balance
// Test the harmonic balance routine

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
  cout << "\tflag_mixer_incomplete: \t\t " 
       << booleans[m.flag_mixer_incomplete()] << endl;
  cout << "\tflag_state_invalid: \t\t " 
       << booleans[m.flag_state_invalid()] << endl;
  cout << "\tflag_balance_inaccurate: \t " 
       << booleans[m.flag_balance_inaccurate()] << endl;
  cout << "\tharmonics: \t\t\t " << m.harmonics() << endl;
  cout << "\tjunctions: \t\t\t " << m.junctions() << endl;

  // create a couple of junctions and a bias circuit

  ivcurve iv("iv.dat","ikk.dat");
  parameter Rn = 10*Ohm;
  parameter Vn = 3*mVolt;
  parameter Cap = 0.1*pFarad;          // *** Junction Capacitance Here ***

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

  j1_bias.source_voltage = Vn*.75; j1_bias.R = 5*Ohm;
  j2_bias.source_voltage = Vn*.8; j2_bias.R = 10*Ohm;

  circuit bias;
  bias.add_port(j1_bias, 1);
  bias.add_port(j2_bias, 1);

  // Give the mixer a local oscillator, with an impedance:
  double LO, IF;
  voltage_source LO_source;
  LO_source.source_f = 0.5*Vn*VoltToFreq;       // 1/2 the gap frequency
  LO_source.source_width = 1*GHz;
  LO_source.source_voltage = 0.5*Vn/RmsToPeak;  // Peak value 1/2 the gap voltage
  LO_source.R = 50*Ohm;

  // Show the Bias and LO circuits
  device::f = 0;
  cout << "The Bias circuit (S,B):";
  bias.get_data().S.show();
  bias.get_data().B.show();
  device::f = LO_source.source_f;
  cout << "\nThe LO source (S,B):";
  LO_source.get_data().S.show();
  LO_source.get_data().B.show();


  // New LO and IF frequencies, and harmonics:
  LO = LO_source.source_f;
  m.set_LO(LO).harmonics(3);              // *** HARMONICS SET HERE ***
  device::f = IF = .01 * Vn*VoltToFreq;

  // now just connect 1 junction and a single voltage bias source to the mixer:

  m.add_junction(j1).set_bias(j1_bias);

  // Set the RF and IF circuits to simple straight-through 2-ports:
  branch b1; b1.set_branches(2); branch b2(b1);
  m.set_if(b1).set_rf(b2).set_balance_terminator(LO_source, 2);

  // set the operating state:
  m.initialize_operating_state();

  // mixer status should now say everything is ready:
  cout << "Added LO and set operating state of junction. Status values:" << endl;
  cout << "\tflag_mixer_incomplete: \t\t " 
       << booleans[m.flag_mixer_incomplete()] << endl;
  cout << "\tflag_state_invalid: \t\t " 
       << booleans[m.flag_state_invalid()] << endl;
  cout << "\tflag_balance_inaccurate: \t " 
       << booleans[m.flag_balance_inaccurate()] << endl;
  cout << "\tharmonics: \t\t\t " << m.harmonics() << endl;
  cout << "\tjunctions: \t\t\t " << m.junctions() << endl;

  // display j1's operating state:
  cout << "\nJunction Operating State:" << endl;
  cout << "voltage (millivolt RMS):";
  (j1.V()/mVolt).show();
  cout << "current (milliamp RMS):";
  (j1.I()/mAmp).show();

  // balance the mixer
  cout << "Attempting balance of the single-junction mixer..." << endl;
  int result = m.balance();
  cout << "balance() return value: " << result << endl;

  cout << "\nStatus values:" << endl;
  cout << "\tflag_mixer_incomplete: \t\t "
       << booleans[m.flag_mixer_incomplete()] << endl;
  cout << "\tflag_state_invalid: \t\t "
       << booleans[m.flag_state_invalid()] << endl;
  cout << "\tflag_balance_inaccurate: \t "
       << booleans[m.flag_balance_inaccurate()] << endl;
  cout << "\tharmonics: \t\t\t " << m.harmonics() << endl;
  cout << "\tjunctions: \t\t\t " << m.junctions() << endl;
  cout << "\nJunction Operating State:" << endl;
  cout << "voltage (millivolt RMS):";
  (j1.V()/mVolt).show();
  cout << "current (milliamp RMS):";
  (j1.I()/mAmp).show();

  // finally, a 2-junction mixer:
  resistor r1; r1.series(); r1.R = 1*Ohm;
  resistor r2(r1);
  b1.set_branches(3);
  circuit rf;
  rf.connect(b1,1,r1,1);
  rf.connect(b1,2,r2,1);
  rf.add_port(r1,2);
  rf.add_port(r2,2);
  rf.add_port(b1,3);

  m.set_if(rf).set_rf(rf).set_bias(bias).set_balance_terminator(LO_source, 3);
  m.add_junction(j2).initialize_operating_state();

  cout << "\nShould be a complete, 2 junction mixer. Status values:" << endl;
  cout << "\tflag_mixer_incomplete: \t\t " 
       << booleans[m.flag_mixer_incomplete()] << endl;
  cout << "\tflag_state_invalid: \t\t "
       << booleans[m.flag_state_invalid()] << endl;
  cout << "\tflag_balance_inaccurate: \t "
       << booleans[m.flag_balance_inaccurate()] << endl;
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

  // balance the mixer
  cout << "Attempting balance of the two-junction mixer..." << endl;
  result = m.balance();
  cout << "balance() return value: " << result << endl;

  cout << "\nStatus values:" << endl;
  cout << "\tflag_mixer_incomplete: \t\t "
       << booleans[m.flag_mixer_incomplete()] << endl;
  cout << "\tflag_state_invalid: \t\t "
       << booleans[m.flag_state_invalid()] << endl;
  cout << "\tflag_balance_inaccurate: \t "
       << booleans[m.flag_balance_inaccurate()] << endl;
  cout << "\tharmonics: \t\t\t " << m.harmonics() << endl;
  cout << "\tjunctions: \t\t\t " << m.junctions() << endl;
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
