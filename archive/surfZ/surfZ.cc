// surfZ.cc

// Create a superconducting film and use it to calculate the surface impedance
// and other surface parameters.

#include "supermix.h"

int main(int argc, char **argv)
{

	parameter Vgap, Tc, rho_normal, Thick, Temp;

  // Get the command line parameters
  if (argc != 6)
  {
	  cout << "# Give me gap voltage, critical temperature, normal resistivity, thickness and temperature\n";
	  return 1;
  } else
  {
	  Vgap = atof(argv[1]);
	  Tc = atof(argv[2]);
	  rho_normal = atof(argv[3]);
	  Thick = atof(argv[4]);
	  Temp = atof(argv[5])*Kelvin;
  }

  device::T  = Temp;
  device::f  = 100*GHz;
  device::Z0 = 50*Ohm;


  // The materials and the microstrip:

  super_film scFilm;
    scFilm.Vgap       = Vgap*mVolt;
    scFilm.Tc         = Tc*Kelvin;
    scFilm.rho_normal = rho_normal*Micro*Ohm*Centi*Meter;
    scFilm.Thick      = Thick*Nano*Meter;

  std::cout << "# Superconducting film complex conductivity and surface impedance\n# \n"
            << "# Gap Voltage: \t" << Vgap/mVolt << " mV\n"
            << "# Transition Temp: \t" << Tc/Kelvin << " K\n"
            << "# Normal Resistivity: \t" << rho_normal/Micro*Ohm*Centi*Meter << " uOhm cm\n"
            << "# Temperature: \t" << Temp/Kelvin<< " K\n# \n"
            << "# Frequency Sweep:"
            << "# Freq (GHz) | Sigma (Mho/meter) | Surf Imp (Ohm/square) | Surf Induct (pH/square) \n";

  complex sigma, surfZ;
  parameter surfL;

  std::cout.setf(ios_base::scientific, ios_base::floatfield);

  for (parameter freq=5.0*GHz; freq <= 1e4*GHz; freq = freq+5.0*GHz)
  {
    sigma = scFilm.sigma(freq, Temp);
    surfZ = scFilm.Zsurf(freq, Temp);
    surfL = surfZ.imaginary/(2*Pi*freq);
    std::cout << freq/GHz << "\t"
              << sigma.real/(Mho) << "\t"
              << -sigma.imaginary/(Mho) << "\t"
              << surfZ.real/(Ohm/Meter) << "\t"
              << surfZ.imaginary/(Ohm/Meter) << "\t"
              << surfL/(Pico*Henry) << std::endl;
  }
}
