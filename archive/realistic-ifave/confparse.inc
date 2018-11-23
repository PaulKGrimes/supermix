/*******************************************************************************
** confparse.inc - file included in realistic.c, containing code to parse the **
**              conf files, setting the globals created in globals.inc        **
** ========================================================================== **
** Author - Paul Grimes                                                       **
** Date   - 01/09/03     - created from SuperMix_Simple version               **
** ========================================================================== **
** SuperMix parameters are currently created as globals, so that dotconf      **
** functions that interpret each line in the .conf file can alter them as     **
** side effects.                                                              **
**                                                                            **
** We intend to use all caps for global parameter names, and camelback for    **
** SuperMix internal parameter names                                          **
** ========================================================================== **
** Changes:                                                                   **
*******************************************************************************/


DOTCONF_CB(cb_fallback);

// Declare each of the .conf parsing functions
static DOTCONF_CB(mixer_config);
static DOTCONF_CB(output);
// Simulation control parameters
static DOTCONF_CB(scanned_variable);
static DOTCONF_CB(scan_unit);
static DOTCONF_CB(scan_low);
static DOTCONF_CB(scan_high);
static DOTCONF_CB(scan_step);
static DOTCONF_CB(frequency);
static DOTCONF_CB(if_freq);
static DOTCONF_CB(bias);
static DOTCONF_CB(lo_power);
static DOTCONF_CB(lo_temp);

// RF loss parameters
//  Beamsplitter
static DOTCONF_CB(bs_epsilon);
static DOTCONF_CB(bs_loss_tan);
static DOTCONF_CB(bs_thickness);
static DOTCONF_CB(bs_angle);
static DOTCONF_CB(bs_temp);
//  Dewar Window
static DOTCONF_CB(dw_epsilon);
static DOTCONF_CB(dw_loss_tan);
static DOTCONF_CB(dw_thickness);
static DOTCONF_CB(dw_angle);
static DOTCONF_CB(dw_temp);
//  Misc RF losses
static DOTCONF_CB(misc_atten);
static DOTCONF_CB(misc_temp);
// Lossy Microstrip
static DOTCONF_CB(lossy_input);

// Mixer setup parameters
    // Environment parameters
static DOTCONF_CB(temp);
static DOTCONF_CB(z0);
static DOTCONF_CB(if_z0);
static DOTCONF_CB(harmonics);
    // Junction parameters
static DOTCONF_CB(iv_curve);
static DOTCONF_CB(kk_curve);
static DOTCONF_CB(rna);
static DOTCONF_CB(scap);
static DOTCONF_CB(area);
static DOTCONF_CB(vgap);
    // Tuner parameters
static DOTCONF_CB(tuner_type);
static DOTCONF_CB(touchstone_file);

    // Single stub tuner
static DOTCONF_CB(single_stub_type);
static DOTCONF_CB(single_stub_input);
static DOTCONF_CB(single_stub_fan);

    // double stub tuner
static DOTCONF_CB(double_stub_1_type);
static DOTCONF_CB(double_stub_1_input);
static DOTCONF_CB(double_stub_1_fan);
static DOTCONF_CB(double_stub_2_type);
static DOTCONF_CB(double_stub_2_input);
static DOTCONF_CB(double_stub_2_fan);

    // triple stub tuner
static DOTCONF_CB(triple_stub_1_type);
static DOTCONF_CB(triple_stub_1_input);
static DOTCONF_CB(triple_stub_1_fan);
static DOTCONF_CB(triple_stub_2_type);
static DOTCONF_CB(triple_stub_2_input);
static DOTCONF_CB(triple_stub_2_fan);
static DOTCONF_CB(triple_stub_3_type);
static DOTCONF_CB(triple_stub_3_input);
static DOTCONF_CB(triple_stub_3_fan);

    // belitsky tuner
static DOTCONF_CB(belitsky_input_strip);
static DOTCONF_CB(belitsky_tuning_strip);

    // RF choke
static DOTCONF_CB(rf_choke_type);
static DOTCONF_CB(radial_choke_type);
static DOTCONF_CB(radial_choke_input);
static DOTCONF_CB(radial_choke_fan);
static DOTCONF_CB(radial_choke_output);

    // IF circuit
static DOTCONF_CB(if_finline_s2p);
static DOTCONF_CB(if_connect_s2p);
static DOTCONF_CB(if_finline_out);
static DOTCONF_CB(if_feed_term);

// Default materials
static DOTCONF_CB(top_default);
static DOTCONF_CB(top_thickness);
static DOTCONF_CB(ground_default);
static DOTCONF_CB(ground_thickness);
static DOTCONF_CB(insul_default);
static DOTCONF_CB(insul_thickness);
static DOTCONF_CB(cover_default);
static DOTCONF_CB(cover_thickness);

// Material properties
static DOTCONF_CB(nb_properties);
static DOTCONF_CB(nbtin_properties);
static DOTCONF_CB(sio_properties);

