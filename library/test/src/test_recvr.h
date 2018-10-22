// test_recvr.h
// header file for test_recvr.cc and test_recvr_2.cc
// simulates a single-junction receiver with RF matching network
// 3/25/99
//
// 9/22/00: fixed Nb Tc. Now Tc = 9.2 Kelvin.
// updated 11/16/99 for new super_film class


#include "supermix.h"
#include "parameter/function_real_parameter.h"


// **************************************************************
// conveniently hold the characteristics of a superconducting film
struct sc_material { const double Vgap, Tc, rho_normal; }
  nb    = {2.9*mVolt, 9.2*Kelvin, 5.*Micro*Ohm*Centi*Meter},
  nbtin = {5.0*mVolt, 15.75*Kelvin, 30.*Micro*Ohm*Centi*Meter};


// **************************************************************
// an interpolated superconducting film given its physical film
// characteristics at construction
class film : public super_film {
public:
  // constructors do all kinds of set up:
  film(const sc_material & sc, abstract_real_parameter *t)
  { 
    tol = 5.0e-4;
    maxpts = 10000; 
    Vgap = sc.Vgap; Tc = sc.Tc; 
    rho_normal = sc.rho_normal;
    Thick = t;
  }

  film(const sc_material & sc, double t)
  { 
    tol = 5.0e-4;
    maxpts = 10000; 
    Vgap = sc.Vgap; Tc = sc.Tc; 
    rho_normal = sc.rho_normal;
    Thick = t;
  }
};


// **************************************************************
// Define vacuum dielectric.
class vacuum_diel : public const_diel {
public:
  // default constructor just sets physical parameters
  vacuum_diel()
  {
    eps = 1.0;
    tand = 0.0;
  }
};


// **************************************************************
// Define SiO dielectric.
class sio_diel : public const_diel {
public:
  // default constructor just sets physical parameters
  sio_diel()
  {
    eps = 5.6;
    tand = 0.0;
  }
};


// **************************************************************
// conveniently hold the materials of a microstrip
struct ms_material { surfimp & groundplane;
		     dielectric & substrate;
		     surfimp & topplane;
		     dielectric & superstrate; };


// **************************************************************
// conveniently hold the width and length of, say, a microstrip
struct wl { parameter w, l; };


// **************************************************************
// Define a microstrip class which takes characteristics in constructor
class ms : public microstrip {
public:
  // constructor must have materials, dimensions, substrate thickness
  ms(ms_material & mm, wl & dim, parameter & th)
  { 
    ground_plane(mm.groundplane).top_strip(mm.topplane);
    substrate(mm.substrate).superstrate(mm.superstrate);
    width = &dim.w; length = &dim.l; sub_thick = &th;
  }
};

// **************************************************************
// declare the global objects used to buid the receiver

parameter FILM_THICK  = 2000*Angstrom;
parameter SIO_THICK_1 = 1500*Angstrom;
parameter SIO_THICK_2 = 4500*Angstrom;
parameter LO_freq     = 345*GHz;
parameter LO_power    = 25.0*Nano*Watt;
parameter V_bias      = 2.2*mVolt;
int       harmonics   = 3;
complex_parameter RF_IMPEDANCE = complex(40*Ohm,-20*Ohm);
complex_parameter IF_IMPEDANCE = complex(50*Ohm);
parameter IF_freq     = 6*GHz;
const double TEMPERATURE = 4.2*Kelvin;

const char * IDC_FILE = "iv.dat";
const char * IKK_FILE = "ikk.dat";
const char * OPT_IF_IMP_FILE = "test_recvr.data";

// the groundplane and top superconducting films
film gp(nb,&FILM_THICK), top(nb,&FILM_THICK);

// the dielectric materials
sio_diel sio;
vacuum_diel vacuum;

// the materials list for the microstrips
ms_material ms_matl = {gp,sio,top,vacuum};

// the dimensions of the microstrips
wl MS_1 = {  5.0*Micron, 75.0*Micron }; // nearest antenna
wl MS_2 = { 19.0*Micron, 84.0*Micron };
wl MS_3 = {  2.0*Micron, 22.5*Micron }; // includes 2.5 near SIS

// the microstrips
ms ms_1(ms_matl, MS_1, SIO_THICK_2),
   ms_2(ms_matl, MS_2, SIO_THICK_2),
   ms_3(ms_matl, MS_3, SIO_THICK_1);

// sis device
ivcurve iv(IDC_FILE,IKK_FILE);
sis_device sis;
voltage_source bias;

// microstrip cascade
cascade lin_ckt;

// the mixer
mixer mix;

// other pieces we need
transformer ant, IF_load;
generator LO_source;
circuit RF, IF;
