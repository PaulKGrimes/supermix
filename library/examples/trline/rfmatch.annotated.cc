// rfmatch.cc

// This program performs a modest analysis of an SIS receiver using
// purely linear circuit analysis in the same way as earlier tools
// like pcircuit. It simulates superconducting microstrip RF
// circuitry and evaluates its matching characteristics into a fixed
// load representing the SIS junction:

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
// program code below. The SIS junction is represented in a purely linear
// fashion as a parallel RC combination. The power coupled into the R of
// the RC is what we determine in this program.

#include "supermix.h"

// ==========================================================================
// THE PHYSICAL SPECIFICATIONS OF THE RF CIRCUIT:

// THE CHARACTERISTICS OF THE SUPERCONDUCTING FILMS
//   Here we just create a new class to hold the relevant physical
//   parameters needed. We immediately declare two objects of the
//   class, holding the relevant numbers for Noibium and NbTiN.
//
//   The member variables of the class are of type parameter, so
//   they could be controlled by the optimizer, if desired. See
//   lna/lna_opt.cc for details.
//
//   Notice how we can initialize the member variables in simple
//   classes like this one.
struct sc_material { parameter Vgap, Tc, rho_normal; }
  nb    = { 2.9*mVolt,  9.20*Kelvin,  5.0*Micro*Ohm*Centi*Meter },
  nbtin = { 5.0*mVolt, 15.75*Kelvin, 30.0*Micro*Ohm*Centi*Meter };

// THE WIDTHS AND LENGTHS OF THE MICROSTRIPS
//   Another newly-created class, this time to hold the dimensions
//   of the individual microstrip circuit elements.
struct wl { parameter width, length; }
  RF1  = { 5.8*Micron, 11.2*Micron },   // transformer section 1 (nearest antenna)
  RF2  = { 3.3*Micron, 15.0*Micron },   // transformer section 2
  L1   = { 5.0*Micron,  6.9*Micron/2 }, // tuning inductor from SIS to virtual ground
  L2   = { 5.0*Micron,  2.5*Micron };   // tuning inductor between transformer and SIS

//   We don't need new classes to hold the remaining info; we'll
//   just use parameters.

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

//   The SIS junction objects in supermix want to know the actual normal
//   resistance and capacitance of a junction. Since the SIS objects hold
//   this information in parameters, they can be set to shadow other
//   parameter-like variables (objects of type abstract_real_parameter).
//   See parameter/real_parameter.h and parameter/abstract_real_parameter.h
//   for further details. Here we create two objects each with their own
//   unique class. Each one will act just like a double in equations, with
//   the additional feature of being shadowable by other parameters.
//
//   To qualify as an abstract_real_parameter, an object must define the get()
//   member function, which returns its value.
//
//   Note that we use "struct" instead of "class" in the type definitions. This
//   just ensures that all members of a class object are public (accessible). 

// Rn, THE NORMAL RESISTANCE:
struct calc_Rn : public abstract_real_parameter {
  double get() const { return RNA/AREA; }
} Rn;

// Cap, THE JUNCTION CAPACITANCE:
struct calc_Cap : public abstract_real_parameter {
  double get() const { return SCAP*AREA; }
} Cap;


