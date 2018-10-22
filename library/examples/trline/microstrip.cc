// microstrip.cc

// Create a superconducting microstrip line and use it to make a
// simple 1/4 wave transformer, matching 30 Ohms to a 10 Ohm load
// at 230 GHz.

#include "supermix.h"

int main()
{
  device::T  = 4.2*Kelvin;  // operating temperature
  device::f  = 230*GHz;     // this is the design frequency for the match
  device::Z0 = 30*Ohm;      // one of the impedances to match 


  // The materials and the microstrip:

  super_film nb;
    nb.Vgap       = 2.9*mVolt;
    nb.Tc         = 9.2*Kelvin;
    nb.rho_normal = 5.0*Micro*Ohm*Centi*Meter;
    nb.Thick      = 3000*Angstrom;

  const_diel SiO(5.6, .0001), air(1.0,0);

  microstrip line;
    line.superstrate(air).substrate(SiO).top_strip(nb).ground_plane(nb);
    line.sub_thick = 2500*Angstrom; 


  // Determine the microstrip line width and length:

  double Zo = sqrt(30*Ohm * 10*Ohm); // match the real part of Zchar to this

  line.width = 1.0*Micron;           // initial guess for line width
  while( fabs(line.Zchar(device::f,device::T).real/Zo - 1.0) > 1.0e-6 )
    // increase width if Zchar too high, decrease width if too low:
    line.width *= line.Zchar(device::f,device::T).real/Zo;

  line.length = .25 * 2*Pi/line.Kprop(device::f,device::T).imaginary;


  // test the transformer response:

  zterm load(10*Ohm);
  cascade stub;
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
