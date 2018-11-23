// Quick bit of code to simulate the gubbins mixer's IF filter

#include <stdio.h>
#include <string.h>
#include "supermix.h"

int main(int argc, char **argv)
	{
		parameter Freq = 217*GHz;
		
		parameter L1 = 52.4*Micron;
		parameter L2 = 152.8*Micron;
		parameter L3 = 108.1*Micron;
		parameter L4 = 150.5*Micron;
		parameter L5 = 94.3*Micron;
		parameter L6 = 118.4*Micron;
		parameter L7 = 129*Micron;
		
		parameter W1 = 13.0*Micron;
		parameter W2 = 1.9*Micron;
		parameter W3 = 4.51*Micron;
		parameter W4 = 1.9*Micron;
		parameter W5 = 8.06*Micron;
		parameter W6 = 1.9*Micron;
		parameter W7 = 3.5*Micron;
		
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
		
		microstrip line1;
			line1.ground_plane(Ground);
			line1.substrate(Insulation);
			line1.top_strip(Top);
			line1.superstrate(Cover);
		
			line1.length = & L1;
			line1.width = & W1;
			line1.sub_thick = & H1;
		
		microstrip line2 = line1;
			line2.width = & W2;
			line2.length = & L2;
		
		microstrip line3 = line1;
			line3.width = & W3;
			line3.length = & L3;
		
		microstrip line4 = line1;
			line4.width = & W4;
			line4.length = & L4;
		
		microstrip line5 = line1;
			line5.width = & W5;
			line5.length = & L5;
		
		microstrip line6 = line1;
			line6.width = & W6;
			line6.length = & L6;
		
		microstrip line7 = line1;
			line7.width = & W7;
			line7.length = & L7;
		
		
		filter.connect(line1, 2, line2, 1);
		filter.connect(line2, 2, line3, 1);
		filter.connect(line3, 2, line4, 1);
		filter.connect(line4, 2, line5, 1);
		filter.connect(line5, 2, line6, 1);
		filter.connect(line6, 2, line7, 1);
		
		filter.add_port(line1, 1);
		filter.add_port(line7, 2);
		
		// Start scan over S parameters
		for (int f=1; f<=350; f++)
		{
			Freq = f*GHz;
			std::cout << Freq/GHz << "\t" << filter.S(1,1).real << "\t" << filter.S(1,1).imaginary << "\t" << filter.S(1,2).real << "\t" << filter.S(1,2).imaginary << "\t"<< filter.S(2,1).real << "\t" << filter.S(2,1).imaginary << "\t"<< filter.S(2,2).real << "\t" << filter.S(2,2).imaginary << std::endl;
		}
	}
