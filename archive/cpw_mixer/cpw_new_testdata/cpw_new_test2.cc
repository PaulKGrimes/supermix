// cpw_new_test.cc

// Create a superconducting CPW and output some parameters

#include "supermix.h"
#include "cpw_new.h"

int main(int argc, char **argv)
{
	
	parameter T0, S0, W0, L0, E0, H0;
	
  // Get the command line parameters
  if (argc != 7)
  {
	  cout << "# Give me a metal thickness, width, gap, length, dielectric constant and thickness\n";
	  return 1;
  } else
  {
	  T0 = atof(argv[1]);
	  W0 = atof(argv[2]);
	  S0 = atof(argv[3]);
	  L0 = atof(argv[4]);
	  E0 = atof(argv[5]);
	  H0 = atof(argv[6]);
  }
	
  device::T  = 4.2*Kelvin;  // operating temperature
  device::f  = 230.0*GHz;     // this is the design frequency for the match
  device::Z0 = 50*Ohm;


  // The materials and the coplanar waveguide:

  super_film nb;
    nb.Vgap       = 2.9*mVolt;
    nb.Tc         = 9.2*Kelvin;
    nb.rho_normal = 5.0*Micro*Ohm*Centi*Meter;
    nb.Thick      = T0*Nano*Meter;

  // Loss value for float silicon at 10K
  const_diel Si(E0, 1.0e-4), air(1.0,0);

  cpw_new line1;
    line1.substrate(Si).top_strip(nb);
    line1.sub_thick = H0*Micron; 
  
  line1.width = W0*Micron;
  line1.space = S0*Micron;
  line1.length = L0*Micron;
  
  std::cout.setf(ios_base::scientific, ios_base::floatfield);
  std::cout.precision(4);
  
    std::cout << "# CPW New physical characteristics:" << endl
       << "#  Temperature:       " << device::T/Kelvin << " Kelvin" << endl
       << "#  Nb thickness:      " << nb.Thick/Angstrom << " Angstrom" << endl
       << "#  Si thickness:      " << line1.sub_thick/Micron << " Micron" << endl
       << "#  line width:        " << line1.width/Micron << " Micron" << endl
       << "#  line gap:          " << line1.space/Micron << " Micron" << endl
       << "#  line length:       " << line1.length/Micron << " Micron" << endl;
       
       
    std::cout << "# \n# Frequency sweep data.\n"
       << "# Freq | New Z_0 | New lambda_g | New Elec length | New loss | Nb Surface Resistance\n";
    
    parameter Z0 = device::Z0;

    for (parameter freq=5.0*GHz; freq <= 1e4*GHz; freq = freq+5.0*GHz)
    {
        device::f = freq;
        std::cout << freq/GHz << "\t"
            << line1.Zchar(device::f,device::T).real/Ohm << "\t"
            << line1.Kprop(device::f, device::T).imaginary << "\t"
            << line1.length/Micron*360./(2*Pi)*line1.Kprop(device::f,device::T).imaginary << "\t";
        // To get the loss via SdB function we have to match device::Z0 to the line impedance
        device::Z0 = line1.Zchar(device::f,device::T).real;
        std::cout  << line1.get_data().SdB(2,1) << endl;
    }   
    device::Z0 = Z0;
}