// Create the mapping of .conf keywords to functions
static const configoption_t options[] = {
    // General parameters
    {"MIXER_CONFIG",        ARG_STR,    mixer_config,       NULL,   CTX_ALL },
    {"OUTPUT",              ARG_STR,    output,             NULL,   CTX_ALL },
    // Simulation control parameters
    {"SCANNED_VARIABLE",    ARG_STR,    scanned_variable,   NULL,   CTX_ALL },
    {"SCAN_UNIT",           ARG_STR,    scan_unit,          NULL,   CTX_ALL },
    {"SCAN_LOW",            ARG_LIST,   scan_low,           NULL,   CTX_ALL },
    {"SCAN_HIGH",           ARG_LIST,   scan_high,          NULL,   CTX_ALL },
    {"SCAN_STEP",           ARG_LIST,   scan_step,          NULL,   CTX_ALL },
    {"FREQUENCY",           ARG_LIST,   frequency,          NULL,   CTX_ALL },
    {"IF_FREQ",             ARG_LIST,   if_freq,            NULL,   CTX_ALL },
    {"BIAS",                ARG_LIST,   bias,               NULL,   CTX_ALL },
    {"LO_POWER",            ARG_LIST,   lo_power,           NULL,   CTX_ALL },
    {"LO_TEMP",             ARG_LIST,   lo_temp,            NULL,   CTX_ALL },
    // RF Loss parameters
    //  Beamsplitter
    {"BS_EPSILON",          ARG_LIST,   bs_epsilon,         NULL,   CTX_ALL },
    {"BS_LOSS_TAN",         ARG_LIST,   bs_loss_tan,        NULL,   CTX_ALL },
    {"BS_THICKNESS",        ARG_LIST,   bs_thickness,       NULL,   CTX_ALL },
    {"BS_ANGLE",            ARG_LIST,   bs_angle,           NULL,   CTX_ALL },
    {"BS_TEMP",             ARG_LIST,   bs_temp,            NULL,   CTX_ALL },
    //  Dewar Window
    {"DW_EPSILON",          ARG_LIST,   dw_epsilon,         NULL,   CTX_ALL },
    {"DW_LOSS_TAN",         ARG_LIST,   dw_loss_tan,        NULL,   CTX_ALL },
    {"DW_THICKNESS",        ARG_LIST,   dw_thickness,       NULL,   CTX_ALL },
    {"DW_ANGLE",            ARG_LIST,   dw_angle,           NULL,   CTX_ALL },
    {"DW_TEMP",             ARG_LIST,   dw_temp,            NULL,   CTX_ALL },
    //  Misc RF Losses
    {"MISC_ATTENUATION",    ARG_LIST,   misc_atten,         NULL,   CTX_ALL },
    {"MISC_TEMP",           ARG_LIST,   misc_temp,          NULL,   CTX_ALL },
    //  Lossy Input Microstrip
    {"LOSSY_INPUT",         ARG_LIST,   lossy_input,        NULL,   CTX_ALL },
    // Mixer setup parameters
    // General enivronment parameters
    {"TEMP",                ARG_LIST,   temp,               NULL,   CTX_ALL },
    {"Z0",                  ARG_LIST,   z0,                 NULL,   CTX_ALL },
    {"IF_Z0",               ARG_LIST,   if_z0,              NULL,   CTX_ALL },
    {"HARMONICS",           ARG_LIST,   harmonics,          NULL,   CTX_ALL },
    // Junction parameters
    {"IV_CURVE",            ARG_STR,    iv_curve,           NULL,   CTX_ALL },
    {"KK_CURVE",            ARG_STR,    kk_curve,           NULL,   CTX_ALL },
    {"RNA",                 ARG_LIST,   rna,                NULL,   CTX_ALL },
    {"SCAP",                ARG_LIST,   scap,               NULL,   CTX_ALL },
    {"AREA",                ARG_LIST,   area,               NULL,   CTX_ALL },
    {"VGAP",                ARG_LIST,   vgap,               NULL,   CTX_ALL },
    // Tuner parameters
    {"TUNER_TYPE",          ARG_STR,    tuner_type,         NULL,   CTX_ALL },
    {"TOUCHSTONE_FILE",     ARG_STR,    touchstone_file,    NULL,   CTX_ALL },
        // Single stub tuner
    {"SINGLE_STUB_TYPE",    ARG_STR,    single_stub_type,   NULL,   CTX_ALL },
    {"SINGLE_STUB_INPUT",   ARG_LIST,   single_stub_input,  NULL,   CTX_ALL },
    {"SINGLE_STUB_FAN",     ARG_LIST,   single_stub_fan,    NULL,   CTX_ALL },
        // Double stub tuner
    {"DOUBLE_STUB_1_TYPE",  ARG_STR,    double_stub_1_type,  NULL,   CTX_ALL },
    {"DOUBLE_STUB_1_INPUT", ARG_LIST,   double_stub_1_input, NULL,   CTX_ALL },
    {"DOUBLE_STUB_1_FAN",   ARG_LIST,   double_stub_1_fan,   NULL,   CTX_ALL },
    {"DOUBLE_STUB_2_TYPE",  ARG_STR,    double_stub_2_type,  NULL,   CTX_ALL },
    {"DOUBLE_STUB_2_INPUT", ARG_LIST,   double_stub_2_input, NULL,   CTX_ALL },
    {"DOUBLE_STUB_2_FAN",   ARG_LIST,   double_stub_2_fan,   NULL,   CTX_ALL },
        // Triple stub tuner
    {"TRIPLE_STUB_1_TYPE",  ARG_STR,    triple_stub_1_type,  NULL,   CTX_ALL },
    {"TRIPLE_STUB_1_INPUT", ARG_LIST,   triple_stub_1_input, NULL,   CTX_ALL },
    {"TRIPLE_STUB_1_FAN",   ARG_LIST,   triple_stub_1_fan,   NULL,   CTX_ALL },
    {"TRIPLE_STUB_2_TYPE",  ARG_STR,    triple_stub_2_type,  NULL,   CTX_ALL },
    {"TRIPLE_STUB_2_INPUT", ARG_LIST,   triple_stub_2_input, NULL,   CTX_ALL },
    {"TRIPLE_STUB_2_FAN",   ARG_LIST,   triple_stub_2_fan,   NULL,   CTX_ALL },
    {"TRIPLE_STUB_3_TYPE",  ARG_STR,    triple_stub_3_type,  NULL,   CTX_ALL },
    {"TRIPLE_STUB_3_INPUT", ARG_LIST,   triple_stub_3_input, NULL,   CTX_ALL },
    {"TRIPLE_STUB_3_FAN",   ARG_LIST,   triple_stub_3_fan,   NULL,   CTX_ALL },
        // Belitsky tuner
    {"BELITSKY_INPUT_STRIP",  ARG_LIST,  belitsky_input_strip,  NULL,   CTX_ALL },
    {"BELITSKY_TUNING_STRIP", ARG_LIST,  belitsky_tuning_strip, NULL,   CTX_ALL },
        // RF choke
    {"RF_CHOKE_TYPE",        ARG_STR,    rf_choke_type,       NULL,   CTX_ALL },
    {"RADIAL_CHOKE_TYPE",    ARG_STR,    radial_choke_type,   NULL,   CTX_ALL },
    {"RADIAL_CHOKE_INPUT",   ARG_LIST,   radial_choke_input,  NULL,   CTX_ALL },
    {"RADIAL_CHOKE_FAN",     ARG_LIST,   radial_choke_fan,    NULL,   CTX_ALL },
    {"RADIAL_CHOKE_OUTPUT",  ARG_LIST,   radial_choke_output, NULL,   CTX_ALL },
        // IF Circuit
    {"IF_FINLINE_S2P",      ARG_STR,    if_finline_s2p,    NULL,   CTX_ALL },
    {"IF_CONNECT_S2P",      ARG_STR,    if_connect_s2p,    NULL,   CTX_ALL },
    {"IF_FINLINE_OUT",      ARG_LIST,   if_finline_out,    NULL,   CTX_ALL },
    {"IF_FEED_TERM",        ARG_LIST,   if_feed_term,      NULL,   CTX_ALL },
    
    
    // Default materials
    {"TOP_DEFAULT",         ARG_STR,    top_default,        NULL,   CTX_ALL },
    {"TOP_THICKNESS",       ARG_LIST,   top_thickness,      NULL,   CTX_ALL },
    {"GROUND_DEFAULT",      ARG_STR,    ground_default,     NULL,   CTX_ALL },
    {"GROUND_THICKNESS",    ARG_LIST,   ground_thickness,   NULL,   CTX_ALL },
    {"INSUL_DEFAULT",       ARG_STR,    insul_default,      NULL,   CTX_ALL },
    {"INSUL_THICKNESS",     ARG_LIST,   insul_thickness,    NULL,   CTX_ALL },
    {"COVER_DEFAULT",       ARG_STR,    cover_default,      NULL,   CTX_ALL },
    {"COVER_THICKNESS",     ARG_LIST,   cover_thickness,    NULL,   CTX_ALL },
    // Material Properties
    {"NB_PROPERTIES",       ARG_LIST,   nb_properties,      NULL,   CTX_ALL },
    {"NBTIN_PROPERTIES",    ARG_LIST,   nbtin_properties,   NULL,   CTX_ALL },
    {"SIO_PROPERTIES",      ARG_LIST,   sio_properties,     NULL,   CTX_ALL },
    // Fallback
    {"",                    ARG_NAME,   cb_fallback,        NULL,   0       },
    LAST_OPTION
};


