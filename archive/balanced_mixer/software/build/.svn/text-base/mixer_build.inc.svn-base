/*******************************************************************************
** mixer_build.inc - file included in balanced.c, containing code to build the**
**                   balanced mixer                                           **
** ========================================================================== **
** Author - Paul Grimes                                                       **
** Date   - 21/10/04                                                          **
** ========================================================================== **
**                                                                            **
** ========================================================================== **
** Changes:                                                                   **
*******************************************************************************/

// Define global device properties
device::T   = & TEMP;
device::Z0  = & Z0;
device::f   = & IF_FREQ; // Mixers use the IF freq as the normalising freq

// Build the circuit layers
#include "build_layers.inc"
    
// Build the RF, IF and Bias circuits
#include "build_circuits.inc"

// Create the balance terminators
//  Hot/Cold Load
zterm Load;
    Load.Z = & Z0;
    Load.Temp = & LOAD_NOISE_TEMP;

// Local Oscillator
generator Lo;
    Lo.source_f = & Frequency;
    Lo.source_power = & Lo_Power;
    Lo.Temp = & LO_NOISE_TEMP;
    Lo.R = & Z0;

// Create the SIS junction(s) IV curve
ivcurve Iv(IV_CURVE.c_str(), KK_CURVE.c_str());

sis_device Sis_1a;
sis_device Sis_1b;
sis_device Sis_2a;
sis_device Sis_2b;

// Set up the four SIS junctions
    Sis_1a.set_iv(Iv);
    Sis_1a.Vn = VGAP;
    Sis_1a.Rn = RNA/AREA_1a;
    Sis_1a.Cap = SCAP*AREA_1a;
    Sis_1b.set_iv(Iv);
    Sis_1b.Vn = VGAP;
    Sis_1b.Rn = RNA/AREA_1b;
    Sis_1b.Cap = SCAP*AREA_1b;

    Sis_2a.set_iv(Iv);
    Sis_2a.Vn = & VGAP;
    Sis_2a.Rn = RNA/AREA_2a;
    Sis_2a.Cap = SCAP*AREA_2a;
    Sis_2b.set_iv(Iv);
    Sis_2b.Vn = & VGAP;
    Sis_2b.Rn = RNA/AREA_2b;
    Sis_2b.Cap = SCAP*AREA_2b;


// finally create the mixer and its ports
mixer mix;
    mix.harmonics(HARMONICS);
    mix.set_LO(& Frequency);
    mix.set_rf(RF);
    mix.set_if(IF);
    
    mix.add_junction(Sis_1a);
    mix.add_junction(Sis_1b);
    mix.add_junction(Sis_2a);
    mix.add_junction(Sis_2b);
    mix.set_bias(Bias);
    mix.set_balance_terminator(Lo, 6);
    mix.set_balance_terminator(Load, 5);

    RF_USB_PORT  = mix.port(5, 1);
    RF_LSB_PORT  = mix.port(5,-1);
    LO_USB_PORT  = mix.port(6, 1);
    LO_LSB_PORT  = mix.port(6,-1);
    SUM_IF_PORT  = mix.port(5, 0);
    DIFF_IF_PORT = mix.port(6, 0);
