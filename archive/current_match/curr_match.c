/*******************************************************************************
** curr_match.c - SuperMix program to match experimental Pumped IV curves with**
**            simulated curves by altering LO power and junction capacitance  **
** ========================================================================== **
** Author - Paul Grimes                                                       **
** Date   - 06/08/03                                                          **
** ========================================================================== **
** Initially we wil just try to match the LO Power for a single IV curve      **
** ========================================================================== **
** Changes:                                                                   **
**          08/08/03 - Altered file to use the pump_curve class instead of    **
**          iv_match error term, as this will allow multiple pumped curves    **
**          11/08/03 - Altered to match new form of pump_curve constructor    **
**          and object.                                                       **
** ========================================================================== **
** Bugs:                                                                      **
*******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <dotconf.h>
#include "supermix.h"
#include "pump_curve.h"

// Include SuperMix parameters to be controlled by .conf files
#include "globals.inc"

// Include the code for setting the operating point variables
// according to the units in use
#include "operating_point.inc"

// Include the code for parsing the .conf files
#include "confparse.inc"
#include "confprint.inc"




int main(int argc, char **argv)
{
    // Sets up the dotconf stuff
    configfile_t *configfile;

    configfile = dotconf_create("curr_match.conf",
                        options, NULL, CASE_INSENSITIVE);
                        
    if (!configfile)
    {
        printf("Error opening config file \"curr_match.conf\".\n");
        return 1;
    }
    
    if (dotconf_command_loop(configfile) == 0)
    {
        printf("Error reading config file \"curr_match.conf\".\n");
    }
    
    dotconf_cleanup(configfile);
    // Finished with simple.conf
        
    // Now we work on the mixer configuration file
    
    configfile_t *mixerconfigfile;
    
    mixerconfigfile = dotconf_create((char *)MIXER_CONFIG.c_str(),
                        options, NULL, CASE_INSENSITIVE);
                        
    if (!mixerconfigfile)
    {
        printf("Error opening mixer config file.\n\tMake sure that MIXER_CONFIG in \"curr_match.conf\" is set correctly.\n");
        return 1;
    }
    
    if (dotconf_command_loop(mixerconfigfile) == 0)
    {
        printf("Error opening mixer config file.\n Make sure that MIXER_CONFIG in \"curr_match.conf\" is set correctly.\n");
    }
    
    dotconf_cleanup(mixerconfigfile);
    // finished with the mixer config file
    
    // Output config file data to output file 
    outputConf();
    
    // Build the mixer
#include "mixer_build.inc"
    
    cout << "# Built the mixer..." << endl;

    // Initialise the operating point
    Frequency = 700*GHz;
    Bias_Voltage = 2.0*mVolt;
    Lo_Power = 75.0*Nano*Watt;

    cout << "# Initial operating point:" << endl;
    cout << "# Frequency " << Frequency/GHz << " GHz\n";
    cout << "# Bias      " << Bias_Voltage/mVolt << " mV\n";
    cout << "# LO Power  " << Lo_Power/(Nano*Watt) << " nW\n";
    // Initialise the mixer
    
    mix.initialize_operating_state();
    
    // Start of optimiser work
    
    cout << "# Creating the error function..." << endl;
    // Create the (specific) capacitance varying error function
    error_func efCap;
    
    cap = efCap.vary(50*fFarad, cap, 100*fFarad);
    //rn = efCap.vary(14*Ohm, rn, 26*Ohm);
    
    // Create the bias sweeper
    sweeper bias_sweep;
        bias_sweep.sweep(Bias_Voltage, BIAS_LOW, BIAS_HIGH, (BIAS_HIGH-BIAS_LOW)/BIAS_POINTS);
    
    // Allow for small offsets in the data    
    parameter I_off = 0.0, V_off = 0.0;
    //I_off  = efCap.vary( -1, 0, 1, Micro*Amp);
    //V_off  = efCap.vary( -.01, 0, .01, mVolt);

    // initialise the pump_curve error terms, and add 
    for (int i = 0; i < pumped_curves.size(); ++i)
    {
        pumped_curves[i].i_offset(&I_off);
        pumped_curves[i].v_offset(&V_off);
        pumped_curves[i].measured.linear().build();

        pumped_curves[i].matched_lo_power = efCap.vary(1.0*Nano*Watt, Lo_Power, 300*Nano*Watt);

        /* // Test code
        cout << "Address of mix          : " << &mix <<          " Value of pumped_curves[i].mix      : " << pumped_curves[i].mix << endl;
        cout << "Address of Bias_Voltage : " << &Bias_Voltage << " Value of pumped_curves[i].V        : " << pumped_curves[i].V << endl;
        cout << "Address of Frequency    : " << &Frequency <<    " Value of pumped_curves[i].Lo_Freq  : " << pumped_curves[i].Lo_Freq << endl;
        cout << "Address of Bias_Voltage : " << &Lo_Power <<     " Value of pumped_curves[i].Lo_Power : " << pumped_curves[i].Lo_Power << endl;


        cout << "Value of pumped_curves[i].matched_lo_power : " << pumped_curves[i].matched_lo_power << endl;
        cout << "Value of pumped_curves[i].frequency        : " << pumped_curves[i].frequency << endl;
        cout << "Value of pumped_curves[i].vUnits           : " << pumped_curves[i].vUnits << endl;
        cout << "Value of pumped_curves[i].iUnits           : " << pumped_curves[i].iUnits << endl;
        cout << "Value of pumped_curves[i].filename         : " << pumped_curves[i].filename << endl;
        cout << "Value of pumped_curves[i].V_off            : " << pumped_curves[i].V_off << endl;
        cout << "Value of pumped_curves[i].I_off            : " << pumped_curves[i].I_off << endl;

        cout << "Testing pumped_curves[" << i << "]: file " << pumped_curves[i].filename << endl;

        cout << pumped_curves[i].get() << endl;*/

        efCap.add_term(1.0/(Micro*Amp*Micro*Amp), pumped_curves[i], bias_sweep);
    }

    // Create the Powell optimiser
    powell opt(efCap);
        opt.verbose();
        opt.FTOL = 0.00000001;
        opt.FOCUS = 0;

    cout << "# calling D = Pmax-Pmin" << endl;
    
    real_vector D = efCap.get_max_parms() - efCap.get_min_parms();
        
    cout << "# Starting the optimisation..." << endl;
        
    // Carry out the optimisation
    double final_error = opt.minimize();

    cout << "# Final Error " << final_error << endl;
    cout << "# Final Rn " << rn/Ohm << " Ohms\n";
    cout << "# Final Capacitance " << cap/fFarad << " fF\n";
    cout << "# Final LO Power " << pumped_curves[0].matched_lo_power/(Nano*Watt) << " nW\n";
    cout << "# \n";
    cout << "# V(mV)" << "\t" << "I(uA)" << endl;
    
    Lo_Power = pumped_curves[0].matched_lo_power;
    Frequency = pumped_curves[0].frequency;
    Vector currents;
    
    // We sweep over the Bias Voltage range, generating the response
    for(Bias_Voltage = 0.0; Bias_Voltage <= 9.0*mVolt; Bias_Voltage += 0.02*mVolt)
    {

        // Here's where we calculate the total bias current
        mix.balance();             // calculate the SIS operating state
        currents = mix.I_junc(0);  // vector of DC bias currents
        int min = currents.minindex();
        int max = currents.maxindex();

        // Sum the junction currents
        double resp = 0.0;
        for(int i=min; i<=max; i++) resp += currents[i].real;

        // Include offsets in the output, so it can be compared directly
        // with the measured data
        cout << fixed << setprecision(3)
         << (Bias_Voltage + V_off)/mVolt
         << "\t"
         << scientific << setprecision(3) << (resp + I_off)/(Micro*Amp)
         << endl;
    }

        
    return 0;
}

