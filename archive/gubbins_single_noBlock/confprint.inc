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
    printf("# Output from \"simple\" SuperMix based program\n");
    printf("# Scanning %s from %f to %f %s in steps of %f %s\n#\n", SCANNED_VARIABLE.c_str(),
                SCAN_LOW/1.0, SCAN_HIGH/1.0, SCAN_unitStr.c_str(), SCAN_STEP/1.0, SCAN_unitStr.c_str());
    
    printf("# FREQUENCY %f %s\n", FREQUENCY/FREQUENCY_unit, FREQUENCY_unitStr.c_str());    
    printf("# BIAS %f %s\n", BIAS/BIAS_unit, BIAS_unitStr.c_str());
    printf("# LO_POWER %f %s\n#\n", LO_POWER/LO_POWER_unit, LO_POWER_unitStr.c_str());
    printf("# IF_FREQ %f %s\n", IF_FREQ/IF_FREQ_unit, IF_FREQ_unitStr.c_str());
    
    printf("# Data from %s\n#\n", MIXER_CONFIG.c_str());
    
    printf("# TEMP %f %s\n", TEMP/TEMP_unit, TEMP_unitStr.c_str());
    printf("# Z0 %f %s\n", Z0/Z0_unit, Z0_unitStr.c_str());
    printf("# IF_Z0 %f %s\n", IF_Z0/IF_Z0_unit, IF_Z0_unitStr.c_str());   
    printf("# HARMONICS %d\n#\n", HARMONICS);
    
    printf("# IV_CURVE %s\n", IV_CURVE.c_str());
    printf("# KK_CURVE %s\n", KK_CURVE.c_str());
    printf("# RNA %f %s\n", RNA/RNA_unit, RNA_unitStr.c_str());
    printf("# SCAP %f %s\n", SCAP/SCAP_unit, SCAP_unitStr.c_str());
    printf("# AREA %f %s\n", AREA/AREA_unit, AREA_unitStr.c_str());
    printf("# VGAP %f %s\n#\n", VGAP/VGAP_unit, VGAP_unitStr.c_str());
    
	 printf("# FINLINE_S2P %s\n", FINLINE_S2P.c_str());
    printf("# TUNER_INPUT %f %f\n", TUNER_INPUT_length/TUNER_INPUT_unit, TUNER_INPUT_width/TUNER_INPUT_unit);
    printf("# IF_OUT_STRIP %f %f\n", IF_OUT_STRIP_length/IF_OUT_STRIP_unit, IF_OUT_STRIP_width/IF_OUT_STRIP_unit);
    printf("# IF_CONNECTION %s\n", IF_CONNECTION.c_str());
    
    if (TUNER_TYPE == "EXTERNAL")
    {
        printf("# TUNER_TYPE EXTERNAL\n");
        printf("# TOUCHSTONE_FILE %s\n", TOUCHSTONE_FILE.c_str());
    } 
    else if (TUNER_TYPE == "EXT_WITH_RFCHOKE")
    {
        printf("# TUNER_TYPE EXT_WITH_RFCHOKE\n");
        printf("# TOUCHSTONE_FILE %s\n", TOUCHSTONE_FILE.c_str());
        printf("# RF_CHOKE_TYPE %s\n", RF_CHOKE_TYPE.c_str());
        if (RF_CHOKE_TYPE == "RADIAL")
        {
            printf("# RADIAL_CHOKE_TYPE %s\n", RADIAL_CHOKE_TYPE.c_str());
            printf("# RADIAL_CHOKE_INPUT %f %f %f\n", 
                RADIAL_CHOKE_INPUT_length/RADIAL_CHOKE_INPUT_length_unit,
                RADIAL_CHOKE_INPUT_width1/RADIAL_CHOKE_INPUT_width1_unit,
                RADIAL_CHOKE_INPUT_width2/RADIAL_CHOKE_INPUT_width2_unit);
            printf("# RADIAL_CHOKE_OUTPUT %f %f\n", 
                RADIAL_CHOKE_OUTPUT_length/RADIAL_CHOKE_OUTPUT_length_unit,
                RADIAL_CHOKE_OUTPUT_width/RADIAL_CHOKE_OUTPUT_width_unit);
            printf("# RADIAL_CHOKE_FAN %f %f\n", 
                RADIAL_CHOKE_FAN_radius/RADIAL_CHOKE_FAN_radius_unit,
                RADIAL_CHOKE_FAN_angle/RADIAL_CHOKE_FAN_angle_unit);
        }
        else if (RF_CHOKE_TYPE == "FILTER")
        {
            printf("# FILTER_CHOKE_SECTIONS %d\n", FILTER_CHOKE_SECTIONS);
            printf("# FILTER_CHOKE_INPUT %f %f\n", FILTER_CHOKE_INPUT_length/FILTER_CHOKE_INPUT_length_unit, FILTER_CHOKE_INPUT_width/FILTER_CHOKE_INPUT_width_unit);
            printf("# FILTER_CHOKE_LINE1 %f %f\n", FILTER_CHOKE_LINE1_length/FILTER_CHOKE_LINE1_length_unit, FILTER_CHOKE_LINE1_width/FILTER_CHOKE_LINE1_width_unit);
            printf("# FILTER_CHOKE_LINE2 %f %f\n", FILTER_CHOKE_LINE2_length/FILTER_CHOKE_LINE2_length_unit, FILTER_CHOKE_LINE2_width/FILTER_CHOKE_LINE2_width_unit);
            printf("# FILTER_CHOKE_LINE3 %f %f\n", FILTER_CHOKE_LINE3_length/FILTER_CHOKE_LINE3_length_unit, FILTER_CHOKE_LINE3_width/FILTER_CHOKE_LINE3_width_unit);
            printf("# FILTER_CHOKE_LINE4 %f %f\n", FILTER_CHOKE_LINE4_length/FILTER_CHOKE_LINE4_length_unit, FILTER_CHOKE_LINE4_width/FILTER_CHOKE_LINE4_width_unit);
            printf("# FILTER_CHOKE_LINE5 %f %f\n", FILTER_CHOKE_LINE5_length/FILTER_CHOKE_LINE5_length_unit, FILTER_CHOKE_LINE5_width/FILTER_CHOKE_LINE5_width_unit);
            printf("# FILTER_CHOKE_LINE6 %f %f\n", FILTER_CHOKE_LINE6_length/FILTER_CHOKE_LINE6_length_unit, FILTER_CHOKE_LINE6_width/FILTER_CHOKE_LINE6_width_unit);
            printf("# FILTER_CHOKE_OUTPUT %f %f\n", FILTER_CHOKE_OUTPUT_length/FILTER_CHOKE_OUTPUT_length_unit, FILTER_CHOKE_OUTPUT_width/FILTER_CHOKE_OUTPUT_width_unit);
        }
    }
    else if (TUNER_TYPE == "RFCHOKE")
    {
        printf("# TUNER_TYPE RFCHOKE\n");
        printf("# RF_CHOKE_TYPE %s\n", RF_CHOKE_TYPE.c_str());
        if (RF_CHOKE_TYPE == "RADIAL")
        {
            printf("# RADIAL_CHOKE_TYPE %s\n", RADIAL_CHOKE_TYPE.c_str());
            printf("# RADIAL_CHOKE_INPUT %f %f %f\n", 
                RADIAL_CHOKE_INPUT_length/RADIAL_CHOKE_INPUT_length_unit,
                RADIAL_CHOKE_INPUT_width1/RADIAL_CHOKE_INPUT_width1_unit,
                RADIAL_CHOKE_INPUT_width2/RADIAL_CHOKE_INPUT_width2_unit);
            printf("# RADIAL_CHOKE_OUTPUT %f %f\n", 
                RADIAL_CHOKE_OUTPUT_length/RADIAL_CHOKE_OUTPUT_length_unit,
                RADIAL_CHOKE_OUTPUT_width/RADIAL_CHOKE_OUTPUT_width_unit);
            printf("# RADIAL_CHOKE_FAN %f %f\n", 
                RADIAL_CHOKE_FAN_radius/RADIAL_CHOKE_FAN_radius_unit,
                RADIAL_CHOKE_FAN_angle/RADIAL_CHOKE_FAN_angle_unit);
        }
        else if (RF_CHOKE_TYPE == "FILTER")
        {
            printf("# FILTER_CHOKE_SECTIONS %d\n", FILTER_CHOKE_SECTIONS);
            printf("# FILTER_CHOKE_INPUT %f %f\n", FILTER_CHOKE_INPUT_length/FILTER_CHOKE_INPUT_length_unit, FILTER_CHOKE_INPUT_width/FILTER_CHOKE_INPUT_width_unit);
            printf("# FILTER_CHOKE_LINE1 %f %f\n", FILTER_CHOKE_LINE1_length/FILTER_CHOKE_LINE1_length_unit, FILTER_CHOKE_LINE1_width/FILTER_CHOKE_LINE1_width_unit);
            printf("# FILTER_CHOKE_LINE2 %f %f\n", FILTER_CHOKE_LINE2_length/FILTER_CHOKE_LINE2_length_unit, FILTER_CHOKE_LINE2_width/FILTER_CHOKE_LINE2_width_unit);
            printf("# FILTER_CHOKE_LINE3 %f %f\n", FILTER_CHOKE_LINE3_length/FILTER_CHOKE_LINE3_length_unit, FILTER_CHOKE_LINE3_width/FILTER_CHOKE_LINE3_width_unit);
            printf("# FILTER_CHOKE_LINE4 %f %f\n", FILTER_CHOKE_LINE4_length/FILTER_CHOKE_LINE4_length_unit, FILTER_CHOKE_LINE4_width/FILTER_CHOKE_LINE4_width_unit);
            printf("# FILTER_CHOKE_LINE5 %f %f\n", FILTER_CHOKE_LINE5_length/FILTER_CHOKE_LINE5_length_unit, FILTER_CHOKE_LINE5_width/FILTER_CHOKE_LINE5_width_unit);
            printf("# FILTER_CHOKE_LINE6 %f %f\n", FILTER_CHOKE_LINE6_length/FILTER_CHOKE_LINE6_length_unit, FILTER_CHOKE_LINE6_width/FILTER_CHOKE_LINE6_width_unit);
            printf("# FILTER_CHOKE_OUTPUT %f %f\n", FILTER_CHOKE_OUTPUT_length/FILTER_CHOKE_OUTPUT_length_unit, FILTER_CHOKE_OUTPUT_width/FILTER_CHOKE_OUTPUT_width_unit);
        }
    }
    else if (TUNER_TYPE == "SINGLE_STUB")
    {
        printf("# TUNER_TYPE SINGLE_STUB\n");
        printf("# SINGLE_STUB_TYPE %s\n", SINGLE_STUB_TYPE.c_str());
        printf("# SINGLE_STUB_INPUT %f %f %f\n",
           SINGLE_STUB_INPUT_length/SINGLE_STUB_INPUT_length_unit,
           SINGLE_STUB_INPUT_width1/SINGLE_STUB_INPUT_width1_unit,
           SINGLE_STUB_INPUT_width2/SINGLE_STUB_INPUT_width2_unit);
        printf("# SINGLE_STUB_FAN %f %f\n", 
            SINGLE_STUB_FAN_radius/SINGLE_STUB_FAN_radius_unit,
            SINGLE_STUB_FAN_angle/SINGLE_STUB_FAN_angle_unit);
    }
    else if (TUNER_TYPE == "DOUBLE_STUB")
    {
        printf("# TUNER_TYPE DOUBLE_STUB\n");
        printf("# DOUBLE_STUB_1_TYPE %s\n", DOUBLE_STUB_1_TYPE.c_str());
        printf("# DOUBLE_STUB_1_INPUT %f %f %f\n",
           DOUBLE_STUB_1_INPUT_length/DOUBLE_STUB_1_INPUT_length_unit,
           DOUBLE_STUB_1_INPUT_width1/DOUBLE_STUB_1_INPUT_width1_unit,
           DOUBLE_STUB_1_INPUT_width2/DOUBLE_STUB_1_INPUT_width2_unit);
        printf("# DOUBLE_STUB_1_FAN %f %f\n", 
            DOUBLE_STUB_1_FAN_radius/DOUBLE_STUB_1_FAN_radius_unit,
            DOUBLE_STUB_1_FAN_angle/DOUBLE_STUB_1_FAN_angle_unit);
        printf("# DOUBLE_STUB_2_TYPE %s\n", DOUBLE_STUB_2_TYPE.c_str());
        printf("# DOUBLE_STUB_2_INPUT %f %f %f\n",
           DOUBLE_STUB_2_INPUT_length/DOUBLE_STUB_2_INPUT_length_unit,
           DOUBLE_STUB_2_INPUT_width1/DOUBLE_STUB_2_INPUT_width1_unit,
           DOUBLE_STUB_2_INPUT_width2/DOUBLE_STUB_2_INPUT_width2_unit);
        printf("# DOUBLE_STUB_2_FAN %f %f\n", 
            DOUBLE_STUB_2_FAN_radius/DOUBLE_STUB_2_FAN_radius_unit,
            DOUBLE_STUB_2_FAN_angle/DOUBLE_STUB_2_FAN_angle_unit);
    }  
    else if (TUNER_TYPE == "TRIPLE_STUB")
    {
        printf("# TUNER_TYPE TRIPLE_STUB\n");
        printf("# TRIPLE_STUB_1_TYPE %s\n", TRIPLE_STUB_1_TYPE.c_str());
        printf("# TRIPLE_STUB_1_INPUT %f %f %f\n",
           TRIPLE_STUB_1_INPUT_length/TRIPLE_STUB_1_INPUT_length_unit,
           TRIPLE_STUB_1_INPUT_width1/TRIPLE_STUB_1_INPUT_width1_unit,
           TRIPLE_STUB_1_INPUT_width2/TRIPLE_STUB_1_INPUT_width2_unit);
        printf("# TRIPLE_STUB_1_FAN %f %f\n", 
            TRIPLE_STUB_1_FAN_radius/TRIPLE_STUB_1_FAN_radius_unit,
            TRIPLE_STUB_1_FAN_angle/TRIPLE_STUB_1_FAN_angle_unit);
        printf("# TRIPLE_STUB_2_TYPE %s\n", TRIPLE_STUB_2_TYPE.c_str());
        printf("# TRIPLE_STUB_2_INPUT %f %f %f\n",
           TRIPLE_STUB_2_INPUT_length/TRIPLE_STUB_2_INPUT_length_unit,
           TRIPLE_STUB_2_INPUT_width1/TRIPLE_STUB_2_INPUT_width1_unit,
           TRIPLE_STUB_2_INPUT_width2/TRIPLE_STUB_2_INPUT_width2_unit);
        printf("# TRIPLE_STUB_2_FAN %f %f\n", 
            TRIPLE_STUB_2_FAN_radius/TRIPLE_STUB_2_FAN_radius_unit,
            TRIPLE_STUB_2_FAN_angle/TRIPLE_STUB_2_FAN_angle_unit);
        printf("# TRIPLE_STUB_3_TYPE %s\n", TRIPLE_STUB_3_TYPE.c_str());
        printf("# TRIPLE_STUB_3_INPUT %f %f %f\n",
           TRIPLE_STUB_3_INPUT_length/TRIPLE_STUB_3_INPUT_length_unit,
           TRIPLE_STUB_3_INPUT_width1/TRIPLE_STUB_3_INPUT_width1_unit,
           TRIPLE_STUB_3_INPUT_width2/TRIPLE_STUB_3_INPUT_width2_unit);
        printf("# TRIPLE_STUB_3_FAN %f %f\n", 
            TRIPLE_STUB_3_FAN_radius/TRIPLE_STUB_3_FAN_radius_unit,
            TRIPLE_STUB_3_FAN_angle/TRIPLE_STUB_3_FAN_angle_unit);
    }
    else if (TUNER_TYPE == "BELITSKY")
    {
        printf("# TUNER_TYPE BELITSKY\n");
        printf("# BELITSKY_INPUT_STRIP %f %f %f\n",
           BELITSKY_INPUT_STRIP_length/BELITSKY_INPUT_STRIP_length_unit,
           BELITSKY_INPUT_STRIP_width/BELITSKY_INPUT_STRIP_width_unit,
           BELITSKY_INPUT_STRIP_thick/BELITSKY_INPUT_STRIP_thick_unit);
        printf("# BELITSKY_TUNING_STRIP %f %f %f\n",
           BELITSKY_TUNING_STRIP_length/BELITSKY_TUNING_STRIP_length_unit,
           BELITSKY_TUNING_STRIP_width/BELITSKY_TUNING_STRIP_width_unit,
           BELITSKY_TUNING_STRIP_thick/BELITSKY_TUNING_STRIP_thick_unit);
    }
    else //if (TUNER_TYPE == "BEL_WITH_RFCHOKE")
    {
        printf("# TUNER_TYPE BEL_WITH_RFCHOKE\n");
        printf("# BELITSKY_INPUT_STRIP %f %f %f\n",
           BELITSKY_INPUT_STRIP_length/BELITSKY_INPUT_STRIP_length_unit,
           BELITSKY_INPUT_STRIP_width/BELITSKY_INPUT_STRIP_width_unit,
           BELITSKY_INPUT_STRIP_thick/BELITSKY_INPUT_STRIP_thick_unit);
        printf("# BELITSKY_TUNING_STRIP %f %f %f\n",
           BELITSKY_TUNING_STRIP_length/BELITSKY_TUNING_STRIP_length_unit,
           BELITSKY_TUNING_STRIP_width/BELITSKY_TUNING_STRIP_width_unit,
           BELITSKY_TUNING_STRIP_thick/BELITSKY_TUNING_STRIP_thick_unit);
        printf("# RF_CHOKE_TYPE %s\n", RF_CHOKE_TYPE.c_str());
        if (RF_CHOKE_TYPE == "RADIAL")
        {
            printf("# RADIAL_CHOKE_TYPE %s\n", RADIAL_CHOKE_TYPE.c_str());
            printf("# RADIAL_CHOKE_INPUT %f %f %f\n", 
                RADIAL_CHOKE_INPUT_length/RADIAL_CHOKE_INPUT_length_unit,
                RADIAL_CHOKE_INPUT_width1/RADIAL_CHOKE_INPUT_width1_unit,
                RADIAL_CHOKE_INPUT_width2/RADIAL_CHOKE_INPUT_width2_unit);
            printf("# RADIAL_CHOKE_FAN %f %f\n", 
                RADIAL_CHOKE_FAN_radius/RADIAL_CHOKE_FAN_radius_unit,
                RADIAL_CHOKE_FAN_angle/RADIAL_CHOKE_FAN_angle_unit);
        }
        else if (RF_CHOKE_TYPE == "FILTER")
        {
            printf("# FILTER_CHOKE_SECTIONS %d\n", FILTER_CHOKE_SECTIONS);
            printf("# FILTER_CHOKE_INPUT %f %f\n", FILTER_CHOKE_INPUT_length/FILTER_CHOKE_INPUT_length_unit, FILTER_CHOKE_INPUT_width/FILTER_CHOKE_INPUT_width_unit);
            printf("# FILTER_CHOKE_LINE1 %f %f\n", FILTER_CHOKE_LINE1_length/FILTER_CHOKE_LINE1_length_unit, FILTER_CHOKE_LINE1_width/FILTER_CHOKE_LINE1_width_unit);
            printf("# FILTER_CHOKE_LINE2 %f %f\n", FILTER_CHOKE_LINE2_length/FILTER_CHOKE_LINE2_length_unit, FILTER_CHOKE_LINE2_width/FILTER_CHOKE_LINE2_width_unit);
            printf("# FILTER_CHOKE_LINE3 %f %f\n", FILTER_CHOKE_LINE3_length/FILTER_CHOKE_LINE3_length_unit, FILTER_CHOKE_LINE3_width/FILTER_CHOKE_LINE3_width_unit);
            printf("# FILTER_CHOKE_LINE4 %f %f\n", FILTER_CHOKE_LINE4_length/FILTER_CHOKE_LINE4_length_unit, FILTER_CHOKE_LINE4_width/FILTER_CHOKE_LINE4_width_unit);
            printf("# FILTER_CHOKE_LINE5 %f %f\n", FILTER_CHOKE_LINE5_length/FILTER_CHOKE_LINE5_length_unit, FILTER_CHOKE_LINE5_width/FILTER_CHOKE_LINE5_width_unit);
            printf("# FILTER_CHOKE_LINE6 %f %f\n", FILTER_CHOKE_LINE6_length/FILTER_CHOKE_LINE6_length_unit, FILTER_CHOKE_LINE6_width/FILTER_CHOKE_LINE6_width_unit);
            printf("# FILTER_CHOKE_OUTPUT %f %f\n", FILTER_CHOKE_OUTPUT_length/FILTER_CHOKE_OUTPUT_length_unit, FILTER_CHOKE_OUTPUT_width/FILTER_CHOKE_OUTPUT_width_unit);
        }
    }   

    
    printf("#\n# TOP_DEFAULT %s\n", TOP_DEFAULT.c_str());
    printf("# TOP_THICKNESS %f\n", TOP_THICKNESS/TOP_THICKNESS_unit);
    printf("# GROUND_DEFAULT %s\n", GROUND_DEFAULT.c_str());
    printf("# GROUND_THICKNESS %f\n", GROUND_THICKNESS/GROUND_THICKNESS_unit);
    printf("# INSUL_DEFAULT %s\n", INSUL_DEFAULT.c_str());
    printf("# INSUL_THICKNESS %f\n", INSUL_THICKNESS/INSUL_THICKNESS_unit);
    printf("# COVER_DEFAULT %s\n", COVER_DEFAULT.c_str());
    printf("# COVER_THICKNESS %f\n#\n", COVER_THICKNESS/COVER_THICKNESS_unit);
    
    printf("# NB_PROPERTIES %f %f %f\n", 
            NB_Vgap/mVolt, NB_Tc/Kelvin, NB_Rho_normal/(Micro*Ohm*Centi*Meter));
    printf("# NBTIN_PROPERTIES %f %f %f\n", 
            NBTIN_Vgap/mVolt, NBTIN_Tc/Kelvin, NBTIN_Rho_normal/(Micro*Ohm*Centi*Meter));
    printf("# SIO_PROPERTIES %f %f\n", 
            SIO_epsilon/1.0, SIO_loss_tangent/1.0);
    printf("# AIR_PROPERTIES %f %f\n", 
            AIR_epsilon/1.0, AIR_loss_tangent/1.0);
    printf("#\n# End of configuration file data\n# \n");

    return 0;
}
