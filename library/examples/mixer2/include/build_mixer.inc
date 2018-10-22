// build_mixer.inc

// code to declare and build the full twinslot SIS mixer model
// include this inside main(); needs to have specs.h included before main().

// ==========================================================================
// Here is a block diagram of the receiver circuitry:
//
//                   +-+                             +-+
//            stub_1 | | RF_left   tune_1   RF_right | | stub_2
//      +---+   |\   | |  +---+     +---+     +---+  | |   /|
// IF --|   |---| >--| |--|   |--+--|   |--+--|   |--| |--< | 
//      +---+   |/   | |  +---+  |  +---+  |  +---+  | |   \|
//     IF_filt       | |         |         |         | |
//                   +-+         X sis_1   X sis_2   +-+
//                   ant         |         |         ant
//                              ///       ///
//
//  ant:  two component antennas of the twin-slot RF/LO antenna
//  stub_1, stub_2: 90 degree radial stubs
//  sis_1, sis_2:   the two SIS devices
//  tune_1:         the SIS capacitance tuning inductor
//
//  IF_filt:  3-stage RF blocking filter for the IF output:
//
//        +---+   +---+   +---+
//   IF --|   |---|   |---|   |-- RF
//        +---+   +---+   +---+
//        if_3    if_2    if_1 
//
//  RF_left, RF_right: microstrip RF matching transformers
//
//        +---+   +---+   +---+
//  ant --|   |---|   |---|   |-- SIS
//        +---+   +---+   +---+
//        rf_1    rf_2    tune_2
//
// The IF load is represented by a transformer which presents the
// specified IF load impedance to the IF filter. The antennas are
// represented using a twin_slot_antenna device whose impedance is
// read from a Touchstone file.
//
// The DC bias circuit is a perfect voltage source to which the 2 SIS
// junctions are connected in parallel.
//
// The LO signal is represented by a generator which is used as a
// balance terminator for the RF input during mixer harmonic balance. 
// ==========================================================================

// Note that nearly all parameters are set to shadow some global value
// found in specs.h. By using this method we make it easier to control all
// of the various receiver characteristics in a uniform way.

// ==========================================================================
// SET THE GLOBAL VARIABLES:
//   TEMPERATURE and IF_FREQ are defined in specs.h

device::T  = & TEMPERATURE;
device::Z0 = 50*Ohm;
device::f  = & IF_FREQ;       // mixers use the IF freq as a reference


// ==========================================================================
// CREATE THE SUPERCONDUCTING FILMS AND THE DIELECTRIC MATERIALS
//   TOP, GP, SUB, SUP are defined in specs.h
super_film top;
  top.Vgap       = & TOP.Vgap;
  top.Tc         = & TOP.Tc; 
  top.rho_normal = & TOP.rho_normal;
  top.Thick      = & TOP_T;

super_film gp;
  gp.Vgap        = & GP.Vgap;
  gp.Tc          = & GP.Tc; 
  gp.rho_normal  = & GP.rho_normal;
  gp.Thick       = & GP_T;

const_diel substrate;
  substrate.eps  = & SUB.eps;
  substrate.tand = & SUB.tand;

const_diel supstrate;
  supstrate.eps  = & SUP.eps;
  supstrate.tand = & SUP.tand;


// ==========================================================================
// CREATE THE MICROSTRIPS
//   RF1, RF2, L1, L2, IF1, IF2, and IF3 are defined in specs.h

// For convenience, we first create a generic microstrip using our materials.
microstrip ms;
  ms.ground_plane(gp);
  ms.substrate(substrate);
  ms.top_strip(top);
  ms.superstrate(supstrate);

// Now copy this microstrip into the objects we'll actually use.
microstrip rf_1(ms);
  rf_1.width       = & RF1.width;
  rf_1.length      = & RF1.length;
  rf_1.sub_thick   = & RF1.thickness;;