/*******************************
** The .conf parsing functions
*/

DOTCONF_CB(mixer_config)
{
    /*printf("%s:%ld: %s: [%s]\n",
       cmd->configfile->filename, cmd->configfile->line,
           cmd->name, cmd->data.str);*/
    MIXER_CONFIG = cmd->data.str;

    return NULL;
}

DOTCONF_CB(output)
{
    /*printf("%s:%ld: %s: [%s]\n",
        cmd->configfile->filename, cmd->configfile->line,
            cmd->name, cmd->data.str);*/
    OUTPUT = cmd->data.str;
    return NULL;
}

// Simulation control parameters


DOTCONF_CB(scanned_variable)
{
    SCANNED_VARIABLE = cmd->data.str;
    return NULL;
}

DOTCONF_CB(scan_unit)
{
    /*printf("%s:%ld: %s: [%s]\n",
        cmd->configfile->filename, cmd->configfile->line,
            cmd->name, cmd->data.str);*/
    SCAN_unitStr = cmd->data.str;

         if (SCAN_unitStr == "GHz")   {SCAN_unit = GHz;}
    else if (SCAN_unitStr == "mVolt") {SCAN_unit = mVolt;}
    else if (SCAN_unitStr == "nW")    {SCAN_unit = Nano*Watt;}
    else if (SCAN_unitStr == "Vs")    {SCAN_unit = mVolt;} // Check this unit
    else {SCAN_unit = 1.0;}

    return NULL;
}

