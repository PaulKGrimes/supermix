// matchivfts.cc
// Optimize parameters to simultaneously match laboratory pumped IV and
// Fourier Transform Spectrometer (FTS) response data, to evaluate the
// deviation between design and production receiver characteristics.  
//
// Familiarize yourself with hotcold.cc before studying this example,
// which uses the same mixer model (specs.h and build_mixer.inc).
//
// Review the descriptions of the fts_match and iv_match error term
// classes found in the SuperMix header file error_terms.h

#include "specs.h"


int main(int argc, char** argv)
{

# include "build_mixer.inc"

  // ==========================================================================
  // Information regarding the FTS and IV match

  // The FTS measured data info:
  char   *ftsname = "fts.data";   // file with the FTS data set
  double Vbias    = 2.4*mVolt;    // bias voltage for the data set
  double power    = 1*Nano*Watt;  // LO power to simulate FTS

  // The FTS match RF frequency range:
  double LO_min   = 500*GHz;
  double LO_max   = 1300*GHz;
  double LO_step  = 20*GHz;


  // The Pumped IV measured data info:
  char   *ivname = "pumpediv.data"; // file with the IV data set
  double freq    =  750*GHz;        // LO freq for the data

  // The Pumped IV match bias voltage range:
  double V_min   = -VGAP * 1.2;
  double V_max   =  VGAP * 1.2;
  double V_step  =  0.2*mVolt;


  // ==========================================================================
  // Error function declarations and Parameters to Optimize

  error_func ef;

  // Here are the physical SIS parameters which we figure might be different
  // from their design values and which could significantly affect the
  // measured behavior of the circuit. We'll use the optimizer to get an
  // estimate of their actual values by matching the model to the measured
  // data. We set the initial value of each parameter to its current value,
  // assigned in specs.h

  AREA     = ef.vary(AREA*0.7, AREA, AREA*1.3);
  RNA      = ef.vary( RNA*0.8,  RNA,  RNA*1.2);


  // Since our lab setup for the pumped IV measurement had no capability
  // to determine the LO power, we must include it as a parameter to be
  // determined during the optimization:

  LO_POWER = ef.vary(50, 100, 200, Nano*Watt);


  // Lastly, the measured IV data has unknown current and voltage offset
  // errors. We include these as additional parameters controlled by the
  // optimizer:

  parameter I_off = 0.0, V_off = 0.0;
    I_off  = ef.vary( -10, 0, 10, Micro*Amp),
    V_off  = ef.vary( -.1, 0, .1, mVolt);


  // ==========================================================================
  // The FTS match sweeper and error term


  // The FTS match sweeper needs to control two parameters:
  //   (1) Set the SIS bias voltage to the value used in the data set
  //   (2) Sweep the LO frequency over the RF range of the FTS data
  //
  // We need to set the bias voltage because the sweeper for the pumped
  // IV term will be changing it. We need to keep setting it back to
  // the FTS value with each iteration of the FTS sweep. The initialize()
  // member function was included for just such a contingency. As many
  // parameters as you need may be reset at each sweeper iteration by
  // adding more initialize() calls for the sweeper setup. 

  sweeper fts_sweep;
    fts_sweep.initialize(V_BIAS, Vbias);
    fts_sweep.sweep(LO_FREQ, LO_min, LO_max, LO_step);


  // The FTS match error term is a special one designed to be used
  // specifically to match measured FTS data (see error_terms.h). Given
  // an FTS data file with an arbitrary scale factor in its response,
  // it compares the calculated mixer DC bias current response from the
  // model to the FTS file data. Note that in the code below we directly
  // access the interpolator of the FTS data to change it to linear
  // interpolation mode. The purpose of the f_correct() member function
  // is described in error_terms.h

  fts_match fts_term(mix, LO_FREQ, LO_POWER, ftsname, GHz, power);
    fts_term.f_correct();
    fts_term.measured.linear().build(); // since data is noisy, don't spline

  // Add the term to the error function. Since class fts_match always
  // returns an error value for the curve match between 0 and 1, we
  // use the error term weight (100 in this case) to scale its return value.
  ef.add_term(100.0, fts_term, fts_sweep);


  // ==========================================================================
  // The IV match sweeper and error term

  // The IV match sweeper needs to control two parameters:
  //   (1) Set the LO frequency to the value used in the data set
  //   (2) Sweep the SIS bias voltage over the bias range of the data
  //
  // We need to set the LO frequency because the sweeper for the FTS term
  // will be changing it. We need to keep setting it back to the proper
  // value with each iteration of the IV sweep. Again, the initialize()
  // member function was included for just such a contingency.

  sweeper iv_sweep;
    iv_sweep.sweep(V_BIAS, V_min, V_max, V_step);
    iv_sweep.initialize(LO_FREQ, freq);


  // The IV match error term is another special one whose class is
  // described in error_terms.h  Again we directly access the data
  // interpolator to use linear interpolation. Note that we have to
  // call build() following the call to linear() in order to actually
  // affect the behavior of the interpolator. We also set the offsets to
  // shadow the parameters the optimizer will be controlling.

  iv_match iv_term(mix, V_BIAS, ivname, mVolt, Micro*Amp);
    iv_term.measured.linear().build();
    iv_term.i_offset(&I_off);
    iv_term.v_offset(&V_off);

  // Add the iv term to the error function. The weight should result in an
  // error term value of ~1.0 for an average error of 1 uA.
  ef.add_term(1.0/(Micro*Amp*Micro*Amp), iv_term, iv_sweep);


  // ==========================================================================
  // Optimize:

  // We use the powell optimizer since we expect the design values to be
  // pretty close to the actual values. A local optimizer should be just what
  // we need, rather than a global optimizer like montecarlo. See the example
  // file lna/lna_opt.annotated.cc for more details about using powell.

  powell opt(ef);
    opt.verbose();
    opt.FTOL = 0.0001;

  // Initial error function stats:
  cerr << "Initial error function and parameter stats:" << endl;
  cerr << ef() << " : ";
  cerr << ef.get_func_breakdown() << endl
       << "Parameter values: " << ef.get_parms_user() << endl;

  cerr << "Starting optimization. This may take a while..." << endl;
  double final_error = opt.minimize();

  // final error function stats:
  cerr << "Final error function and parameter stats:" << endl;
  cerr << final_error << " : " << ef.get_func_breakdown() << endl
       << "Parameter values: " << ef.get_parms_user() << endl;

  // ==========================================================================
  // Here we generate and output the optimized response

  Vector currents;  // Used to temporarily hold SIS bias currents

  // Output a nice header giving details and the final parameter values
  // found by the optimizer:

  cout << fixed << setprecision(3);
  cout << "# " << argv[0] << ":" << endl
       << "# Match Pumped IV and FTS response" << endl
       << "# IV  data file: " << ivname << endl
       << "# FTS data file: " << ftsname << endl
       << "# Control Parameter Values:" << endl
       << "#     LO freq    = " << LO_FREQ/GHz << " GHz" << endl
       << "#     Min Bias   = " << V_min/mVolt << " mV" << endl 
       << "#     Max Bias   = " << V_max/mVolt << " mV" << endl 
       << "#     FTS Bias   = " << Vbias/mVolt << " mV" << endl 
       << "#     FTS LO pwr = " << power/(Nano*Watt) << " nW" << endl
       << "# Optimized Parameters:" << endl
       << "#     SUB1_T     = " << SUB1_T/Angstrom << " Angstrom" << endl 
       << "#     AREA       = " << AREA/(Micron*Micron) << " sq uM" << endl 
       << "#     RNA        = " << RNA/(Ohm*Micron*Micron) << " Ohm sq uM"
       << endl 
       << "#     IV LO pwr  = " << LO_POWER/(Nano*Watt) << " nW" << endl
       << "#     I offset   = " << I_off/(Micro*Amp) << " uA" << endl
       << "#     V offset   = " << V_off/(mVolt) << " mV" << endl;


  // Now output the pumped IV response using the optimized values:
  cout << "# V(mV)" << "\t" << "I(uA)" << endl;

  LO_FREQ = freq;

  // We sweep over the Bias Voltage range, generating the response
  for(V_BIAS = -1.5*VGAP; V_BIAS <= 1.5*VGAP; V_BIAS += 0.02*mVolt) {

    // Here's where we calculate the total bias current
    mix.balance();             // calculate the SIS operating state
    currents = mix.I_junc(0);  // vector of DC bias currents
    int min = currents.minindex();
    int max = currents.maxindex();

    // Sum the junction currents
    double resp = 0.0;
    for(int i=min; i<=max; i++) resp += currents[i].real;

    // Include offsets in the output, so it can be compared directly
    // with the measured data
    cout << fixed << setprecision(3)
	 << (V_BIAS + V_off)/mVolt
	 << "\t"
	 << scientific << setprecision(3) << (resp + I_off)/(Micro*Amp)
	 << endl;
  }


  // Now for the FTS response
  cout << "#" << endl;
  cout << "# f(GHz)" << "\t" << "response" << endl;

  V_BIAS = Vbias;

  // First we need the dark current, which is independent of input RF (LO)
  // frequency, since the RF power is zero:
  LO_POWER = 0;
  mix.balance();
  currents = mix.I_junc(0);
  int min = currents.minindex();
  int max = currents.maxindex();
  double dark_current = 0.0;
  for(int i=min; i<=max; i++) dark_current += currents[i].real;

  // Now we sweep over the LO frequency range, generating the response
  LO_POWER = power;
  for(LO_FREQ = LO_min; LO_FREQ <= LO_max; LO_FREQ += LO_step) {
    mix.balance();
    currents = mix.I_junc(0);
    double resp = 0.0;
    for(int i=min; i<=max; i++) resp += currents[i].real;
    resp -= dark_current;

    // Scale the response by a freq "correction" factor, since the data
    // set included such a factor:
    resp *= LO_FREQ;

    // Scale the response so that it matches the FTS data. Use the scale
    // value found by fts_term during the final error function calculation.
    resp *= fts_term.scale();

    // Output the generated response
    cout << fixed << setprecision(0) << LO_FREQ/GHz
	 << "\t"
	 << scientific << setprecision(2) << resp
	 << endl;
  }

}
