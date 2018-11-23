/*******************************************************************************
** mixer_build.inc - file included in cpw_trafo.c, containing code to build   **
**                   the cpw based trafo tuned mixer                          **
** ========================================================================== **
** Author - Paul Grimes                                                       **
** Date   - 22/07/03                                                          **
** ========================================================================== **
**                                                                            **
** ========================================================================== **
** Changes:                                                                   **
** 27/05/09 - Removed Belitsky options, added Substrate as CPW substrate      **
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

const_diel Substrate;
    if (SUBSTRATE_DEFAULT == "SI")
    {
        Substrate.eps = & SI_epsilon;
        Substrate.tand = & SI_loss_tangent;
    }
    else // assume air
    {
        Substrate.eps = & AIR_epsilon;
        Substrate.tand = & AIR_loss_tangent;
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


// Build the Rf and If circuits
#include "build_tuner.inc"

generator Lo;
    Lo.source_f = & Frequency;
    Lo.source_power = & Lo_Power;
    Lo.Temp = & Lo_Temp;


printf("# building IV curves...\n");

// Create the SIS junction(s) IV curve
ivcurve Iv(IV_CURVE.c_str(), KK_CURVE.c_str());
printf("# IV curves complete\n");

sis_device Sis_1;

voltage_source Bias_Source;
branch Bias_Branch;
circuit Bias;


// finally create the mixer and it's ports
mixer mix;
    mix.harmonics(HARMONICS);
    mix.set_LO(& Frequency);
    mix.set_rf(Rf);
    mix.set_if(If);

// This would normally be switched, but were assuming no Belitsky option here
// Create SIS junction
Sis_1.set_iv(Iv);
Sis_1.Vn = VGAP;
Sis_1.Rn = RNA/AREA;
Sis_1.Cap = SCAP*AREA;

// Create the Bias circuit
Bias_Source.source_f = 0.0;
Bias_Source.source_voltage = & Bias_Voltage;

mix.add_junction(Sis_1);
mix.set_bias(Bias_Source);
mix.set_balance_terminator(Lo, 2);
// Define port numbers
USB_PORT = mix.port(2,  1);
LSB_PORT = mix.port(2, -1);
IF_PORT  = mix.port(2,  0);

