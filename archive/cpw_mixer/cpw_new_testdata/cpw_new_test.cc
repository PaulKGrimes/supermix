// cpw_new_test.cc

// Create a superconducting CPW and output some parameters

#include "supermix.h"

int main(int argc, char **argv)
{

	parameter F0, S0, W0, L0;

  // Get the command line parameters
  if (argc != 5)
  {
	  cout << "# Give me frequency, width, gap and length\n";
	  return 1;
  } else
  {
	  F0 = atof(argv[1]);
	  W0 = atof(argv[2]);
	  S0 = atof(argv[3]);
	  L0 = atof(argv[4]);
  }

  device::T  = 4.2*Kelvin;  // operating temperature
  device::f  = F0*GHz;     // this is the design frequency for the match
  device::Z0 = 50*Ohm;


  // The materials and the coplanar waveguide:

  super_film nb;
    nb.Vgap       = 2.9*mVolt;
    nb.Tc         = 9.2*Kelvin;
    nb.rho_normal = 5.0*Micro*Ohm*Centi*Meter;
    nb.Thick      = 200*Nano*Meter;

  // Loss value for float silicon at 10K
  const_diel Si(11.9, 2.0e-4), air(1.0,0);

cpw_new Trafo1;
    Trafo1.top_strip(nb);
    Trafo1.substrate(Si);
    Trafo1.width = & W0;
    Trafo1.space = & S0;
    Trafo1.length = & L0;
    Trafo1.sub_thick = 70*Micron;

  cpw line2;
    line2.substrate(Si).top_strip(nb);
    line2.sub_thick = 70*Micron;

  line2.width = W0*Micron;
  line2.space = S0*Micron;
  line2.length = L0*Micron;

    std::cout << "# CPW New physical characteristics:" << endl
       << "#  Temperature:       " << device::T/Kelvin << " Kelvin" << endl
       << "#  Nb thickness:      " << nb.Thick/Angstrom << " Angstrom" << endl
       << "#  Si thickness:      " << Trafo1.sub_thick/Micron << " Micron" << endl
       << "#  line width:        " << Trafo1.width/Micron << " Micron" << endl
       << "#  line gap:          " << Trafo1.space/Micron << " Micron" << endl
       << "#  line length:       " << Trafo1.length/Micron << " Micron" << endl
       << "#  line Zchar:        " << Trafo1.Zchar(device::f,device::T).real/Ohm << " Ohm" << endl
       << "#  line elec length:  " << Trafo1.length/Micron*360./(2*Pi)*Trafo1.Kprop(device::f,device::T).imaginary << " Deg" << endl
       << "#  guided wavelength: " << 2*Pi/Trafo1.Kprop(device::f, device::T).imaginary << " Micron" << endl;
    // To get the loss via SdB function we have to match device::Z0 to the line impedance
    parameter Z0 = device::Z0;
    device::Z0 = Trafo1.Zchar(device::f,device::T).real;
    std::cout  << "#  loss:              " << Trafo1.get_data().SdB(2,1) << " dB" << endl << "# " << endl;
    device::Z0 = Z0;
/*
    std::cout << "# CPW Old physical characteristics:" << endl
       << "#  Temperature:       " << device::T/Kelvin << " Kelvin" << endl
       << "#  Nb thickness:      " << nb.Thick/Angstrom << " Angstrom" << endl
       << "#  Si thickness:      " << line2.sub_thick/Micron << " Micron" << endl
       << "#  line width:        " << line2.width/Micron << " Micron" << endl
       << "#  line gap:          " << line2.space/Micron << " Micron" << endl
       << "#  line length:       " << line2.length/Micron << " Micron" << endl
       << "#  line Zchar:        " << line2.Zchar(device::f,device::T).real/Ohm << " Ohm" << endl
       << "#  line elec length:  " << line2.length/Micron*360./(2*Pi)*line2.Kprop(device::f,device::T).imaginary << " Deg" << endl
       << "#  guided wavelength: " << 2*Pi/line2.Kprop(device::f, device::T).imaginary << " Micron" << endl;
    // To get the loss via SdB function we have to match device::Z0 to the line impedance
    Z0 = device::Z0;
    device::Z0 = line2.Zchar(device::f,device::T).real;
    std::cout  << "#  loss:              " << line2.get_data().SdB(2,1) << " dB" << endl << "# " << endl;
    device::Z0 = Z0;*/
}
