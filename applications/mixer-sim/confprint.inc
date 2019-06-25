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

    printf("# Data from %s\n#\n", MIXER_CONFIG.c_str());

    printf("# TEMP %f %s\n", TEMP/TEMP_unit, TEMP_unitStr.c_str());
    printf("# Z0 %f %s\n", Z0/Z0_unit, Z0_unitStr.c_str());
    printf("# SOURCE_Z0 %f %fi %s\n", real(SOURCE_Z0)/SOURCE_Z0_unit, imag(SOURCE_Z0)/Z0_unit, SOURCE_Z0_unitStr.c_str());
    printf("# IF_FREQ %f %s\n", IF_FREQ/IF_FREQ_unit, IF_FREQ_unitStr.c_str());
    printf("# IF_Z0 %f %s\n", IF_Z0/IF_Z0_unit, IF_Z0_unitStr.c_str());
    printf("# HARMONICS %d\n#\n", HARMONICS);

    printf("# IV_CURVE %s\n", IV_CURVE.c_str());
    printf("# KK_CURVE %s\n", KK_CURVE.c_str());
    printf("# JUNCTION_RNA %f %s\n", JUNCTION_RNA/JUNCTION_RNA_unit, JUNCTION_RNA_unitStr.c_str());
    printf("# JUNCTION_SCAP %f %s\n", JUNCTION_SCAP/JUNCTION_SCAP_unit, JUNCTION_SCAP_unitStr.c_str());
    printf("# JUNCTION_AREA %f %s\n", JUNCTION_AREA/JUNCTION_AREA_unit, JUNCTION_AREA_unitStr.c_str());
    printf("# JUNCTION_VGAP %f %s\n#\n", JUNCTION_VGAP/JUNCTION_VGAP_unit, JUNCTION_VGAP_unitStr.c_str());

    if (TUNER_TYPE == "EXTERNAL")
    {
        printf("# TUNER_TYPE EXTERNAL\n");
        printf("# RF_TOUCHSTONE_FILE %s\n", RF_TOUCHSTONE_FILE.c_str());
        printf("# IF_TOUCHSTONE_FILE %s\n", IF_TOUCHSTONE_FILE.c_str());
    }
    else if (TUNER_TYPE == "EXT_WITH_RFCHOKE")
    {
        printf("# TUNER_TYPE EXT_WITH_RFCHOKE\n");
        printf("# RF_TOUCHSTONE_FILE %s\n", RF_TOUCHSTONE_FILE.c_str());
        printf("# IF_TOUCHSTONE_FILE %s\n", IF_TOUCHSTONE_FILE.c_str());
        printf("# RF_CHOKE_TYPE %s\n", RF_CHOKE_TYPE.c_str());
        if (RF_CHOKE_TYPE == "RADIAL")
        {
            printf("# RADIAL_CHOKE_TYPE %s\n", RADIAL_CHOKE_TYPE.c_str());
            printf("# RADIAL_CHOKE_INPUT %f %f %f\n",
                RADIAL_CHOKE_INPUT_length/RADIAL_CHOKE_INPUT_length_unit,
                RADIAL_CHOKE_INPUT_width/RADIAL_CHOKE_INPUT_width_unit,
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
                RADIAL_CHOKE_INPUT_width/RADIAL_CHOKE_INPUT_width_unit,
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
            printf("# FILTER_CHOKE_OUTPUT %f %f\n", FILTER_CHOKE_OUTPUT_length/FILTER_CHOKE_OUTPUT_length_unit, FILTER_CHOKE_OUTPUT_width/FILTER_CHOKE_OUTPUT_width_unit);
        }
    }
    else if (TUNER_TYPE == "SINGLE_STUB" || TUNER_TYPE == "PARALLEL_STUB")
    {
        printf("# TUNER_TYPE %s\n", TUNER_TYPE.c_str());
        printf("# SINGLE_STUB_TYPE %s\n", SINGLE_STUB_TYPE.c_str());
        printf("# SINGLE_STUB_INPUT %f %f %f\n",
           SINGLE_STUB_INPUT_length/SINGLE_STUB_INPUT_length_unit,
           SINGLE_STUB_INPUT_width/SINGLE_STUB_INPUT_width_unit,
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
           DOUBLE_STUB_1_INPUT_width/DOUBLE_STUB_1_INPUT_width_unit,
           DOUBLE_STUB_1_INPUT_width2/DOUBLE_STUB_1_INPUT_width2_unit);
        printf("# DOUBLE_STUB_1_FAN %f %f\n",
            DOUBLE_STUB_1_FAN_radius/DOUBLE_STUB_1_FAN_radius_unit,
            DOUBLE_STUB_1_FAN_angle/DOUBLE_STUB_1_FAN_angle_unit);
        printf("# DOUBLE_STUB_2_TYPE %s\n", DOUBLE_STUB_2_TYPE.c_str());
        printf("# DOUBLE_STUB_2_INPUT %f %f %f\n",
           DOUBLE_STUB_2_INPUT_length/DOUBLE_STUB_2_INPUT_length_unit,
           DOUBLE_STUB_2_INPUT_width/DOUBLE_STUB_2_INPUT_width_unit,
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
           TRIPLE_STUB_1_INPUT_width/TRIPLE_STUB_1_INPUT_width_unit,
           TRIPLE_STUB_1_INPUT_width2/TRIPLE_STUB_1_INPUT_width2_unit);
        printf("# TRIPLE_STUB_1_FAN %f %f\n",
            TRIPLE_STUB_1_FAN_radius/TRIPLE_STUB_1_FAN_radius_unit,
            TRIPLE_STUB_1_FAN_angle/TRIPLE_STUB_1_FAN_angle_unit);
        printf("# TRIPLE_STUB_2_TYPE %s\n", TRIPLE_STUB_2_TYPE.c_str());
        printf("# TRIPLE_STUB_2_INPUT %f %f %f\n",
           TRIPLE_STUB_2_INPUT_length/TRIPLE_STUB_2_INPUT_length_unit,
           TRIPLE_STUB_2_INPUT_width/TRIPLE_STUB_2_INPUT_width_unit,
           TRIPLE_STUB_2_INPUT_width2/TRIPLE_STUB_2_INPUT_width2_unit);
        printf("# TRIPLE_STUB_2_FAN %f %f\n",
            TRIPLE_STUB_2_FAN_radius/TRIPLE_STUB_2_FAN_radius_unit,
            TRIPLE_STUB_2_FAN_angle/TRIPLE_STUB_2_FAN_angle_unit);
        printf("# TRIPLE_STUB_3_TYPE %s\n", TRIPLE_STUB_3_TYPE.c_str());
        printf("# TRIPLE_STUB_3_INPUT %f %f %f\n",
           TRIPLE_STUB_3_INPUT_length/TRIPLE_STUB_3_INPUT_length_unit,
           TRIPLE_STUB_3_INPUT_width/TRIPLE_STUB_3_INPUT_width_unit,
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
        printf("# BELITSKY_JUNCTION_1_AREA %f %s\n",
            BELITSKY_JUNCTION_1_AREA/BELITSKY_JUNCTION_1_AREA_unit,
            BELITSKY_JUNCTION_1_AREA_unitStr.c_str());
        printf("# BELITSKY_TUNING_STRIP %f %f %f\n",
           BELITSKY_TUNING_STRIP_length/BELITSKY_TUNING_STRIP_length_unit,
           BELITSKY_TUNING_STRIP_width/BELITSKY_TUNING_STRIP_width_unit,
           BELITSKY_TUNING_STRIP_thick/BELITSKY_TUNING_STRIP_thick_unit);
        printf("# BELITSKY_JUNCTION_2_AREA %f %s\n",
            BELITSKY_JUNCTION_2_AREA/BELITSKY_JUNCTION_2_AREA_unit,
            BELITSKY_JUNCTION_2_AREA_unitStr.c_str());
    }
    else if (TUNER_TYPE == "BEL_WITH_RFCHOKE")
    {
        printf("# TUNER_TYPE BEL_WITH_RFCHOKE\n");
        printf("# BELITSKY_INPUT_STRIP %f %f %f\n",
           BELITSKY_INPUT_STRIP_length/BELITSKY_INPUT_STRIP_length_unit,
           BELITSKY_INPUT_STRIP_width/BELITSKY_INPUT_STRIP_width_unit,
           BELITSKY_INPUT_STRIP_thick/BELITSKY_INPUT_STRIP_thick_unit);
        printf("# BELITSKY_JUNCTION_1_AREA %f %s\n",
            BELITSKY_JUNCTION_1_AREA/BELITSKY_JUNCTION_1_AREA_unit,
            BELITSKY_JUNCTION_1_AREA_unitStr.c_str());
        printf("# BELITSKY_TUNING_STRIP %f %f %f\n",
           BELITSKY_TUNING_STRIP_length/BELITSKY_TUNING_STRIP_length_unit,
           BELITSKY_TUNING_STRIP_width/BELITSKY_TUNING_STRIP_width_unit,
           BELITSKY_TUNING_STRIP_thick/BELITSKY_TUNING_STRIP_thick_unit);
        printf("# BELITSKY_JUNCTION_2_AREA %f %s\n",
            BELITSKY_JUNCTION_2_AREA/BELITSKY_JUNCTION_2_AREA_unit,
            BELITSKY_JUNCTION_2_AREA_unitStr.c_str());
        printf("# RF_CHOKE_TYPE %s\n", RF_CHOKE_TYPE.c_str());
        if (RF_CHOKE_TYPE == "RADIAL")
        {
            printf("# RADIAL_CHOKE_TYPE %s\n", RADIAL_CHOKE_TYPE.c_str());
            printf("# RADIAL_CHOKE_INPUT %f %f %f\n",
                RADIAL_CHOKE_INPUT_length/RADIAL_CHOKE_INPUT_length_unit,
                RADIAL_CHOKE_INPUT_width/RADIAL_CHOKE_INPUT_width_unit,
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
            printf("# FILTER_CHOKE_OUTPUT %f %f\n", FILTER_CHOKE_OUTPUT_length/FILTER_CHOKE_OUTPUT_length_unit, FILTER_CHOKE_OUTPUT_width/FILTER_CHOKE_OUTPUT_width_unit);
         }
    }
    else if (TUNER_TYPE == "SERIES_DISTRIBUTED_2")
    {
        printf("# TUNER_TYPE SERIES_DISTRIBUTED_2\n");
        printf("# SDM_INPUT_TYPE %s\n",
           SDM_INPUT_TYPE.c_str());
        printf("# SDM_INPUT %f %f %f\n",
           SDM_INPUT_length/SDM_INPUT_length_unit,
           SDM_INPUT_width/SDM_INPUT_width_unit,
           SDM_INPUT_width2/SDM_INPUT_width2_unit);
        printf("# SDM_JUNCTION_1_AREA %f %s\n",
            SDM_JUNCTION_1_AREA/SDM_JUNCTION_1_AREA_unit,
            SDM_JUNCTION_1_AREA_unitStr.c_str());
        printf("# SDM_TUNER_12_TYPE %s\n",
           SDM_TUNER_12_TYPE.c_str());
        printf("# SDM_TUNER_12 %f %f %f\n",
           SDM_TUNER_12_length/SDM_TUNER_12_length_unit,
           SDM_TUNER_12_width/SDM_TUNER_12_width_unit,
           SDM_TUNER_12_width2/SDM_TUNER_12_width2_unit);
        printf("# SDM_JUNCTION_2_AREA %f %s\n",
            SDM_JUNCTION_2_AREA/SDM_JUNCTION_2_AREA_unit,
            SDM_JUNCTION_2_AREA_unitStr.c_str());
    }
    else if (TUNER_TYPE == "SERIES_EXTERNAL_2")
    {
        printf("# TUNER_TYPE %s\n", TUNER_TYPE.c_str());
        printf("# RF_TOUCHSTONE_FILE %s\n", RF_TOUCHSTONE_FILE.c_str());
        printf("# IF_TOUCHSTONE_FILE %s\n", IF_TOUCHSTONE_FILE.c_str());
        printf("# SDM_JUNCTION_1_AREA %f %s\n",
            SDM_JUNCTION_1_AREA/SDM_JUNCTION_1_AREA_unit,
            SDM_JUNCTION_1_AREA_unitStr.c_str());
        printf("# SDM_JUNCTION_2_AREA %f %s\n",
            SDM_JUNCTION_2_AREA/SDM_JUNCTION_2_AREA_unit,
            SDM_JUNCTION_2_AREA_unitStr.c_str());
    }
    else if (TUNER_TYPE == "SERIES_DISTRIBUTED_3")
    {
        printf("# TUNER_TYPE SERIES_DISTRIBUTED_3\n");
        printf("# SDM_INPUT_TYPE %s\n",
           SDM_INPUT_TYPE.c_str());
        printf("# SDM_INPUT %f %f %f\n",
           SDM_INPUT_length/SDM_INPUT_length_unit,
           SDM_INPUT_width/SDM_INPUT_width_unit,
           SDM_INPUT_width2/SDM_INPUT_width2_unit);
        printf("# SDM_JUNCTION_1_AREA %f %s\n",
            SDM_JUNCTION_1_AREA/SDM_JUNCTION_1_AREA_unit,
            SDM_JUNCTION_1_AREA_unitStr.c_str());
        printf("# SDM_TUNER_12_TYPE %s\n",
           SDM_TUNER_12_TYPE.c_str());
        printf("# SDM_TUNER_12 %f %f %f\n",
           SDM_TUNER_12_length/SDM_TUNER_12_length_unit,
           SDM_TUNER_12_width/SDM_TUNER_12_width_unit,
           SDM_TUNER_12_width2/SDM_TUNER_12_width2_unit);
        printf("# SDM_JUNCTION_2_AREA %f %s\n",
            SDM_JUNCTION_2_AREA/SDM_JUNCTION_2_AREA_unit,
            SDM_JUNCTION_2_AREA_unitStr.c_str());
        printf("# SDM_TUNER_23_TYPE %s\n",
           SDM_TUNER_23_TYPE.c_str());
        printf("# SDM_TUNER_23 %f %f %f\n",
           SDM_TUNER_23_length/SDM_TUNER_23_length_unit,
           SDM_TUNER_23_width/SDM_TUNER_23_width_unit,
           SDM_TUNER_23_width2/SDM_TUNER_23_width2_unit);
        printf("# SDM_JUNCTION_3_AREA %f %s\n",
            SDM_JUNCTION_3_AREA/SDM_JUNCTION_3_AREA_unit,
            SDM_JUNCTION_3_AREA_unitStr.c_str());
    }
    else if (TUNER_TYPE == "SERIES_EXTERNAL_3")
    {
        printf("# TUNER_TYPE %s\n", TUNER_TYPE.c_str());
        printf("# RF_TOUCHSTONE_FILE %s\n", RF_TOUCHSTONE_FILE.c_str());
        printf("# IF_TOUCHSTONE_FILE %s\n", IF_TOUCHSTONE_FILE.c_str());
        printf("# SDM_JUNCTION_1_AREA %f %s\n",
            SDM_JUNCTION_1_AREA/SDM_JUNCTION_1_AREA_unit,
            SDM_JUNCTION_1_AREA_unitStr.c_str());
        printf("# SDM_JUNCTION_2_AREA %f %s\n",
            SDM_JUNCTION_2_AREA/SDM_JUNCTION_2_AREA_unit,
            SDM_JUNCTION_2_AREA_unitStr.c_str());
        printf("# SDM_JUNCTION_3_AREA %f %s\n",
            SDM_JUNCTION_3_AREA/SDM_JUNCTION_3_AREA_unit,
            SDM_JUNCTION_3_AREA_unitStr.c_str());
    }
    else if (TUNER_TYPE == "SERIES_DISTRIBUTED_4")
    {
        printf("# TUNER_TYPE SERIES_DISTRIBUTED_4\n");
        printf("# SDM_INPUT_TYPE %s\n",
           SDM_INPUT_TYPE.c_str());
        printf("# SDM_INPUT %f %f %f\n",
           SDM_INPUT_length/SDM_INPUT_length_unit,
           SDM_INPUT_width/SDM_INPUT_width_unit,
           SDM_INPUT_width2/SDM_INPUT_width2_unit);
        printf("# SDM_JUNCTION_1_AREA %f %s\n",
            SDM_JUNCTION_1_AREA/SDM_JUNCTION_1_AREA_unit,
            SDM_JUNCTION_1_AREA_unitStr.c_str());
        printf("# SDM_TUNER_12_TYPE %s\n",
           SDM_TUNER_12_TYPE.c_str());
        printf("# SDM_TUNER_12 %f %f %f\n",
           SDM_TUNER_12_length/SDM_TUNER_12_length_unit,
           SDM_TUNER_12_width/SDM_TUNER_12_width_unit,
           SDM_TUNER_12_width2/SDM_TUNER_12_width2_unit);
        printf("# SDM_JUNCTION_2_AREA %f %s\n",
            SDM_JUNCTION_2_AREA/SDM_JUNCTION_2_AREA_unit,
            SDM_JUNCTION_2_AREA_unitStr.c_str());
        printf("# SDM_TUNER_23_TYPE %s\n",
           SDM_TUNER_23_TYPE.c_str());
        printf("# SDM_TUNER_23 %f %f %f\n",
           SDM_TUNER_23_length/SDM_TUNER_23_length_unit,
           SDM_TUNER_23_width/SDM_TUNER_23_width_unit,
           SDM_TUNER_23_width2/SDM_TUNER_23_width2_unit);
        printf("# SDM_JUNCTION_3_AREA %f %s\n",
            SDM_JUNCTION_3_AREA/SDM_JUNCTION_3_AREA_unit,
            SDM_JUNCTION_3_AREA_unitStr.c_str());
        printf("# SDM_TUNER_34_TYPE %s\n",
           SDM_TUNER_34_TYPE.c_str());
        printf("# SDM_TUNER_34 %f %f %f\n",
           SDM_TUNER_34_length/SDM_TUNER_34_length_unit,
           SDM_TUNER_34_width/SDM_TUNER_34_width_unit,
           SDM_TUNER_34_width2/SDM_TUNER_34_width2_unit);
        printf("# SDM_JUNCTION_4_AREA %f %s\n",
            SDM_JUNCTION_4_AREA/SDM_JUNCTION_4_AREA_unit,
            SDM_JUNCTION_4_AREA_unitStr.c_str());
    }
    else if (TUNER_TYPE == "SERIES_EXTERNAL_4")
    {
        printf("# TUNER_TYPE %s\n", TUNER_TYPE.c_str());
        printf("# RF_TOUCHSTONE_FILE %s\n", RF_TOUCHSTONE_FILE.c_str());
        printf("# IF_TOUCHSTONE_FILE %s\n", IF_TOUCHSTONE_FILE.c_str());
        printf("# SDM_JUNCTION_1_AREA %f %s\n",
            SDM_JUNCTION_1_AREA/SDM_JUNCTION_1_AREA_unit,
            SDM_JUNCTION_1_AREA_unitStr.c_str());
        printf("# SDM_JUNCTION_2_AREA %f %s\n",
            SDM_JUNCTION_2_AREA/SDM_JUNCTION_2_AREA_unit,
            SDM_JUNCTION_2_AREA_unitStr.c_str());
        printf("# SDM_JUNCTION_3_AREA %f %s\n",
            SDM_JUNCTION_3_AREA/SDM_JUNCTION_3_AREA_unit,
            SDM_JUNCTION_3_AREA_unitStr.c_str());
        printf("# SDM_JUNCTION_4_AREA %f %s\n",
            SDM_JUNCTION_4_AREA/SDM_JUNCTION_4_AREA_unit,
            SDM_JUNCTION_4_AREA_unitStr.c_str());
    }
    else
    {
        printf("# TUNER_TYPE %s not implemeted!\n", TUNER_TYPE.c_str());
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
    printf("# SIO2_PROPERTIES %f %f\n",
            SIO2_epsilon/1.0, SIO2_loss_tangent/1.0);
    printf("# AIR_PROPERTIES %f %f\n",
            AIR_epsilon/1.0, AIR_loss_tangent/1.0);
    printf("#\n# End of configuration file data\n# \n");

    return 0;
}