microstrip rf_2(ms);
  rf_2.width       = & RF2.width;
  rf_2.length      = & RF2.length;
  rf_2.sub_thick   = & RF2.thickness;
microstrip tune_1(ms);
  tune_1.width     = & L1.width;
  tune_1.length    = & L1.length;
  tune_1.sub_thick = & L1.thickness;
microstrip tune_2(ms);
  tune_2.width     = & L2.width;
  tune_2.length    = & L2.length;
  tune_2.sub_thick = & L2.thickness;
microstrip if_1(ms);
  if_1.width       = & IF1.width;
  if_1.length      = & IF1.length;
  if_1.sub_thick   = & IF1.thickness;;
microstrip if_2(ms);
  if_2.width       = & IF2.width;
  if_2.length      = & IF2.length;
  if_2.sub_thick   = & IF2.thickness;
microstrip if_3(ms);
  if_3.width       = & IF3.width;
  if_3.length      = & IF3.length;
  if_3.sub_thick   = & IF3.thickness;


// ==========================================================================
// CREATE THE RADIAL STUBS
//   radial_stub objects are defined in radial_stub.h
radial_stub stub_1;
  stub_1.angle     = 90*Degree;
  stub_1.radius    = & STUB_RAD;
  stub_1.length    = & ST_S.width;
  stub_1.width     = & ST_S.length;
  stub_1.sub_thick = & ST_S.thickness;
  stub_1.ground_plane(gp);
  stub_1.substrate(substrate);
  stub_1.top_strip(top);
  stub_1.superstrate(supstrate);

radial_stub stub_2(stub_1);  // stub_2 is identical to stub_1


// ==========================================================================
// BUILD TWINSLOT ANTENNA USING TOUCHSTONE DATA
//   See antenna.h for details on this object:
twin_slot_antenna ant;

// Now we need the antenna impedance information. Since the antenna file
// data may be for either a 1-port or a 2-port, we need to know which.
// class touchstone_read (in io.h) has a function which will do the trick:
int antenna_ports = touchstone_read::ports(ANT_FILE);

// Next we create an element which interpolates the antenna file data
// to get its S matrix; this object is then passed to the antenna. Class
// sdata_interp is found in sdata_interp.h
sdata_interp antenna_response(antenna_ports);
  antenna_response.touchstone(ANT_FILE);
ant.set(antenna_response);


// ==========================================================================
// BUILD A TRANSFORMER TO REPRESENT THE IF LOAD IMPEDANCE
//   We declare an interpolator, but use it only if there's file data.
complex_interp if_interp(device::f);
if( IF_FILE ) {
  // file name pointer is not null
  if_interp.touchstone(IF_FILE,'Z');  // convert Touchstone data to impedance
  IF_IMPEDANCE = & if_interp;
}
// The IF transformer impedance shadows IF_IMPEDANCE
transformer IF_load;
  IF_load.Z1 = & IF_IMPEDANCE;


// ==========================================================================
// THE SIS JUNCTIONS

// Classes which actually calculate the SIS Rn and Capacitance:
product_calc SIS_Cap(SCAP, AREA);
ratio_calc   SIS_Rn (RNA,  AREA);

// The IV curve data:
ivcurve iv(IDC_FILE, IKK_FILE);

// The actual SIS devices:
sis_device sis_1;
  sis_1.set_iv(iv);
  sis_1.Vn  = & VGAP;
  sis_1.Rn  = & SIS_Rn;
  sis_1.Cap = & SIS_Cap;

sis_device sis_2(sis_1);  


// ==========================================================================
// THE MISCELLANEOUS OTHER COMPONENTS TO BUILD THE CIRCUITS
open_term t1, t2;      // terminators for stubs
branch    b1, b2;      // branches to connect SIS junctions into circuit


// ==========================================================================
// BUILD THE LINEAR CIRCUITRY USED IN THE MIXER

