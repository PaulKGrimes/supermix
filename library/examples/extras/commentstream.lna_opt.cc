// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ********************************************************************
// commentstream.lna_opt.cc
//
// This program demonstrates how to use the commentstream class to
// automatically add a comment prefix character to output lines you
// wish to be comments so that the lines are not confused with data.
//
// We simply rewrite the lna/lna_opt.cc example to take advantage of
// the commentstream class. The program will build a two-stage low
// noise amplifier, and optimize the component values.
//
// We want the optimizer verbose output to be prefixed by a comment
// delimiter string of "# ", along with the header info and everything
// else besides numerical data.

#include "supermix.h"
#include "extras/commentstream.h"
 
int main()
{
  // ==========================================================================
  // This code is the same as in lna_opt.cc:

  device::T = 4.2 * Kelvin;
  device::Z0 = 50. * Ohm;

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

  // Setting up the optimization algorithm:
  powell opt(ef);
  opt.verbose();
  opt.FTOL = 0.00001;

  // ==========================================================================
  // Here's where we use the commentstream class:
  //
  // supermix/include/extras/commentstream.h contains the declaration
  // of the commentstream class. The idea is for the class to act as a
  // buffer between your program and another ostream, so that when you
  // send output to the commentstream, the output eventually is sent
  // on to that underlying ostream. What the commentstream class does
  // is to ensure that your desired comment prefix string is prepended
  // to every line output through it.

  string comment_prefix = "# ";

  // Here we create our commentstream, which is an ostream (like
  // std::cout). The constructor requires the name of the ostream to
  // which the output should be sent and a string with the comment
  // delimiter. 
  commentstream comment(cout, comment_prefix);

  // Now we can send ouput to our comment ostream:

  comment << "2 stage cryogenic low-noise amplifier" << endl << endl;
  comment << "Starting optimization:" << endl << endl;

  // The optimizer sends its verbose output to error::stream() (see
  // supermix/include/error.h), so we tell the error class to use our
  // comment ostream:

  error::stream(comment);

  // Now we perform the optimization, and the optimizer outputs its
  // info with a comment prefix on each line

  double error_val = opt.minimize();

  // We display the optimized parameter values, again with comment
  // prefixes courtesy our commentstream:

  comment << "Final error function value: " << error_val  << endl ;
  comment << "Error function breakdown by term:" << endl;
  comment << ef.get_func_breakdown() << endl;

  comment << "Final parameters are:" << endl;
  comment << "lg1(nH)" << " " << "lg2(nH)" << " " << "rd1(O) " << " " << "rd2(O)";

  ef.get_parms_user().show(comment) ;

  // Now display the response of our amplifier, with a comment
  // delimiter on the column header:

  comment << endl << "Response:" << endl;
  comment << setw(6) << "Freq" << "  "  
	  << setw(8) << "S21(dB)" << "  "  
	  << setw(8) << "S22(dB)" << "  "  
	  << setw(8) << "Tn(K)" << "  "  
	  << setw(8) << "NF(dB)" << endl;

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

    if(response.unconditionally_stable())
      comment << "Unconditionally Stable" << endl;
    else
      cout << endl;
  }

  cout << endl;
}
