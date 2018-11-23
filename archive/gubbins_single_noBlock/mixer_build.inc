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
device::Z0  = 20*Ohm;  
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
    
normal_film If_Top;
    If_Top.rho = & CU_Rho;
    If_Top.Thick = & IF_TOP_THICKNESS;
    
normal_film If_Ground;
    If_Ground.rho = & CU_Rho;
    If_Ground.Thick = & IF_GROUND_THICKNESS;
    
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

const_diel If_Board;
    if (IF_BOARD_DEFAULT == "SIO") // unlikely
    {
        If_Board.eps = & SIO_epsilon;
        If_Board.tand = & SIO_loss_tangent;
    }
    else // assume RT6010LM
    {
        If_Board.eps = & RT6010LM_epsilon;
        If_Board.tand = & RT6010LM_loss_tangent;
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
sis_device Sis_2;

voltage_source Bias_Source;
branch Bias_Branch;
circuit Bias;

printf("# IF Circuit Size : %d\n", If.size());

// finally create the mixer and it's ports
mixer mix;
    mix.harmonics(HARMONICS);
    mix.set_LO(& Frequency);
    mix.set_rf(Rf);
    mix.set_if(If);
    // Need to switch here as number of junction depends on tuner type    
    if (TUNER_TYPE == "BELITSKY" || TUNER_TYPE == "BEL_WITH_RFCHOKE")
    {
        // Create the two SIS junctions
        Sis_1.set_iv(Iv);
        Sis_1.Vn = VGAP;
        Sis_1.Rn = RNA/AREA;
        Sis_1.Cap = SCAP*AREA;

        Sis_2.set_iv(Iv);
        Sis_2.Vn = & VGAP;
        Sis_2.Rn = RNA/AREA;
        Sis_2.Cap = SCAP*AREA;    

        // Create the 2 junction bias circuit
        Bias_Source.source_f = 0.0;
        Bias_Source.source_voltage = & Bias_Voltage;
        Bias.connect(Bias_Source, 1, Bias_Branch, 1);
        Bias.add_port(Bias_Branch, 2);
        Bias.add_port(Bias_Branch, 3);
        
        mix.add_junction(Sis_1);
        mix.add_junction(Sis_2);
        mix.set_bias(Bias);
        mix.set_balance_terminator(Lo, 3);
        
        USB_PORT = mix.port(3,  1);
        LSB_PORT = mix.port(3, -1);
        IF_PORT  = mix.port(3,  0);
    
    }
    else
    {
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

    }
    
    
    

