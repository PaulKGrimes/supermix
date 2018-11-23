/******************************************************************************
* OP_scan_grid.c                                                              *
*                                                                             *
* Program for scanning the error function used for optimising the operating   *
* point in Z_scan code                                                        *
*                                                                             *
* 17/09/2004 (c) Paul Grimes                                                  *
*                                                                             *
* usage: $ ./OP_scan_grid <LO_freq> <Z_rf> <Z_if>                             *
******************************************************************************/

#include "supermix.h"
#include "arbitrary_impedance.h"
#include "error_term.h"

int main(int argc, char *argv[])
{
    parameter LO_F, Z_RF, Z_IF;
    
    // Read command-line options
    if (argc != 4)
    {   
        std::cerr << "Error: wrong number of command line options\n";
        std::cerr << "\tusage: $ ./Z_scan_grid_opt_bias_LO <LO_freq>\n";
        
        return 1;    
    }
    else
    {   
        LO_F = atof(argv[1]);
        Z_RF = atof(argv[2]);
        Z_IF = atof(argv[3]);
    }


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
    
    voltage_source bias;
        bias.source_f = 0.0;
        bias.R = 0.0;
    
    arbitrary_impedance Rf;
        Rf.set(Complex(Z_RF, 0.0));
    
    transformer If;
        If.Z1 = Z_IF;
    
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
    
    mixer_gain et(mix, OUT, LSB);
    mixer_stability stability(mix, OUT, &If.Z1);
 
 
    error_func op_point;
        parameter LO_V = op_point.vary(0.01, 0.1, 10.0);
        parameter BIAS = op_point.vary(0.01, 1.0,  1.0);
        
    op_point.add_term(1.0, et);
    op_point.add_term(1000.0, stability);
    
    do_nothing_optimizer opt(op_point);
    //opt.quiet();
    //opt.FTOL = 1.0e-6;
    //opt.FOCUS = 0.0;

    state_tag tag;
    parameter erf;

    LO.source_voltage = & LO_V;
    bias.source_voltage = & BIAS;

    std::cout << "# SuperMix Z_scan_simple output\n";
    std::cout << "# Using response curves Ideal_30.idc and Ideal_30.ikk\n";
    std::cout << "# LO signal f=" << LO_F << std::endl;
    std::cout << "# Z_RF=" << Z_RF << std::endl;
    std::cout << "# Z_IF=" << Z_IF << std::endl;
    
    std::cout << "# Bias | LO_voltage | error function | LSB Gain | USB Gain | Noise | stability\n";
    
    for (int i=1; i<=100; i++)
    {
        for (int j=1; j<=100; j++)
        {
            LO_V = (double)i/(10.0*RmsToPeak);
            BIAS = (double)j/50.0;
            
            erf = opt.minimize();
            
            std::cout << bias.source_voltage << "\t";
            std::cout << LO.source_voltage*RmsToPeak << "\t";
            std::cout << erf << "\t";
            std::cout << abs(mix.get_data().S[OUT][LSB]) << "\t";
            std::cout << abs(mix.get_data().S[OUT][USB]) << "\t";
            std::cout << abs(mix.get_term_data().C[OUT][OUT]) << "\t";
            LO.Temp = 297*Kelvin;
            std::cout << abs(mix.get_term_data().C[OUT][OUT]) << "\t";
            LO.Temp = 77*Kelvin;
            std::cout << abs(mix.get_term_data().C[OUT][OUT]) << "\t";
            LO.Temp = 0;
            std::cout << stability.get(tag) << std::endl;
        }
    }
    
    return 0;
}

