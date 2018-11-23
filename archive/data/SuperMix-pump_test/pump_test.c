/******************************************************************************
* pump_test.c                                                                 *
*                                                                             *
* Program for simple pumping of an SIS junction by an LO signal               *
*                                                                             *
* 03/08/2004 (c) Paul Grimes                                                  *
*                                                                             *
* usage: $ ./pump_test <LO_freq> <LO_voltage>
******************************************************************************/

#include <iostream>
#include "supermix.h"

int main(int argc, char *argv[])
{
    parameter LO_F, LO_V;
    
    // Read command-line options
    if (argc != 3)
    {   std::cerr << "Error: wrong number of command line options\n";
        std::cerr << "\tusage: $ ./pump_test <LO_freq> <LO_voltage>\n";
        return 1;    }
    else
    {   LO_F = atof(argv[1]);
        LO_V = atof(argv[2]);  }

    // Set the globals
    device::Z0 = 1.0;
    device::T  = 1.0;
    device::f  = 0.01*VoltToFreq;
    
    // Define the IV curves
    ivcurve iv( "Ideal_30.idc", "Ideal_30_SM.ikk" );
    
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
    
    parameter Bias = 0.85;
    
    voltage_source bias;
        bias.source_f = 0.0;
        bias.R = 0.0;
        bias.source_voltage = & Bias;
    
    transformer Rf;
    
    transformer If;
        If.Z1 = 8.0;
    
    mixer mix;
        mix.add_junction( sis );
        mix.harmonics( 1 );
        mix.set_rf( Rf );
        mix.set_if( If );
        mix.set_bias( bias );
        mix.set_balance_terminator ( LO, 2 );
        mix.set_LO( & LO.source_f );
        
    mix.initialize_operating_state();
    
    mix.balance();
    
    std::cout << "# SuperMix pump_test output\n";
    std::cout << "# Using response curves Ideal_30.idc and Ideal_30.ikk\n";
    std::cout << "# Pump signal f=" << LO_F << " V=" << LO_V << " Z=1.0+0.0i\n";
    
    std::cout << "# \n# Bias | DC current | DC Voltage | Re(Pump current) | Re(Pump voltage) | Im(Pump Current) | Im(Pump voltage)\n";
    
    for (int i=0; i<=400; i++)
    {
        Bias = double(i)/100.0;
        mix.balance();
        
        std::cout << Bias << "\t";
        std::cout << abs(mix.I_junc( 0 )[1]) << "\t";
        std::cout << abs(mix.V_junc( 0 )[1]) << "\t";    
        std::cout << real(mix.I_junc( 1 )[1]) << "\t";
        std::cout << real(mix.V_junc( 1 )[1]) << "\t";
        std::cout << imag(mix.I_junc( 1 )[1]) << "\t";
        std::cout << imag(mix.V_junc( 1 )[1]) << endl;
    }
    
    return 0;
}