// The RF connections from antenna signal to SIS
circuit RF_left;
  RF_left.connect( rf_1, 2,  rf_2,   1 );  // rf_1 and rf_2 form a transformer
  RF_left.connect( rf_2, 2,  tune_2, 1 );  // bit of tuning inductor
  RF_left.add_port( rf_1,   1 );  // to antenna  (port 1)
  RF_left.add_port( tune_2, 2 );  // to SIS      (port 2)

circuit RF_right(RF_left);

// the IF filter
circuit IF_filt;
  IF_filt.connect( if_1, 2, if_2, 1 );
  IF_filt.connect( if_2, 2, if_3, 1 );
  IF_filt.add_port( if_1, 1 );  // to radial stub
  IF_filt.add_port( if_3, 2 );  // to IF load

// The RF circuitry, with junction connections
circuit RF;
  RF.connect( t1,       1,  IF_filt,  2 );  // at RF assume IF load is an open
  RF.connect( IF_filt,  1,  stub_1,   2 );
  RF.connect( stub_1,   1,  ant,      2 );
  RF.connect( ant,      3,  RF_left,  1 );  // this antenna feeds in phase
  RF.connect( RF_left,  2,  b1,       1 );
  RF.connect( b1,       3,  tune_1,   1 );  // connect in the tuning inductor
  RF.connect( tune_1,   2,  b2,       1 );
  RF.connect( b2,       3,  RF_right, 2 ); 
  RF.connect( RF_right, 1,  ant,      4 );  // this antenna feeds out of phase
  RF.connect( ant,      5,  stub_2,   1 );
  RF.connect( stub_2,   2,  t2,       1 );
  RF.add_port( b1,   2 );  // sis_1 goes here    (port 1)
  RF.add_port( b2,   2 );  // sis_2 goes here    (port 2)
  RF.add_port( ant,  1 );  // the RF wave input  (port 3)

// The IF circuitry, with junction connections
circuit IF;
  IF.connect( IF_load,  1,  IF_filt,  2 );
  IF.connect( IF_filt,  1,  stub_1,   2 );
  IF.connect( stub_1,   1,  RF_left,  1 );  // antenna disappears at IF freq
  IF.connect( RF_left,  2,  b1,       1 );
  IF.connect( b1,       3,  tune_1,   1 );
  IF.connect( tune_1,   2,  b2,       1 );
  IF.connect( b2,       3,  RF_right, 2 ); 
  IF.connect( RF_right, 1,  stub_2,   1 );  // antenna disappears at IF freq
  IF.connect( stub_2,   2,  t2,       1 );
  IF.add_port( b1,      2 );  // sis_1 goes here   (port 1)
  IF.add_port( b2,      2 );  // sis_2 goes here   (port 2)
  IF.add_port( IF_load, 2 );  // the IF output     (port 3)


// ==========================================================================
// THE BIAS SUPPLY CIRCUIT
voltage_source batt;
  batt.source_f       = 0.0;  // battery is a DC source
  batt.source_voltage = & V_BIAS;
circuit BIAS;
  BIAS.connect(batt, 1, b1, 1);
  BIAS.add_port( b1, 2 );  // sis_1 goes here   (port 1)
  BIAS.add_port( b1, 3 );  // sis_2 goes here   (port 2)


// ==========================================================================
// THE LOCAL OSCILLATOR SOURCE
generator LO;
  LO.source_f     = & LO_FREQ;
  LO.source_power = & LO_POWER;

// ==========================================================================
// BUILD THE MIXER OBJECT
mixer mix;
  mix.harmonics(HARMONICS);
  mix.add_junction(sis_1).add_junction(sis_2);
  mix.set_LO(& LO_FREQ);
  mix.set_rf(RF).set_if(IF).set_bias(BIAS);
  mix.set_balance_terminator(LO, 3);

RF_PORT = mix.port(3, 1);  // upper sideband RF input (harmonic 1)
IF_PORT = mix.port(3, 0);  // IF output (harmonic 0)