DOTCONF_CB(scan_low)
{
    // Code that actually sets the global parameters
    SCAN_LOW = atof(cmd->data.list[0]);

    return NULL;
}

DOTCONF_CB(scan_high)
{
    // Code that actually sets the global parameters
    SCAN_HIGH = atof(cmd->data.list[0]);

    return NULL;
}

DOTCONF_CB(scan_step)
{
    // Code that actually sets the global parameters
    SCAN_STEP = atof(cmd->data.list[0]);

    return NULL;
}

DOTCONF_CB(frequency)
{
    FREQUENCY_unitStr = cmd->data.list[1];

    if (FREQUENCY_unitStr == "GHz") {FREQUENCY_unit = GHz;}
    else {FREQUENCY_unit = 1.0;}

    FREQUENCY = atof(cmd->data.list[0])*FREQUENCY_unit;

    return NULL;
}

DOTCONF_CB(bias)
{
    BIAS_unitStr = cmd->data.list[1];

    if (BIAS_unitStr == "mV") {BIAS_unit = mVolt;}
    else {BIAS_unit = 1.0;}

    BIAS = atof(cmd->data.list[0])*BIAS_unit;

    return NULL;
}

DOTCONF_CB(lo_power)
{
    LO_POWER_unitStr = cmd->data.list[1];

    if (LO_POWER_unitStr == "nW") {LO_POWER_unit = Nano*Watt;}
    else {LO_POWER_unit = 1.0;}

    LO_POWER = atof(cmd->data.list[0])*LO_POWER_unit;

    return NULL;
}

DOTCONF_CB(lo_temp)
{
    Lo_Temp = atof(cmd->data.list[0])*Kelvin;

    return NULL;
}

// RF Loss parameters
//  Beamsplitter
DOTCONF_CB(bs_epsilon)
{
    BS_EPSILON = atof(cmd->data.list[0]);

    return NULL;
}

DOTCONF_CB(bs_loss_tan)
{
    BS_LOSS_TAN = atof(cmd->data.list[0]);

    return NULL;
}

DOTCONF_CB(bs_thickness)
{
    BS_THICKNESS = atof(cmd->data.list[0])*Micron;

    return NULL;
}

DOTCONF_CB(bs_angle)
{
    BS_ANGLE = atof(cmd->data.list[0])*Degree;

    return NULL;
}

DOTCONF_CB(bs_temp)
{
    BS_TEMP = atof(cmd->data.list[0])*Kelvin;

    return NULL;
}

//  Dewar Window
DOTCONF_CB(dw_epsilon)
{
    DW_EPSILON = atof(cmd->data.list[0]);

    return NULL;
}

DOTCONF_CB(dw_loss_tan)
{
    DW_LOSS_TAN = atof(cmd->data.list[0]);

    return NULL;
}

DOTCONF_CB(dw_thickness)
{
    DW_THICKNESS = atof(cmd->data.list[0])*Micron;

    return NULL;
}

DOTCONF_CB(dw_angle)
{
    DW_ANGLE = atof(cmd->data.list[0])*Degree;

    return NULL;
}

DOTCONF_CB(dw_temp)
{
    DW_TEMP = atof(cmd->data.list[0])*Kelvin;

    return NULL;
}

//  Misc RF Losses
DOTCONF_CB(misc_atten)
{
    MISC_ATTEN = atof(cmd->data.list[0]);

    return NULL;
}

DOTCONF_CB(misc_temp)
{
    MISC_TEMP = atof(cmd->data.list[0])*Kelvin;

    return NULL;
}

DOTCONF_CB(lossy_input)
{
    LOSSY_INPUT_length_unit = Micron;
    LOSSY_INPUT_length_unitStr = "um";
    LOSSY_INPUT_length = atof(cmd->data.list[0])*LOSSY_INPUT_length_unit;
    
    LOSSY_INPUT_width_unit = Micron;
    LOSSY_INPUT_width_unitStr = "um";
    LOSSY_INPUT_width = atof(cmd->data.list[1])*LOSSY_INPUT_width_unit;
    
    return NULL;
}



// Mixer setup parameters
    // Environment parameters

DOTCONF_CB(temp)
{
    TEMP_unitStr = cmd->data.list[1];
    
    if (TEMP_unitStr == "K") {TEMP_unit = Kelvin;}
    else {TEMP_unit = 1.0;}
    
    TEMP = atof(cmd->data.list[0])*TEMP_unit;
    
    return NULL;
}

DOTCONF_CB(z0)
{
    Z0_unitStr = cmd->data.list[1];
    
    if (Z0_unitStr == "Ohms") {Z0_unit = Ohm;}
    else {Z0_unit = 1.0;}
    
    Z0 = atof(cmd->data.list[0])*Z0_unit;
    
    return NULL;
}

DOTCONF_CB(if_freq)
{
    IF_FREQ_unitStr = cmd->data.list[1];
    
    if (IF_FREQ_unitStr == "GHz") {IF_FREQ_unit = GHz;}
    else {IF_FREQ_unit = 1.0;}
    
    IF_FREQ = atof(cmd->data.list[0])*IF_FREQ_unit;
    
    return NULL;
}

