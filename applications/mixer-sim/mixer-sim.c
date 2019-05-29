/*******************************************************************************
** simple.c - test of using dotconf library to control SuperMix parameters at **
**            run time.                                                       **
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

    configfile = dotconf_create("mixer-sim.conf",
                        options, NULL, CASE_INSENSITIVE);

    if (!configfile)
    {
        printf("Error opening config file \"mixer-sim.conf\".\n");
        return 1;
    }

    if (dotconf_command_loop(configfile) == 0)
    {
        printf("Error reading config file \"mixer-sim.conf\".\n");
    }

    dotconf_cleanup(configfile);
    // Finished with simple.conf

    // Now we work on the mixer configuration file

    configfile_t *mixerconfigfile;

    mixerconfigfile = dotconf_create((char *)MIXER_CONFIG.c_str(),
                        options, NULL, CASE_INSENSITIVE);

    if (!mixerconfigfile)
    {
        printf("Error opening mixer config file.\n\tMake sure that MIXER_CONFIG in \"simple.conf\" is set correctly.\n");
        return 1;
    }

    if (dotconf_command_loop(mixerconfigfile) == 0)
    {
        printf("Error opening mixer config file.\n Make sure that MIXER_CONFIG in \"simple.conf\" is set correctly.\n");
    }

    dotconf_cleanup(mixerconfigfile);
    // finished with the mixer config file

    // Output config file data to output file
    outputConf();

    printf("# building the mixer...\n");

    // Build the mixer
#include "mixer_build.inc"

    printf("# built mixer\n");

    printf("# Number of junctions: %d\n", NumSIS);
    printf("# IF_PORT: %d\n", IF_PORT);
    printf("# LSB_PORT: %d\n", LSB_PORT);
    printf("# USB_PORT: %d\n", USB_PORT);


    // Initialise the operating point
    Frequency = get_Frequency(FREQUENCY);
    Bias_Voltage = get_Bias(BIAS);
    Lo_Power = get_Lo_Power(LO_POWER);

    // Initialise the mixer

    mix.initialize_operating_state();

    printf("# IF Port Freq: %f GHz\n", if_freq/GHz);
    printf("# LSB Port Freq: %f GHz\n", lsb_freq/GHz);
    printf("# USB Port Freq: %f GHz\n", usb_freq/GHz);

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
    complex Zemb2;
    complex Zemb3;
    complex ZembUSB;
    complex ZembUSB2;
    complex ZembUSB3;
    complex ZembLSB;
    complex ZembLSB2;
    complex ZembLSB3;
    complex ZIfstub;
    complex ZIfemb;
    complex ZIfemb2;
    complex ZIfemb3;
    complex TwoPiI(0.0, 2*Pi);

    if (SCANNED_VARIABLE == "FREQUENCY")
    {
        printf("# Results:\n#\n");
        if (TUNER_TYPE == "BELITSKY" || TUNER_TYPE == "BEL_WITH_RFCHOKE")
        {
            printf("# %s (%s)| LSB Gain | USB Gain | IF Output 0K (K)| IF Output 77K (K)| IF Output 297K (K)| DC Current (junc 1) (uA)| DC Current (junc 2) (uA)| Embedding Impedance (junc 1) | Embedding Impedance (junc2)\n"
                , SCANNED_VARIABLE.c_str(), SCAN_unitStr.c_str());
        }
        else if (TUNER_TYPE == "SERIES_DISTRIBUTED_2" || "SERIES_EXTERNAL_2")
        {
            printf("# %s (%s)| LSB Gain | USB Gain | IF Output 0K (K)| IF Output 77K (K)| IF Output 297K (K)| DC Current (junc 1) (uA)| DC Current (junc 2) (uA)| DC Voltage (junc 1) (mV)| DC Voltage (junc 2) (mV)| Embedding Impedance (junc 1) | Embedding Impedance (junc2)\n"
                , SCANNED_VARIABLE.c_str(), SCAN_unitStr.c_str());
        }
        else if (TUNER_TYPE == "SERIES_DISTRIBUTED_3" || "SERIES_EXTERNAL_3")
        {
            printf("# %s (%s)| LSB Gain | USB Gain | IF Output 0K (K)| IF Output 77K (K)| IF Output 297K (K)| DC Current (junc 1) (uA)| DC Current (junc 2) (uA)| DC Current (junc 3) (uA)| DC Voltage (junc 1) (mV)| DC Voltage (junc 2) (mV)| DC Voltage (junc 3) (mV)| Embedding Impedance (junc 1) | Embedding Impedance (junc2) | Embedding Impedance (junc3)\n"
                , SCANNED_VARIABLE.c_str(), SCAN_unitStr.c_str());
        }
        else if (TUNER_TYPE == "SERIES_DISTRIBUTED_4" || "SERIES_EXTERNAL_4")
        {
            printf("# %s (%s)| LSB Gain | USB Gain | IF Output 0K (K)| IF Output 77K (K)| IF Output 297K (K)| DC Current (junc 1) (uA)| DC Current (junc 2) (uA)| DC Voltage (junc 1) (mV)| DC Voltage (junc 2) (mV)| Embedding Impedance (junc 1) | Embedding Impedance (junc2)\n"
                , SCANNED_VARIABLE.c_str(), SCAN_unitStr.c_str());
        }
        else
        {
            printf("# %s (%s)| LSB Gain | USB Gain | IF Output 0K (K)| IF Output 77K (K)| IF Output 297K (K)| DC Current (uA)| Embedding Impedance\n"
                , SCANNED_VARIABLE.c_str(), SCAN_unitStr.c_str());
        }


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

            if (TUNER_TYPE == "BELITSKY" || TUNER_TYPE == "BEL_WITH_RFCHOKE")
            {
                // Calculate the embedding impedance seen by each junction, at each frequency of interest
                Zstub = ((1.0 + tuner_data.S[1][1])/(1.0-tuner_data.S[1][1]))*device::Z0;
                Zemb  = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_1.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_1.Rn;

                Zstub = ((1.0 + tuner_usb_data.S[1][1])/(1.0-tuner_usb_data.S[1][1]))*device::Z0;
                ZembUSB  = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_1.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_1.Rn;

                Zstub = ((1.0 + tuner_lsb_data.S[1][1])/(1.0-tuner_lsb_data.S[1][1]))*device::Z0;
                ZembLSB  = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_1.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_1.Rn;

                ZIfstub = ((1.0 + if_data.S[1][1])/(1.0-if_data.S[1][1]))*device::Z0;
                ZIfemb = (1.0/((TwoPiI*device::f/GHz*1.0e-9*Sis_1.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_1.Rn;

                Zstub = ((1.0 + tuner_data.S[2][2])/(1.0-tuner_data.S[2][2]))*device::Z0;
                Zemb2 = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_2.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/(Sis_2.Rn);

                Zstub = ((1.0 + tuner_usb_data.S[2][2])/(1.0-tuner_usb_data.S[2][2]))*device::Z0;
                ZembUSB2  = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_2.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_2.Rn;

                Zstub = ((1.0 + tuner_lsb_data.S[2][2])/(1.0-tuner_lsb_data.S[2][2]))*device::Z0;
                ZembLSB2  = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_2.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_2.Rn;

                ZIfstub = ((1.0 + if_data.S[2][2])/(1.0-if_data.S[2][2]))*device::Z0;
                ZIfemb2 = (1.0/((TwoPiI*device::f/GHz*1.0e-9*Sis_2.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_2.Rn;

                printf("%f      %f      %f      %f      %f      %f      %f      %f      %f %fi      %f %fi      %f %fi      %f %fi      %f %fi      %f %fi      %f %fi      %f %fi\n",
                    s,
                    abs(result.S[IF_PORT][LSB_PORT]),
                    abs(result.S[IF_PORT][USB_PORT]),
                    term_result.C[IF_PORT][IF_PORT].real/Kelvin,
                    cold_result.C[IF_PORT][IF_PORT].real/Kelvin,
                    hot_result.C[IF_PORT][IF_PORT].real/Kelvin,
                    abs(mix.I_junc(0)[1])/(Micro*Amp),
                    abs(mix.I_junc(0)[2])/(Micro*Amp),
                    Zemb.real, Zemb.imaginary,
                    Zemb2.real, Zemb2.imaginary,
                    ZIfemb.real, ZIfemb.imaginary,
                    ZIfemb2.real, ZIfemb.imaginary,
                    ZembLSB.real, ZembLSB.imaginary,
                    ZembLSB2.real, ZembLSB2.imaginary,
                    ZembUSB.real, ZembUSB.imaginary,
                    ZembUSB2.real, ZembUSB2.imaginary);
            }
             else if (TUNER_TYPE == "SERIES_DISTRIBUTED_2" || "SERIES_EXTERNAL_2")
            {
                // Calculate the embedding impedance seen by each junction, at each frequency of interest
                Zstub = ((1.0 + tuner_data.S[1][1])/(1.0-tuner_data.S[1][1]))*device::Z0;
                Zemb  = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_1.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_1.Rn;

                Zstub = ((1.0 + tuner_usb_data.S[1][1])/(1.0-tuner_usb_data.S[1][1]))*device::Z0;
                ZembUSB  = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_1.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_1.Rn;

                Zstub = ((1.0 + tuner_lsb_data.S[1][1])/(1.0-tuner_lsb_data.S[1][1]))*device::Z0;
                ZembLSB  = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_1.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_1.Rn;

                ZIfstub = ((1.0 + if_data.S[1][1])/(1.0-if_data.S[1][1]))*device::Z0;
                ZIfemb = (1.0/((TwoPiI*device::f/GHz*1.0e-9*Sis_1.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_1.Rn;

                Zstub = ((1.0 + tuner_data.S[2][2])/(1.0-tuner_data.S[2][2]))*device::Z0;
                Zemb2 = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_2.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/(Sis_2.Rn);

                Zstub = ((1.0 + tuner_usb_data.S[2][2])/(1.0-tuner_usb_data.S[2][2]))*device::Z0;
                ZembUSB2  = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_2.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_2.Rn;

                Zstub = ((1.0 + tuner_lsb_data.S[2][2])/(1.0-tuner_lsb_data.S[2][2]))*device::Z0;
                ZembLSB2  = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_2.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_2.Rn;

                ZIfstub = ((1.0 + if_data.S[2][2])/(1.0-if_data.S[2][2]))*device::Z0;
                ZIfemb2 = (1.0/((TwoPiI*device::f/GHz*1.0e-9*Sis_2.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_2.Rn;

                printf("%f      %f      %f      %f      %f      %f      %f      %f      %f      %f      %f %fi      %f %fi      %f %fi      %f %fi      %f %fi      %f %fi      %f %fi      %f %fi\n",
                    s,
                    abs(result.S[IF_PORT][LSB_PORT]),
                    abs(result.S[IF_PORT][USB_PORT]),
                    term_result.C[IF_PORT][IF_PORT].real/Kelvin,
                    cold_result.C[IF_PORT][IF_PORT].real/Kelvin,
                    hot_result.C[IF_PORT][IF_PORT].real/Kelvin,
                    abs(mix.I_junc(0)[1])/(Micro*Amp),
                    abs(mix.I_junc(0)[2])/(Micro*Amp),
                    abs(mix.V_junc(0)[1])/(Milli*Volt),
                    abs(mix.V_junc(0)[2])/(Milli*Volt),
                    Zemb.real, Zemb.imaginary,
                    Zemb2.real, Zemb2.imaginary,
                    ZIfemb.real, ZIfemb.imaginary,
                    ZIfemb2.real, ZIfemb.imaginary,
                    ZembLSB.real, ZembLSB.imaginary,
                    ZembLSB2.real, ZembLSB2.imaginary,
                    ZembUSB.real, ZembUSB.imaginary,
                    ZembUSB2.real, ZembUSB2.imaginary);
            }
            else if (TUNER_TYPE == "SERIES_DISTRIBUTED_3" || "SERIES_EXTERNAL_3")
            {
                // Calculate the embedding impedance seen by each junction, at each frequency of interest
                Zstub = ((1.0 + tuner_data.S[1][1])/(1.0-tuner_data.S[1][1]))*device::Z0;
                Zemb  = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_1.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_1.Rn;

                Zstub = ((1.0 + tuner_usb_data.S[1][1])/(1.0-tuner_usb_data.S[1][1]))*device::Z0;
                ZembUSB  = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_1.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_1.Rn;

                Zstub = ((1.0 + tuner_lsb_data.S[1][1])/(1.0-tuner_lsb_data.S[1][1]))*device::Z0;
                ZembLSB  = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_1.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_1.Rn;

                ZIfstub = ((1.0 + if_data.S[1][1])/(1.0-if_data.S[1][1]))*device::Z0;
                ZIfemb = (1.0/((TwoPiI*device::f/GHz*1.0e-9*Sis_1.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_1.Rn;

                Zstub = ((1.0 + tuner_data.S[2][2])/(1.0-tuner_data.S[2][2]))*device::Z0;
                Zemb2 = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_2.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/(Sis_2.Rn);

                Zstub = ((1.0 + tuner_usb_data.S[2][2])/(1.0-tuner_usb_data.S[2][2]))*device::Z0;
                ZembUSB2  = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_2.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_2.Rn;

                Zstub = ((1.0 + tuner_lsb_data.S[2][2])/(1.0-tuner_lsb_data.S[2][2]))*device::Z0;
                ZembLSB2  = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_2.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_2.Rn;

                ZIfstub = ((1.0 + if_data.S[2][2])/(1.0-if_data.S[2][2]))*device::Z0;
                ZIfemb2 = (1.0/((TwoPiI*device::f/GHz*1.0e-9*Sis_2.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_2.Rn;

                Zstub = ((1.0 + tuner_data.S[3][3])/(1.0-tuner_data.S[3][3]))*device::Z0;
                Zemb3 = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_3.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/(Sis_3.Rn);

                Zstub = ((1.0 + tuner_usb_data.S[3][3])/(1.0-tuner_usb_data.S[3][3]))*device::Z0;
                ZembUSB3  = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_3.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_3.Rn;

                Zstub = ((1.0 + tuner_lsb_data.S[3][3])/(1.0-tuner_lsb_data.S[3][3]))*device::Z0;
                ZembLSB3  = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_3.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_3.Rn;

                ZIfstub = ((1.0 + if_data.S[3][3])/(1.0-if_data.S[3][3]))*device::Z0;
                ZIfemb3 = (1.0/((TwoPiI*device::f/GHz*1.0e-9*Sis_3.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_3.Rn;

                printf("%f      %f      %f      %f      %f      %f      %f      %f      %f      %f      %f      %f      %f %fi      %f %fi      %f %fi      %f %fi      %f %fi      %f %fi      %f %fi      %f %fi      %f %fi      %f %fi      %f %fi      %f %fi\n",
                    s,
                    abs(result.S[IF_PORT][LSB_PORT]),
                    abs(result.S[IF_PORT][USB_PORT]),
                    term_result.C[IF_PORT][IF_PORT].real/Kelvin,
                    cold_result.C[IF_PORT][IF_PORT].real/Kelvin,
                    hot_result.C[IF_PORT][IF_PORT].real/Kelvin,
                    abs(mix.I_junc(0)[1])/(Micro*Amp),
                    abs(mix.I_junc(0)[2])/(Micro*Amp),
                    abs(mix.I_junc(0)[3])/(Micro*Amp),
                    abs(mix.V_junc(0)[1])/(Milli*Volt),
                    abs(mix.V_junc(0)[2])/(Milli*Volt),
                    abs(mix.V_junc(0)[3])/(Milli*Volt),
                    Zemb.real, Zemb.imaginary,
                    Zemb2.real, Zemb2.imaginary,
                    Zemb3.real, Zemb3.imaginary,
                    ZIfemb.real, ZIfemb.imaginary,
                    ZIfemb2.real, ZIfemb2.imaginary,
                    ZIfemb3.real, ZIfemb3.imaginary,
                    ZembLSB.real, ZembLSB.imaginary,
                    ZembLSB2.real, ZembLSB2.imaginary,
                    ZembLSB3.real, ZembLSB3.imaginary,
                    ZembUSB.real, ZembUSB.imaginary,
                    ZembUSB2.real, ZembUSB2.imaginary,
                    ZembUSB3.real, ZembUSB3.imaginary);
           }
            else
            {
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

        if (TUNER_TYPE == "BELITSKY" || TUNER_TYPE == "BEL_WITH_RFCHOKE")
        {
            // Calculate the embedding impedance seen by each junction, at each frequency of interest
            Zstub = ((1.0 + tuner_data.S[1][1])/(1.0-tuner_data.S[1][1]))*device::Z0;
            Zemb  = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_1.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_1.Rn;

            Zstub = ((1.0 + tuner_usb_data.S[1][1])/(1.0-tuner_usb_data.S[1][1]))*device::Z0;
            ZembUSB  = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_1.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_1.Rn;

            Zstub = ((1.0 + tuner_lsb_data.S[1][1])/(1.0-tuner_lsb_data.S[1][1]))*device::Z0;
            ZembLSB  = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_1.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_1.Rn;

            ZIfstub = ((1.0 + if_data.S[1][1])/(1.0-if_data.S[1][1]))*device::Z0;
            ZIfemb = (1.0/((TwoPiI*device::f/GHz*1.0e-9*Sis_1.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_1.Rn;

            Zstub = ((1.0 + tuner_data.S[2][2])/(1.0-tuner_data.S[2][2]))*device::Z0;
            Zemb2 = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_1.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/(Sis_1.Rn);

            Zstub = ((1.0 + tuner_usb_data.S[2][2])/(1.0-tuner_usb_data.S[2][2]))*device::Z0;
            ZembUSB2  = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_1.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_1.Rn;

            Zstub = ((1.0 + tuner_lsb_data.S[2][2])/(1.0-tuner_lsb_data.S[2][2]))*device::Z0;
            ZembLSB2  = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_1.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_1.Rn;

            ZIfstub = ((1.0 + if_data.S[2][2])/(1.0-if_data.S[2][2]))*device::Z0;
            ZIfemb2 = (1.0/((TwoPiI*device::f/GHz*1.0e-9*Sis_1.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_1.Rn;

            printf("# Junction 1 LO Embedding Impedance : %f+%fi\n", Zemb.real/1.0, Zemb.imaginary/1.0);
            printf("# Junction 2 LO Embedding Impedance : %f+%fi\n# \n", Zemb2.real/1.0, Zemb2.imaginary/1.0);
            printf("# Junction 1 IF Embedding Impedance : %f+%fi\n", ZIfemb.real/1.0, ZIfemb.imaginary/1.0);
            printf("# Junction 2 IF Embedding Impedance : %f+%fi\n# \n", ZIfemb2.real/1.0, ZIfemb2.imaginary/1.0);
            printf("# Junction 1 LSB Embedding Impedance : %f+%fi\n", ZembLSB.real/1.0, ZembLSB.imaginary/1.0);
            printf("# Junction 2 LSB Embedding Impedance : %f+%fi\n# \n", ZembLSB2.real/1.0, ZembLSB2.imaginary/1.0);
            printf("# Junction 1 USB Embedding Impedance : %f+%fi\n", ZembUSB.real/1.0, ZembUSB.imaginary/1.0);
            printf("# Junction 2 USB Embedding Impedance : %f+%fi\n# \n", ZembUSB2.real/1.0, ZembUSB2.imaginary/1.0);
            printf("# Results:\n#\n");
            printf("# %s (%s)| LSB Gain | USB Gain | IF Output 0K (K)| IF Output 77K (K)| IF Output 297K (K)| DC Current (junc 1) (uA)| DC Current (junc 2) (uA)\n"
                , SCANNED_VARIABLE.c_str(), SCAN_unitStr.c_str());
        }
        else if (TUNER_TYPE == "SERIES_DISTRIBUTED_3" || "SERIES_EXTERNAL_3")
        {
            // Calculate the embedding impedance seen by each junction, at each frequency of interest
            Zstub = ((1.0 + tuner_data.S[1][1])/(1.0-tuner_data.S[1][1]))*device::Z0;
            Zemb  = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_1.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_1.Rn;

            Zstub = ((1.0 + tuner_usb_data.S[1][1])/(1.0-tuner_usb_data.S[1][1]))*device::Z0;
            ZembUSB  = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_1.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_1.Rn;

            Zstub = ((1.0 + tuner_lsb_data.S[1][1])/(1.0-tuner_lsb_data.S[1][1]))*device::Z0;
            ZembLSB  = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_1.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_1.Rn;

            ZIfstub = ((1.0 + if_data.S[1][1])/(1.0-if_data.S[1][1]))*device::Z0;
            ZIfemb = (1.0/((TwoPiI*device::f/GHz*1.0e-9*Sis_1.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_1.Rn;

            Zstub = ((1.0 + tuner_data.S[2][2])/(1.0-tuner_data.S[2][2]))*device::Z0;
            Zemb2 = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_2.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/(Sis_2.Rn);

            Zstub = ((1.0 + tuner_usb_data.S[2][2])/(1.0-tuner_usb_data.S[2][2]))*device::Z0;
            ZembUSB2  = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_2.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_2.Rn;

            Zstub = ((1.0 + tuner_lsb_data.S[2][2])/(1.0-tuner_lsb_data.S[2][2]))*device::Z0;
            ZembLSB2  = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_2.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_2.Rn;

            ZIfstub = ((1.0 + if_data.S[2][2])/(1.0-if_data.S[2][2]))*device::Z0;
            ZIfemb2 = (1.0/((TwoPiI*device::f/GHz*1.0e-9*Sis_2.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_2.Rn;

            Zstub = ((1.0 + tuner_data.S[3][3])/(1.0-tuner_data.S[3][3]))*device::Z0;
            Zemb3 = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_3.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/(Sis_3.Rn);

            Zstub = ((1.0 + tuner_usb_data.S[3][3])/(1.0-tuner_usb_data.S[3][3]))*device::Z0;
            ZembUSB3  = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_3.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_3.Rn;

            Zstub = ((1.0 + tuner_lsb_data.S[3][3])/(1.0-tuner_lsb_data.S[3][3]))*device::Z0;
            ZembLSB3  = (1.0/((TwoPiI*Frequency/GHz*1.0e9*Sis_3.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_3.Rn;

            ZIfstub = ((1.0 + if_data.S[3][3])/(1.0-if_data.S[3][3]))*device::Z0;
            ZIfemb3 = (1.0/((TwoPiI*device::f/GHz*1.0e-9*Sis_3.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/Sis_3.Rn;

            printf("# Junction 1 LO Embedding Impedance : %f+%fi\n", Zemb.real/1.0, Zemb.imaginary/1.0);
            printf("# Junction 2 LO Embedding Impedance : %f+%fi\n# \n", Zemb2.real/1.0, Zemb2.imaginary/1.0);
            printf("# Junction 3 LO Embedding Impedance : %f+%fi\n# \n", Zemb3.real/1.0, Zemb3.imaginary/1.0);
            printf("# Junction 1 IF Embedding Impedance : %f+%fi\n", ZIfemb.real/1.0, ZIfemb.imaginary/1.0);
            printf("# Junction 2 IF Embedding Impedance : %f+%fi\n# \n", ZIfemb2.real/1.0, ZIfemb2.imaginary/1.0);
            printf("# Junction 3 IF Embedding Impedance : %f+%fi\n# \n", ZIfemb3.real/1.0, ZIfemb3.imaginary/1.0);
            printf("# Junction 1 LSB Embedding Impedance : %f+%fi\n", ZembLSB.real/1.0, ZembLSB.imaginary/1.0);
            printf("# Junction 2 LSB Embedding Impedance : %f+%fi\n# \n", ZembLSB2.real/1.0, ZembLSB2.imaginary/1.0);
            printf("# Junction 3 LSB Embedding Impedance : %f+%fi\n# \n", ZembLSB3.real/1.0, ZembLSB3.imaginary/1.0);
            printf("# Junction 1 USB Embedding Impedance : %f+%fi\n", ZembUSB.real/1.0, ZembUSB.imaginary/1.0);
            printf("# Junction 2 USB Embedding Impedance : %f+%fi\n# \n", ZembUSB2.real/1.0, ZembUSB2.imaginary/1.0);
            printf("# Junction 3 USB Embedding Impedance : %f+%fi\n# \n", ZembUSB3.real/1.0, ZembUSB3.imaginary/1.0);
            printf("# Results:\n#\n");
            printf("# %s (%s)| LSB Gain | USB Gain | IF Output 0K (K)| IF Output 77K (K)| IF Output 297K (K)| DC Current (junc 1) (uA)| DC Current (junc 2) (uA)| DC Voltage (junc 1) (mV)| DC Voltage (junc 2) (mV)\n"
                , SCANNED_VARIABLE.c_str(), SCAN_unitStr.c_str());
        }
        else
        {
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
        }


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

            if (TUNER_TYPE == "BELITSKY" || TUNER_TYPE == "BEL_WITH_RFCHOKE")
            {
                printf("%f      %f      %f      %f      %f      %f      %f      %f\n",
                    s,
                    abs(result.S[IF_PORT][LSB_PORT]),
                    abs(result.S[IF_PORT][USB_PORT]),
                    term_result.C[IF_PORT][IF_PORT].real/Kelvin,
                    cold_result.C[IF_PORT][IF_PORT].real/Kelvin,
                    hot_result.C[IF_PORT][IF_PORT].real/Kelvin,
                    abs(mix.I_junc(0)[1])/(Micro*Amp),
                    abs(mix.I_junc(0)[2])/(Micro*Amp));
            }
            else if (TUNER_TYPE == "SERIES_DISTRIBUTED_2" || "SERIES_EXTERNAL_2")
            {
                printf("%f      %f      %f      %f      %f      %f      %f      %f      %f      %f\n",
                    s,
                    abs(result.S[IF_PORT][LSB_PORT]),
                    abs(result.S[IF_PORT][USB_PORT]),
                    term_result.C[IF_PORT][IF_PORT].real/Kelvin,
                    cold_result.C[IF_PORT][IF_PORT].real/Kelvin,
                    hot_result.C[IF_PORT][IF_PORT].real/Kelvin,
                    abs(mix.I_junc(0)[1])/(Micro*Amp),
                    abs(mix.I_junc(0)[2])/(Micro*Amp),
                    abs(mix.V_junc(0)[1])/(Milli*Volt),
                    abs(mix.V_junc(0)[2])/(Milli*Volt));
            }
            else if (TUNER_TYPE == "SERIES_DISTRIBUTED_3" || "SERIES_EXTERNAL_3")
            {
                printf("%f      %f      %f      %f      %f      %f      %f      %f      %f      %f      %f      %f\n",
                    s,
                    abs(result.S[IF_PORT][LSB_PORT]),
                    abs(result.S[IF_PORT][USB_PORT]),
                    term_result.C[IF_PORT][IF_PORT].real/Kelvin,
                    cold_result.C[IF_PORT][IF_PORT].real/Kelvin,
                    hot_result.C[IF_PORT][IF_PORT].real/Kelvin,
                    abs(mix.I_junc(0)[1])/(Micro*Amp),
                    abs(mix.I_junc(0)[2])/(Micro*Amp),
                    abs(mix.I_junc(0)[3])/(Micro*Amp),
                    abs(mix.V_junc(0)[1])/(Milli*Volt),
                    abs(mix.V_junc(0)[2])/(Milli*Volt),
                    abs(mix.V_junc(0)[3])/(Milli*Volt));
            }
            else
            {
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
    }
    // End of SuperMix work

    return 0;
}
