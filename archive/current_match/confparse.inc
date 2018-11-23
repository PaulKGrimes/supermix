/*******************************************************************************
** globals.inc - file included in curr_match.c, containing code to parse the  **
**              conf files, setting the globals created in globals.inc        **
** ========================================================================== **
** Author - Paul Grimes                                                       **
** Date   - 18/07/03                                                          **
** ========================================================================== **
** SuperMix parameters are currently created as globals, so that dotconf      **
** functions that interpret each line in the .conf file can alter them as     **
** side effects.                                                              **
**                                                                            **
** We intend to use all caps for global parameter names, and camelback for    **
** SuperMix internal parameter names                                          **
** ========================================================================== **
** Changes:                                                                   **
**      06/08/03 - Altered from SuperMix_simple form to that required for     **
**                 curr_match.c                                               **
*******************************************************************************/


DOTCONF_CB(cb_fallback);

// Declare each of the .conf parsing functions
static DOTCONF_CB(mixer_config);
static DOTCONF_CB(output);
// Simulation control parameters
static DOTCONF_CB(bias_low);
static DOTCONF_CB(bias_high);
static DOTCONF_CB(bias_points);
static DOTCONF_CB(pumped_curve);

// Mixer setup parameters
    // Environment parameters
static DOTCONF_CB(temp);
static DOTCONF_CB(z0);
static DOTCONF_CB(if_freq); 
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
    {"BIAS_LOW",            ARG_LIST,   bias_low,           NULL,   CTX_ALL },
    {"BIAS_HIGH",           ARG_LIST,   bias_high,          NULL,   CTX_ALL },
    {"BIAS_POINTS",         ARG_INT,    bias_points,        NULL,   CTX_ALL },
    {"PUMPED_CURVE",        ARG_LIST,   pumped_curve,       NULL,   CTX_ALL },
    // Mixer setup parameters
    // General enivronment parameters
    {"TEMP",                ARG_LIST,   temp,               NULL,   CTX_ALL },
    {"Z0",                  ARG_LIST,   z0,                 NULL,   CTX_ALL },
    {"IF_FREQ",             ARG_LIST,   if_freq,            NULL,   CTX_ALL },
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

DOTCONF_CB(bias_low)
{
    BIAS_LOW_unitStr = cmd->data.list[1];
    
    if (BIAS_LOW_unitStr == "mV") {BIAS_LOW_unit = mVolt;}
    else {BIAS_LOW_unit = 1.0;}
    
    BIAS_LOW = atof(cmd->data.list[0])*BIAS_LOW_unit;
    
    return NULL;
}

DOTCONF_CB(bias_high)
{
    BIAS_HIGH_unitStr = cmd->data.list[1];
    
    if (BIAS_HIGH_unitStr == "mV") {BIAS_HIGH_unit = mVolt;}
    else {BIAS_HIGH_unit = 1.0;}
    
    BIAS_HIGH = atof(cmd->data.list[0])*BIAS_HIGH_unit;
    
    return NULL;
}

DOTCONF_CB(bias_points)
{
    BIAS_POINTS = cmd->data.value;
    
    return NULL;
}

DOTCONF_CB(pumped_curve)
{
    std::string file = cmd->data.list[0];

    parameter freq = atof(cmd->data.list[1])*GHz;
    double multi = atof(cmd->data.list[3]);
    
    printf("# PUMPED_CURVE %s %f GHz %f\n", file.c_str(), freq/GHz, multi);
    
    // Create the pump_curve object (locally)
    pump_curve new_pump_curve(mix, Bias_Voltage, &Lo_Power, &Frequency,
                file, freq, Volt, multi*Amp);
    cout << "# Created pump_curve object : " << &new_pump_curve << endl;

    // Push the local pump_curve object onto the global pumped_curves collection
    pumped_curves.push_back(new_pump_curve);

    printf("# Pushed pump_curve object onto vector\n# \n");

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
    
    if (IF_Z0_unitStr == "Ohms") {IF_Z0_unit = GHz;}
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


