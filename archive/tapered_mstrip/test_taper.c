#include "supermix.h"

int main()
{
  device::T  = 4.2*Kelvin;  // operating temperature
  device::f  = 230*GHz;     // this is the design frequency for the match
  device::Z0 = 50*Ohm;      // one of the imdepances to match 


  // The materials and the microstrip:

    super_film nb;
    nb.Vgap       = 2.9*mVolt;
    nb.Tc         = 9.2*Kelvin;
    nb.rho_normal = 5.0*Micro*Ohm*Centi*Meter;
    nb.Thick      = 4000*Angstrom;

    const_diel SiO(5.8, .0), air(1.0,0);

    tapered_mstrip line;
    line.superstrate(air).substrate(SiO).top_strip(nb).ground_plane(nb);
    line.sub_thick = 4250*Angstrom; 
    line.width1 = 0.8*Micron;
    line.width2 = 7.0*Micron;
    line.length = 1000.0*Micron;
    
    tapered_mstrip line2(line);
    
    circuit test;
    test.connect(line, 2, line2, 2);
    test.add_port(line, 1);
    test.add_port(line2, 1);
    
    
    for(device::f = 200*GHz; device::f <= 900*GHz; device::f += 5*GHz) 
    {
        cout << setw(6) 
            << device::f/GHz << "\t" 
            << test.get_data().SdB(1,1) << "\t"
            << test.get_data().SdB(1,2) << endl;
    }
    
}
