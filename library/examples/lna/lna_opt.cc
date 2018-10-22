// Build a two-stage low noise amplifier, and optimize the component values.
// See lna_opt.annotated.cc for many detailed comments.

#include "supermix.h"
 
int main()
{
  device::T = 4.2 * Kelvin;
  device::Z0 = 50. * Ohm;

  // ==========================================================================
  // Building the circuit:

  fhx13x t1, t2;	    // Field-effect transistors made by Fujitsu.
  t1.Ls.L = 0.05 * nHenry;  // adjusting the source inductance values a bit...
  t2.Ls.L = 0.05 * nHenry;

  inductor lg1, lg2;	    // Gate tuning inductors.
  resistor rd1, rd2;	    // Drain bias resistors.
  lg1.series();
  lg2.series();
  rd1.parallel();
  rd2.parallel();
  // The inductance and resistance values will be found by the optimizer

  circuit amp;
  int input = amp.add_port(lg1, 1);
  amp.connect(lg1, 2, t1, 1);
  amp.connect(t1, 2, rd1, 1);
  amp.connect(rd1, 2, lg2, 1);
  amp.connect(lg2, 2, t2, 1);
  amp.connect(t2, 2, rd2, 1);
  int output = amp.add_port(rd2, 2);

  // ==========================================================================
  // Setting up the error function for the optimization:

  error_func ef;
  lg1.L = ef.vary(  1.0,    3.0,    20.0,  nHenry );
  lg2.L = ef.vary(  0.2,    1.0,    10.0,  nHenry );
  rd1.R = ef.vary( 30.0,  100.0,   500.0,  Ohm    );
  rd2.R = ef.vary( 30.0,  100.0,   150.0,  Ohm    );

  sweeper gain_band, stability_band;

       gain_band.sweep( device::f, 4.0*GHz,  8.0*GHz, 0.25*GHz );
  stability_band.sweep( device::f, 0.1*GHz, 20.1*GHz, 0.50*GHz );

  // Optimize for stability.
  amp_k         stb1_term(amp);
  amp_mag_delta stb2_term(amp);

  // Keep the gain high.
  gain_dB  gain_term(amp, input, output);
  gain_term.above(20.0);

  // Optimize for flat gain.
  gain_dB  flatness_term(amp, input, output);
  flatness_term.flat();

  // Optimize the output match.
  gain_dB  out_match_term(amp, output, output);
  out_match_term.below(-15.0);

  // Optimize the input match.
  gain_dB  in_match_term(amp, input, input);
  in_match_term.below(-10.0);

  // Optimize for low noise.
  input_tn noise_term(amp, input, output);
  noise_term.match(0.0);

  ef.add_term( 10.0,       stb1_term,  stability_band );
  ef.add_term( 10.0,       stb2_term,  stability_band );
  ef.add_term(  1.5,       gain_term,       gain_band );
  ef.add_term( 20.0,   flatness_term,       gain_band );
  ef.add_term(  1.0,  out_match_term,       gain_band );
  ef.add_term(  1.0,   in_match_term,       gain_band );
  ef.add_term( 10.0,      noise_term,       gain_band );

  // ==========================================================================
  // Setting up the optimization algorithm:

  powell opt(ef);
  opt.verbose();
  opt.FTOL = 0.00001;

  // ==========================================================================
  // Perform the optimization, displaying the resulting optimized values:

  cout << endl << "2 stage cryogenic low-noise amplifier" << endl << endl;
  cout << "Starting optimization:" << endl << endl;

  double error_val = opt.minimize();

  cout << endl << "Final error function value: " << error_val  << endl ;
  cout << "Error function breakdown by term:" << endl;
  cout << ef.get_func_breakdown() << endl;

  cout << "Final parameters are:" << endl;
  cout << "lg1(nH)" << " " << "lg2(nH)" << " " << "rd1(O) " << " " << "rd2(O)";

  ef.get_parms_user().show() ;


  // ==========================================================================
  // Now display the response of our amplifier using the optimized values:

  cout << endl << "Response:" << endl << endl;
  cout << setw(8) << "Freq" << "  "  
       << setw(8) << "S21(dB)" << "  "  
       << setw(8) << "S22(dB)" << "  "  
       << setw(8) << "Tn(K)" << "  "  
       << setw(8) << "NF(dB)" << endl << endl;

  ampdata response;

  for(double freq = 1.0; freq <=12.0; freq += 0.5)
  {
    device::f = freq * GHz;
    response = amp.get_data();

    cout << fixed << setprecision(4) 
	 << setw(8) << freq << "  "
         << setw(8) << response.SdB(output,input) << "  " 
	 << setw(8) << response.SdB(output,output) << "  "
         << setw(8) << response.tn(output,input)/Kelvin << "  " 
	 << setw(8) << response.NF(output,input) << "  ";

    if(response.unconditionally_stable()) cout << "Unconditionally Stable";

    cout << endl;
  }

  cout << endl;
}
