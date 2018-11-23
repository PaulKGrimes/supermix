/*******************************************************************************
** balanced.c - simulation of a balanced mixer using SuperMix and the dotconf **
**              library                                                       **
** ========================================================================== **
** Author - Paul Grimes                                                       **
** Date   - 15/10/04                                                          **
** ========================================================================== **
** SuperMix parameters are currently created as globals, so that dotconf      **
** functions that interpret each line in the .conf file can alter them as     **
** side effects.                                                              **
**                                                                            **
** We intend to use all caps for global parameter names, and camelback for    **
** SuperMix internal parameter names                                          **
** ========================================================================== **
** Changes:                                                                   **
** ========================================================================== **
** Bugs:                                                                      **
*******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <dotconf.h>
#include "supermix.h"

// Include SuperMix parameters to be controlled by .conf files
#include "globals.h"

// Include the code for parsing the .conf files
#include "confparse.h"
#include "confprint.h"

// Include the code for setting the operating point variables
// according to the units in use
#include "operating_point.h"


int main(int argc, char **argv)
{
    // Sets up the dotconf stuff
    configfile_t *configfile;

    configfile = dotconf_create("balanced.conf",
                        options, NULL, CASE_INSENSITIVE);

    if (!configfile)
    {
        printf("Error opening config file \"balanced.conf\".\n");
        return 1;
    }

    if (dotconf_command_loop(configfile) == 0)
    {
        printf("Error reading config file \"balanced.conf\".\n");
    }

    dotconf_cleanup(configfile);
    // Finished with balanced.conf

    // Now we work on the mixer configuration file

    configfile_t *mixerconfigfile;

    mixerconfigfile = dotconf_create((char *)MIXER_CONFIG.c_str(),
                        options, NULL, CASE_INSENSITIVE);

    if (!mixerconfigfile)
    {
        printf("Error opening mixer config file.\n\tMake sure that MIXER_CONFIG in \"balanced.conf\" is set correctly.\n");
        return 1;
    }

    if (dotconf_command_loop(mixerconfigfile) == 0)
    {
        printf("Error opening mixer config file.\n Make sure that MIXER_CONFIG in \"balanced.conf\" is set correctly.\n");
    }

    dotconf_cleanup(mixerconfigfile);
    // finished with the mixer config file

    // Output config file data to output file - function defined in confprint.h
    outputConf();

    // Frequency is required to build RF terminators
    Frequency = get_Frequency(FREQUENCY);
    
    printf("# Building mixer...\n");
    // Build the mixer
#include "mixer_build.inc"
    
    printf("#    Done\n");

    printf("# Setting operating point...\n");
    // Initialise the operating point
    Bias_Voltage_1 = get_Bias(BIAS_1);
    Bias_Voltage_2 = get_Bias(BIAS_2);
    Lo_Power = get_Lo_Power(LO_POWER);
    printf("#    Done\n");
    
    // Initialise the mixer
    printf("# Initialising mixer...\n");
    mix.initialize_operating_state();
    printf("#    Done\n");
    
    parameter scan_var;
    sdata result;
    sdata term_result;
    sdata hot_result;
    sdata cold_result;

    printf("# Results:\n#\n");
    printf("# %s | RF->SUM LSB | RF->SUM USB | RF->DIFF LSB | RF->DIFF USB | LO->SUM LSB | LO->SUM USB | LO->DIFF LSB | LO->DIFF USB | SUM IF Output | DIFF IF Output | COLD SUM IF | COLD DIFF IF | HOT SUM IF | HOT DIFF IF | DC Current (1a) | DC Current (1b) | DC Current (2a) | DC Current (2b)\n", SCANNED_VARIABLE.c_str());


    // Start the scan loop, outputting data
    for( double s = SCAN_LOW/1.0;
                s <= SCAN_HIGH/1.0;
                s += SCAN_STEP/1.0)
    {
        // Set the altered parameter
        scan_var = s;
        if (SCANNED_VARIABLE == "FREQUENCY")
        {   Frequency = get_Frequency(scan_var*GHz);}
        else if (SCANNED_VARIABLE == "BIAS_1")
        {   Bias_Voltage_1 = get_Bias(scan_var*mVolt);}
        else if (SCANNED_VARIABLE == "BIAS_2")
        {   Bias_Voltage_2 = get_Bias(scan_var*mVolt);}
        else if (SCANNED_VARIABLE == "LO_POWER")
        {   Lo_Power = get_Lo_Power(scan_var*Nano*Watt);}
        else // (SCANNED_VARIABLE == "IF_FREQ")
        {   device::f = s*GHz;  }

        // Balance the mixer
        mix.balance();
        
        // Get the mixer data
        result = mix.get_data();
        term_result = mix.get_term_data();
        
        // Get hot/cold load data
        Load.Temp = 77*Kelvin;
        mix.balance();
        cold_result = mix.get_term_data();
        Load.Temp = 297*Kelvin;
        mix.balance();
        hot_result = mix.get_term_data();
        Load.Temp = & LOAD_NOISE_TEMP;
        mix.balance();
        
        //printf("%d\t%d\t%d\t%d\t%d\t%d\n", SUM_IF_PORT, DIFF_IF_PORT, RF_LSB_PORT, RF_USB_PORT, LO_LSB_PORT, LO_USB_PORT);
        
        printf("%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n",
                    s,
                    abs(result.S[ SUM_IF_PORT][RF_LSB_PORT]),
                    abs(result.S[ SUM_IF_PORT][RF_USB_PORT]),
                    abs(result.S[DIFF_IF_PORT][RF_LSB_PORT]),
                    abs(result.S[DIFF_IF_PORT][RF_USB_PORT]),
                    abs(result.S[ SUM_IF_PORT][LO_LSB_PORT]),
                    abs(result.S[ SUM_IF_PORT][LO_USB_PORT]),
                    abs(result.S[DIFF_IF_PORT][LO_LSB_PORT]),
                    abs(result.S[DIFF_IF_PORT][LO_USB_PORT]),
                    
                    term_result.C[ SUM_IF_PORT][ SUM_IF_PORT].real/Kelvin,
                    term_result.C[DIFF_IF_PORT][DIFF_IF_PORT].real/Kelvin,
                    
                    cold_result.C[ SUM_IF_PORT][ SUM_IF_PORT].real/Kelvin,
                    cold_result.C[DIFF_IF_PORT][DIFF_IF_PORT].real/Kelvin,
                    
                     hot_result.C[ SUM_IF_PORT][ SUM_IF_PORT].real/Kelvin,
                     hot_result.C[DIFF_IF_PORT][DIFF_IF_PORT].real/Kelvin,
                    
                    // These four lines for outputting IF hybrid data
                    //abs(IF_Hybrid.get_data().S[2][1]),
                    //abs(IF_Hybrid.get_data().S[3][1]),
                    //abs(IF_Hybrid.get_data().S[2][4]),
                    //abs(IF_Hybrid.get_data().S[3][4])
                    
                    abs(mix.I_junc(0)[1])/(Micro*Amp),
                    abs(mix.I_junc(0)[2])/(Micro*Amp),
                    abs(mix.I_junc(0)[3])/(Micro*Amp),
                    abs(mix.I_junc(0)[4])/(Micro*Amp)
                    );
    }   

    // End of SuperMix work
        
    return 0;
}

