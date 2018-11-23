// Quick bit of code to simulate the gubbins mixer's IF filter

#include <stdio.h>
#include <string.h>
#include "supermix.h"

int main(int argc, char **argv)
	{
		parameter Freq = 20*GHz;
		
		parameter Zout = 50*Ohm;
		
		parameter L1 = 396*Micron;
		parameter L2 = 2683*Micron;
		parameter L3 = 2459*Micron;
		parameter L4 = 2501*Micron;
		parameter L5 = 2341*Micron;

		parameter W1 = 1090*Micron;
		parameter W2 = 698*Micron;
		parameter W3 = 533*Micron;
		parameter W4 = 401*Micron;
		parameter W5 = 305*Micron;
		
		parameter T1 = 17*Micron;
		parameter H1 = 254*Micron;
		
		device::T = 4.2*Kelvin;
		device::f = &Freq;
		device::Z0 = 20*Ohm;
		
		sdata_interp IF_Connect(2, device::f);
			IF_Connect.touchstone("SIS_singleEnded_IF_v6_20.s2p");
			
		normal_film Ground;
			Ground.rho = 0.002e-8*Ohm*Meter;
			Ground.Thick = & T1;
		
		normal_film Top;
			Top.rho = 0.002e-8*Ohm*Meter;
			Top.Thick = & T1;
		
		const_diel Insulation;
			Insulation.eps = 10.2;
			Insulation.tand = 0.0004;
		
		const_diel Cover;
			Cover.eps = 1.0;
			Cover.tand = 0.0;
		
		
		circuit filter;
		
		transformer IF_load;
			IF_load.Z1 = & Zout;
			IF_load.Z2 = & device::Z0;
		
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
		
		filter.connect(IF_Connect, 1, line1, 1);
		filter.connect(line1, 2, line2, 1);
		filter.connect(line2, 2, line3, 1);
		filter.connect(line3, 2, line4, 1);
		filter.connect(line4, 2, line5, 1);
		filter.connect(line5, 2, IF_load, 1);
		filter.add_port(IF_Connect, 2);
		filter.add_port(IF_load, 2);
		
		// Start scan over S parameters
		for (int f=1; f<=500; f++)
		{
			Freq = 0.1*f*GHz;
			std::cout << Freq/GHz << "\t" << filter.S(1,1).real << "\t" << filter.S(1,1).imaginary << "\t" << filter.S(1,2).real << "\t" << filter.S(1,2).imaginary << "\t"<< filter.S(2,1).real << "\t" << filter.S(2,1).imaginary << "\t"<< filter.S(2,2).real << "\t" << filter.S(2,2).imaginary << std::endl;
		}
	}
