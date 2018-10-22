// Build a two-stage low noise amplifier, and simulate it
// from 4 to 8 GHz.

// Look at examples/filter/filter.annotated.cc for more detailed comments
// concerning the objects and functions used.

#include "supermix.h"
 
int main()
{
  // Set the global temperature and normalization impedance.
  device::T = 4.2 * Kelvin;
  device::Z0 = 50. * Ohm;

  // ==========================================================================
  // Building the circuit:

  // Declare the components to be used to make the amplifier.
  // There are two of each because this will be a two-stage amplifier.

  // fets are 2-ports, with a common-source configuration (see fet.h)
  fhx13x   t1, t2;	// Field-effect transistors made by Fujitsu (see hemt.h).
  inductor lg1, lg2;	// Gate tuning inductors.
  resistor rd1, rd2;	// Drain bias resistors.

  // Set the parameter values for the components.
  lg1.L = 2.5 * nHenry;
  lg1.series();         // series is the default, but who can remember such things
  lg2.L = 1.4 * nHenry;
  lg2.series();
  rd1.R = 100.0 * Ohm;
  rd1.parallel();
  rd2.R = 55. * Ohm;
  rd2.parallel();
  
  // Connect the components into a circuit.
  circuit amp;
  int input = amp.add_port(lg1, 1);  // Input test port
  amp.connect(lg1, 2, t1, 1);	     // Make the connections...
  amp.connect(t1, 2, rd1, 1);
  amp.connect(rd1, 2, lg2, 1);
  amp.connect(lg2, 2, t2, 1);
  amp.connect(t2, 2, rd2, 1);
  int output = amp.add_port(rd2, 2); // Output test port


  // ==========================================================================
  // Now display the response of our amplifier:

  // The scattering and noise matrix results will be held in an sdata instance.
  sdata response;

  // Set options for formatted output. We'll print numbers right-justified,
  // fixed point.
  cout << fixed << right;

  cout << endl << "2 stage cryogenic low-noise amplifier" << endl << endl;

  // Here you also see what you need to do to set a specific output field width;
  // using setw() only affects the very next number output, so you have to call
  // it over and over again (kind of inconvenient, but that's C++).
  cout << setw(8) << "Freq" << "  "  
       << setw(8) << "S21(dB)" << "  "  
       << setw(8) << "S22(dB)" << "  "  
       << setw(8) << "Tn(K)" << "  "  
       << setw(8) << "NF(dB)" << endl << endl;

  // Loop through frequency, simulating the amplifier and printing results.
  for(device::f = 4.0*GHz; device::f <= 8.0*GHz; device::f += 0.5*GHz)
  {
    response = amp.get_data();

    // Print out the frequency, gain, output match, noise temp, and noise figure.
    // See sdata.h for details about the member functions called.
    // Note use of units.
    cout << fixed << setprecision(4) 
	 << setw(8) << device::f/GHz << "  "
         << setw(8) << response.SdB(output,input) << "  " 
	 << setw(8) << response.SdB(output,output) << "  "
         << setw(8) << response.tn(output,input)/Kelvin << "  " 
	 << setw(8) << response.NF(output,input) << "  "
         << endl;
  }
}
