// Program to calculate scattering parameters of capacitor inline with 3micron microstrip
//
// Paul Grimes, 13th November 2003
//

#include "supermix.h"

int main(int argc, char** argv)
{
    device::T  = 4.2*Kelvin;
    device::f  = 230*GHz;
    device::Z0 = 20*Ohm;


    // The materials and the microstrip:

    super_film nb;
        nb.Vgap       = 2.9*mVolt;
        nb.Tc         = 9.2*Kelvin;
        nb.rho_normal = 5.0*Micro*Ohm*Centi*Meter;
        nb.Thick      = 4000*Angstrom;

    const_diel SiO(5.8, .0001), air(1.0,0);

    microstrip line;
        line.superstrate(air).substrate(SiO).top_strip(nb).ground_plane(nb);
        line.sub_thick = 4750*Angstrom;
        line.width = 2.5*Micron;
        line.length = 44.0*Micron;

    microstrip line2;
        line2.superstrate(air).substrate(SiO).top_strip(nb).ground_plane(nb);
        line2.sub_thick = 4750*Angstrom;
        line2.width = 14*Micron;
        line2.length = 42*Micron;

    microstrip line3;
        line3.superstrate(air).substrate(SiO).top_strip(nb).ground_plane(nb);
        line3.sub_thick = 4750*Angstrom;
        line3.width = 2.5*Micron;
        line3.length = 44.0*Micron;

    microstrip line4;
        line4.superstrate(air).substrate(SiO).top_strip(nb).ground_plane(nb);
        line4.sub_thick = 4750*Angstrom;
        line4.width = 14*Micron;
        line4.length = 42*Micron;

    microstrip line5;
        line5.superstrate(air).substrate(SiO).top_strip(nb).ground_plane(nb);
        line5.sub_thick = 4750*Angstrom;
        line5.width = 2.5*Micron;
        line5.length = 44.0*Micron;

    microstrip line6;
        line6.superstrate(air).substrate(SiO).top_strip(nb).ground_plane(nb);
        line6.sub_thick = 4900*Angstrom;
        line6.width = 9.5*Micron;
        line6.length = 90*Micron;

    microstrip line7;
        line7.superstrate(air).substrate(SiO).top_strip(nb).ground_plane(nb);
        line7.sub_thick = 4900*Angstrom;
        line7.width = 2.5*Micron;
        line7.length = 145.0*Micron;


    radial_stub stub;
        stub.superstrate(air).substrate(SiO).top_strip(nb).ground_plane(nb);
        stub.sub_thick = 4900*Angstrom;
        stub.width = 3.0*Micron;
        stub.length = 125*Micron;
        stub.radius = 60*Micron;
        stub.angle = 180*Degree;

    microstrip stubout;
        stubout.superstrate(air).substrate(SiO).top_strip(nb).ground_plane(nb);
        stubout.sub_thick = 4900*Angstrom;
        stubout.width = 3.0*Micron;
        stubout.length = 125.0*Micron;



    // create the circuit
    circuit filter;
        filter.connect(line, 2, line2, 1);
        filter.connect(line2, 2, line3, 1);
		filter.connect(line3, 2, line4, 1);
		filter.connect(line4, 2, line5, 1);
//		filter.connect(line5, 2, line6, 1);
//		filter.connect(line6, 2, line7, 1);
		
        filter.add_port(line, 1);
        filter.add_port(line5, 2);

    circuit radial;
        radial.connect(stub, 2, stubout, 1);

        radial.add_port(stub, 1);
        radial.add_port(stubout, 2);

    // Start output
    cout << "# 3 stage filter response" << endl;

    sdata fdata, rdata;

    // Loop over frequencies to get the s parameters of this circuit
    for (double f=1; f<=1000; f+=1)
    {
        device::f = f*GHz;

        fdata = filter.get_data();
        rdata = radial.get_data();

        cout << f << "\t" << fdata.SdB(1, 1) << "\t" << arg(fdata.S[1][1])/Degree << "\t" << fdata.SdB(2, 1) << "\t" << rdata.SdB(1, 1) << "\t" << arg(rdata.S[1][1])/Degree << "\t" << rdata.SdB(2, 1) << endl;
    }

    return 0;
}