// ==========================================================================
// With the global definitions out of the way, we start the main routine:
int main()
{
  // SET THE GLOBAL TEMPERATURE AND NORMALIZATION IMPEDANCE
  //   See how we use our newly created type calc_Rn. device::Z0 is of type
  //   parameter, so it can "shadow" another parameter-like variable if
  //   assigned its address (the purpose of the '&'). When device::Z0 is
  //   asked for its value, it in turn asks Rn's value by calling Rn.get().
  //   Rn then asks RNA and AREA for their values before calculating their
  //   ratio. Consequently, if the program changes either RNA or AREA,
  //   subsequent accesses of device::Z0 will return a different result.
  //   Since device::Z0 determines the normalizing impedance for S matrix
  //   calculations, changes to RNA or AREA will have wide-ranging effects.
  device::T  = 4.2*Kelvin;
  device::Z0 = & Rn;       // we use the SIS Rn as our normalizing impedance


  // CREATE THE SUPERCONDUCTING FILMS AND THE DIELECTRIC MATERIALS
  //   See surfaceZ.h for a description of super_film, which is derived from
  //   type surfimp. super_film will calculate its surface impedance by
  //   first building an interpolation table, greatly speeding up repeated
  //   accesses. The interpolation table is dynamically extended in range
  //   as required, so its use is transparent to the user.
  //
  //   Dielectrics are defined in trlines.h. We use type const_diel, which
  //   uses two parameters to hold its characteristics: a dielectric constant
  //   and a loss tangent.

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
  //   class microstrip is defined in trlines.h. It requires materials,
  //   dimensions, and the dielectric layer thickness.

  // For convenience, we first create a generic microstrip using our materials.
  microstrip ms;
    ms.ground_plane(gp);
    ms.substrate(sio);
    ms.top_strip(top);
    ms.superstrate(air);

  // Now the ones we actually use, copying the generic one. We must assign
  // dimensions as well.

  // rf_1 and rf_2 make up the matching transformer between the antenna and
  // the SIS junction. rf_1 will be connected to the antenna.
  microstrip rf_1(ms);
    rf_1.width     = & RF1.width;
    rf_1.length    = & RF1.length;
    rf_1.sub_thick = & SIO_THICKNESS;
  microstrip rf_2(ms);
    rf_2.width     = & RF2.width;
    rf_2.length    = & RF2.length;
    rf_2.sub_thick = & SIO_THICKNESS;

  // tune_1 is the inductance which tunes out the SIS junction capacitance.
  // Since there is a virtual ground midway between the two halves of the
  // twinslot receiver, we use only 1/2 of the total tuning length. tune_2
  // is the bit between the SIS and the RF matching transformer.
  microstrip tune_1(ms);
    tune_1.width     = & L1.width;
    tune_1.length    = & L1.length;
    tune_1.sub_thick = & TUNE_THICKNESS;
  microstrip tune_2(ms);
    tune_2.width     = & L2.width;
    tune_2.length    = & L2.length;
    tune_2.sub_thick = & TUNE_THICKNESS;


  // CREATE THE ANTENNA
  //   The antenna will be modelled as a transformer, transforming the
  //   normalizing impedance device::Z0 to the antenna embedding impedance
  //   given by the file named in ANT_FILE. We use an interpolation of the
  //   file's complex impedance vs. frequency data, employing a variable of
  //   type complex_interp (defined in complex_interp.h). The interpolation
  //   will return the complex impedance at the current frequency stored in
  //   the global frequency variable device::f. We also must supply the
  //   units used in the file (GHz and Ohm). 
  //
  //   The definition of a transformer is in transformer.h. We set the
  //   impedance of port 2 (Z2) to shadow the interpolation of the
  //   antenna impedance. The other port's impedance will shadow device::Z0
  //   by default. This shadowing works because the transformer's impedances
  //   are held in variables of type complex_parameter
  //   (parameter/complex_parameter.h) and a complex_interp is derived from
  //   abstract_complex_parameter. Shadowing works the same way as for the
  //   real_parameter types.
  complex_interp Z_ant(device::f, ANT_FILE, GHz, Ohm);
  transformer antenna;
    antenna.Z2 = & Z_ant;


  // A CAPACITOR TO MODEL THE SIS JUNCTION
  //   The SIS junction's capacitance is modelled here using a lumped-element
  //   capacitor (see elements.h) with a value calculated using the special
  //   calc_Cap class we defined previously.
  capacitor sis_cap;
    sis_cap.parallel();
    sis_cap.C = & Cap;   // shadows our junction capacitance calculator


  // BUILD THE COMPLETE RF CIRCUIT MODEL
  //   We'll create the branch and short that we need as well (see elements.h).
  //   Refer to the comments in filter/filter.annotated.cc for a description of
  //   code similar to that below.

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

  // Again refer to the comments in filter/filter.annotated.cc for a description.

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
