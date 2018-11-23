/*******************************************************************************
** build_layers.inc - file included in balanced.c, containing code to build   **
**                    the balanced mixer                                      **
** ========================================================================== **
** Author - Paul Grimes                                                       **
** Date   - 21/10/04                                                          **
** ========================================================================== **
**                                                                            **
** ========================================================================== **
** Changes:                                                                   **
*******************************************************************************/

// Create the superconducting films used
super_film Wiring1;
    // switch according to material to be used
    if (WIRING_1 == "NB")
    {
        Wiring1.Vgap    = & NB_Vgap;
        Wiring1.Tc      = & NB_Tc;
        Wiring1.rho_normal = & NB_Rho_normal;
    }
    else
    {
        Wiring1.Vgap    = & NBTIN_Vgap;
        Wiring1.Tc      = & NBTIN_Tc;
        Wiring1.rho_normal = & NBTIN_Rho_normal;
    }
    Wiring1.Thick = & WIRING_1_THICKNESS;

super_film Ground1;
    // switch according to material to be used
    if (GROUND_1 == "NB")
    {
        Ground1.Vgap    = & NB_Vgap;
        Ground1.Tc      = & NB_Tc;
        Ground1.rho_normal = & NB_Rho_normal;
    }
    else
    {
        Ground1.Vgap    = & NBTIN_Vgap;
        Ground1.Tc      = & NBTIN_Tc;
        Ground1.rho_normal = & NBTIN_Rho_normal;
    }
    Ground1.Thick = & GROUND_1_THICKNESS;

// Create the superconducting films used
super_film Wiring2;
    // switch according to material to be used
    if (WIRING_2 == "NB")
    {
        Wiring2.Vgap    = & NB_Vgap;
        Wiring2.Tc      = & NB_Tc;
        Wiring2.rho_normal = & NB_Rho_normal;
    }
    else
    {
        Wiring2.Vgap    = & NBTIN_Vgap;
        Wiring2.Tc      = & NBTIN_Tc;
        Wiring2.rho_normal = & NBTIN_Rho_normal;
    }
    Wiring2.Thick = & WIRING_2_THICKNESS;

super_film Ground2;
    // switch according to material to be used
    if (GROUND_2 == "NB")
    {
        Ground2.Vgap    = & NB_Vgap;
        Ground2.Tc      = & NB_Tc;
        Ground2.rho_normal = & NB_Rho_normal;
    }
    else
    {
        Ground2.Vgap    = & NBTIN_Vgap;
        Ground2.Tc      = & NBTIN_Tc;
        Ground2.rho_normal = & NBTIN_Rho_normal;
    }
    Ground2.Thick = & GROUND_2_THICKNESS;

    
// Create the dielectrics
const_diel Insulation;
    if (INSULATION == "SIO")
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
    if (COVER == "SIO") // unlikely
    {
        Cover.eps = & SIO_epsilon;
        Cover.tand = & SIO_loss_tangent;
    }
    else // assume air
    {
        Cover.eps = & AIR_epsilon;
        Cover.tand = & AIR_loss_tangent;
    }

normal_film IF_Wiring;
    IF_Wiring.rho = & IF_WIRING_rho;
    IF_Wiring.Thick = & IF_WIRING_thick;
    
normal_film IF_Ground(IF_Wiring);
    
const_diel IF_Cover(Cover);
    
const_diel IF_Insulation;
    IF_Insulation.eps = & IF_INSUL_epsilon;
    IF_Insulation.tand = & IF_INSUL_loss_tangent;
