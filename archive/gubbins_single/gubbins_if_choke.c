// Quick bit of code to simulate the gubbins mixer's IF filter

#include <stdio.h>
#include <string.h>
#include "supermix.h"

int main(int argc, char **argv)
	{
		parameter Freq = 230*GHz;
		
		parameter L1 = 125*Micron;
		parameter L2 = 125*Micron;
		parameter R1 = 30*Micron;
		parameter W1 = 3.0*Micron;
		parameter W2 = 3.0*Micron;
		parameter A1 = 180*Degree;
		
		parameter T1 = 0.4*Micron;
		parameter H1 = 0.425*Micron;
		
		device::T = 4.2*Kelvin;
		device::f = &Freq;
		device::Z0 = 20*Ohm;
		
		super_film Ground;
			Ground.Vgap = 2.85*mVolt;
			Ground.Tc = 9.3*Kelvin;
			Ground.rho_normal = 5.0*Micro*Ohm*Centi*Meter;
			Ground.Thick = & T1;
		
		super_film Top;
			Top.Vgap = 2.85*mVolt;
			Top.Tc = 9.3*Kelvin;
			Top.rho_normal = 5.0*Micro*Ohm*Centi*Meter;
			Top.Thick = & T1;
		
		const_diel Insulation;
			Insulation.eps = 5.8;
			Insulation.tand = 0.0004;
		
		const_diel Cover;
			Cover.eps = 1.0;
			Cover.tand = 0.0;
		
		
		circuit filter;
		
		radial_stub choke;
			choke.ground_plane(Ground);
			choke.substrate(Insulation);
			choke.top_strip(Top);
			choke.superstrate(Cover);
		
			choke.length = & L1;
			choke.width = & W1;
			choke.radius = & R1;
			choke.angle = & A1;
			choke.sub_thick = & H1;
			
		
		microstrip output;
			output.ground_plane(Ground);
			output.substrate(Insulation);
			output.top_strip(Top);
			output.superstrate(Cover);
		
			output.length = & L2;
			output.width = & W2;
			output.sub_thick = & H1;
			
		
		filter.connect(choke, 2, output, 1);
		
		filter.add_port(choke, 1);
		filter.add_port(output, 2);
		
		// Start scan over S parameters
		for (int f=1; f<=350; f++)
		{
			Freq = f*GHz;
			std::cout << Freq/GHz << "\t" << filter.S(1,1).real << "\t" << filter.S(1,1).imaginary << "\t" << filter.S(1,2).real << "\t" << filter.S(1,2).imaginary << "\t"<< filter.S(2,1).real << "\t" << filter.S(2,1).imaginary << "\t"<< filter.S(2,2).real << "\t" << filter.S(2,2).imaginary << std::endl;
		}
	}
