/*******************************************************************************
** mixer_build.inc - file included in simple.c, containing code to build the  **
**                   simple mixer                                             **
** ========================================================================== **
** Author - Paul Grimes                                                       **
** Date   - 22/07/03                                                          **
** ========================================================================== **
**                                                                            **
** ========================================================================== **
** Changes:                                                                   **
*******************************************************************************/

// Define global device properties
device::T   = & TEMP;
device::Z0  = 50*Ohm;
device::f   = & IF_FREQ; // Mixers use the IF freq as the normalising freq

// Create the superconducting films used
super_film Top;
    // switch according to material to be used
    if (TOP_DEFAULT == "NB")
    {
        Top.Vgap    = & NB_Vgap;
        Top.Tc      = & NB_Tc;
        Top.rho_normal = & NB_Rho_normal;
    }
    else
    {
        Top.Vgap    = & NBTIN_Vgap;
        Top.Tc      = & NBTIN_Tc;
        Top.rho_normal = & NBTIN_Rho_normal;
    }
    Top.Thick = & TOP_THICKNESS;

super_film Ground;
    // switch according to material to be used
    if (GROUND_DEFAULT == "NB")
    {
        Ground.Vgap    = & NB_Vgap;
        Ground.Tc      = & NB_Tc;
        Ground.rho_normal = & NB_Rho_normal;
    }
    else
    {
        Ground.Vgap    = & NBTIN_Vgap;
        Ground.Tc      = & NBTIN_Tc;
        Ground.rho_normal = & NBTIN_Rho_normal;
    }
    Ground.Thick = & GROUND_THICKNESS;

// Create the dielectrics
const_diel Insulation;
    if (INSUL_DEFAULT == "SIO")
    {
        Insulation.eps = & SIO_epsilon;
        Insulation.tand = & SIO_loss_tangent;
    }
    else // assume air
    {
        Insulation.eps = & AIR_epsilon;
        Insulation.tand = & AIR_loss_tangent;
    }

const_diel Cover;
    if (COVER_DEFAULT == "SIO") // unlikely
    {
        Cover.eps = & SIO_epsilon;
        Cover.tand = & SIO_loss_tangent;
    }
    else // assume air
    {
        Cover.eps = & AIR_epsilon;
        Cover.tand = & AIR_loss_tangent;
    }

generator Lo;
    Lo.source_f = & Frequency;
    Lo.source_power = & Lo_Power;
    Lo.Temp = & Lo_Temp;


printf("# building IV curves...\n");

// Create the SIS junction(s) IV curve
ivcurve Iv(IV_CURVE.c_str(), KK_CURVE.c_str());
printf("# IV curves complete\n");

// Declare the SIS junctions.
sis_device Sis_1;
sis_device Sis_2;
sis_device Sis_3;
sis_device Sis_4;

// Create the bias source
voltage_source Bias_Source;
Bias_Source.source_f = 0.0;
Bias_Source.source_voltage = & Bias_Voltage;

// Build the Rf and If circuits
#include "build_tuner.inc"

// finally create the mixer and its ports
mixer mix;
    mix.harmonics(HARMONICS);
    mix.set_LO(& Frequency);
    mix.set_rf(Rf);
    mix.set_if(If);

    mix.add_junction(Sis_1);
    if (TUNER_TYPE == "BELITSKY" || TUNER_TYPE == "BEL_WITH_RFCHOKE" || TUNER_TYPE == "SERIES_DISTRIBUTED_2" || TUNER_TYPE == "SERIES_EXTERNAL_2")
    {
        mix.add_junction(Sis_2);
    }
    if (TUNER_TYPE == "SERIES_DISTRIBUTED_3" || TUNER_TYPE == "SERIES_EXTERNAL_3")
    {
        mix.add_junction(Sis_2);
        mix.add_junction(Sis_3);
    }
    if (TUNER_TYPE == "SERIES_DISTRIBUTED_4" || TUNER_TYPE == "SERIES_EXTERNAL_4")
    {
        mix.add_junction(Sis_2);
        mix.add_junction(Sis_3);
        mix.add_junction(Sis_4);
    }
    mix.set_bias(Bias);

    int NumSIS = mix.junctions();
    mix.set_balance_terminator(Lo, NumSIS+1);

    USB_PORT = mix.port(NumSIS+1,  1);
    LSB_PORT = mix.port(NumSIS+1, -1);
    IF_PORT  = mix.port(NumSIS+1,  0);
