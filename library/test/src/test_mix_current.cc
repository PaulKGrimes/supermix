// test_mix_current
//
// test the mixer_current and inst_circuit classes

#include "supermix.h"

int main()
{
  // get this out of the way right now:
  cout << "\nTest mixer_currents device:" << endl << endl;
  parameter IF_freq, LO_freq;
  device::T = 4*Kelvin;

  // the 3 junctions, with their common iv curve:
  ivcurve iv("iv.dat","ikk.dat");
  sis_basic_device j1; {
    j1.set_iv(iv);
    j1.Vn = 3*mVolt;
    j1.Rn = 10*Ohm;
    j1.Cap = 0;
  }
  sis_basic_device j2(j1), j3(j1);

  // now the mixer:
  mixer m; {
    m.harmonics(3).set_LO(&LO_freq);
    m.add_junction(j1).add_junction(j2).add_junction(j3);
  }

  // the mixer shouldn't know its size yet...
  cout <<"The mixer shouldn't know its size yet (size = 0):" << endl
       <<"m.size(): " << m.size() << endl <<endl;

  // the RF and IF circuits will be simple branches:
  branch RF(4);  // 3 junction ports, 1 antenna port
  branch IF(4);
  m.set_rf(RF).set_if(IF);

  // the LO source:
  voltage_source LO_source; {
    LO_source.source_f = &LO_freq;
    LO_source.R = 0;
    LO_source.source_voltage = 3.0*mVolt/RmsToPeak;
  }
  m.set_balance_terminator(LO_source, 4);

  // now for mixer_currents:
  mixer_currents junction_bias_currents(m,3); {
    junction_bias_currents.set_harmonic(0);
  }

  // build a simple 3-junction bias circuit using a branch:
  voltage_source battery; {
    battery.source_voltage = 1.5*mVolt;
  }
  branch bias_output(5);
  series_tee bias_current;
  circuit int_bias; {
    int_bias.connect(bias_current,2, battery,1);
    int_bias.connect(bias_current,3, bias_output,1);
    int_bias.add_port(bias_output,2);    // to junction 1
    int_bias.add_port(bias_output,3);    // to junction 2
    int_bias.add_port(bias_output,4);    // to junction 3
    int_bias.add_port(bias_current,1);   // to ammeter
    int_bias.add_port(bias_output,5);    // to voltmeter
  }

  // now use inst_circuit to complete the bias circuit:
  cout << "The arguments to inst_circuit:" << endl
       << "int_bias.size() (should be 5): " << int_bias.size() << endl
       << "junction_bias_currents.size() (should be 3): "
       << junction_bias_currents.size() << endl
       << "number of voltmeters: " << 1 << endl << endl;

  inst_circuit bias(int_bias, junction_bias_currents, 1);  // one voltmeter
  m.set_bias(bias);

  // finally set the LO freq and balance the mixer:
  LO_freq = 1.5*mVolt*VoltToFreq;
  
  m.balance();

  // does a mixer know its size now that its complete?
  cout <<"The mixer should know its size now." << endl
       <<"m.size(): " << m.size() << endl <<endl;

  // print out the individual junction DC states:
  cout << "The individual junction bias voltages and currents:" << endl << endl;
  cout << setw(14) << "Junction:" 
       << setw(14) << "V(mVolt):" 
       << setw(14) << "I(mAmp):"
       << endl;
  cout << setw(14) << "j1"
       << setw(14) << j1.V().read(0).real/mVolt
       << setw(14) << j1.I().read(0).real/mAmp
       << endl;
  cout << setw(14) << "j2"
       << setw(14) << j2.V().read(0).real/mVolt
       << setw(14) << j2.I().read(0).real/mAmp
       << endl;
  cout << setw(14) << "j3"
       << setw(14) << j3.V().read(0).real/mVolt
       << setw(14) << j3.I().read(0).real/mAmp
       << endl;
  cout << endl;

  // Now the voltage and current from the instrumented bias circuit:
  device::f = 0;  // turns on DC bias source
  cout << "The bias circuit's voltage and current readings:" << endl;
  cout << "  V(mVolt):  " << bias.voltages().read(1).real/mVolt
       << "  I(mAmp):  " << bias.currents().read(1).real/mAmp
       << endl << endl;

}