DOTCONF_CB(if_z0)
{
    IF_Z0_unitStr = cmd->data.list[1];
    
    if (IF_Z0_unitStr == "Ohms") {IF_Z0_unit = Ohm;}
    else {IF_Z0_unit = 1.0;}
    
    IF_Z0 = atof(cmd->data.list[0])*IF_Z0_unit;
    
    return NULL;
}

DOTCONF_CB(harmonics)
{
    HARMONICS = atoi(cmd->data.list[0]);
    
    return NULL;
}

    // Junction parameters
DOTCONF_CB(iv_curve)
{
    IV_CURVE = cmd->data.str;
    return NULL;
}
    
DOTCONF_CB(kk_curve)
{
    KK_CURVE = cmd->data.str;
    return NULL;
}

DOTCONF_CB(rna)
{

    RNA_unit = Ohm*Micron*Micron;
    RNA = atof(cmd->data.list[0])*RNA_unit;
    RNA_unitStr = "Ohm*um^2";
    
    return NULL;
}

DOTCONF_CB(scap)
{
    SCAP_unit = fFarad/Micron/Micron;
    SCAP = atof(cmd->data.list[0])*SCAP_unit;
    SCAP_unitStr = "fF/um^2";
    
    return NULL;
}

DOTCONF_CB(area)
{
    AREA_unit = Micron*Micron;
    AREA = atof(cmd->data.list[0])*AREA_unit;
    AREA_unitStr = "um^2";
    
    return NULL;
}

DOTCONF_CB(vgap)
{
    VGAP_unit = mVolt;
    VGAP = atof(cmd->data.list[0])*VGAP_unit;
    VGAP_unitStr = "mV";
    
    return NULL;
}

// Tuner parameters
DOTCONF_CB(tuner_type)
{
    TUNER_TYPE = cmd->data.str;
    return NULL;
}

DOTCONF_CB(touchstone_file)
{
    TOUCHSTONE_FILE = cmd->data.str;
    return NULL;
}

    // Single stub tuner
DOTCONF_CB(single_stub_type)
{
    SINGLE_STUB_TYPE = cmd->data.str;
    return NULL;
}

DOTCONF_CB(single_stub_input)
{
    SINGLE_STUB_INPUT_length_unit = Micron;
    SINGLE_STUB_INPUT_length_unitStr = "um";
    SINGLE_STUB_INPUT_length = atof(cmd->data.list[0])*SINGLE_STUB_INPUT_length_unit;
    
    SINGLE_STUB_INPUT_width1_unit = Micron;
    SINGLE_STUB_INPUT_width1_unitStr = "um";
    SINGLE_STUB_INPUT_width1 = atof(cmd->data.list[1])*SINGLE_STUB_INPUT_width1_unit;
    
    SINGLE_STUB_INPUT_width2_unit = Micron;
    SINGLE_STUB_INPUT_width2_unitStr = "um";
    SINGLE_STUB_INPUT_width2 = atof(cmd->data.list[2])*SINGLE_STUB_INPUT_width2_unit;
    
    return NULL;
}

DOTCONF_CB(single_stub_fan)
{
    SINGLE_STUB_FAN_radius_unit = Micron;
    SINGLE_STUB_FAN_radius_unitStr = "um";
    SINGLE_STUB_FAN_radius = atof(cmd->data.list[0])*SINGLE_STUB_FAN_radius_unit;
    
    SINGLE_STUB_FAN_angle_unit = Degree;
    SINGLE_STUB_FAN_angle_unitStr = "degrees";
    SINGLE_STUB_FAN_angle = atof(cmd->data.list[1])*SINGLE_STUB_FAN_angle_unit;
    
    return NULL;
}

    // Double stub tuner
DOTCONF_CB(double_stub_1_type)
{
    DOUBLE_STUB_1_TYPE = cmd->data.str;
    return NULL;
}

DOTCONF_CB(double_stub_1_input)
{
    DOUBLE_STUB_1_INPUT_length_unit = Micron;
    DOUBLE_STUB_1_INPUT_length_unitStr = "um";
    DOUBLE_STUB_1_INPUT_length = atof(cmd->data.list[0])*DOUBLE_STUB_1_INPUT_length_unit;
    
    DOUBLE_STUB_1_INPUT_width1_unit = Micron;
    DOUBLE_STUB_1_INPUT_width1_unitStr = "um";
    DOUBLE_STUB_1_INPUT_width1 = atof(cmd->data.list[1])*DOUBLE_STUB_1_INPUT_width1_unit;
    
    DOUBLE_STUB_1_INPUT_width2_unit = Micron;
    DOUBLE_STUB_1_INPUT_width2_unitStr = "um";
    DOUBLE_STUB_1_INPUT_width2 = atof(cmd->data.list[2])*DOUBLE_STUB_1_INPUT_width2_unit;
    
    return NULL;
}

DOTCONF_CB(double_stub_1_fan)
{
    DOUBLE_STUB_1_FAN_radius_unit = Micron;
    DOUBLE_STUB_1_FAN_radius_unitStr = "um";
    DOUBLE_STUB_1_FAN_radius = atof(cmd->data.list[0])*DOUBLE_STUB_1_FAN_radius_unit;
    
    DOUBLE_STUB_1_FAN_angle_unit = Degree;
    DOUBLE_STUB_1_FAN_angle_unitStr = "degrees";
    DOUBLE_STUB_1_FAN_angle = atof(cmd->data.list[1])*DOUBLE_STUB_1_FAN_angle_unit;
    
    return NULL;
}

