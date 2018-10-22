// rfmatch.cc

// Create the RF circuit for 1/2 of a twinslot, quasioptical SIS
// mixer. Analyze its frequency response for matching into an SIS
// junction with specified physical characteristics.
//
// The circuit will be modelled as follows:
//
// RF     ---     ---     ---     ---   branch    ---     ---      short
// IN o--|   |---|   |---|   |---|   |-----+-----|   |---|   |---+   to
//        ---     ---     ---     ---      |      ---     ---    | ground
//      antenna   rf_1    rf_2   tune_2    o    sis_cap  tune_1  V
//                                        OUT
//                                    (into SIS Rn)
//
// Below each element is the name of the object representing it in the
// program code below. The SIS junction is modelled in a purely linear
// fashion as a parallel RC combination. The power coupled into the R of
// the RC is what we determine in this program.

#include "supermix.h"

// ==========================================================================
// THE PHYSICAL SPECIFICATIONS OF THE RF CIRCUIT:

// THE CHARACTERISTICS OF THE SUPERCONDUCTING FILMS
struct sc_material { parameter Vgap, Tc, rho_normal; }
  nb    = { 2.9*mVolt,  9.20*Kelvin,  5.0*Micro*Ohm*Centi*Meter },
  nbtin = { 5.0*mVolt, 15.75*Kelvin, 30.0*Micro*Ohm*Centi*Meter };

// THE WIDTHS AND LENGTHS OF THE MICROSTRIPS
struct wl { parameter width, length; }
  RF1  = { 5.8*Micron, 11.2*Micron },   // transformer section 1 (nearest antenna)
  RF2  = { 3.3*Micron, 15.0*Micron },   // transformer section 2
  L1   = { 5.0*Micron,  6.9*Micron/2 }, // tuning inductor from SIS to virtual ground
  L2   = { 5.0*Micron,  2.5*Micron };   // tuning inductor between transformer and SIS

// THE LAYER THICKNESSES
parameter
  GP_THICKNESS   = 3000.*Angstrom,      // ground plane superconductor thickness
  TOP_THICKNESS  = 3000.*Angstrom,      // top strip superconductor thickness
  SIO_THICKNESS  = 4500.*Angstrom,      // SiO layer generally
  TUNE_THICKNESS = 2500.*Angstrom;      // SiO in tuning inductor

// SIS JUNCTION PARAMETERS
parameter
  RNA  = 21.8*Ohm*Micron*Micron,        // normal resistance - area product
  SCAP = 82.0*fFarad/Micron/Micron,     // specific capacitance (per area)
  AREA = 1.2*1.2*Micron*Micron;         // effective junction area

// THE FILE NAME FOR THE ANTENNA IMPEDANCE INFORMATION
const char * const ANT_FILE = "Zslot.750";


// ==========================================================================
// CLASSES WHICH ACTUALLY CALCULATE THE SIS RN AND CAPACITANCE:

struct calc_Rn : public abstract_real_parameter {
  double get() const { return RNA/AREA; }
} Rn;
struct calc_Cap : public abstract_real_parameter {
  double get() const { return SCAP*AREA; }
} Cap;


// ==========================================================================
int main()
{
  // SET THE GLOBAL TEMPERATURE AND NORMALIZATION IMPEDANCE
  device::T  = 4.2*Kelvin;
  device::Z0 = & Rn;       // we use the SIS Rn as our normalizing impedance


  // CREATE THE SUPERCONDUCTING FILMS AND THE DIELECTRIC MATERIALS

  // The top film is Niobium.
  super_film top;
    top.Vgap       = & nb.Vgap;
    top.Tc         = & nb.Tc; 
    top.rho_normal = & nb.rho_normal;
    top.Thick      = & TOP_THICKNESS;

  // The groundplane is NbTiN.
  super_film gp;
    gp.Vgap        = & nbtin.Vgap;
    gp.Tc          = & nbtin.Tc; 
    gp.rho_normal  = & nbtin.rho_normal;
    gp.Thick       = & GP_THICKNESS;

  // Air is the dielectric above the microstrips; we just treat it as vacuum.
  const_diel air;
    air.eps  = 1.0;
    air.tand = 0.0;

  // SiO is the dielectric between the strip and the groundplane.
  const_diel sio;
    sio.eps  = 5.6;
    sio.tand = 0.0;


  // CREATE THE MICROSTRIPS USING THE ABOVE MATERIALS

  // For convenience, we first create a generic microstrip using our materials.
  microstrip ms;
    ms.ground_plane(gp);
    ms.substrate(sio);
    ms.top_strip(top);
    ms.superstrate(air);

  microstrip rf_1(ms);
    rf_1.width     = & RF1.width;
    rf_1.length    = & RF1.length;
    rf_1.sub_thick = & SIO_THICKNESS;
  microstrip rf_2(ms);
    rf_2.width     = & RF2.width;
    rf_2.length    = & RF2.length;
    rf_2.sub_thick = & SIO_THICKNESS;
  microstrip tune_1(ms);
    tune_1.width     = & L1.width;
    tune_1.length    = & L1.length;
    tune_1.sub_thick = & TUNE_THICKNESS;
  microstrip tune_2(ms);
    tune_2.width     = & L2.width;
    tune_2.length    = & L2.length;
    tune_2.sub_thick = & TUNE_THICKNESS;


  // CREATE THE ANTENNA
  complex_interp Z_ant;
  Z_ant.touchstone(ANT_FILE, 'Z');
  transformer antenna;
    antenna.Z2 = & Z_ant;


  // A CAPACITOR TO MODEL THE SIS JUNCTION
  capacitor sis_cap;
    sis_cap.parallel();
    sis_cap.C = & Cap;   // shadows our junction capacitance calculator


  // BUILD THE COMPLETE RF CIRCUIT MODEL

  branch tee;
  short_term ground;

  circuit rf;
    rf.connect( antenna, 2,  rf_1,    1 );
    rf.connect( rf_1,    2,  rf_2,    1 );
    rf.connect( rf_2,    2,  tune_2,  1 );
    rf.connect( tune_2,  2,  tee,     1 );
    rf.connect( tee,     2,  sis_cap, 1 );
    rf.connect( sis_cap, 2,  tune_1,  1 );
    rf.connect( tune_1,  2,  ground,  1 );

  int input  = rf.add_port( antenna, 1 );    // RF power input to antenna
  int output = rf.add_port( tee,     3 );    // power output into SIS resistance


  // ==========================================================================
  // PERFORM THE CALCULATION AND OUTPUT THE RESULTS

  cout << fixed << setprecision(3);
  cout << "# 750 GHz Twinslot RF Match, antenna -> SIS" << endl;
  cout << "# SIS Rn (Ohm): " << Rn/Ohm << " ; SIS Cap (fF): " << Cap/fFarad << endl;
  cout << "#" << endl;
  cout << "# F(THz)" << "\t" << "S21" << "\t" << "Phase" << endl;

  complex::out_degree();
  complex::out_separator("\t");
  
  for(double f = 300.0; f <= 1500.0; f += 1.0)  {
    device::f = f*GHz;
    sdata response = rf.get_data();
    complex S21 = response.S[output][input];
    cout << "   " << device::f/(1000*GHz)<< "\t" << S21 << endl;
  }

}
