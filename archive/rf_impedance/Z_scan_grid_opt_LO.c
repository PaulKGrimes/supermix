/******************************************************************************
* Z_scan_grid_opt_LO.c                                                        *
*                                                                             *
* Program for scanning the input impedance of a simple mixer                  *
*                                                                             *
* 03/08/2004 (c) Paul Grimes                                                  *
*                                                                             *
* usage: $ ./Z_scan_grid_opt_LO <LO_freq> <bias>                              *
******************************************************************************/

#include <iostream>
#include "supermix.h"
#include "arbitrary_impedance.h"
#include "error_term.h"

int main(int argc, char *argv[])
{
    parameter LO_F, BIAS, BIAS_RANGE, IF_Tn;
    
    // Read command-line options
    if (argc != 4)
    {   
        std::cerr << "Error: wrong number of command line options\n";
        std::cerr << "\tusage: $ ./Z_scan_grid_opt_LO <LO_freq> <bias> <bias_range> <IF noise>\n";
        return 1;    
    }
    else
    {   
        LO_F = atof(argv[1]);
        BIAS = atof(argv[2]);
        //BIAS_RANGE = atof(argv[3]);
        IF_Tn = atof(argv[3]);
    }

    parameter Bias = BIAS;

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
        bias.source_voltage = & Bias;
    
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
    
    mixer_SSB_noise et(mix, OUT, LSB, IF_Tn);
    
    error_func LO_power;
        parameter LO_V = LO_power.vary(0.01, 1.0, 10.0);
        
    real_vector Start_values(1);
    Start_values[1] = 1.0;
        
    /*// Create sweeper to vary bias in a 1/2 bias_range range about the set bias point
    parameter bias_lo = BIAS-(BIAS_RANGE/2.0);
    parameter bias_hi = BIAS+(BIAS_RANGE/2.0);
    parameter bias_step = (bias_hi - bias_lo) / 10.0;
    
    sweeper bias_sweep;
        bias_sweep.sweep( Bias, bias_lo, bias_hi, bias_step );*/
    
    LO_power.add_term(1.0, et);
    
    powell opt(LO_power);
    opt.FTOL = 1.0e-8;
    opt.FOCUS = 0.5;
    opt.CLOSENESS = 100;
    opt.ITMAX = 25;
    opt.verbose();
    
    /*montecarlo opt(LO_power);
    opt.npoints(100);
    opt.p().CLOSENESS = 10000;
    opt.p().ITMAX = 25;
    opt.p().FOCUS = 0.0;*/
    

    LO.source_voltage = & LO_V;

    std::cout << "# SuperMix Z_scan_grid_opt_LO output\n";
    std::cout << "# Using response curves Ideal_30.idc and Ideal_30.ikk\n";
    std::cout << "# LO signal f=" << LO_F << " V=" << LO_V << std::endl;
    std::cout << "# Bias voltage=" << BIAS << std::endl;
    
    std::cout << "# \n# Z.rf | Z.if | LO volts | DC current | LSB Gain | USB Gain | LSB Noise | USB_Noise | error | iterations\n";
    
    parameter result;
    
    for (int i=1; i<=50; i++)
    {
        for (int j=1; j<=50; j++)
        {
            Rf.set(Complex(i,0)/5.0);
            If.Z1 = Complex(j,0)/5.0;
            
            LO_power.set_parms(Start_values);
            
            result = opt.minimize();
            
            mix.balance();
            
            std::cout << real(Rf.Z) << "\t";
            std::cout << real(If.Z1) << "\t";
            std::cout << LO.source_voltage*RmsToPeak << "\t";
            std::cout << abs(mix.I_junc( 0 )[1]) << "\t";
            std::cout << abs(mix.get_data().S[OUT][LSB]) << "\t";
            std::cout << abs(mix.get_data().S[OUT][USB]) << "\t";
            std::cout << abs(mix.get_term_data().C[OUT][OUT])/norm(mix.get_data().S[OUT][LSB]) << "\t";            
            std::cout << abs(mix.get_term_data().C[OUT][OUT])/norm(mix.get_data().S[OUT][USB]) << "\t";
            std::cout << result << "\t";
            std::cout << opt.num_iter();
            std::cout << std::endl;
        }
    }
    
    return 0;
}