DOTCONF_CB(double_stub_2_type)
{
    DOUBLE_STUB_2_TYPE = cmd->data.str;
    return NULL;
}

DOTCONF_CB(double_stub_2_input)
{
    DOUBLE_STUB_2_INPUT_length_unit = Micron;
    DOUBLE_STUB_2_INPUT_length_unitStr = "um";
    DOUBLE_STUB_2_INPUT_length = atof(cmd->data.list[0])*DOUBLE_STUB_2_INPUT_length_unit;
    
    DOUBLE_STUB_2_INPUT_width1_unit = Micron;
    DOUBLE_STUB_2_INPUT_width1_unitStr = "um";
    DOUBLE_STUB_2_INPUT_width1 = atof(cmd->data.list[1])*DOUBLE_STUB_2_INPUT_width1_unit;
    
    DOUBLE_STUB_2_INPUT_width2_unit = Micron;
    DOUBLE_STUB_2_INPUT_width2_unitStr = "um";
    DOUBLE_STUB_2_INPUT_width2 = atof(cmd->data.list[2])*DOUBLE_STUB_2_INPUT_width2_unit;
    
    return NULL;
}

DOTCONF_CB(double_stub_2_fan)
{
    DOUBLE_STUB_2_FAN_radius_unit = Micron;
    DOUBLE_STUB_2_FAN_radius_unitStr = "um";
    DOUBLE_STUB_2_FAN_radius = atof(cmd->data.list[0])*DOUBLE_STUB_1_FAN_radius_unit;
    
    DOUBLE_STUB_2_FAN_angle_unit = Degree;
    DOUBLE_STUB_2_FAN_angle_unitStr = "degrees";
    DOUBLE_STUB_2_FAN_angle = atof(cmd->data.list[1])*DOUBLE_STUB_1_FAN_angle_unit;
    
    return NULL;
}

    // Triple stub tuner
DOTCONF_CB(triple_stub_1_type)
{
    TRIPLE_STUB_1_TYPE = cmd->data.str;
    return NULL;
}

DOTCONF_CB(triple_stub_1_input)
{
    TRIPLE_STUB_1_INPUT_length_unit = Micron;
    TRIPLE_STUB_1_INPUT_length_unitStr = "um";
    TRIPLE_STUB_1_INPUT_length = atof(cmd->data.list[0])*TRIPLE_STUB_1_INPUT_length_unit;
    
    TRIPLE_STUB_1_INPUT_width1_unit = Micron;
    TRIPLE_STUB_1_INPUT_width1_unitStr = "um";
    TRIPLE_STUB_1_INPUT_width1 = atof(cmd->data.list[1])*TRIPLE_STUB_1_INPUT_width1_unit;
    
    TRIPLE_STUB_1_INPUT_width2_unit = Micron;
    TRIPLE_STUB_1_INPUT_width2_unitStr = "um";
    TRIPLE_STUB_1_INPUT_width2 = atof(cmd->data.list[2])*TRIPLE_STUB_1_INPUT_width2_unit;
    
    return NULL;
}

DOTCONF_CB(triple_stub_1_fan)
{
    TRIPLE_STUB_1_FAN_radius_unit = Micron;
    TRIPLE_STUB_1_FAN_radius_unitStr = "um";
    TRIPLE_STUB_1_FAN_radius = atof(cmd->data.list[0])*TRIPLE_STUB_1_FAN_radius_unit;
    
    TRIPLE_STUB_1_FAN_angle_unit = Degree;
    TRIPLE_STUB_1_FAN_angle_unitStr = "degrees";
    TRIPLE_STUB_1_FAN_angle = atof(cmd->data.list[1])*TRIPLE_STUB_1_FAN_angle_unit;
    
    return NULL;
}

DOTCONF_CB(triple_stub_2_type)
{
    TRIPLE_STUB_2_TYPE = cmd->data.str;
    return NULL;
}

DOTCONF_CB(triple_stub_2_input)
{
    TRIPLE_STUB_2_INPUT_length_unit = Micron;
    TRIPLE_STUB_2_INPUT_length_unitStr = "um";
    TRIPLE_STUB_2_INPUT_length = atof(cmd->data.list[0])*TRIPLE_STUB_2_INPUT_length_unit;
    
    TRIPLE_STUB_2_INPUT_width1_unit = Micron;
    TRIPLE_STUB_2_INPUT_width1_unitStr = "um";
    TRIPLE_STUB_2_INPUT_width1 = atof(cmd->data.list[1])*TRIPLE_STUB_2_INPUT_width1_unit;
    
    TRIPLE_STUB_2_INPUT_width2_unit = Micron;
    TRIPLE_STUB_2_INPUT_width2_unitStr = "um";
    TRIPLE_STUB_2_INPUT_width2 = atof(cmd->data.list[2])*TRIPLE_STUB_2_INPUT_width2_unit;
    
    return NULL;
}

