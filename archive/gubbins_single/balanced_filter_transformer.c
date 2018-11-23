// Quick bit of coe to simulate the gubbins mixer's IF filter

#include <stdio.h>
#include <string.h>
#include "supermix.h"

int main(int argc, char **argv)
	{
		parameter Freq = 230*GHz;
		
		parameter L1 = 70*Micron;
		parameter L2 = 185*Micron;
		parameter W1 = 4*Micron;
		parameter W2 = 5*Micron;
		parameter C1 = 64*fFarad;
		parameter H1 = 0.425*Micron;
		parameter T1 = 0.4*Micron;
		
		parameter Z0 = 10*Ohm;
		
		device::T = 4.2*Kelvin;
		device::f = &Freq;
		device::Z0 = 10*Ohm;
		
		transformer Input;
			Input.Z1 = & device::Z0;
			Input.Z2 = & Z0;
		
		transformer Output;
			Output.Z1 = & Z0;
			Output.Z2 = & device::Z0;
		
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
		
		microstrip input1;
			input1.ground_plane(Ground);
			input1.substrate(Insulation);
			input1.top_strip(Top);
			input1.superstrate(Cover);
		
			input1.length = & L1;
			input1.width = & W1;
			input1.sub_thick = & H1;
		
		microstrip centre;
			centre.ground_plane(Ground);
			centre.substrate(Insulation);
			centre.top_strip(Top);
			centre.superstrate(Cover);
		
			centre.length = & L2;
			centre.width = & W2;
			centre.sub_thick = & H1;
		
		
		microstrip input2;
			input2.ground_plane(Ground);
			input2.substrate(Insulation);
			input2.top_strip(Top);
			input2.superstrate(Cover);
		
			input2.length = & L1;
			input2.width = & W1;
			input2.sub_thick = & H1;
		
		capacitor cap1;
			cap1.C = & C1;
			cap1.series();
			
		capacitor cap2;
			cap2.C = & C1;
			cap2.series();
		
		filter.connect(Input, 2, input1, 1);
		filter.connect(input1, 2, cap1, 1);
		filter.connect(cap1, 2, centre, 1);
		filter.connect(centre, 2, cap2, 1);
		filter.connect(cap2, 2, input2, 1);
		filter.connect(input2, 2, Output, 1);
		filter.add_port(Input, 1);
		filter.add_port(Output, 2);
		
		// Start scan over S parameters
		for (int f=1; f<=500; f++)
		{
			Freq = f*GHz;
			std::cout << Freq/GHz << "\t" << filter.S(1,1).real << "\t" << filter.S(1,1).imaginary << "\t" << filter.S(1,2).real << "\t" << filter.S(1,2).imaginary << "\t"<< filter.S(2,1).real << "\t" << filter.S(2,1).imaginary << "\t"<< filter.S(2,2).real << "\t" << filter.S(2,2).imaginary << std::endl;
		}
	}
