// test_inst
// test the inst_circuit device

#include "supermix.h"
#include "instrument.h"

int main()
{
  // get rid of tiny imaginary parts
  Complex::rel_tolerance = Complex::abs_tolerance = 1.0e-10;

  cout << "\nTest the instrumented circuit device." << endl << endl
       << fixed << setprecision(2);

  parameter V = 1.0*Volt;   // this will be the source voltage

  // make the simple voltage source with output current and voltage ports:

  voltage_source vs;
  vs.source_voltage = &V;
  vs.R = 500 * Ohm;

  series_tee ammeter;
  branch voltmeter;

  circuit source;
  source.connect(vs,1, ammeter,2);
  source.connect(ammeter,3, voltmeter,1);
  source.add_port(voltmeter,3);  // to load
  source.add_port(ammeter,1);    // to read current
  source.add_port(voltmeter,2);  // to read voltage

  zterm load;
  load.Z = 500 * Ohm;

  inst_circuit test(source,load,1);  // instrumented source, use the load, 1 voltmeter

  // Now check that the instrumented circuit appears the same as the original source
  // to the outside world:
  
  sdata s;
  cout << "Check that the instrumented and original sources are identical:" << endl;
  cout << "\nThe original source sdata (size, S, C, B):" << endl
       << "   " << vs.size() << "   ";
  s = vs.get_data();
  cout << s.S << "   " << s.C << "   " << s.B << endl;
  cout << "\nThe instrumented source sdata (size, S, C, B):" << endl
       << "   " << test.size() << "   ";
  s = test.get_data();
  cout << s.S << "   " << s.C << "   " << s.B << endl;

  int i;

  // Now sweep the source voltage:

  cout << "\nSweeping the battery (input) voltage:" << endl
       << "The output voltage should be 1/2 the input, current 1 mA/Volt:" << endl << endl
       << setw(15) << "Input(V):" << setw(15) << "Output(V):" << setw(15) << "Current(mA):"
       << endl;

  for (i = 0; i <= 10; ++i) {
    V = i * 1.0 * Volt;
    cout << setw(15) << V/Volt 
	 << setw(15) << test.voltages().read(1).real/Volt
	 << setw(15) << test.currents().read(1).real/mAmp << endl;
  }

  // Next leave the voltage fixed and sweep the load resistor

  cout << "\nSweeping the load resistance:" << endl
       << "The input voltage fixed at " << vs.source_voltage/Volt << " Volts" << endl << endl
       << setw(15) << "Load(Ohm)" << setw(15) << "Output(V):" << setw(15) << "Current(mA):"
       << endl;

  for (i = 0; i <= 10; ++i) {
    load.Z = i * 250 * Ohm;
    cout << setw(15) << complex(load.Z).real/Ohm 
	 << setw(15) << test.voltages().read(1).real/Volt
	 << setw(15) << test.currents().read(1).real/mAmp << endl;
  }

  // make another instrumented circuit, this time instrumenting load vice source

  circuit load2;
  load2.connect(ammeter,3, voltmeter, 1);
  load2.connect(voltmeter,3, load,1);
  load2.add_port(ammeter,2);    // the external connection
  load2.add_port(ammeter,1);    // the current port
  load2.add_port(voltmeter,2);  // the voltage port

  inst_circuit test2(load2, vs, 1);

  // now repeat the same measurements:

  cout << "\nSweeping the battery (input) voltage:" << endl
       << "The output voltage should be 1/2 the input, current 1 mA/Volt:" << endl << endl
       << setw(15) << "Input(V):" << setw(15) << "Output(V):" << setw(15) << "Current(mA):"
       << endl;

  load.Z = 500 * Ohm;
  for (i = 0; i <= 10; ++i) {
    V = i * 1.0 * Volt;
    cout << setw(15) << V/Volt 
	 << setw(15) << test2.voltages().read(1).real/Volt
	 << setw(15) << test2.currents().read(1).real/mAmp << endl;
  }

  // Next leave the voltage fixed and sweep the load resistor

  cout << "\nSweeping the load resistance:" << endl
       << "The input voltage fixed at " << vs.source_voltage/Volt << " Volts" << endl << endl
       << setw(15) << "Load(Ohm)" << setw(15) << "Output(V):" << setw(15) << "Current(mA):"
       << endl;

  for (i = 0; i <= 10; ++i) {
    load.Z = i * 250 * Ohm;
    cout << setw(15) << complex(load.Z).real/Ohm 
	 << setw(15) << test2.voltages().read(1).real/Volt
	 << setw(15) << test2.currents().read(1).real/mAmp << endl;
  }

  // Now try a series RLC circuit, measuring voltages across L and C vs freq:

  capacitor C(1.0e-14/(2*Pi) * Farad); C.parallel();
  inductor  L(1.0e-8/(2*Pi) * Henry);  L.parallel();
  series_tee join;

  circuit resonant;
  resonant.add_port(voltmeter,1);        // to driver
  resonant.connect(voltmeter,2, join,3);
  resonant.connect(join,1, L,1);
  resonant.connect(join,2, C,1);
  resonant.add_port(voltmeter,3);  // voltage across LC port
  resonant.add_port(L,2);          // inductor voltmeter port
  resonant.add_port(C,2);          // capacitor voltmeter port

  // set up source:
  vs.R = 50*Ohm;
  V = 1*Volt;
  parameter F;      // source freq;
  device::f = &F;
  vs.source_f = &F;

  inst_circuit test3(resonant, vs, 3);

  complex::out_degree(); complex::out_delimited();
  cout << "\n100 GHz Resonant RLC circuit, Q = 20, input = 1 Volt" << endl
       << "Sweeping input freq:" << endl << endl
       << setw(15) << "Freq(GHz):" << setw(20) << "Total(Volt,Phase):"
       << setw(20) << "VL(Volt,Phase):" 
       << setw(20) << "VC(Volt,Phase):"
       << endl;

  for (i = -10; i <= 10; ++i) {
    F = (100.0 + 0.0999999*i*i*i) * GHz;  // vice .1 so F >= 0 for i = -10
    cout << setw(15) << F/GHz 
	 << setw(20) << test3.voltages().read(1)/Volt
	 << setw(20) << test3.voltages().read(2)/Volt
	 << setw(20) << test3.voltages().read(3)/Volt << endl;
  }

}
  
