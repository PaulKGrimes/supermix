/******************************************************************************
* Z_scan_simple.c                                                             *
*                                                                             *
* Program for scanning the input impedance of a simple mixer                  *
*                                                                             *
* 03/08/2004 (c) Paul Grimes                                                  *
*                                                                             *
* usage: $ ./Z_scan_simple <LO_freq> <bias> <LO_voltage>                      *
******************************************************************************/

#include <iostream>
#include "supermix.h"
#include "arbitrary_impedance.h"

int main(int argc, char *argv[])
{
    parameter LO_F, BIAS, LO_V;
    
    // Read command-line options
    if (argc != 4)
    {   std::cerr << "Error: wrong number of command line options\n";
        std::cerr << "\tusage: $ ./Z_scan_simple <LO_freq> <bias> <LO_voltage>\n";
        return 1;    }
    else
    {   LO_F = atof(argv[1]);
        BIAS = atof(argv[2]);
        LO_V = atof(argv[3]);  }


    // Set the globals
    device::Z0 = 1.0;
    device::T  = 1.0;
    device::f  = 0.01*VoltToFreq;
    
    // Define the IV curves
    ivcurve iv( "Ideal_30.idc", "Ideal_30.ikk" );
    
    // Create the circuit components
    sis_device sis;
        sis.Rn = 1.0;
        sis.Cap = 0.0;
        sis.Vn  = 1.0;
        sis.set_iv( iv );
    
    voltage_source LO;
        LO.source_f = LO_F * VoltToFreq;
        LO.R = 1.0;
        LO.source_voltage = LO_V/RmsToPeak;
    
    voltage_source bias;
        bias.source_f = 0.0;
        bias.R = 0.0;
        bias.source_voltage = & BIAS;
    
    arbitrary_impedance Rf;
        Rf.set(Complex(1.0, 0.0));
    
    transformer If;
        If.Z1 = 1.0;
    
    mixer mix;
        mix.add_junction( sis );
        mix.harmonics( 1 );
        mix.set_rf( Rf );
        mix.set_if( If );
        mix.set_bias( bias );
        mix.set_balance_terminator ( LO, 2 );
        mix.set_LO( & LO.source_f );
        
    int USB = mix.port(2, 1);
    int LSB = mix.port(2, -1);
    int OUT = mix.port(2, 0);

    mix.initialize_operating_state();
    
    mix.balance();
    
    std::cout << "# SuperMix Z_scan_simple output\n";
    std::cout << "# Using response curves Ideal_30.idc and Ideal_30.ikk\n";
    std::cout << "# LO signal f=" << LO_F << " V=" << LO_V << std::endl;
    std::cout << "# Bias voltage=" << BIAS << std::endl;
    
    std::cout << "# \n# RF impedance | DC current | LSB Gain | USB Gain | Noise\n";
    
    for (int i=1; i<=1000; i++)
    {
        Rf.set(Complex(i,0)/100.0);
        mix.balance();
        
        std::cout << real(Rf.Z) << "\t";
        std::cout << abs(mix.I_junc( 0 )[1]) << "\t";
        std::cout << abs(mix.get_data().S[OUT][LSB]) << "\t";
        std::cout << abs(mix.get_data().S[OUT][USB]) << "\t";
        std::cout << abs(mix.get_term_data().C[OUT][OUT])/norm(mix.get_data().S[OUT][LSB]) << "\t"; 
        std::cout << abs(mix.get_term_data().C[OUT][OUT])/norm(mix.get_data().S[OUT][USB]) << std::endl;
        
    }
    
    return 0;
}