DOTCONF_CB(triple_stub_2_fan)
{
    TRIPLE_STUB_2_FAN_radius_unit = Micron;
    TRIPLE_STUB_2_FAN_radius_unitStr = "um";
    TRIPLE_STUB_2_FAN_radius = atof(cmd->data.list[0])*TRIPLE_STUB_1_FAN_radius_unit;
    
    TRIPLE_STUB_2_FAN_angle_unit = Degree;
    TRIPLE_STUB_2_FAN_angle_unitStr = "degrees";
    TRIPLE_STUB_2_FAN_angle = atof(cmd->data.list[1])*TRIPLE_STUB_1_FAN_angle_unit;
    
    return NULL;
}

DOTCONF_CB(triple_stub_3_type)
{
    TRIPLE_STUB_3_TYPE = cmd->data.str;
    return NULL;
}

DOTCONF_CB(triple_stub_3_input)
{
    TRIPLE_STUB_3_INPUT_length_unit = Micron;
    TRIPLE_STUB_3_INPUT_length_unitStr = "um";
    TRIPLE_STUB_3_INPUT_length = atof(cmd->data.list[0])*TRIPLE_STUB_3_INPUT_length_unit;
    
    TRIPLE_STUB_3_INPUT_width1_unit = Micron;
    TRIPLE_STUB_3_INPUT_width1_unitStr = "um";
    TRIPLE_STUB_3_INPUT_width1 = atof(cmd->data.list[1])*TRIPLE_STUB_3_INPUT_width1_unit;
    
    TRIPLE_STUB_3_INPUT_width2_unit = Micron;
    TRIPLE_STUB_3_INPUT_width2_unitStr = "um";
    TRIPLE_STUB_3_INPUT_width2 = atof(cmd->data.list[2])*TRIPLE_STUB_3_INPUT_width2_unit;
    
    return NULL;
}

DOTCONF_CB(triple_stub_3_fan)
{
    TRIPLE_STUB_3_FAN_radius_unit = Micron;
    TRIPLE_STUB_3_FAN_radius_unitStr = "um";
    TRIPLE_STUB_3_FAN_radius = atof(cmd->data.list[0])*TRIPLE_STUB_1_FAN_radius_unit;
    
    TRIPLE_STUB_3_FAN_angle_unit = Degree;
    TRIPLE_STUB_3_FAN_angle_unitStr = "degrees";
    TRIPLE_STUB_3_FAN_angle = atof(cmd->data.list[1])*TRIPLE_STUB_1_FAN_angle_unit;
    
    return NULL;
}

// Belitsky tuner
DOTCONF_CB(belitsky_input_strip)
{
    BELITSKY_INPUT_STRIP_length_unit = Micron;
    BELITSKY_INPUT_STRIP_length_unitStr = "um";
    BELITSKY_INPUT_STRIP_length = atof(cmd->data.list[0])*BELITSKY_INPUT_STRIP_length_unit;
    
    BELITSKY_INPUT_STRIP_width_unit = Micron;
    BELITSKY_INPUT_STRIP_width_unitStr = "um";
    BELITSKY_INPUT_STRIP_width = atof(cmd->data.list[1])*BELITSKY_INPUT_STRIP_width_unit;

    return NULL;
}

DOTCONF_CB(belitsky_tuning_strip)
{
    BELITSKY_TUNING_STRIP_length_unit = Micron;
    BELITSKY_TUNING_STRIP_length_unitStr = "um";
    BELITSKY_TUNING_STRIP_length = atof(cmd->data.list[0])*BELITSKY_TUNING_STRIP_length_unit;
    
    BELITSKY_TUNING_STRIP_width_unit = Micron;
    BELITSKY_TUNING_STRIP_width_unitStr = "um";
    BELITSKY_TUNING_STRIP_width = atof(cmd->data.list[1])*BELITSKY_TUNING_STRIP_width_unit;

    return NULL;
}


// RF Choke
DOTCONF_CB(rf_choke_type)
{
    RF_CHOKE_TYPE = cmd->data.str;
    return NULL;
}

DOTCONF_CB(radial_choke_type)
{
    RADIAL_CHOKE_TYPE = cmd->data.str;
    return NULL;
}

DOTCONF_CB(radial_choke_input)
{
    RADIAL_CHOKE_INPUT_length_unit = Micron;
    RADIAL_CHOKE_INPUT_length_unitStr = "um";
    RADIAL_CHOKE_INPUT_length = atof(cmd->data.list[0])*RADIAL_CHOKE_INPUT_length_unit;
    
    RADIAL_CHOKE_INPUT_width1_unit = Micron;
    RADIAL_CHOKE_INPUT_width1_unitStr = "um";
    RADIAL_CHOKE_INPUT_width1 = atof(cmd->data.list[1])*RADIAL_CHOKE_INPUT_width1_unit;
    
    RADIAL_CHOKE_INPUT_width2_unit = Micron;
    RADIAL_CHOKE_INPUT_width2_unitStr = "um";
    RADIAL_CHOKE_INPUT_width2 = atof(cmd->data.list[2])*RADIAL_CHOKE_INPUT_width2_unit;
    
    return NULL;
}

DOTCONF_CB(radial_choke_fan)
{
    RADIAL_CHOKE_FAN_radius_unit = Micron;
    RADIAL_CHOKE_FAN_radius_unitStr = "um";
    RADIAL_CHOKE_FAN_radius = atof(cmd->data.list[0])*TRIPLE_STUB_1_FAN_radius_unit;
    
    RADIAL_CHOKE_FAN_angle_unit = Degree;
    RADIAL_CHOKE_FAN_angle_unitStr = "degrees";
    RADIAL_CHOKE_FAN_angle = atof(cmd->data.list[1])*TRIPLE_STUB_1_FAN_angle_unit;
    
    return NULL;
}

