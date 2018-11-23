/*******************************************************************************
** globals.inc - file included in simple.c, containing code to parse the conf **
**              files, setting the globals created in globals.inc             **
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
static DOTCONF_CB(bias);
static DOTCONF_CB(lo_power);

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

    // Trafo Tuner
static DOTCONF_CB(trafo1_type);
static DOTCONF_CB(trafo1);
static DOTCONF_CB(trafo2_type);
static DOTCONF_CB(trafo2);
static DOTCONF_CB(trafo3_type);
static DOTCONF_CB(trafo3);

    // RF choke
static DOTCONF_CB(stubin_type);
static DOTCONF_CB(stubin);
static DOTCONF_CB(radial_stub_input);
static DOTCONF_CB(radial_stub_fan);

// Default materials
static DOTCONF_CB(top_default);
static DOTCONF_CB(top_thickness);
static DOTCONF_CB(ground_default);
static DOTCONF_CB(ground_thickness);
static DOTCONF_CB(insul_default);
static DOTCONF_CB(insul_thickness);
static DOTCONF_CB(substrate_default);
static DOTCONF_CB(substrate_thickness);
static DOTCONF_CB(cover_default);
static DOTCONF_CB(cover_thickness);

// Material properties
static DOTCONF_CB(nb_properties);
static DOTCONF_CB(nbtin_properties);
static DOTCONF_CB(sio_properties);
static DOTCONF_CB(si_properties);

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
    {"BIAS",                ARG_LIST,   bias,               NULL,   CTX_ALL },
    {"LO_POWER",            ARG_LIST,   lo_power,           NULL,   CTX_ALL },
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
    // Trafo Tuner
    {"TRAFO1_TYPE",         ARG_STR,    trafo1_type,        NULL,   CTX_ALL },
    {"TRAFO1",              ARG_LIST,   trafo1,             NULL,   CTX_ALL },
    {"TRAFO2_TYPE",         ARG_STR,    trafo2_type,        NULL,   CTX_ALL },
    {"TRAFO2",              ARG_LIST,   trafo2,             NULL,   CTX_ALL },
    {"TRAFO3_TYPE",         ARG_STR,    trafo3_type,        NULL,   CTX_ALL },
    {"TRAFO3",              ARG_LIST,   trafo3,             NULL,   CTX_ALL },
    // RF Choke
    {"STUBIN_TYPE",         ARG_STR,    stubin_type,        NULL,   CTX_ALL },
    {"STUBIN",              ARG_LIST,   stubin,             NULL,   CTX_ALL },
    {"RADIAL_STUB_INPUT",   ARG_LIST,   radial_stub_input,  NULL,   CTX_ALL },
    {"RADIAL_STUB_FAN",     ARG_LIST,   radial_stub_fan,    NULL,   CTX_ALL },
    // Default materials
    {"TOP_DEFAULT",         ARG_STR,    top_default,        NULL,   CTX_ALL },
    {"TOP_THICKNESS",       ARG_LIST,   top_thickness,      NULL,   CTX_ALL },
    {"GROUND_DEFAULT",      ARG_STR,    ground_default,     NULL,   CTX_ALL },
    {"GROUND_THICKNESS",    ARG_LIST,   ground_thickness,   NULL,   CTX_ALL },
    {"INSUL_DEFAULT",       ARG_STR,    insul_default,      NULL,   CTX_ALL },
    {"INSUL_THICKNESS",     ARG_LIST,   insul_thickness,    NULL,   CTX_ALL },
    {"SUBSTRATE_DEFAULT",   ARG_STR,    substrate_default,  NULL,   CTX_ALL },
    {"SUBSTRATE_THICKNESS", ARG_LIST,   substrate_thickness,NULL,   CTX_ALL },
    {"COVER_DEFAULT",       ARG_STR,    cover_default,      NULL,   CTX_ALL },
    {"COVER_THICKNESS",     ARG_LIST,   cover_thickness,    NULL,   CTX_ALL },
    // Material Properties
    {"NB_PROPERTIES",       ARG_LIST,   nb_properties,      NULL,   CTX_ALL },
    {"NBTIN_PROPERTIES",    ARG_LIST,   nbtin_properties,   NULL,   CTX_ALL },
    {"SIO_PROPERTIES",      ARG_LIST,   sio_properties,     NULL,   CTX_ALL },
    {"SI_PROPERTIES",       ARG_LIST,   si_properties,      NULL,   CTX_ALL },
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
// Trafo Tuner

DOTCONF_CB(trafo1_type)
{
    TRAFO1_TYPE = cmd->data.str;
    return NULL;
}

DOTCONF_CB(trafo1)
{
    TRAFO1_length_unit = Micron;
    TRAFO1_length_unitStr = "um";
    TRAFO1_length = atof(cmd->data.list[1])*TRAFO1_length_unit;

    TRAFO1_width_unit = Micron;
    TRAFO1_width_unitStr = "um";
    TRAFO1_width = atof(cmd->data.list[0])*TRAFO1_width_unit;

    TRAFO1_space_unit = Micron;
    TRAFO1_space_unitStr = "um";
    TRAFO1_space = atof(cmd->data.list[2])*TRAFO1_space_unit;

    return NULL;
}

DOTCONF_CB(trafo2_type)
{
    TRAFO2_TYPE = cmd->data.str;
    return NULL;
}

DOTCONF_CB(trafo2)
{
    TRAFO2_length_unit = Micron;
    TRAFO2_length_unitStr = "um";
    TRAFO2_length = atof(cmd->data.list[0])*TRAFO2_length_unit;

    TRAFO2_width_unit = Micron;
    TRAFO2_width_unitStr = "um";
    TRAFO2_width = atof(cmd->data.list[1])*TRAFO2_width_unit;

    TRAFO2_space_unit = Micron;
    TRAFO2_space_unitStr = "um";
    TRAFO2_space = atof(cmd->data.list[2])*TRAFO2_space_unit;

    return NULL;
}


DOTCONF_CB(trafo3_type)
{
    TRAFO3_TYPE = cmd->data.str;
    return NULL;
}

DOTCONF_CB(trafo3)
{
    TRAFO3_length_unit = Micron;
    TRAFO3_length_unitStr = "um";
    TRAFO3_length = atof(cmd->data.list[0])*TRAFO3_length_unit;

    TRAFO3_width_unit = Micron;
    TRAFO3_width_unitStr = "um";
    TRAFO3_width = atof(cmd->data.list[1])*TRAFO3_width_unit;

    TRAFO3_space_unit = Micron;
    TRAFO3_space_unitStr = "um";
    TRAFO3_space = atof(cmd->data.list[2])*TRAFO3_space_unit;

    return NULL;
}

DOTCONF_CB(stubin_type)
{
    STUBIN_TYPE = cmd->data.str;
    return NULL;
}

DOTCONF_CB(stubin)
{
    STUBIN_length_unit = Micron;
    STUBIN_length_unitStr = "um";
    STUBIN_length = atof(cmd->data.list[0])*STUBIN_length_unit;

    STUBIN_width_unit = Micron;
    STUBIN_width_unitStr = "um";
    STUBIN_width = atof(cmd->data.list[1])*STUBIN_width_unit;

    STUBIN_space_unit = Micron;
    STUBIN_space_unitStr = "um";
    STUBIN_space = atof(cmd->data.list[2])*STUBIN_space_unit;

    return NULL;
}


// RF Choke
DOTCONF_CB(radial_stub_input)
{
    RADIAL_STUB_INPUT_length_unit = Micron;
    RADIAL_STUB_INPUT_length_unitStr = "um";
    RADIAL_STUB_INPUT_length = atof(cmd->data.list[0])*RADIAL_STUB_INPUT_length_unit;

    RADIAL_STUB_INPUT_width_unit = Micron;
    RADIAL_STUB_INPUT_width_unitStr = "um";
    RADIAL_STUB_INPUT_width = atof(cmd->data.list[1])*RADIAL_STUB_INPUT_width_unit;

    return NULL;
}

DOTCONF_CB(radial_stub_fan)
{
    RADIAL_STUB_FAN_radius_unit = Micron;
    RADIAL_STUB_FAN_radius_unitStr = "um";
    RADIAL_STUB_FAN_radius = atof(cmd->data.list[0])*RADIAL_STUB_FAN_radius_unit;

    RADIAL_STUB_FAN_angle_unit = Degree;
    RADIAL_STUB_FAN_angle_unitStr = "degrees";
    RADIAL_STUB_FAN_angle = atof(cmd->data.list[1])*RADIAL_STUB_FAN_angle_unit;

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

DOTCONF_CB(substrate_default)
{
    SUBSTRATE_DEFAULT = cmd->data.str;
    return NULL;
}

DOTCONF_CB(substrate_thickness)
{
    SUBSTRATE_THICKNESS_unit = Micron;
    SUBSTRATE_THICKNESS_unitStr = "um";
    SUBSTRATE_THICKNESS = atof(cmd->data.list[0])*SUBSTRATE_THICKNESS_unit;

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

DOTCONF_CB(si_properties)
{
    SI_epsilon = atof(cmd->data.list[0]);
    SI_loss_tangent = atof(cmd->data.list[1]);

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


