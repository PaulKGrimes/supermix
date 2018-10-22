// test_recvr_2.cc
// build and run a single-junction receiver
// run the optimizer on it
// 3/25/99
//
// 7/29:99:  updated for new optimizer

#include "test_recvr.h"


int main()
{
  // set global variables, and complex number output format
  device::T = TEMPERATURE;
  device::f = &IF_freq;
  complex::out_cartesian();
  complex::out_space();
  complex::out_separator('\t');

  // set up junction
  sis.Rn = 36.0*Ohm;
  sis.Vn = 2.85*mVolt;
  sis.Cap = 40.0*Femto*Farad;
  sis.set_iv(iv);
  bias.source_voltage = &V_bias;

  // set up ms cascade
  lin_ckt.add(ms_3).add(ms_2).add(ms_1);

  // set up LO source
  LO_source.source_f = &LO_freq;
  LO_source.source_width = 1*MHz;
  LO_source.source_power = &LO_power;

  // set up antenna and if terminator
  ant.Z1 = &RF_IMPEDANCE;
  ant.Z2 = device::Z0;
  IF_load.Z1 = &IF_IMPEDANCE;
  IF_load.Z2 = device::Z0;

  // build RF and IF circuits
  RF.connect(lin_ckt, 2, ant, 1);
  RF.add_port(lin_ckt, 1);
  RF.add_port(ant, 2);

  IF.connect(lin_ckt, 2, IF_load , 1);
  IF.add_port(lin_ckt, 1);
  IF.add_port(IF_load, 2);

  // build mixer
  mix.set_LO(&LO_freq).set_bias(bias).set_rf(RF).set_if(IF);
  mix.add_junction(sis);
  mix.set_balance_terminator(LO_source,2);
  mix.harmonics(harmonics);

// **************************************************************
  // Generate IF input impedance as seen by mixer IF for
  // optimum gain; keep S[rf][rf] < target
  {
  complex::out_degree();
  complex::out_separator('\t');

  const double S11_target = -3.0;

  cout << "# IF input S parameter v. IF frequency, optimum Conversion gain while" << endl
       << "# limiting receiver RF reflection coef to " << S11_target << " dB"  << endl
       << "# Junction Rn 36 Ohm; Cap 40 fF; Vg 2.85 mV" << endl
       << "# LO Power 25 nW, 345 GHz" << endl
       << "# Vbias 2.4 mV" << endl
       << "# IV curves: " << IDC_FILE << ", " << IKK_FILE << endl
       << "# IF freq (GHz); Sif (mag, deg); G(if,rf) (dB); S(rf,rf) (dB); S(rf,rf) (mag, deg)" << endl
       ; 

  double R = sis.Rn;
  LO_power = 25.0*Nano*Watt;
  LO_freq = 345.0*GHz;
  V_bias = 2.4*mVolt;

  parameter Zr, Zi;   // real and imaginary parts of matching impedance
  IF_IMPEDANCE = complex_parameter(Zr,Zi);


  int if_port = mix.port(2,0);
  int rf_port = mix.port(2,1);  // USB


  // the error function and the parameters it controls, and the optimizer:
  error_func e;
  sweeper sw;                   // the sweeper controlling IF frequency (needed by error_func)
  gain_dB G(mix, rf_port, if_port, error_term_mode::MATCH, 10.0); // 10 dB conversion gain
  gain_dB Grf(mix, rf_port, rf_port, error_term_mode::BELOW, S11_target);
  e.add_term(1.0, sw, G);                // e will optimize conversion gain
  e.add_term(1000.0, sw, Grf);           // and will keep RF stable
  Zr = e.vary(0.1*R, 2*R, 100*R, Ohm);   // by varying real and imag parts of
  Zi = e.vary(-100*R, 0, 100*R, Ohm);    // IF load impedance
  powell opt(e);
  opt.FTOL = 1.0e-6;

  mix.initialize_operating_state().balance();
  for( double f = 1.0*GHz; f <= 12.5*GHz; f += 1.0*GHz) {
    sw.sweep(IF_freq, &f, 1);  // just sets IF frequency to f
    opt.minimize();
    complex Sif = (IF_IMPEDANCE - device::Z0)/(IF_IMPEDANCE + device::Z0);
    const sdata & Sout = mix.get_last_data();

    cout.precision(6);
    cout << f/GHz 
	 << "\t" << Sif
	 << "\t" << Sout.SdB(if_port,rf_port)  // Gmix
	 << "\t" << Sout.SdB(rf_port,rf_port)  // S11 in dB
	 << "\t" << Sout.S[rf_port][rf_port]
	 << endl;
  }
  // now put load back to default
  IF_IMPEDANCE = device::Z0;
  
  }

}