DOTCONF_CB(radial_choke_output)
{
    RADIAL_CHOKE_OUTPUT_type = cmd->data.list[0];
    
    if (RADIAL_CHOKE_OUTPUT_type == "TERMINATED") 
    {
    } 
    else 
    {
        RADIAL_CHOKE_OUTPUT_type = "MSTRIP";
        RADIAL_CHOKE_OUTPUT_length_unitStr = "um";
        RADIAL_CHOKE_OUTPUT_length_unit = Micron;
        RADIAL_CHOKE_OUTPUT_length = atof(cmd->data.list[0])*Micron;
        RADIAL_CHOKE_OUTPUT_width_unitStr = "um";
        RADIAL_CHOKE_OUTPUT_width_unit = Micron;
        RADIAL_CHOKE_OUTPUT_width = atof(cmd->data.list[1])*Micron;
    }   

    return NULL;
}

// IF Circuit
DOTCONF_CB(if_finline_s2p)
{
    IF_FINLINE_S2P = cmd->data.str;
    return NULL;
}

DOTCONF_CB(if_connect_s2p)
{
    IF_CONNECT_S2P = cmd->data.str;
    return NULL;
}

DOTCONF_CB(if_finline_out)
{
    IF_FINLINE_OUT_length_unit = Micron;
    IF_FINLINE_OUT_length_unitStr = "um";
    IF_FINLINE_OUT_length = atof(cmd->data.list[0])*IF_FINLINE_OUT_length_unit;
    
    IF_FINLINE_OUT_width_unit = Micron;
    IF_FINLINE_OUT_width_unitStr = "um";
    IF_FINLINE_OUT_width = atof(cmd->data.list[1])*IF_FINLINE_OUT_width_unit;

    return NULL;
}

DOTCONF_CB(if_feed_term)
{
    IF_FEED_TERM_unit = Ohm;
    IF_FEED_TERM_unitStr = "Ohm";
    IF_FEED_TERM = atof(cmd->data.list[0])*IF_FEED_TERM_unit;
 
    return NULL;
}

// Default Materials
DOTCONF_CB(top_default)
{
    TOP_DEFAULT = cmd->data.str;
    return NULL;
}

DOTCONF_CB(top_thickness)
{
    TOP_THICKNESS_unit = Angstrom;
    TOP_THICKNESS_unitStr = "A";
    TOP_THICKNESS = atof(cmd->data.list[0])*TOP_THICKNESS_unit;
 
    return NULL;
}

DOTCONF_CB(ground_default)
{
    GROUND_DEFAULT = cmd->data.str;
    return NULL;
}

DOTCONF_CB(ground_thickness)
{
    GROUND_THICKNESS_unit = Angstrom;
    GROUND_THICKNESS_unitStr = "A";
    GROUND_THICKNESS = atof(cmd->data.list[0])*GROUND_THICKNESS_unit;
 
    return NULL;
}

DOTCONF_CB(insul_default)
{
    INSUL_DEFAULT = cmd->data.str;
    return NULL;
}

DOTCONF_CB(insul_thickness)
{
    INSUL_THICKNESS_unit = Angstrom;
    INSUL_THICKNESS_unitStr = "A";
    INSUL_THICKNESS = atof(cmd->data.list[0])*INSUL_THICKNESS_unit;
 
    return NULL;
}

DOTCONF_CB(cover_default)
{
    COVER_DEFAULT = cmd->data.str;
    return NULL;
}

DOTCONF_CB(cover_thickness)
{
    COVER_THICKNESS_unit = Angstrom;
    COVER_THICKNESS_unitStr = "A";
    COVER_THICKNESS = atof(cmd->data.list[0])*COVER_THICKNESS_unit;
 
    return NULL;
}

// Material properties
DOTCONF_CB(nb_properties)
{
    NB_Vgap = atof(cmd->data.list[0])*mVolt;
    NB_Tc = atof(cmd->data.list[1])*Kelvin;
    NB_Rho_normal = atof(cmd->data.list[2])*Micro*Ohm*Centi*Meter;
    
    return NULL;
}

DOTCONF_CB(nbtin_properties)
{
    NBTIN_Vgap = atof(cmd->data.list[0])*mVolt;
    NBTIN_Tc = atof(cmd->data.list[1])*Kelvin;
    NBTIN_Rho_normal = atof(cmd->data.list[2])*Micro*Ohm*Centi*Meter;
    
    return NULL;
}

DOTCONF_CB(sio_properties)
{
    SIO_epsilon = atof(cmd->data.list[0]);
    SIO_loss_tangent = atof(cmd->data.list[1]);
    
    return NULL;
}

// Fall back catches any unknown keywords  
DOTCONF_CB(cb_fallback)
{
    //int i;
    printf("Fallback handler called for \"%s\". Got %d args\n", cmd->name, cmd->arg_count);
    /*for (i = 0; i < cmd->arg_count; i++)
        printf("\tArg #%d: %s\n", i, cmd->data.list[i]);*/
    return NULL;   
}


