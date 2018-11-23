/*******************************************************************************
** cpw_trafo.cc - Simulation of combined CPW/microstrip Trafo tuned mixer.    **
**            Uses dotConf library to set operating point at run time.        **
** ========================================================================== **
** Author - Paul Grimes                                                       **
** Date   - 14/07/03                                                          **
** ========================================================================== **
** SuperMix parameters are currently created as globals, so that dotconf      **
** functions that interpret each line in the .conf file can alter them as     **
** side effects.                                                              **
**                                                                            **
** We intend to use all caps for global parameter names, and camelback for    **
** SuperMix internal parameter names                                          **
** ========================================================================== **
** Changes:                                                                   **
** 27/05/09 - created from simple.c                                           **
**          - ditched use of dotconf for building tuning circuit: too complex **
**            for use with mixed microstrip/CPW circuit.  Reduced confparse & **
**            confprint accordingly                                           **
** ========================================================================== **
** Bugs:                                                                      **
** 14/7/03 - unitStr outputs incorrectly (poor use of pointers?)              **
**   Resolved 18/06/03 - char * arrays set in DOTCONF_CB get deallocated      **
**   on exit from DOTCONF_CB.  Use std::string instead of char * and use      **
**   string.c_str() to return char * as necessary                             **
*******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <dotconf.h>
#include "supermix.h"

// Include SuperMix parameters to be controlled by .conf files
#include "globals.inc"

// Include the code for parsing the .conf files
#include "confparse.inc"
#include "confprint.inc"

// Include the code for setting the operating point variables
// according to the units in use
#include "operating_point.inc"


int main(int argc, char **argv)
{
    // Sets up the dotconf stuff
    configfile_t *configfile;

    configfile = dotconf_create("cpw_trafo.conf",
                        options, NULL, CASE_INSENSITIVE);

    if (!configfile)
    {
        printf("Error opening config file \"cpw_trafo.conf\".\n");
        return 1;
    }

    if (dotconf_command_loop(configfile) == 0)
    {
        printf("Error reading config file \"cpw_trafo.conf\".\n");
    }

    dotconf_cleanup(configfile);
    // Finished with simple.conf

    // Now we work on the mixer configuration file

    configfile_t *mixerconfigfile;

    mixerconfigfile = dotconf_create((char *)MIXER_CONFIG.c_str(),
                        options, NULL, CASE_INSENSITIVE);

    if (!mixerconfigfile)
    {
        printf("Error opening mixer config file.\n\tMake sure that MIXER_CONFIG in \"cpw_trafo.conf\" is set correctly.\n");
        return 1;
    }

    if (dotconf_command_loop(mixerconfigfile) == 0)
    {
        printf("Error opening mixer config file.\n Make sure that MIXER_CONFIG in \"cpw_trafo.conf\" is set correctly.\n");
    }

    dotconf_cleanup(mixerconfigfile);
    // finished with the mixer config file

    // Output config file data to output file
    outputConf();

    printf("# building the mixer...\n");

    // Build the mixer
#include "mixer_build.inc"

    printf("# built mixer\n");

    // Initialise the operating point
    Frequency = get_Frequency(FREQUENCY);
    Bias_Voltage = get_Bias(BIAS);
    Lo_Power = get_Lo_Power(LO_POWER);

    // Initialise the mixer

    mix.initialize_operating_state();

    parameter scan_var;
    sdata result;
    sdata term_result;
    sdata hot_result;
    sdata cold_result;
    sdata tuner_data;
    sdata tuner_usb_data;
    sdata tuner_lsb_data;
    sdata if_data;
    complex Zstub; // Reused for each junction
    complex Zemb;
    complex ZembUSB;
    complex ZembLSB;
    complex ZIfstub;
    complex ZIfemb;
    complex TwoPiI(0.0, 2*Pi);

    if (SCANNED_VARIABLE == "FREQUENCY")
    {
        printf("# Results:\n#\n");

            printf("# %s (%s)| LSB Gain | USB Gain | IF Output 0K (K)| IF Output 77K (K)| IF Output 297K (K)| DC Current (uA)| Embedding Impedance\n"
                , SCANNED_VARIABLE.c_str(), SCAN_unitStr.c_str());


        // Start the scan loop, outputting data
        for( double s = SCAN_LOW/1.0;
                    s <= SCAN_HIGH/1.0;
                    s += SCAN_STEP/1.0)
        {
            // Set the altered parameter
            scan_var = s*SCAN_unit;
            if (SCANNED_VARIABLE == "FREQUENCY")
            {   Frequency = get_Frequency(scan_var);}
            else if (SCANNED_VARIABLE == "BIAS")
            {   Bias_Voltage = get_Bias(scan_var);}
            else // (SCANNED_VARIABLE == "LO_POWER")
            {   Lo_Power = get_Lo_Power(scan_var);}

            // Get the tuner data so we can calculate the embedding impedance
            device::f = Frequency;
            tuner_data = Rf.get_data();
            device::f = Frequency + IF_FREQ;
            tuner_usb_data = Rf.get_data();
            device::f = Frequency - IF_FREQ;
            tuner_lsb_data = Rf.get_data();
            device::f = IF_FREQ;
            if_data = If.get_data();

            Lo_Temp = 0.0*Kelvin;
            // Balance the mixer
            mix.balance();

            // Get the mixer data
            result = mix.get_data();
            term_result = mix.get_term_data();
            Lo_Temp = 77.0*Kelvin;
            cold_result = mix.get_term_data();
            Lo_Temp = 297.0*Kelvin;
            hot_result = mix.get_term_data();

            // Calculate the embedding impedance seen by each junction, at each frequency of interest
            Zstub = ((1.0 + tuner_data.S[1][1])/(1.0-tuner_data.S[1][1]))*device::Z0;
            Zemb  = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_1.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_1.Rn;

            Zstub = ((1.0 + tuner_usb_data.S[1][1])/(1.0-tuner_usb_data.S[1][1]))*device::Z0;
            ZembUSB  = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_1.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_1.Rn;

            Zstub = ((1.0 + tuner_lsb_data.S[1][1])/(1.0-tuner_lsb_data.S[1][1]))*device::Z0;
            ZembLSB  = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_1.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_1.Rn;

            ZIfstub = ((1.0 + if_data.S[1][1])/(1.0-if_data.S[1][1]))*device::Z0;
            ZIfemb = (1.0/((TwoPiI*device::f/GHz*1.0e-9*Sis_1.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_1.Rn;

            printf("%f      %f      %f      %f      %f      %f      %f      %f %fi      %f %fi      %f %fi      %f %fi\n",
                s,
                abs(result.S[IF_PORT][LSB_PORT]),
                abs(result.S[IF_PORT][USB_PORT]),
                term_result.C[IF_PORT][IF_PORT].real/Kelvin,
                cold_result.C[IF_PORT][IF_PORT].real/Kelvin,
                hot_result.C[IF_PORT][IF_PORT].real/Kelvin,
                abs(mix.I_junc(0)[1])/(Micro*Amp),
                Zemb.real, Zemb.imaginary,
                ZIfemb.real, ZIfemb.imaginary,
                ZembLSB.real, ZembLSB.imaginary,
                ZembUSB.real, ZembUSB.imaginary);
        }
    }
    else if (SCANNED_VARIABLE == "IF_FREQ")
    {
	    printf("# Results:\n#\n");

        printf("# %s (%s)| LSB Gain | USB Gain | IF Output 0K (K)| IF Output 77K (K)| IF Output 297K (K)| DC Current (uA)| Embedding Impedance\n"
            , SCANNED_VARIABLE.c_str(), SCAN_unitStr.c_str());


        // Start the scan loop, outputting data
        for( double s = SCAN_LOW/1.0;
                    s <= SCAN_HIGH/1.0;
                    s += SCAN_STEP/1.0)
        {
            // Set the altered parameter
            scan_var = s*SCAN_unit;
            if (SCANNED_VARIABLE == "FREQUENCY")
            {   Frequency = get_Frequency(scan_var);}
	        else if (SCANNED_VARIABLE == "IF_FREQ")
	        {	If_Freq = get_If_Freq(scan_var);}
            else if (SCANNED_VARIABLE == "BIAS")
            {   Bias_Voltage = get_Bias(scan_var);}
            else // (SCANNED_VARIABLE == "LO_POWER")
            {   Lo_Power = get_Lo_Power(scan_var);}

            // Get the tuner data so we can calculate the embedding impedance
            device::f = Frequency;
            tuner_data = Rf.get_data();
            device::f = Frequency + If_Freq;
            tuner_usb_data = Rf.get_data();
            device::f = Frequency - If_Freq;
            tuner_lsb_data = Rf.get_data();
            device::f = If_Freq;
            if_data = If.get_data();

            Lo_Temp = 0.0*Kelvin;
            // Balance the mixer
            mix.balance();

            // Get the mixer data
            result = mix.get_data();
            term_result = mix.get_term_data();
            Lo_Temp = 77.0*Kelvin;
            cold_result = mix.get_term_data();
            Lo_Temp = 297.0*Kelvin;
            hot_result = mix.get_term_data();


            // Calculate the embedding impedance seen by each junction, at each frequency of interest
            Zstub = ((1.0 + tuner_data.S[1][1])/(1.0-tuner_data.S[1][1]))*device::Z0;
            Zemb  = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_1.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_1.Rn;

            Zstub = ((1.0 + tuner_usb_data.S[1][1])/(1.0-tuner_usb_data.S[1][1]))*device::Z0;
            ZembUSB  = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_1.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_1.Rn;

            Zstub = ((1.0 + tuner_lsb_data.S[1][1])/(1.0-tuner_lsb_data.S[1][1]))*device::Z0;
            ZembLSB  = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_1.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_1.Rn;

            ZIfstub = ((1.0 + if_data.S[1][1])/(1.0-if_data.S[1][1]))*device::Z0;
            ZIfemb = (1.0/((TwoPiI*device::f/GHz*1.0e-9*Sis_1.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_1.Rn;

            printf("%f      %f      %f      %f      %f      %f      %f      %f %fi      %f %fi      %f %fi      %f %fi\n",
                s,
                norm(result.S[IF_PORT][LSB_PORT]),
                norm(result.S[IF_PORT][USB_PORT]),
                term_result.C[IF_PORT][IF_PORT].real/Kelvin,
                cold_result.C[IF_PORT][IF_PORT].real/Kelvin,
                hot_result.C[IF_PORT][IF_PORT].real/Kelvin,
                abs(mix.I_junc(0)[1])/(Micro*Amp),
                Zemb.real, Zemb.imaginary,
                ZIfemb.real, ZIfemb.imaginary,
                ZembLSB.real, ZembLSB.imaginary,
                ZembUSB.real, ZembUSB.imaginary);

        }
    }
    else // We are scanning BIAS or LO_POWER
    {
        // Get the tuner data so we can calculate the embedding impedance
        device::f = Frequency;
        tuner_data = Rf.get_data();
        device::f = Frequency + IF_FREQ;
        tuner_usb_data = Rf.get_data();
        device::f = Frequency - IF_FREQ;
        tuner_lsb_data = Rf.get_data();
        device::f = IF_FREQ;
        if_data = If.get_data();


        // Calculate the embedding impedance seen by each junction
        Zstub = ((1.0 + tuner_data.S[1][1])/(1.0-tuner_data.S[1][1]))*device::Z0;
        Zemb = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_1.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_1.Rn;

        Zstub = ((1.0 + tuner_usb_data.S[1][1])/(1.0-tuner_usb_data.S[1][1]))*device::Z0;
        ZembUSB  = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_1.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_1.Rn;

        Zstub = ((1.0 + tuner_lsb_data.S[1][1])/(1.0-tuner_lsb_data.S[1][1]))*device::Z0;
        ZembLSB  = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_1.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_1.Rn;

        ZIfstub = ((1.0 + if_data.S[1][1])/(1.0-if_data.S[1][1]))*device::Z0;
        ZIfemb = (1.0/((TwoPiI*device::f/GHz*1.0e-9*Sis_1.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_1.Rn;

        printf("# Junction Embedding Impedance : %f+%fi\n", Zemb.real/1.0, Zemb.imaginary/1.0);
        printf("# Junction IF Embedding Impedance : %f+%fi\n", ZIfemb.real/1.0, ZIfemb.imaginary/1.0);
        printf("# Junction LSB Embedding Impedance : %f+%fi\n", ZembLSB.real/1.0, ZembLSB.imaginary/1.0);
        printf("# Junction USB Embedding Impedance : %f+%fi\n", ZembUSB.real/1.0, ZembUSB.imaginary/1.0);
        printf("# Results:\n#\n");
        printf("# %s (%s)| LSB Gain | USB Gain | IF Output 0K (K)| IF Output 77K (K)| IF Output 297K (K)| DC Current (uA)\n"
            , SCANNED_VARIABLE.c_str(), SCAN_unitStr.c_str());


        // Start the scan loop, outputting data
        for( double s = SCAN_LOW/1.0;
                    s <= SCAN_HIGH/1.0;
                    s += SCAN_STEP/1.0)
        {
            // Set the altered parameter
            scan_var = s*SCAN_unit;
            if (SCANNED_VARIABLE == "FREQUENCY")
            {   Frequency = get_Frequency(scan_var);}
            else if (SCANNED_VARIABLE == "BIAS")
            {   Bias_Voltage = get_Bias(scan_var);}
            else // (SCANNED_VARIABLE == "LO_POWER")
            {   Lo_Power = get_Lo_Power(scan_var);}

            Lo_Temp = 0.0*Kelvin;
            // Balance the mixer
            mix.balance();

            // Get the mixer data
            result = mix.get_data();
            term_result = mix.get_term_data();
            Lo_Temp = 77.0*Kelvin;
            cold_result = mix.get_term_data();
            Lo_Temp = 297.0*Kelvin;
            hot_result = mix.get_term_data();

                printf("%f      %f      %f      %f      %f      %f      %f\n",
                    s,
                    abs(result.S[IF_PORT][LSB_PORT]),
                    abs(result.S[IF_PORT][USB_PORT]),
                    term_result.C[IF_PORT][IF_PORT].real/Kelvin,
                    cold_result.C[IF_PORT][IF_PORT].real/Kelvin,
                    hot_result.C[IF_PORT][IF_PORT].real/Kelvin,
                    abs(mix.I_junc(0)[1])/(Micro*Amp));
        }
    }
    // End of SuperMix work

    return 0;

}

