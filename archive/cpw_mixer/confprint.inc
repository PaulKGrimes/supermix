/*******************************************************************************
** confprint.inc - file included in simple.c, containing definition of        **
**                 function for outputting conf file data to simple output    **
** ========================================================================== **
** Author - Paul Grimes                                                       **
** Date   - 22/07/03                                                          **
** ========================================================================== **
** All lines output are proceeded by a # so that they are invisible to        **
** standard data handling programs                                            **
** ========================================================================== **
** Changes:                                                                   **
*******************************************************************************/


// Function to output conf file data to
int outputConf()
{
    printf("# Output from \"cpw_trafo\" SuperMix based program\n");
    printf("# Scanning %s from %f to %f %s in steps of %f %s\n#\n", SCANNED_VARIABLE.c_str(),
                SCAN_LOW/1.0, SCAN_HIGH/1.0, SCAN_unitStr.c_str(), SCAN_STEP/1.0, SCAN_unitStr.c_str());

    printf("# FREQUENCY %f %s\n", FREQUENCY/FREQUENCY_unit, FREQUENCY_unitStr.c_str());
    printf("# BIAS %f %s\n", BIAS/BIAS_unit, BIAS_unitStr.c_str());
    printf("# LO_POWER %f %s\n#\n", LO_POWER/LO_POWER_unit, LO_POWER_unitStr.c_str());

    printf("# Data from %s\n#\n", MIXER_CONFIG.c_str());

    printf("# TEMP %f %s\n", TEMP/TEMP_unit, TEMP_unitStr.c_str());
    printf("# Z0 %f %s\n", Z0/Z0_unit, Z0_unitStr.c_str());
    printf("# IF_FREQ %f %s\n", IF_FREQ/IF_FREQ_unit, IF_FREQ_unitStr.c_str());
    printf("# IF_Z0 %f %s\n", IF_Z0/IF_Z0_unit, IF_Z0_unitStr.c_str());
    printf("# HARMONICS %d\n#\n", HARMONICS);

    printf("# IV_CURVE %s\n", IV_CURVE.c_str());
    printf("# KK_CURVE %s\n", KK_CURVE.c_str());
    printf("# RNA %f %s\n", RNA/RNA_unit, RNA_unitStr.c_str());
    printf("# SCAP %f %s\n", SCAP/SCAP_unit, SCAP_unitStr.c_str());
    printf("# AREA %f %s\n", AREA/AREA_unit, AREA_unitStr.c_str());
    printf("# VGAP %f %s\n#\n", VGAP/VGAP_unit, VGAP_unitStr.c_str());

    if (TUNER_TYPE == "EXTERNAL")
    {
        printf("# TUNER_TYPE EXTERNAL\n");
        printf("# TOUCHSTONE_FILE %s\n", TOUCHSTONE_FILE.c_str());
    }

    else if (TUNER_TYPE == "TRAFO")
    {
        printf("# TUNER_TYPE %s\n", TUNER_TYPE.c_str());
        printf("# TRAFO1_TYPE %s\n", TRAFO1_TYPE.c_str());
        printf("# TRAFO1 %f %f %f\n", TRAFO1_length/TRAFO1_length_unit,
            TRAFO1_width/TRAFO1_width_unit, TRAFO1_space/TRAFO1_space_unit);
        printf("# TRAFO2_TYPE %s\n", TRAFO2_TYPE.c_str());
        printf("# TRAFO2 %f %f %f\n", TRAFO2_length/TRAFO2_length_unit,
            TRAFO2_width/TRAFO2_width_unit, TRAFO2_space/TRAFO2_space_unit);
        printf("# TRAFO3_TYPE %s\n", TRAFO3_TYPE.c_str());
        printf("# TRAFO3 %f %f %f\n", TRAFO3_length/TRAFO3_length_unit,
            TRAFO3_width/TRAFO3_width_unit, TRAFO3_space/TRAFO3_space_unit);
        printf("# STUBIN_TYPE %s\n", STUBIN_TYPE.c_str());
        printf("# STUBIN %f %f %f\n", STUBIN_length/STUBIN_length_unit,
            STUBIN_width/STUBIN_width_unit, STUBIN_space/STUBIN_space_unit);
        printf("# RADIAL_STUB_INPUT %f %f\n", RADIAL_STUB_INPUT_length/RADIAL_STUB_INPUT_length_unit,
            RADIAL_STUB_INPUT_width/RADIAL_STUB_INPUT_width_unit);
        printf("# RADIAL_STUB_FAN %f %f\n", RADIAL_STUB_FAN_radius/RADIAL_STUB_FAN_radius_unit,
            RADIAL_STUB_FAN_angle/Degree);
    }


    printf("#\n# TOP_DEFAULT %s\n", TOP_DEFAULT.c_str());
    printf("# TOP_THICKNESS %f\n", TOP_THICKNESS/TOP_THICKNESS_unit);
    printf("# GROUND_DEFAULT %s\n", GROUND_DEFAULT.c_str());
    printf("# GROUND_THICKNESS %f\n", GROUND_THICKNESS/GROUND_THICKNESS_unit);
    printf("# INSUL_DEFAULT %s\n", INSUL_DEFAULT.c_str());
    printf("# INSUL_THICKNESS %f\n", INSUL_THICKNESS/INSUL_THICKNESS_unit);
    printf("# SUBSTRATE_DEFAULT %s\n", SUBSTRATE_DEFAULT.c_str());
    printf("# SUBSTRATE_THICKNESS %f\n", SUBSTRATE_THICKNESS/SUBSTRATE_THICKNESS_unit);
    printf("# COVER_DEFAULT %s\n", COVER_DEFAULT.c_str());
    printf("# COVER_THICKNESS %f\n#\n", COVER_THICKNESS/COVER_THICKNESS_unit);

    printf("# NB_PROPERTIES %f %f %f\n",
            NB_Vgap/mVolt, NB_Tc/Kelvin, NB_Rho_normal/(Micro*Ohm*Centi*Meter));
    printf("# NBTIN_PROPERTIES %f %f %f\n",
            NBTIN_Vgap/mVolt, NBTIN_Tc/Kelvin, NBTIN_Rho_normal/(Micro*Ohm*Centi*Meter));
    printf("# SIO_PROPERTIES %f %f\n",
            SIO_epsilon/1.0, SIO_loss_tangent/1.0);
    printf("# SI_PROPERTIES %f %f\n",
            SI_epsilon/1.0, SI_loss_tangent/1.0);
    printf("# AIR_PROPERTIES %f %f\n",
            AIR_epsilon/1.0, AIR_loss_tangent/1.0);
    printf("#\n# End of configuration file data\n# \n");

    return 0;
}
