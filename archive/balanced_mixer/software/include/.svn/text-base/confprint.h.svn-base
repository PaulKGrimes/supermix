/*******************************************************************************
** confprint.h -   file included in balanced.c, containing definition of      **
**                 function for outputting conf file data to simple output    **
** ========================================================================== **
** Author - Paul Grimes                                                       **
** Date   - 15/10/04 - created from SuperMix-realistic version                **
** ========================================================================== **
** All lines output are proceeded by a # so that they are invisible to        **
** standard data handling programs                                            **
** ========================================================================== **
** Changes:                                                                   **
**	23/10/04 : PKG : Altered to fit new balanced config file	      **
*******************************************************************************/


// Global function to output conf file data to output.
void outputConf()
{
    printf("# Output from \"balanced\" SuperMix based program\n");
    printf("# Scanning %s from %f to %f in steps of %f\n#\n", SCANNED_VARIABLE.c_str(),
                SCAN_LOW/1.0, SCAN_HIGH/1.0, SCAN_STEP/1.0) ;
    
    printf("# FREQUENCY %f GHz\n", FREQUENCY/GHz);    
    printf("# BIAS_1 %f mV\n", BIAS_1/mVolt);
    printf("# BIAS_2 %f mV\n", BIAS_2/mVolt);
    printf("# LO_POWER %f NW\n", LO_POWER/(Nano*Watt));
    printf("# LO_TEMP %f K\n#\n", LO_NOISE_TEMP/Kelvin);
    printf("# IF_FREQ %f GHz\n", IF_FREQ/GHz);
    
    printf("# Data from %s\n#\n", MIXER_CONFIG.c_str());

    printf("# TEMP %f K\n", TEMP/Kelvin);
    printf("# Z0 %f Ohm\n", Z0/Ohm);
    printf("# HARMONICS %d\n#\n", HARMONICS);
    
    printf("# # Junction parameters\n");
    printf("# IV_CURVE %s\n", IV_CURVE.c_str());
    printf("# KK_CURVE %s\n", KK_CURVE.c_str());
    printf("# RNA %f Ohm um^2\n", RNA/(Ohm*Micron*Micron));
    printf("# SCAP %f fF/um^2\n", SCAP/(fFarad/(Micron*Micron)));
    printf("# AREA_1a %f um^2\n", AREA_1a/(Micron*Micron));
    printf("# AREA_1b %f um^2\n", AREA_1b/(Micron*Micron));
    printf("# AREA_2a %f um^2\n", AREA_2a/(Micron*Micron));
    printf("# AREA_2b %f um^2\n", AREA_2b/(Micron*Micron));
    printf("# VGAP %f mV\n#\n", VGAP/mVolt);
    
    printf("# # RF circuit parameters\n");
    printf("# FINLINE_WIDTH %f um\n", FINLINE_WIDTH/Micron);
    printf("# FINLINE_LENGTH %f um\n", FINLINE_LENGTH/Micron);
    printf("# RF_HYBRID %s\n", RF_HYBRID.c_str());
    printf("# RF_HYBRID_FILENAME %s\n", EXTERNAL_RF_HYBRID_FILENAME.c_str());
    printf("# CONNECT1_WIDTH %f um\n", CONNECT_STRIP1_WIDTH/Micron);
    printf("# CONNECT1_LENGTH %f um\n", CONNECT_STRIP1_LENGTH/Micron);
    printf("# DC_BLOCK_CAP %f fF\n", DC_BLOCK_CAP/fFarad);
    printf("# CONNECT2_WIDTH %f um\n", CONNECT_STRIP2_WIDTH/Micron);
    printf("# CONNECT2_LENGTH %f um\n", CONNECT_STRIP2_LENGTH/Micron);
    printf("# INPUT_WIDTH %f um\n", INPUT_STRIP_WIDTH/Micron);
    printf("# INPUT_LENGTH %f um\n", INPUT_STRIP_LENGTH/Micron);
    printf("# TUNING_WIDTH %f um\n", TUNING_STRIP_WIDTH/Micron);
    printf("# TUNING_LENGTH %f um\n", TUNING_STRIP_LENGTH/Micron);
    printf("# TUNING_THICKNESS %f um\n", TUNING_STRIP_THICKNESS/Micron);
    printf("# RADIAL_CHOKE_WIDTH %f um\n", RF_CHOKE_WIDTH/Micron);
    printf("# RADIAL_CHOKE_LENGTH %f um\n", RF_CHOKE_LENGTH/Micron);
    printf("# RADIAL_CHOKE_RADIUS %f um\n", RF_CHOKE_RADIUS/Micron);
    printf("# RADIAL_CHOKE_ANGLE %f deg\n", RF_CHOKE_ANGLE/Degree);
    
    printf("# # IF circuit parameters\n");
    printf("# IF_OUTPUT_WIDTH %f um\n", IF_OUTPUT_WIDTH/Micron);
    printf("# IF_OUTPUT_LENGTH %f um\n", IF_OUTPUT_LENGTH/Micron);
    printf("# IF_CONNECTION_WIDTH %f um\n", IF_CONNECTION_WIDTH/Micron);
    printf("# IF_CONNECTION_LENGTH_1 %f um\n", IF_CONNECTION_LENGTH_1/Micron);
    printf("# IF_CONNECTION_LENGTH_2 %f um\n", IF_CONNECTION_LENGTH_2/Micron);
    printf("# IF_HYBRID %s\n", IF_HYBRID.c_str());
    printf("# IF_HYBRID_FILENAME %s\n", EXTERNAL_IF_HYBRID_FILENAME.c_str());
    printf("# IF_HYBRID_OUT_Z0 %f Ohm\n", IF_HYBRID_OUT_Z0/Ohm);
    printf("# IF_AMP_Z0 %f Ohm\n", IF_AMP_Z0/Ohm);
    
    printf("# # Bias circuit parameters\n");
    printf("# BIAS_R %f Ohm\n", BIAS_R/Ohm);

    printf("# # Layer parameters\n");
    printf("# WIRING_1 %s\n", WIRING_1.c_str());
    printf("# WIRING_1_THICKNESS %f\n", WIRING_1_THICKNESS/Micron);
    printf("# GROUND_1 %s\n", GROUND_1.c_str());
    printf("# GROUND_1_THICKNESS %f\n", GROUND_1_THICKNESS/Micron);
    printf("# WIRING_2 %s\n", WIRING_2.c_str());
    printf("# WIRING1_THICKNESS %f\n", WIRING_2_THICKNESS/Micron);
    printf("# GROUND1 %s\n", GROUND_2.c_str());
    printf("# GROUND1_THICKNESS %f\n", GROUND_2_THICKNESS/Micron);
    printf("# INSULATION %s\n", INSULATION.c_str());
    printf("# INSUL_THICKNESS %f\n", INSUL_THICKNESS/Micron);
    printf("# COVER %s\n", COVER.c_str());
    printf("# COVER_THICKNESS %f\n#\n", COVER_THICKNESS/Micron);
    
    printf("# # Material properties\n");
    printf("# NB_VGAP %f mV\n", NB_Vgap/mVolt);
    printf("# NB_TC %f K\n", NB_Tc/Kelvin);
    printf("# NB_RHO_NORMAL %f uOhm cm\n", NB_Rho_normal/(Micro*Ohm*Centi*Meter));
    printf("# NBTIN_VGAP %f mV\n", NBTIN_Vgap/mVolt);
    printf("# NBTIN_TC %f K\n", NBTIN_Tc/Kelvin);
    printf("# NBTIN_RHO_NORMAL %f uOhm cm\n", NBTIN_Rho_normal/(Micro*Ohm*Centi*Meter));
    
    printf("# SIO_EPSILON %f \n", SIO_epsilon/1.0);
    printf("# SIO_TAND %f \n", SIO_loss_tangent/1.0);
    
    printf("# # IF board parameters\n");
    printf("# IF_WIRING_RHO %f uOhm cm\n", IF_WIRING_rho/(Micro*Ohm*Centi*Meter));
    printf("# IF_WIRING_THICKNESS %f um\n", IF_WIRING_thick/Micron);
    printf("# IF_INSUL_EPSILON %f \n", IF_INSUL_epsilon/1.0);
    printf("# IF_INSUL_TAND %f\n", IF_INSUL_loss_tangent/1.0);
    printf("# IF_INSUL_THICKNESS %f um\n", IF_INSUL_thickness/Micron);
    printf("#\n# End of configuration file data\n# \n");                            
}
