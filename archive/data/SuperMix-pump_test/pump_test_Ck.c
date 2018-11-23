/******************************************************************************
* pump_test_Ck.c                                                              *
*                                                                             *
* Program for simple pumping of an SIS junction by an LO signal               *
*                                                                             *
* 03/08/2004 (c) Paul Grimes                                                  *
*                                                                             *
* usage: $ ./pump_test <bias> <LO_freq> <LO_voltage>                          *
******************************************************************************/

#include <iostream>
#include "supermix.h"

int main(int argc, char *argv[])
{
    parameter LO_F, LO_V, Bias;
    
    // Read command-line options
    if (argc != 4)
    {   std::cerr << "Error: wrong number of command line options\n";
        std::cerr << "\tusage: $ ./pump_test <bias> <LO_freq> <LO_voltage>\n";
        return 1;    }
    else
    {   Bias = atof(argv[1]);
        LO_F = atof(argv[2]);
        LO_V = atof(argv[3]);  }

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
    
    std::cout << "# SuperMix pump_test_Ck output\n";
    std::cout << "# Using response curves Ideal_30.idc and Ideal_30.ikk\n";
    std::cout << "# Bias Voltage =" << Bias << std::endl;
    std::cout << "# Pump signal f=" << LO_F << " V=" << LO_V << " Z=1.0+0.0i\n";
    
    std::cout << "# Balanced data:\n";
    std::cout << "# Junction Voltage (DC)" << mix.V_junc( 0 )[1].real << "+" << mix.V_junc( 0 )[1].imaginary << "i\n";
    std::cout << "# Junction Current (DC)" << mix.I_junc( 0 )[1].real << "+" << mix.I_junc( 0 )[1].imaginary << "i\n";
    
    std::cout << "# Junction Voltage (LO)" << mix.V_junc( 1 )[1].real << "+" << mix.V_junc( 1 )[1].imaginary << "i\n";
    std::cout << "# Junction Current (LO)" << mix.I_junc( 1 )[1].real << "+" << mix.I_junc( 1 )[1].imaginary << "i\n";
    
    std::cout << "# \n# SIS current calculation test:\n";
    Vector V(2, Index_C);
    V[0] = (Complex)Bias;
    V[1] = (Complex)LO_V/RmsToPeak;
    std::cout << "# V: " << V[0].real << "\t" << V[1].real << " + " << V[1].imaginary << "i\n";
    
    Vector I(2, Index_C);
    I = sis.large_signal(V, LO.source_f, 1);
    std::cout << "# I: " << I[0].real << "\t" << I[1].real << " + " << I[1].imaginary << "i\n";
    
    std::cout << "# Junction Voltage (DC)" << mix.V_junc( 0 )[1].real << "+" << mix.V_junc( 0 )[1].imaginary << "i\n";
    std::cout << "# Junction Current (DC)" << mix.I_junc( 0 )[1].real << "+" << mix.I_junc( 0 )[1].imaginary << "i\n";
    
    std::cout << "# Junction Voltage (LO)" << mix.V_junc( 1 )[1].real << "+" << mix.V_junc( 1 )[1].imaginary << "i\n";
    std::cout << "# Junction Current (LO)" << mix.I_junc( 1 )[1].real << "+" << mix.I_junc( 1 )[1].imaginary << "i\n";
    
    std::cout << "# \n# Freq | Ck.real | Ck.imag\n";
    
    Vector Ck = sis.Ck();
    
    for (int i=Ck.minindex(); i<=Ck.maxindex(); i++)
    {
        std::cout << i*mix.LO/VoltToFreq << "\t";
        std::cout << Ck.get(i).real << "\t";
        std::cout << Ck.get(i).imaginary << "\n";
    }
    
    return 0;
}
