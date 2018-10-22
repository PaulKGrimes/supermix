// specs.h

// The physical specifications of the 750 GHz twinslot SIS receiver model
// This file provides a single location for the specification of all
// physical and electrical characteristics needed to describe the
// twinslot receiver. This data is used by the code in file
// build_mixer.inc to construct the model.

// definitions of the objects and classes used in this file:
#include "classes.h"

// ==========================================================================
// THE PHYSICAL SPECIFICATIONS OF THE RF CIRCUIT:

// SIS JUNCTION PARAMETERS
parameter
  RNA  = 21.8*Ohm*Micron*Micron,     // normal resistance - area product
  SCAP = 82.0*fFarad/Micron/Micron,  // specific capacitance (per area)
  AREA = 1.2*1.2*Micron*Micron,      // effective junction area
  VGAP = 3.0*mVolt;                  // the junction gap voltage


// THE FILE NAMES FOR THE NORMALIZED SIS DC IV CHARACTERISTIC CURVE
const char * IDC_FILE = "idc.dat";   // DC IV characteristic
const char * IKK_FILE = "ikk.dat";   // Kramers-Kronig xform of the DC IV


// THE CHARACTERISTICS OF THE MATERIALS (CF. classes.h):
sc_material
  TOP = nb,             // the material of the top film
  GP  = nbtin;          // the material of the ground plane

diel_material
  SUB = sio,            // the dielectric substrate material (between films)
  SUP = air;            // the superstrate (above the top film)


// THE LAYER THICKNESSES:
parameter
  TOP_T  = 3000.*Angstrom,      // top film superconductor thickness
  GP_T   = 3000.*Angstrom,      // ground plane superconductor thickness
  SUB1_T = 4500.*Angstrom,      // SiO layer generally
  SUB2_T = 2500.*Angstrom;      // SiO in tuning inductor


// THE DIMENSIONS OF THE MICROSTRIPS
wlt
  RF1  = { 5.8*Micron, 11.2*Micron, SUB1_T }, // RF section 1 (nearest antenna)
  RF2  = { 3.3*Micron, 15.0*Micron, SUB1_T }, // RF section 2
  L2   = { 5.0*Micron,  2.5*Micron, SUB2_T }, // tuning bit outside SIS's
  L1   = { 5.0*Micron,  6.9*Micron, SUB2_T }, // tuning inductor between SIS's
  ST_S = { 5.8*Micron,  3.3*Micron, SUB1_T }, // bit leading into radial stubs
  IF1  = { 2.0*Micron, 37.0*Micron, SUB1_T }, // IF section 1 (nearest stub)
  IF2  = {10.0*Micron, 36.0*Micron, SUB1_T }, // IF section 2
  IF3  = { 5.0*Micron,  1.0*Micron, SUB1_T }; // IF section 3 (nearest IF out)


// THE RADIAL STUB RADIUS
parameter
  STUB_RAD = 36.0*Micron; 


// THE GLOBAL VARIABLES NEEDED FOR THE OPERATING STATE
parameter
  TEMPERATURE   = 4.2*Kelvin,    // the receiver physical temperature
  IF_FREQ       = 1.5*GHz,       // the IF output frequency
  LO_FREQ       = 750*GHz,       // the LO source frequency
  LO_POWER      = 100*Nano*Watt, // the LO source power
  V_BIAS        = 2.2*mVolt;     // the SIS bias voltage

int
  HARMONICS     = 2,             // how many harmonics to use
  RF_PORT,                       // RF in port index (set in build_mixer.inc) 
  IF_PORT;                       // IF out port index (set in build_mixer.inc) 

// THE TOUCHSTONE FILE NAMES FOR THE ANTENNA AND IF LOAD IMPEDANCE INFORMATION
const char * const ANT_FILE = "Zslot.750"; // MUST have a file for the antenna
const char * const IF_FILE  = 0;           // 0 means there is no IF file data


// THE PARAMETER GIVING THE IF MATCHING IMPEDANCE
complex_parameter
  IF_IMPEDANCE  = 50*Ohm;        // use the value specified if no IF file data
