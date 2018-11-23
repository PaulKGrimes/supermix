// Program to calculate scattering parameters of capacitor inline with 3micron microstrip
//
// Paul Grimes, 17th September 2003
//

#include "supermix.h"

int main(int argc, char** argv)
{
    // Get the capacitance to use from the cmd line
    parameter cap;

    cap = atof(argv[1])*fFarad;


    device::T  = 4.2*Kelvin;
    device::f  = 700*GHz;
    device::Z0 = 10*Ohm;


    // The materials and the microstrip:

    super_film nb;
        nb.Vgap       = 2.9*mVolt;
        nb.Tc         = 9.2*Kelvin;
        nb.rho_normal = 5.0*Micro*Ohm*Centi*Meter;
        nb.Thick      = 4000*Angstrom;

    const_diel SiO(5.8, .0001), air(1.0,0);

    microstrip line;
        line.superstrate(air).substrate(SiO).top_strip(nb).ground_plane(nb);
        line.sub_thick = 4250*Angstrom;
        line.width = 6.5*Micron;
        line.length = 10.0*Micron;

    // copy line
    microstrip line2;
        line2.superstrate(air).substrate(SiO).top_strip(nb).ground_plane(nb);
        line2.sub_thick = 4250*Angstrom;
        line2.width = 6.5*Micron;
        line2.length = 10.0*Micron;

    // create the capacitor
    capacitor inline_cap;
        inline_cap.set(&cap);
        inline_cap.series();

    // create the circuit
    circuit filter;
        filter.connect(line, 2, inline_cap, 1);
        filter.connect(inline_cap, 2, line2, 1);
        filter.add_port(line, 1);
        filter.add_port(line2, 2);

    // Start output
    cout << "# Effect of series " << cap/fFarad << " fF capacitance embedded in 10 Ohm microstrip" << endl;

    device::f = 0.001*GHz;

    sdata params = filter.get_data();

    cout << device::f/GHz << "\t" << params.SdB(1, 1) << "\t" << params.SdB(1, 2) << "\t" << params.tn(2,1) << endl;

    // Loop over frequencies to get the s parameters of this circuit
    for (double f=5; f<=2000; f+=5)
    {
        device::f = f*GHz;

        params = filter.get_data();

        cout << f << "\t" << params.SdB(1, 1) << "\t" << params.SdB(1, 2) << "\t" << params.tn(2,1) << endl;
    }

    return 0;
}
