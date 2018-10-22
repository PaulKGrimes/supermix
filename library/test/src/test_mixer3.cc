// test_mixer
// Various manipulations of mixer and junction objects
// 7/22/98: Special mods to exercise balance(): should give same output as
//          previous version
// 7/21/98: Changed flag_balance_invalid() to flag_state_invalid(), except in
//          literal strings (to support regression tests).
// 7/17/98: fully exercise building mixers and small signal analysis

#include "supermix.h"
#include <vector>

using namespace std;

int main()
{
  Complex::rel_tolerance = 1.0e-14;  // get rid of tiny imaginary parts

  cout << setprecision(3) << showpoint << fixed << endl;
  complex::out_degree();

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

  // m.set_LO(100*GHz);  // avoid the "set LO frequency" error
  // cout << "\nAttempt to get_data() from an empty mixer: should be a fatal error:" << endl;
  // m.get_data();

  cout << endl;


  // create very simple circuits and attach to the mixer

  resistor r1, r2;
  r1.series(); r1.R = device::Z0;
  r2.series(); r2.R = device::Z0;

  capacitor c1, c2;
  c1.parallel(); c1.C = 100*fFarad;
  c2.parallel(); c2.C = 100*fFarad;

  // the RF circuit:
  circuit Rf;
  Rf.add_port(r1,1);
  Rf.connect(r1,2,c1,1);
  Rf.add_port(c1,2);
  m.set_rf(Rf);  // a two port

  // the IF circuit:
  circuit If;
  If.add_port(r2,1);
  If.connect(r2,2,c2,1);
  If.add_port(c2,2);
  m.set_if(If);  // a two port

  // since there are no junctions yet, the mixer should be complete!
  cout << "Added RF and IF circuits. Mixer should now be complete:" << endl;
  cout << "\tflag_mixer_incomplete: \t\t " << booleans[m.flag_mixer_incomplete()] << endl;

  cout << endl;

  // set an LO and IF frequency, and let's evaluate the mixer!

  parameter LO = 300*GHz;
  parameter IF = 10*GHz;

  m.set_LO(LO);

  // let's look at the RF and IF sdata at the appropriate freqs:

  device::f = IF;
  cout << "The IF circuit S matrix at the IF frequency: " << device::f/GHz << " GHz" ;
  If.get_data().S.show();
  cout << "The IF circuit Noise matrix at the IF frequency:";
  If.get_data().C.show();

  device::f = LO + IF;
  cout << "The RF circuit S matrix at the 1st USB frequency: " << device::f/GHz << " GHz" ;
  Rf.get_data().S.show();
  cout << "The RF circuit Noise matrix at the 1st USB frequency:";
  Rf.get_data().C.show();

  device::f = LO - IF;
  cout << "The RF circuit S matrix at the 1st LSB frequency: " << device::f/GHz << " GHz" ;
  Rf.get_data().S.show();
  cout << "The RF circuit Noise matrix at the 1st LSB frequency:";
  Rf.get_data().C.show();

  // now for the mixer (1 harmonic):

  device::f = IF;  // ensure IF freq is correctly set;

  cout << "The mixer small signal response (S and C matrices), " << m.harmonics() << " harmonics";

  // *** TRY A BALANCE ***
  m.balance();
  // ********************

  m.get_data().S.show();
  m.get_data().C.show();

  // reevaluate with 2 harmonics:

  m.harmonics(2);
  cout << "The mixer small signal response (S and C matrices), " << m.harmonics() << " harmonics";

  // *** TRY A BALANCE ***
  m.balance();
  // ********************

  m.get_data().S.show();
  m.get_data().C.show();

  // now what's the global freq? should be unchanged...
  cout << "device::f = " << device::f/GHz << " GHz. Should be (IF): " << IF/GHz << endl << endl;

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

  // How did this circuit turn out?

  double saved_IF = device::f;  // set freq to 0, so we can see the bias voltages
  device::f = 0;
  cout << "The Bias Circuit:" << endl;
  cout << "\tThe junction gap voltage: \t" << Vn/mVolt << " millivolt" << endl;
  cout << "\tThe bias circuit: j1 at 1/2 gap voltage, j2 at 1/3" << endl;
  cout << "\nImpedance Matrix (should be 2x2, all zeroes)(ohms):";
  ((zdata(bias.get_data()).Z)/Ohm).show();
  cout << "Source Voltages (should be j1 bias, j2 bias)(millivolts):";
  ((zdata(bias.get_data()).Vs)/mVolt).show();
  device::f = saved_IF;

  cout << endl;

  // now just connect 1 junction and a single voltage bias source to the mixer:

  m.add_junction(j1).set_bias(j1_bias);

  // status now:
  cout << "Added a single junction. Status values:" << endl;
  cout << "\tflag_mixer_incomplete: \t\t " << booleans[m.flag_mixer_incomplete()] << endl;
  cout << "\tflag_balance_invalid: \t\t " << booleans[m.flag_state_invalid()] << endl;
  cout << "\tflag_balance_inaccurate: \t " << booleans[m.flag_balance_inaccurate()] << endl;
  cout << "\tharmonics: \t\t\t " << m.harmonics() << endl;
  cout << "\tjunctions: \t\t\t " << m.junctions() << endl;

  // change the RF and IF circuits to simple straight-through 2-ports:
  branch b1; b1.set_branches(2); branch b2(b1);
  m.set_if(b1).set_rf(b2);
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

  // Give the mixer a local oscillator:
  voltage_source LO_source;
  LO_source.source_f = 0.5*Vn*VoltToFreq;     // 1/2 the gap frequency
  LO_source.source_width = 1*GHz;
  LO_source.source_voltage = 0.5*Vn/RmsToPeak;  // Peak value 1/2 the gap voltage
  m.set_balance_terminator(LO_source, 2);

  // New LO and IF frequencies:
  LO = LO_source.source_f;
  m.set_LO(LO);
  device::f = IF = .01 * Vn*VoltToFreq;   // IF is .01 of the gap frequency
  

  // Since mixer large-signal code isn't ready, set the junction operating states directly:
  { // limit scope of variables here
    Vector Volts(2,Index_C);
    // junction j1:
    Volts[0] = j1_bias.source_voltage;
    Volts[1] = LO_source.source_voltage;
    j1.large_signal(Volts, LO, 4);  // 4 should be plenty of harmonics

    // junction j2:
    Volts[0] = j2_bias.source_voltage;
    Volts[1] = LO_source.source_voltage;
    j2.large_signal(Volts, LO, 4);  // 4 should be plenty of harmonics
  }

  // display j1's operating state:
  cout << "\n Junction Operating State:" << endl;
  cout << "voltage (millivolt RMS):";
  (j1.V()/mVolt).show();
  cout << "current (milliamp RMS):";
  (j1.I()/mAmp).show();


  // mixer status should now say everything is ready:
  cout << "Added LO and set operating state of junction. Status values:" << endl;
  cout << "\tflag_mixer_incomplete: \t\t " << booleans[m.flag_mixer_incomplete()] << endl;
  cout << "\tflag_balance_invalid: \t\t " << booleans[m.flag_state_invalid()] << endl;
  cout << "\tflag_balance_inaccurate: \t " << booleans[m.flag_balance_inaccurate()] << endl;
  cout << "\tharmonics: \t\t\t " << m.harmonics() << endl;
  cout << "\tjunctions: \t\t\t " << m.junctions() << endl;


  // *** TRY A BALANCE ***
  m.harmonics(4).balance(); m.harmonics(2);
  // ********************

  // calculate mixer, 2 harmonics
  cout << "\nThe mixer small signal response (S and C matrices), " << m.harmonics() << " harmonics";
  m.get_data().S.show();
  m.get_data().C.show();

  cout << "\nThe mixer small signal response Y matrix (1/Ohm), " << m.harmonics() << " harmonics";
  (ydata(m.get_data()).Y*Ohm).show();


  // finally, a 2-junction mixer:
  b1.set_branches(3);
  b2.set_branches(3);
  m.set_if(b1).set_rf(b2).set_bias(bias).set_balance_terminator(LO_source, 3).add_junction(j2);
  cout << "\nShould be a complete, 2 junction mixer. Status values:" << endl;
  cout << "\tflag_mixer_incomplete: \t\t " << booleans[m.flag_mixer_incomplete()] << endl;
  cout << "\tflag_balance_invalid: \t\t " << booleans[m.flag_state_invalid()] << endl;
  cout << "\tflag_balance_inaccurate: \t " << booleans[m.flag_balance_inaccurate()] << endl;
  cout << "\tharmonics: \t\t\t " << m.harmonics() << endl;
  cout << "\tjunctions: \t\t\t " << m.junctions() << endl;

  // calculate mixer, 2 harmonics
  cout << "\nThe mixer small signal response (S and C matrices), " << m.harmonics() << " harmonics";

  // *** TRY A BALANCE ***
  m.harmonics(4).balance(); m.harmonics(2);
  // ********************

  m.get_data().S.show();
  m.get_data().C.show();

}
