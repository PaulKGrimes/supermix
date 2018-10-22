// test_recvr.cc
// build and run a single-junction receiver
// updated 10/1/99 for new complex_interp constructor
// 3/25/99

#include "test_recvr.h"


int main()
{
  // set global variables
  device::T = TEMPERATURE;
  device::f = &IF_freq;

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
  // pumped IV and Hot and Cold Power curves, using optimum IF match for each bias 
  { 
  cout << "# Pumped IV and Hot and Cold IF Power curves; Optimum IF match" << endl
       << "# Junction Rn 36 Ohm; Cap 40 fF; Vg 2.85 mV" << endl
       << "# LO Power 25 nW, 345 GHz" << endl
       << "# IF at 6 GHz" << endl
       << "# IF load impedance optimized for each bias, using conj of output impedance" << endl
       << "# IV curves: " << IDC_FILE << ", " << IKK_FILE << endl
       << "# Vbias (mV); I(no LO) (mA); I(LO) (mA); IFout Noise/5, 80 K source; 290 K source" << endl
       ; 

  LO_freq = 345.0*GHz;
  IF_freq = 6.0*GHz;
  int if_out = mix.port(2,0);
  IF_IMPEDANCE = device::Z0;

  for(double v = 0.05; v <= 4.5; v += .05) {
    V_bias = v*mVolt;
    cout << v ;
    for(double p = 0.0; p <= 25.0; p += 25.0) {
      LO_power = p*Nano*Watt;
      mix.initialize_operating_state().balance();
      cout << "\t" << sis.I()[0].real/(Micro*Amp);
    }

    // here's where we match load
    IF_IMPEDANCE = conj(zdata(mix.get_term_data()).Z[if_out][if_out]);

    for(double t = 80.0; t <= 290.0; t += 210.0) {
      LO_source.Temp = t*Kelvin;
      cout << "\t" << mix.get_term_data().C[1][1].real/Kelvin * .2;
    }
    // now put load back to default for the next loop
    IF_IMPEDANCE = device::Z0;
    cout << endl;
  }
  }
// **************************************************************
  // Gmix vs IF freq and IF load impedance
  {
  cout << "# Conversion Gain v. IF frequency and IF load Impedance" << endl
       << "# Junction Rn 36 Ohm; Cap 40 fF; Vg 2.85 mV" << endl
       << "# LO Power 25 nW, 345 GHz" << endl
       << "# Vbias 2.4 mV" << endl
       << "# IF freq (GHz); G, 2 Rn (dB); G, 5 Rn (dB); G, optimum load (dB)" << endl
       ;

  // set up the optimum IF termination impedance
//    real_table Zif_data = datafile(OPT_IF_IMP_FILE);
//    scalerow(1, Zif_data, GHz);
//    scalerow(2, Zif_data, Ohm);
//    scalerow(3, Zif_data, Ohm);
//    complex_interp opt_IF_z(Zif_data, 1, 2, 3, device::f);
  complex_interp opt_IF_z(device::f, OPT_IF_IMP_FILE, GHz, Ohm);

  int rf_in = mix.port(2,1);
  int if_out = mix.port(2,0);
  complex::out_space();

  V_bias = 2.4*mVolt;
  LO_freq = 345.0*GHz;
  LO_power = 25*Nano*Watt;

  complex Z[3];  // the array of IF matching impedances
  Z[0] = 2*sis.Rn;
  Z[1] = 5*sis.Rn;
  Z[2] = 0.0; // flag to use optimum match

  mix.initialize_operating_state().balance();

  for(double f = 0.2*GHz; f <= 12.0*GHz; f += 0.2*GHz) {
    IF_freq = f;
    cout << f/GHz;

    for(int i = 0; i < 3; ++i) {
      if (Z[i] == 0.0)
	IF_IMPEDANCE = &opt_IF_z;
      else
	IF_IMPEDANCE = Z[i];
      cout << "\t" << mix.get_data().SdB(if_out, rf_in);
    }

    cout << endl;
  }
  // now put load back to default
  IF_IMPEDANCE = device::Z0;
  }
// **************************************************************
  // Gmix and S[rf][rf] vs IF freq and IF load impedance
  {
  complex::out_degree();
  complex::out_separator('\t');

  double Rn_mult = 1.5;

  cout << "# Conversion Gain and S[rf][rf] v. IF frequency" << endl
       << "# IF load impedance = " << Rn_mult << " Rn" << endl
       << "# Junction Rn 36 Ohm; Cap 40 fF; Vg 2.85 mV" << endl
       << "# LO Power 25 nW, 345 GHz" << endl
       << "# Vbias 2.4 mV" << endl
       << "# IF freq (GHz); Gmix (dB); S11 (dB); S11 (mag, deg)" << endl
       ;

  int rf_port = mix.port(2,1);
  int if_port = mix.port(2,0);

  V_bias = 2.4*mVolt;
  LO_freq = 345.0*GHz;
  LO_power = 25*Nano*Watt;

  IF_IMPEDANCE = Rn_mult * sis.Rn;

  mix.initialize_operating_state().balance();

  for(double f = 0.2*GHz; f <= 12.3*GHz; f += 0.2*GHz) {
    IF_freq = f;
    const sdata Sout = mix.get_data();

    cout.precision(6);
    cout << f/GHz 
	 << "\t" << Sout.SdB(if_port,rf_port)  // Gmix
	 << "\t" << Sout.SdB(rf_port,rf_port)  // S11 in dB
	 << "\t" << Sout.S[rf_port][rf_port]
	 << endl;
 
  }
  // now put the load back to default
  IF_IMPEDANCE = device::Z0;
  }
// **************************************************************
  // SSB Noise Temp vs IF freq
  {
  cout << "# SSB Noise Temp v. IF frequency and IF load Impedance" << endl
       << "# Junction Rn 36 Ohm; Cap 40 fF; Vg 2.85 mV" << endl
       << "# LO Power 25 nW, 345 GHz" << endl
       << "# Vbias 2.4 mV" << endl
       << "# IF freq (GHz); Tn, (K)" << endl
       ;

  int rf_in = mix.port(2,1);
  int if_out = mix.port(2,0);
  complex::out_space();

  V_bias = 2.4*mVolt;
  LO_freq = 345.0*GHz;
  LO_power = 25*Nano*Watt;

  IF_IMPEDANCE = 2.0 * sis.Rn;
  mix.initialize_operating_state().balance();

  for(double f = 0.2*GHz; f <= 12.0*GHz; f += 0.2*GHz) {
    IF_freq = f;
    cout << f/GHz << "\t" << mix.get_data().tn(if_out, rf_in)/Kelvin << endl;
  }
  // now put the load back to default
  IF_IMPEDANCE = device::Z0;
  }
}
