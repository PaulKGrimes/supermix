// Build a two-stage low noise amplifier, and optimize the component values.
// Shows how to use the optimizer and its associated classes

// Look at lna.cc first to get details on building and simulating the amplifier
// circuit.

#include "supermix.h"
 
int main()
{
  // Set the global temperature and normalization impedance.
  device::T = 4.2 * Kelvin;
  device::Z0 = 50. * Ohm;

  // ==========================================================================
  // Building the circuit:

  // Declare the components to be used to make the amplifier. Same as in lna.cc
  fhx13x t1, t2;	    // Field-effect transistors made by Fujitsu.
  t1.Ls.L = 0.05 * nHenry;  // adjusting the source inductance values a bit...
  t2.Ls.L = 0.05 * nHenry;

  inductor lg1, lg2;	    // Gate tuning inductors.
  resistor rd1, rd2;	    // Drain bias resistors.
  lg1.series();             // series is the default, but who can remember?
  lg2.series();
  rd1.parallel();
  rd2.parallel();
  // The inductance and resistance values will be found by the optimizer
  
  // Connect the components into a circuit.
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

  // error functions perform two tasks:
  //
  //  (1) provide the interface between an optimization algorithm and the
  //      actual variables to be optimized in your program
  //
  //  (2) calculate an error function value for the optimization algorithm
  //      by summing the values of one or more error terms
  //
  // The interface to the optimizer algorithms which all error functions
  // must provide is defined in optimizer.h, using class abstract_error_func.
  //
  // We'll be using class error_func, defined in error_func.h. It is derived
  // from class error_func_parameters, defined in simple_error_func.h. The
  // program variables it optimizes must be of class real_parameter, defined
  // in parameter/real_parameter.h. Essentially all of the real-valued variable
  // parameters in the various devices are of this type, so they can be
  // controlled by class error_func. error_func's vary() member function gives
  // control of the parameters to the error function. vary() needs minimum,
  // initial, and maximum values for the parameter, along with units:
  //
  // we'll be doing a 4-variable optimization:
  error_func ef;
  lg1.L = ef.vary(  1.0,    3.0,    20.0,  nHenry );
  lg2.L = ef.vary(  0.2,    1.0,    10.0,  nHenry );
  rd1.R = ef.vary( 30.0,  100.0,   500.0,  Ohm    );
  rd2.R = ef.vary( 30.0,  100.0,   150.0,  Ohm    );

  // Each call to ef.vary() creates a new parameter within ef. It returns
  // a pointer to the parameter, so by assigning vary()'s return value to an
  // external parameter, we force the external parameter to shadow the value
  // maintained within ef. See examples/basics/parameters.cc for a description
  // of shadowing. This is how an optimizer exerts control over your model.
  // Now the parameters lg1.L, lg2.L, rd1.R, and rd2.R are shadowing internal,
  // nameless parameters in error_func ef. These internal parameters are varied
  // by the optimizer as it searches for a minimum of ef's error value.

  // Now for "sweepers":
  // Sweepers are used to define ranges (in this case, frequency bands) for
  // sweeping some parameter under the control of an error function object.
  // The error function is calculated from the sum of the error function
  // values at each of the sweeper points. See sweeper.h for details. 
  //
  // In this case, both of the sweepers will control the global frequency
  // variable, device::f, which determines the frequency for circuit response
  // calculations. The stability_band sweeper sweeps over a wider frequency
  // range. The arguments in the call to sweep() give the name of the
  // parameter to be swept, the minimum and maximum values, and the step size.
  // Note the need for units!

  sweeper gain_band, stability_band;

       gain_band.sweep( device::f, 4.0*GHz,  8.0*GHz, 0.25*GHz );
  stability_band.sweep( device::f, 0.1*GHz, 20.1*GHz, 0.50*GHz );


  // Error terms are individual components in an error function. Their design
  // allows for the accumulation of an error value through the range of a
  // parameter sweep by a sweeper object. Several generic error term forms
  // are defined in error_func.h, all derived from class error_term. Error
  // terms based on these generic objects are easy to write from scratch, but
  // several concrete and useful error terms are already defined in
  // error_terms.h. We'll be using these objects, which take a circuit, and
  // maybe port numbers and a target mode and value. The target mode and
  // target value can be provided as additional constructor arguments, but
  // here we'll call member functions to do that job.
  //
  // The classes gain_dB, input_tn, amp_k, and amp_mag_delta are all defined
  // in error_terms.h; they are derived from error_term_mode defined in
  // error_func.h, which also describes the member functions we are calling
  // to set the mode and target for each error term. Error terms derived from
  // error_term_mode generally return a value proportional to the square of the
  // deviation from a target value. error_func averages the error term values
  // across the points of an associated sweeper (if any).

  // Optimize for stability.
  amp_k         stb1_term(amp);
  amp_mag_delta stb2_term(amp);

  // Keep the gain high.
  gain_dB  gain_term(amp, input, output);
  gain_term.above(20.0);  // an error value results if gain is not above 20 dB

  // Optimize for flat gain.
  gain_dB  flatness_term(amp, input, output);
  flatness_term.flat();   // proportional to overall deviation from flatness

  // Optimize the output match.
  gain_dB  out_match_term(amp, output, output);
  out_match_term.below(-15.0); // want to be below the target of -15 dB

  // Optimize the input match.
  gain_dB  in_match_term(amp, input, input);
  in_match_term.below(-10.0);  // want to be below the target of -10 dB

  // Optimize for low noise.
  input_tn noise_term(amp, input, output);
  noise_term.match(0.0); // want the noise to match 0 Kelvin


  // Here's where we add the various error terms to the error function in
  // order to fully define it. add_term() takes a relative weighting factor
  // as its first argument, along with the error term and an optional sweeper.
  // add_term() is defined in error_func.h

  ef.add_term( 10.0,       stb1_term,  stability_band );
  ef.add_term( 10.0,       stb2_term,  stability_band );
  ef.add_term(  1.5,       gain_term,       gain_band );
  ef.add_term( 20.0,   flatness_term,       gain_band );
  ef.add_term(  1.0,  out_match_term,       gain_band );
  ef.add_term(  1.0,   in_match_term,       gain_band );
  ef.add_term( 10.0,      noise_term,       gain_band );

  // ==========================================================================
  // Setting up the optimization algorithm:

  // Optimization algorithms are derived from class minimizer, found in
  // optimizer.h; we'll be using Powell's algorithm, defined in powell.h
  // Minimizers must be given an error function at construction. They also
  // have member functions which control verbosity. Class powell has some
  // variables which can be adjusted to tweak the performance of the
  // algorithm; we adjust one of them from its default value. See powell.h
  // for details.

  powell opt(ef);
  opt.verbose();      // see the iterations as they occur
  opt.FTOL = 0.00001;

  // ==========================================================================
  // Perform the optimization, displaying the resulting optimized values:

  cout << endl << "2 stage cryogenic low-noise amplifier" << endl << endl;
  cout << "Starting optimization:" << endl << endl;

  // Calling the minimize() member function of the optimizer object causes it
  // to "do its thing", finding parameter values which minimize the error
  // function. minimize() returns the final error function value (a double).
  //
  // In the next lines we call minimize() and output the returned error
  // function value. Because we called opt.verbose() earlier, we'll get several
  // lines of output generated internally by the optimizer as it searches for a
  // solution.

  double error_val = opt.minimize();
  cout << endl << "Final error function value: " << error_val  << endl ;

  // We can get a breakdown of the contribution of each error term to the
  // final error function value by calling error_func's member function
  // get_func_breakdown(). It returns a real_vector object (see vector.h
  // -- the one that comes with supermix, NOT the one that comes with the
  // standard C++ library). The stream operator << can be used to output
  // the contents of a real_vector object, by default all on one line.
  // The contributions of the terms are displayed in the same order as the
  // order of the calls to ef.add_term() made previously -- in this
  // case: stb1_term, stb2_term, gain_term, flatness_term, out_match_term,
  // in_match_term, noise_term.

  cout << "Error function breakdown by term:" << endl;
  cout << ef.get_func_breakdown() << endl;

  // Now that the optimization is complete, we can conveniently ask the error
  // function for the final values of the parameter assignments. By calling the
  // member function get_parms_user(), we get a real_vector object. The show()
  // member function of the real_vector class can also be used to display the
  // results, by default all on one line preceded and followed by a newline.
  // The parameters are displayed in the same order as the order of the calls
  // to ef.vary() made previously. In this case the order is lg1.L, lg2.L,
  // rd1.R, and rd2.R. They are displayed in terms of the units provided in
  // the vary() calls; in our case the inductors are in nHenry, the resistors
  // in Ohms.

  cout << "Final parameters are:" << endl;
  cout << "lg1(nH)" << " " << "lg2(nH)" << " " << "rd1(O) " << " " << "rd2(O)";

  ef.get_parms_user().show() ;


  // ==========================================================================
  // Now display the response of our amplifier using the optimized values:

  // Here you also see what you need to do to set a specific output field width
  cout << endl << "Response:" << endl << endl;
  cout << setw(8) << "Freq" << "  "  
       << setw(8) << "S21(dB)" << "  "  
       << setw(8) << "S22(dB)" << "  "  
       << setw(8) << "Tn(K)" << "  "  
       << setw(8) << "NF(dB)" << endl << endl;

  // The scattering and noise matrices will be held in a special form
  // of sdata object, called ampdata. It is defined in ampdata.h and has
  // some additional member functions appropriate for displaying
  // amplifier characteristics.
  ampdata response;

  for(double freq = 1.0; freq <=12.0; freq += 0.5)
  {
    // Set the frequency.
    device::f = freq * GHz;

    // Calcuate the scattering and noise matrices, and hold them in response.
    response = amp.get_data();

    // Print out the frequency, gain, output match, noise temp, noise figure.
    cout << fixed << setprecision(4) 
	 << setw(8) << freq << "  "
         << setw(8) << response.SdB(output,input) << "  " 
	 << setw(8) << response.SdB(output,output) << "  "
         << setw(8) << response.tn(output,input)/Kelvin << "  " 
	 << setw(8) << response.NF(output,input) << "  ";

    // Also let user know if amp is unconditionally stable at this frequency
    if(response.unconditionally_stable()) cout << "Unconditionally Stable";

    cout << endl;
  }

  cout << endl;
}
