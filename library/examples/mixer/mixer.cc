// mixer.cc
//
// a trivially simple, 1 SIS junction mixer simulation
// (with way too many comments, probably)

#include "supermix.h"

// Since SIS junctions are not linear devices, SuperMix does not
// simulate an SIS junction as a circuit element directly. Instead,
// SuperMix includes the class "mixer" which combines nonlinear
// junction elements with the linear RF, IF, and DC bias embedding
// networks to synthesize a single, multi-frequency nport which models
// the response of the circuitry as a harmonic mixer.

// This program illustrates the basic procedure for building a mixer
// model.



int main()
{
  // -----------------------------------------------------------------
  // THE GLOBAL VALUES 

  device::T  = 4.2*Kelvin;     // our superconducting receiver
  device::f  = 1*GHz;          // the IF output frequency 
  device::Z0 = 50.*Ohm;        // the S matrix normalizing impedance


  // -----------------------------------------------------------------
  // SIS DC IV CHARACTERISTIC CURVE DATA

  // The most important defining feature of an SIS device is its DC
  // current-voltage characteristic. SuperMix requires this
  // characteristic to be specified by providing two properly
  // normalized IV curve data files. The two required IV curves are:
  //
  //   (1) DC I-V characteristic with voltages normalized so that the
  //       gap voltage ~ 1.0, and the slope of the I(V) curve far
  //       above the gap == 1.0.
  //
  //   (2) Kramers-Kronig transform of the DC I-V characteristic using
  //       the same voltage and current normalization scale factors as
  //       used in the DC I-V characteristic.
  //
  // The files should start with Voltage = 0.0 and extend through
  // positive values up to at least twice the gap voltage for curve
  // (1) and four times the gap voltage for curve (2). The files
  // should contain sorted voltage - current pairs, one per line. The
  // voltages need not be equally spaced.
  //
  // Examine the files "idc.dat" and "ikk.dat" included with this
  // program file for examples. Note how comment lines can be included
  // in the files. These files were produced using the example
  // programs makeiv.cc and makeikk.cc, in examples/ivcurve

  // The class "ivcurve", defined in "junction.h", is used to access
  // the IV data. The constructor requires the Idc and Ikk file names:

  ivcurve IV("idc.dat","ikk.dat");


  // -----------------------------------------------------------------
  // DEFINING AN SIS DEVICE

  // An SIS junction device is created by declaring an object of type
  // sis_device, which is derived from class junction. The definition
  // of sis_device is in "sisdevice.h", the definition of junction is
  // in "junction.h". There are several member parameters of the
  // device which must be set to fully define the device:

  sis_device SIS;
    SIS.set_iv(IV);        // the IV curve object for this junction
    SIS.Vn  = 2.8*mVolt;   // the voltage normalization of the IV curves
    SIS.Rn  = 10*Ohm;      // the current normalization is Vn/Rn
    SIS.Cap = 140*fFarad;  // the junction capacitance

  // Vn is nominally the gap voltage of the SIS; this is how the IV
  // curves should have their voltages normalized. Rn is the SIS
  // normal resistance, so that 1/Rn is the slope of the real SIS DC
  // IV curve far above the gap. The normalized IV curve has slope = 1
  // above the gap.
  //
  // If the sis_device is given a nonzero capacitance, be sure you do
  // not include its capacitance as part of the linear embedding
  // network to be described below; either include it there or in the
  // sis_device, but not both.

  // If we were going to simulate a multi-SIS circuit, we would need
  // more sis_device declarations; each SIS in a multi-SIS circuit
  // needs its own sis_device object.


  // -----------------------------------------------------------------
  // THE RF AND IF EMBEDDING NETWORKS

  // We'll use an especially simple RF embedding network: The antenna
  // impedance, which we assume to be real, in parallel with a tuning
  // inductance to tune out the SIS junction capacitance. Assume we
  // want the receiver to operate at 345 GHz.

  double Ftune = 345*GHz;   // receiver design frequency
  double Zant  = 8*Ohm;     // the antenna impedance

  // Here's the tuning inductance:

  inductor Tune;
    Tune.parallel();
    Tune.L = 1.0/(2*Pi*Ftune * 2*Pi*Ftune * SIS.Cap);

  // Here's the antenna. We use a transformer to transform the RF
  // input to the antenna impedance. We'll use port 1 as the RF input
  // and port 2 to present the correct impedance to the SIS. The
  // definition for transformer is in "transformer.h"

  transformer Ant;
    Ant.Z2 = Zant;   // Z2 is the impedance for port 2.

  // The complete RF embedding network combines these two
  // components. When we build a mixer using the embedding networks,
  // we have to connect the SIS junction to port 1 of the network, so
  // we must be careful to add ports in the right order using
  // add_port():

  circuit RF;
    RF.connect(Ant, 2, Tune, 1);
    RF.add_port(Tune, 2);         // port 1 must be for the SIS
    RF.add_port(Ant, 1);          // so port 2 is the RF input

  // The IF embedding network will be a simple transformer presenting
  // the IF embedding impedance to the junction (of course, the SIS
  // has a nonzero capacitance, so that automatically gets included in
  // the calculations at the IF frequency as well).

  double ZIF  = 20*Ohm;     // IF presents 2*Rn to the SIS

  transformer IF;
    IF.Z1 = ZIF;            // port 1 must be reserved for the SIS

  // Port 2 of device IF will be the IF ouput port. 


  // -----------------------------------------------------------------
  // THE DC BIAS NETWORK

  // The DC bias network must have exactly as many ports as there are
  // SIS junctions in the mixer circuit, which is only 1 in this
  // case. We'll use a perfect voltage source for the bias
  // supply. Sources, all 1-ports, are defined in "sources.h".

  voltage_source BIAS;
    BIAS.source_f = 0.0;              // source frequency is DC
    BIAS.source_voltage = 2.2*mVolt;  // about the middle of the step


  // -----------------------------------------------------------------
  // THE LOCAL OSCILLATOR POWER SOURCE

  // We'll create a local oscillator source which we can tell the
  // mixer to connect to the RF input during large signal, harmonic
  // balance calculations. The particular source used, a generator, is
  // also defined in "sources.h".

  generator LO;
    LO.source_f = Ftune;              // source frequency is 345 GHz
    LO.source_power = 100*Nano*Watt;  // for about 1.4 mV peak at SIS
    LO.Temp = 0;                      // useful for mixer noise calc's


  // -----------------------------------------------------------------
  // BUILDING THE MIXER

  // Class mixer, defined in "mixer.h", connects nonlinear devices
  // like our SIS into linear embedding networks and allows us to
  // perform harmonic balance calculations and small signal
  // analyses. Here's how to set one up:

  mixer Mix;

    Mix.add_junction(SIS);   // call once for each SIS device

    Mix.set_rf(RF);          // we provide the linear networks
    Mix.set_if(IF);
    Mix.set_bias(BIAS);

    // the LO source is only connected during harmonic balance; here's
    // how we tell the mixer to use it (it's called a balance
    // terminator since it terminates the RF input port during
    // harmonic balance calculations):

    Mix.set_balance_terminator(LO, 2);  // terminates port 2 of RF

    // Mixers need to know two frequencies: the IF frequency and the
    // LO frequency. The global frequency parameter device::f will
    // provide the IF frequency, but a special mixer call must set the
    // mixer's internal LO frequency parameter. Here we tell that
    // parameter to shadow our LO source frequency by passing the
    // address of its frequency parameter.

    Mix.set_LO(& LO.source_f);  // we shadow the LO source frequency

    // Now the description of our mixer is complete. The only thing
    // remaining is to tell the mixer how many harmonics to use in its
    // calculations (we can change this number at any time by calling
    // harmonics() again):

    Mix.harmonics(3);  // use fundamental + 2 harmonics

  // Let's confirm that our mixer is complete by asking it:
  cout << "# Mixer complete? " 
       << ( ( Mix.flag_mixer_incomplete() ) ? "No" : "Yes" )
       << endl;


  // -----------------------------------------------------------------
  // USING THE MIXER: PORTS

  // Mixers are nports, but complicated ones, especially if the number
  // of harmonics is large. The mixer has ports for the IF and for the
  // RF at each sideband, upper and lower, at each harmonic. Keeping
  // track of the port indexing for a mixer can be difficult, so the
  // mixer class provides a member function to tell you the port index
  // you need. Let's use it now:

  // The IF circuit's output is at its port 2; the IF is harmonic 0:

  int IF_PORT  = Mix.port(2, 0);  // arguments are: (port#, harmonic#)

  // The RF circuit's input is at its port 2, the Upper Side Band of
  // the fundamental RF frequency is harmonic +1, the Lower Side Band
  // is harmonic -1:

  int USB_PORT = Mix.port(2, 1);

  // Here are the port index values:
  cout << "# IF port: " << IF_PORT
       << " , USB port: " << USB_PORT << endl;


  // -----------------------------------------------------------------
  // USING THE MIXER: HARMONIC BALANCE

  // The operating state of the nonlinear SIS junction must be
  // determined before we can perform a circuit (small signal)
  // analysis of our receiver. This is the purpose of the mixer
  // harmonic balance routine:

  Mix.initialize_operating_state();  // call this before the first balance
  Mix.balance();                     // that's all there is to it

  // Whenever you change bias voltage or LO power or frequency, call
  // balance() to recalculate the proper SIS operating state. If the
  // change is more than just a small incremental change, it can speed
  // up the balance slightly if you call initialize_operating_state()
  // first.

  // To look at the SIS operating state, class junction provides the
  // member functions V() and I(), which output vectors of the DC and
  // RMS harmonic voltages and currents across the juction:

  cout << endl;
  Complex::out_degree();
  Complex::out_delimited();
  cout << "SIS Operating State (mag,deg) at DC and 1, 2, 3 harmonic:"
       << endl;
  cout << "V (mVolt): " << SIS.V()/mVolt << endl;
  cout << "I (mAmp):  " << SIS.I()/mAmp << endl;


  // -----------------------------------------------------------------
  // USING THE MIXER: SMALL SIGNAL ANALYSIS

  // Once the operating state of the mixer has been set, just use
  // get_data() like you would for any other nport in order to
  // calculate the small signal response:

  sdata s = Mix.get_data();   // s holds the small-signal response

  // Let's look at the mixer gain in dB:
  double gain_db = s.SdB(IF_PORT, USB_PORT);
  cout << endl;
  cout << "Gain: " << gain_db << " dB" << endl;

  // Mixer noise is trickier. Since a mixer is not a 2-port (all those
  // sidebands, remember), just calling s.tn() will give a misleading
  // answer. We use another small-signal analysis function of mixer,
  // get_term_data(). get_term_data() terminates all RF ports, as in a
  // harmonic balance calculation, and then performs a small signal
  // analysis. The result is just a 1-port sdata, if there is only a
  // single IF output port. Because of the terminations at all RF ports,
  // we also get a correct calculation of the total noise including the
  // quantum noise contribution.

  sdata sterm = Mix.get_term_data();

  // Since we set the temperature of our LO source to zero, all our RF
  // inputs are terminated with matched, 0 Kelvin terminations. The
  // output noise power at the IF is now the noise due to the RF quantum
  // noise + any additional noise added by the receiver.

  double output_noise = sterm.C[IF_PORT][IF_PORT].real;  // C is complex

  // If we take this noise and divide by the mixer gain to refer it
  // back to the RF input, then we get the proper single sideband
  // mixer noise temperature. Since the mixer gain is the magnitude
  // squared of the S parameter connecting IF to USB:

  double input_noise = output_noise/norm(s.S[IF_PORT][USB_PORT]);
  cout << "SSB Tn: " << input_noise/Kelvin << " Kelvin" << endl;

  // If we were just to use the tn() member function of the sdata s,
  // we would get:

  cout << "Wrong noise: " << s.tn(IF_PORT, USB_PORT)/Kelvin 
       << " Kelvin" << endl;

  // This is only the excess noise of the SIS mixer over the much
  // greater quantum noise being introduced from all sidebands.


}
