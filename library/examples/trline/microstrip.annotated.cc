// microstrip.cc

// Create a superconducting microstrip line and use it to make a
// simple 1/4 wave transformer, matching 30 Ohms to a 10 Ohm load
// at 230 GHz.

#include "supermix.h"

int main()
{
  // Setting our global parameters (always a good idea to do this first,
  // so we don't forget):

  device::T  = 4.2*Kelvin;  // operating temperature
  device::f  = 230*GHz;     // this is the design frequency for the match
  device::Z0 = 30*Ohm;      // one of the imdepances to match 


  // ************************************************************************
  // THE BASIC PROCEDURE TO CREATE A MICROSTRIP OBJECT:

  // Before we can make a microstrip line, we need to create conductors
  // and dielectrics:

  // Conductors are objects derived from type surfimp (for "surface
  // impedance"), defined in surfaceZ.h.  We'll use a Niobium superconducting
  // film, so we need an object of type super_film. We then need to set the
  // physical characteristics by assigning the appropriate values to the
  // super_film member variables:

  super_film nb;
    nb.Vgap       = 2.9*mVolt;
    nb.Tc         = 9.2*Kelvin;
    nb.rho_normal = 5.0*Micro*Ohm*Centi*Meter;
    nb.Thick      = 3000*Angstrom;

  // Now for the dielectrics. trlines.h defines the class const_diel which
  // we'll use. We'll build a thin-film microstrip using SiO as the substrate.
  // The declarations we're using below take 2 arguments which are used to
  // set the values of the dielectric constant and loss tangent of the
  // material:

  const_diel SiO(5.6, .0001), air(1.0,0);

  // Using these materials, we construct our microstrip object. Class
  // microstrip is also found in trlines.h. See how we can set all the
  // materials in a single statement, if we want. The substrate thickness
  // is also set here to a value typical for thin-film designs:

  microstrip line;
    line.superstrate(air).substrate(SiO).top_strip(nb).ground_plane(nb);
    line.sub_thick = 2500*Angstrom; 

  // ************************************************************************
  // We still need to set the width of the top conductor and the length of the
  // line. The width determines the characteristic impedance and propagation
  // constant of the line, so we must determine it first. To set the width,
  // we'll use a simple iterative solver. The impedance we want is the
  // geometric mean of the impedances to be matched. We get the characteristic
  // impedance of our transmission line by calling the member function
  // Zchar(freq,Temp).

  double Zo = sqrt(30*Ohm * 10*Ohm); // match the real part of Zchar to this

  line.width = 1.0*Micron;           // initial guess for line width
  while( fabs(line.Zchar(device::f,device::T).real/Zo - 1.0) > 1.0e-6 )
    // increase width if Zchar too high, decrease width if too low
    line.width *= line.Zchar(device::f,device::T).real/Zo;

  // Now that we have a width, we need a length. Calculate length from the
  // imaginary part of the propagation constant to give 1/4 wavelength. The
  // member function Kprop(freq,Temp) gives the propagation constant.

  line.length = .25 * 2*Pi/line.Kprop(device::f,device::T).imaginary;


  // ************************************************************************
  // Now to test our transformer. Terminate the line with a 10 Ohm load and
  // look at S11 using our 30 Ohm normalizing impedance. It should be small
  // at 230 GHz.

  zterm load(10*Ohm);   // zterm is a 1-port terminator found in elements.h
  cascade stub;         // cascade is found in circuit.h
    stub.add(line).add(load);  // now we have a 1-port line terminated by load

  cout << "# 1/4 wave superconducting Niobium/SiO microstrip transformer" << endl
       << "# to match 10 Ohms to 30 Ohms at " << device::f/GHz << " GHz." << endl;

  cout << endl
       << "# Microstrip physical characteristics:" << endl
       << "#  Temperature:   " << device::T/Kelvin << " Kelvin" << endl
       << "#  Nb  thickness: " << nb.Thick/Angstrom << " Angstrom" << endl
       << "#  SiO thickness: " << line.sub_thick/Angstrom << " Angstrom" << endl
       << "#  line width:    " << line.width/Micron << " Micron" << endl
       << "#  line length:   " << line.length/Micron << " Micron" << endl
       << "#  line Zo:       " << line.Zchar(device::f,device::T).real/Ohm
       << " Ohm" << endl;

  cout << endl
       << "# f(GHz)" << "\t" << "Match (dB)" << endl
       << "# ------" << "\t" << "----------" << endl;

  for(device::f = 200*GHz; device::f <= 260*GHz; device::f += 5*GHz) {
    cout << "  " << setw(6) 
	 << device::f/GHz << "\t" << stub.get_data().SdB(1,1) << endl;
  }

}
