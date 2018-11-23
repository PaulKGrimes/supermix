// microstrip.cc

// Create a superconducting microstrip line and use it to make a
// simple 1/4 wave transformer, matching 30 Ohms to a 10 Ohm load
// at 230 GHz.

#include "supermix.h"

int main(int argc, char **argv)
{

	parameter F0, L0, W0;

  // Get the command line parameters
  if (argc != 4)
  {
	  cout << "# Give me frequency, width and physical length\n";
	  return 1;
  } else
  {
	  F0 = atof(argv[1]);
	  W0 = atof(argv[2]);
	  L0 = atof(argv[3]);
  }

  device::T  = 4.2*Kelvin;  // operating temperature
  device::f  = F0*GHz;     // this is the design frequency for the match
  device::Z0 = 50*Ohm;


  // The materials and the microstrip:

  super_film nb;
    nb.Vgap       = 2.9*mVolt;
    nb.Tc         = 9.2*Kelvin;
    nb.rho_normal = 5.0*Micro*Ohm*Centi*Meter;
    nb.Thick      = 4000*Angstrom;

  const_diel Si0(5.8, 1.0e-4), air(1.0,0);

  microstrip line1;
    line1.superstrate(air).substrate(Si0).top_strip(nb).ground_plane(nb);
    line1.sub_thick = 4900*Angstrom;

  // Determine the microstrip line width and length:


  line1.width = W0*Micron;           // initial guess for line width

  line1.length = L0*Micron;

  	std::cout << "# Microstrip 1 physical characteristics:" << endl
       << "#  Temperature:       " << device::T/Kelvin << " Kelvin" << endl
       << "#  Nb  thickness:     " << nb.Thick/Angstrom << " Angstrom" << endl
       << "#  SiO thickness:     " << line1.sub_thick/Angstrom << " Angstrom" << endl
       << "#  line width:        " << line1.width/Micron << " Micron" << endl
       << "#  line length:       " << line1.length/Micron << " Micron" << endl
       << "#  line Zchar:        " << line1.Zchar(device::f,device::T).real/Ohm << " Ohm" << endl
	   << "#  line elec length:  " << line1.length/Micron*360./(2*Pi)*line1.Kprop(device::f,device::T).imaginary << " Deg" << endl
	   << "#  guided wavelength: " << 2*Pi/line1.Kprop(device::f, device::T).imaginary << " Micron" << endl;
	   // To get the loss via SdB function we have to match device::Z0 to the line impedance
    parameter Z0 = device::Z0;
    device::Z0 = line1.Zchar(device::f,device::T).real;
    std::cout  << "#  loss:              " << line1.get_data().SdB(2,1) << " dB" << endl << "# " << endl;
    device::Z0 = Z0;
}
