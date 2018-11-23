/*******************************************************************************
** confparse.h - file included in balanced.h, containing code to parse the    **
**               .conf files, setting the globals created in globals.h        **
** ========================================================================== **
** Author - Paul Grimes                                                       **
** Date   - 15/10/04     - created from SuperMix-realistic version            **
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
static DOTCONF_CB(scan_low);
static DOTCONF_CB(scan_high);
static DOTCONF_CB(scan_step);
static DOTCONF_CB(frequency);
static DOTCONF_CB(bias_1);
static DOTCONF_CB(bias_2);
static DOTCONF_CB(lo_power);
static DOTCONF_CB(lo_temp);


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
static DOTCONF_CB(area_1a);
static DOTCONF_CB(area_1b);
static DOTCONF_CB(area_2a);
static DOTCONF_CB(area_2b);
static DOTCONF_CB(vgap);
    

// RF circuit parameters
    // Finline Inputs
static DOTCONF_CB(finline_width);
static DOTCONF_CB(finline_length);

    // RF Hybrid
static DOTCONF_CB(rf_hybrid);
static DOTCONF_CB(external_rf_hybrid_filename);
static DOTCONF_CB(rf_hybrid_z0);

    // Connection strip 1
static DOTCONF_CB(connect1_width);
static DOTCONF_CB(connect1_length);

    // DC/IF blocking capacitor
static DOTCONF_CB(dc_block_cap);

    // Connection strip 2
static DOTCONF_CB(connect2_width);
static DOTCONF_CB(connect2_length);
    
    // Tuner parameters
static DOTCONF_CB(input_strip_width);
static DOTCONF_CB(input_strip_length);

static DOTCONF_CB(tuning_strip_width);
static DOTCONF_CB(tuning_strip_length);
static DOTCONF_CB(tuning_strip_thickness);

    // RF choke
static DOTCONF_CB(radial_choke_width);
static DOTCONF_CB(radial_choke_length);
static DOTCONF_CB(radial_choke_radius);
static DOTCONF_CB(radial_choke_angle);


// IF circuit parameters
    // IF output strip
static DOTCONF_CB(if_output_width);
static DOTCONF_CB(if_output_length);

    // IF connection board
static DOTCONF_CB(if_connection_width);
static DOTCONF_CB(if_connection_length_1);
static DOTCONF_CB(if_connection_length_2);

    // IF hybrid
static DOTCONF_CB(if_hybrid);
static DOTCONF_CB(external_if_hybrid_filename);

    // IF output
static DOTCONF_CB(if_hybrid_out_z0);
static DOTCONF_CB(if_amp_z0);

    // Bias resistance
static DOTCONF_CB(bias_r);

// Layers
static DOTCONF_CB(wiring_1);
static DOTCONF_CB(wiring_1_thickness);
static DOTCONF_CB(ground_1);
static DOTCONF_CB(ground_1_thickness);
static DOTCONF_CB(wiring_2);
static DOTCONF_CB(wiring_2_thickness);
static DOTCONF_CB(ground_2);
static DOTCONF_CB(ground_2_thickness);

static DOTCONF_CB(insulation);
static DOTCONF_CB(insul_thickness);
static DOTCONF_CB(cover);
static DOTCONF_CB(cover_thickness);

// Material properties
static DOTCONF_CB(nb_vgap);
static DOTCONF_CB(nb_tc);
static DOTCONF_CB(nb_rho_normal);

static DOTCONF_CB(nbtin_vgap);
static DOTCONF_CB(nbtin_tc);
static DOTCONF_CB(nbtin_rho_normal);

static DOTCONF_CB(sio_epsilon);
static DOTCONF_CB(sio_loss_tangent);

static DOTCONF_CB(if_wiring_rho);
static DOTCONF_CB(if_wiring_thickness);

static DOTCONF_CB(if_insul_epsilon);
static DOTCONF_CB(if_insul_loss_tangent);
static DOTCONF_CB(if_insul_thickness);

// Create the mapping of .conf keywords to functions
static const configoption_t options[] = {
    // General parameters
    {"MIXER_CONFIG",        ARG_STR,    mixer_config,       NULL,   CTX_ALL },
    {"OUTPUT",              ARG_STR,    output,             NULL,   CTX_ALL },
    // Simulation control parameters
    {"SCANNED_VARIABLE",    ARG_STR,    scanned_variable,   NULL,   CTX_ALL },
    {"SCAN_LOW",            ARG_LIST,   scan_low,           NULL,   CTX_ALL },
    {"SCAN_HIGH",           ARG_LIST,   scan_high,          NULL,   CTX_ALL },
    {"SCAN_STEP",           ARG_LIST,   scan_step,          NULL,   CTX_ALL },
    {"FREQUENCY",           ARG_LIST,   frequency,          NULL,   CTX_ALL },
    {"BIAS_1",              ARG_LIST,   bias_1,             NULL,   CTX_ALL },
    {"BIAS_2",              ARG_LIST,   bias_2,             NULL,   CTX_ALL },
    {"LO_POWER",            ARG_LIST,   lo_power,           NULL,   CTX_ALL },
    {"LO_TEMP",             ARG_LIST,   lo_temp,            NULL,   CTX_ALL },
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
    {"AREA_1a",             ARG_LIST,   area_1a,            NULL,   CTX_ALL },
    {"AREA_1b",             ARG_LIST,   area_1b,            NULL,   CTX_ALL },
    {"AREA_2a",             ARG_LIST,   area_2a,            NULL,   CTX_ALL },
    {"AREA_2b",             ARG_LIST,   area_2b,            NULL,   CTX_ALL },
    {"VGAP",                ARG_LIST,   vgap,               NULL,   CTX_ALL },
    // RF circuit
        // Finline inputs
    {"FINLINE_WIDTH",       ARG_LIST,   finline_width,      NULL,   CTX_ALL },
    {"FINLINE_LENGTH",      ARG_LIST,   finline_length,     NULL,   CTX_ALL },
        // RF Hybrid
    {"RF_HYBRID",           ARG_STR,    rf_hybrid,          NULL,   CTX_ALL },
    {"RF_HYBRID_FILENAME", ARG_STR, external_rf_hybrid_filename, NULL, CTX_ALL },
    {"RF_HYBRID_Z0",        ARG_LIST,   rf_hybrid_z0,       NULL,   CTX_ALL },
        // Connection strip 1
    {"CONNECT1_WIDTH",      ARG_LIST,  connect1_width,      NULL,   CTX_ALL },
    {"CONNECT1_LENGTH",      ARG_LIST,  connect1_length,     NULL,   CTX_ALL },
        // DC Blocking capacitor
    {"DC_BLOCK_CAP",        ARG_LIST,  dc_block_cap,        NULL,   CTX_ALL },
        // Connection strip 2
    {"CONNECT2_WIDTH",      ARG_LIST,  connect2_width,      NULL,   CTX_ALL },
    {"CONNECT2_LENGTH",     ARG_LIST,  connect2_length,     NULL,   CTX_ALL },
        // Tuner input strip
    {"INPUT_WIDTH",         ARG_LIST,  input_strip_width,   NULL,   CTX_ALL },
    {"INPUT_LENGTH",        ARG_LIST,  input_strip_length,  NULL,   CTX_ALL },
        // Tuning strip
    {"TUNING_WIDTH",        ARG_LIST,  tuning_strip_width,  NULL,   CTX_ALL },
    {"TUNING_LENGTH",       ARG_LIST,  tuning_strip_length, NULL,   CTX_ALL },
    {"TUNING_THICKNESS",   ARG_LIST, tuning_strip_thickness, NULL,  CTX_ALL },
        // RF choke
    {"RADIAL_CHOKE_WIDTH",  ARG_LIST,  radial_choke_width,  NULL,   CTX_ALL },
    {"RADIAL_CHOKE_LENGTH", ARG_LIST,  radial_choke_length, NULL,   CTX_ALL },
    {"RADIAL_CHOKE_ANGLE",  ARG_LIST,  radial_choke_angle,  NULL,   CTX_ALL },
    {"RADIAL_CHOKE_RADIUS", ARG_LIST,  radial_choke_radius, NULL,   CTX_ALL },
    // IF circuit
        // IF output strip
    {"IF_OUTPUT_WIDTH",     ARG_LIST,  if_output_width, NULL, CTX_ALL },
    {"IF_OUTPUT_LENGTH",    ARG_LIST, if_output_length, NULL, CTX_ALL },
        // IF connection strip
    {"IF_CONNECT_WIDTH",    ARG_LIST,  if_connection_width,  NULL,  CTX_ALL },
    {"IF_CONNECT_LENGTH_1", ARG_LIST, if_connection_length_1, NULL, CTX_ALL },
    {"IF_CONNECT_LENGTH_2", ARG_LIST, if_connection_length_2, NULL, CTX_ALL },
        // IF hybrid
    {"IF_HYBRID",           ARG_STR,    if_hybrid,          NULL,   CTX_ALL },
    {"IF_HYBRID_FILENAME", ARG_STR, external_if_hybrid_filename, NULL, CTX_ALL },
        // IF output
    {"IF_HYBRID_OUT_Z0",    ARG_LIST,  if_hybrid_out_z0,    NULL,   CTX_ALL },
    {"IF_AMP_Z0",           ARG_LIST,  if_amp_z0,           NULL,   CTX_ALL },
    {"BIAS_R",              ARG_LIST,  bias_r,              NULL,   CTX_ALL },
    
    // Default materials
    {"WIRING_1",            ARG_STR,    wiring_1,           NULL,   CTX_ALL },
    {"WIRING_1_THICKNESS",  ARG_LIST,   wiring_1_thickness, NULL,   CTX_ALL },
    {"GROUND_1",            ARG_STR,    ground_1,           NULL,   CTX_ALL },
    {"GROUND_1_THICKNESS",  ARG_LIST,   ground_1_thickness, NULL,   CTX_ALL },
    {"WIRING_2",            ARG_STR,    wiring_2,           NULL,   CTX_ALL },
    {"WIRING_2_THICKNESS",  ARG_LIST,   wiring_2_thickness, NULL,   CTX_ALL },
    {"GROUND_2",            ARG_STR,    ground_2,           NULL,   CTX_ALL },
    {"GROUND_2_THICKNESS",  ARG_LIST,   ground_2_thickness, NULL,   CTX_ALL },
    {"INSULATION",          ARG_STR,    insulation,         NULL,   CTX_ALL },
    {"INSUL_THICKNESS",     ARG_LIST,   insul_thickness,    NULL,   CTX_ALL },
    {"COVER",               ARG_STR,    cover,              NULL,   CTX_ALL },
    {"COVER_THICKNESS",     ARG_LIST,   cover_thickness,    NULL,   CTX_ALL },
    // Material Properties
    {"NB_VGAP",             ARG_LIST,   nb_vgap,            NULL,   CTX_ALL },
    {"NB_TC",               ARG_LIST,   nb_tc,              NULL,   CTX_ALL },
    {"NB_RHO_NORMAL",       ARG_LIST,   nb_rho_normal,      NULL,   CTX_ALL },
    {"NBTIN_VGAP",          ARG_LIST,   nbtin_vgap,         NULL,   CTX_ALL },
    {"NBTIN_TC",            ARG_LIST,   nbtin_tc,           NULL,   CTX_ALL },
    {"NBTIN_RHO_NORMAL",    ARG_LIST,   nbtin_rho_normal,   NULL,   CTX_ALL },
    {"SIO_EPSILON",         ARG_LIST,   sio_epsilon,        NULL,   CTX_ALL },
    {"SIO_TAND",            ARG_LIST,   sio_loss_tangent,   NULL,   CTX_ALL },
        // IF board materials
    {"IF_WIRING_RHO",       ARG_LIST,   if_wiring_rho,      NULL,   CTX_ALL },
    {"IF_WIRING_THICKNESS", ARG_LIST,  if_wiring_thickness, NULL,   CTX_ALL },
    {"IF_INSUL_EPSILON",    ARG_LIST,   if_insul_epsilon,   NULL,   CTX_ALL },
    {"IF_INSUL_TAND",       ARG_LIST,  if_insul_loss_tangent, NULL, CTX_ALL },
    {"IF_INSUL_THICKNESS",       ARG_LIST,  if_insul_thickness, NULL, CTX_ALL },    
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
    FREQUENCY = atof(cmd->data.list[0])*GHz;
    return NULL;
}

DOTCONF_CB(bias_1)
{
    BIAS_1 = atof(cmd->data.list[0])*mVolt;
    return NULL;
}

DOTCONF_CB(bias_2)
{
    BIAS_2 = atof(cmd->data.list[0])*mVolt;
    return NULL;
}


DOTCONF_CB(lo_power)
{
    LO_POWER = atof(cmd->data.list[0])*Nano*Watt;
    return NULL;
}

DOTCONF_CB(lo_temp)
{
    LO_NOISE_TEMP = atof(cmd->data.list[0])*Kelvin;
    return NULL;
}

// Mixer setup parameters
    // Environment parameters

DOTCONF_CB(temp)
{
    TEMP = atof(cmd->data.list[0])*Kelvin;
    return NULL;
}

DOTCONF_CB(z0)
{
    Z0 = atof(cmd->data.list[0])*Ohm;
    return NULL;
}

DOTCONF_CB(if_freq)
{
    IF_FREQ = atof(cmd->data.list[0])*GHz;
    return NULL;
}

DOTCONF_CB(if_z0)
{
    IF_Z0 = atof(cmd->data.list[0])*Ohm;
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
    RNA = atof(cmd->data.list[0])*Ohm*Micron*Micron;
    return NULL;
}

DOTCONF_CB(scap)
{
    SCAP = atof(cmd->data.list[0])*fFarad/Micron/Micron;    
    return NULL;
}

DOTCONF_CB(area_1a)
{
    AREA_1a = atof(cmd->data.list[0])*Micron*Micron;
    return NULL;
}

DOTCONF_CB(area_1b)
{
    AREA_1b = atof(cmd->data.list[0])*Micron*Micron;
    return NULL;
}

DOTCONF_CB(area_2a)
{
    AREA_2a = atof(cmd->data.list[0])*Micron*Micron;
    return NULL;
}

DOTCONF_CB(area_2b)
{
    AREA_2b = atof(cmd->data.list[0])*Micron*Micron;
    return NULL;
}


DOTCONF_CB(vgap)
{
    VGAP = atof(cmd->data.list[0])*mVolt;
    return NULL;
}

// RF circuit parameters
DOTCONF_CB(finline_width)
{
    FINLINE_WIDTH = atof(cmd->data.list[0])*Micron;
    return NULL;
}

DOTCONF_CB(finline_length)
{
    FINLINE_LENGTH = atof(cmd->data.list[0])*Micron;
    return NULL;
}

DOTCONF_CB(rf_hybrid)
{
    RF_HYBRID = cmd->data.str;
    return NULL;
}

DOTCONF_CB(external_rf_hybrid_filename)
{
    EXTERNAL_RF_HYBRID_FILENAME = cmd->data.str;
    return NULL;
}

DOTCONF_CB(rf_hybrid_z0)
{
    RF_HYBRID_Z0 = atof(cmd->data.list[0])*Ohm;
    return NULL;
}

DOTCONF_CB(connect1_width)
{
    CONNECT_STRIP1_WIDTH = atof(cmd->data.list[0])*Micron;
    return NULL;
}

DOTCONF_CB(connect1_length)
{
    CONNECT_STRIP1_LENGTH = atof(cmd->data.list[0])*Micron;
    return NULL;
}

DOTCONF_CB(dc_block_cap)
{
    DC_BLOCK_CAP = atof(cmd->data.list[0])*fFarad;
    return NULL;
}

DOTCONF_CB(connect2_width)
{
    CONNECT_STRIP2_WIDTH = atof(cmd->data.list[0])*Micron;
    return NULL;
}

DOTCONF_CB(connect2_length)
{
    CONNECT_STRIP2_LENGTH = atof(cmd->data.list[0])*Micron;
    return NULL;
}

DOTCONF_CB(input_strip_width)
{
    INPUT_STRIP_WIDTH = atof(cmd->data.list[0])*Micron;
    return NULL;
}

DOTCONF_CB(input_strip_length)
{
    INPUT_STRIP_LENGTH = atof(cmd->data.list[0])*Micron;
    return NULL;
}

DOTCONF_CB(tuning_strip_width)
{
    TUNING_STRIP_WIDTH = atof(cmd->data.list[0])*Micron;
    return NULL;
}

DOTCONF_CB(tuning_strip_length)
{
    TUNING_STRIP_LENGTH = atof(cmd->data.list[0])*Micron;
    return NULL;
}

DOTCONF_CB(tuning_strip_thickness)
{
    TUNING_STRIP_THICKNESS = atof(cmd->data.list[0])*Micron;
    return NULL;
}


DOTCONF_CB(radial_choke_width)
{
    RF_CHOKE_WIDTH = atof(cmd->data.list[0])*Micron;
    return NULL;
}

DOTCONF_CB(radial_choke_length)
{
    RF_CHOKE_LENGTH = atof(cmd->data.list[0])*Micron;
    return NULL;
}

DOTCONF_CB(radial_choke_radius)
{
    RF_CHOKE_RADIUS = atof(cmd->data.list[0])*Micron;
    return NULL;
}

DOTCONF_CB(radial_choke_angle)
{
    RF_CHOKE_ANGLE = atof(cmd->data.list[0])*Degree;
    return NULL;
}

// IF Circuit
DOTCONF_CB(if_output_width)
{
    IF_OUTPUT_WIDTH = atof(cmd->data.list[0])*Micron;
    return NULL;
}

DOTCONF_CB(if_output_length)
{
    IF_OUTPUT_LENGTH = atof(cmd->data.list[0])*Micron;
    return NULL;
}

DOTCONF_CB(if_connection_width)
{
    IF_CONNECTION_WIDTH = atof(cmd->data.list[0])*Micron;
    return NULL;
}

DOTCONF_CB(if_connection_length_1)
{
    IF_CONNECTION_LENGTH_1 = atof(cmd->data.list[0])*Micron;
    return NULL;
}

DOTCONF_CB(if_connection_length_2)
{
    IF_CONNECTION_LENGTH_2 = atof(cmd->data.list[0])*Micron;
    return NULL;
}

DOTCONF_CB(if_hybrid)
{
    IF_HYBRID = cmd->data.str;
    return NULL;
}

DOTCONF_CB(external_if_hybrid_filename)
{
    EXTERNAL_IF_HYBRID_FILENAME = cmd->data.str;
    return NULL;
}

DOTCONF_CB(if_hybrid_out_z0)
{
    IF_HYBRID_OUT_Z0 = atof(cmd->data.list[0])*Ohm;
    return NULL;
}

DOTCONF_CB(if_amp_z0)
{
    IF_AMP_Z0 = atof(cmd->data.list[0])*Ohm;
    return NULL;
}

DOTCONF_CB(bias_r)
{
    BIAS_R = atof(cmd->data.list[0])*Ohm;
    return NULL;
}


// Default Materials
DOTCONF_CB(wiring_1)
{
    WIRING_1 = cmd->data.str;
    return NULL;
}

DOTCONF_CB(wiring_1_thickness)
{
    WIRING_1_THICKNESS = atof(cmd->data.list[0])*Micron;
    return NULL;
}

DOTCONF_CB(ground_1)
{
    GROUND_1= cmd->data.str;
    return NULL;
}

DOTCONF_CB(ground_1_thickness)
{
    GROUND_1_THICKNESS = atof(cmd->data.list[0])*Micron;
    return NULL;
}

DOTCONF_CB(wiring_2)
{
    WIRING_2 = cmd->data.str;
    return NULL;
}

DOTCONF_CB(wiring_2_thickness)
{
    WIRING_2_THICKNESS = atof(cmd->data.list[0])*Micron;
    return NULL;
}

DOTCONF_CB(ground_2)
{
    GROUND_2= cmd->data.str;
    return NULL;
}

DOTCONF_CB(ground_2_thickness)
{
    GROUND_2_THICKNESS = atof(cmd->data.list[0])*Micron;
    return NULL;
}

DOTCONF_CB(insulation)
{
    INSULATION = cmd->data.str;
    return NULL;
}

DOTCONF_CB(insul_thickness)
{
    INSUL_THICKNESS = atof(cmd->data.list[0])*Micron;
    return NULL;
}

DOTCONF_CB(cover)
{
    COVER = cmd->data.str;
    return NULL;
}

DOTCONF_CB(cover_thickness)
{
    COVER_THICKNESS = atof(cmd->data.list[0])*Micron;
    return NULL;
}

// Material properties
DOTCONF_CB(nb_vgap)
{
    NB_Vgap = atof(cmd->data.list[0])*mVolt;    
    return NULL;
}

DOTCONF_CB(nb_tc)
{
    NB_Tc = atof(cmd->data.list[0])*Kelvin;
    return NULL;
}

DOTCONF_CB(nb_rho_normal)
{
    NB_Rho_normal = atof(cmd->data.list[0])*Micro*Ohm*Centi*Meter;
    return NULL;
}

DOTCONF_CB(nbtin_vgap)
{
    NBTIN_Vgap = atof(cmd->data.list[0])*mVolt;    
    return NULL;
}

DOTCONF_CB(nbtin_tc)
{
    NBTIN_Tc = atof(cmd->data.list[0])*Kelvin;
    return NULL;
}

DOTCONF_CB(nbtin_rho_normal)
{
    NBTIN_Rho_normal = atof(cmd->data.list[0])*Micro*Ohm*Centi*Meter;
    return NULL;
}

DOTCONF_CB(sio_epsilon)
{
    SIO_epsilon = atof(cmd->data.list[0]);
    return NULL;
}

DOTCONF_CB(sio_loss_tangent)
{
    SIO_loss_tangent = atof(cmd->data.list[0]);
    return NULL;
}

DOTCONF_CB(if_wiring_rho)
{
    IF_WIRING_rho = atof(cmd->data.list[0])*Micro*Ohm*Centi*Meter;
    return NULL;
}

DOTCONF_CB(if_wiring_thickness)
{
    IF_WIRING_thick = atof(cmd->data.list[0])*Micron;
    return NULL;
}


DOTCONF_CB(if_insul_epsilon)
{
    IF_INSUL_epsilon = atof(cmd->data.list[0]);
    return NULL;
}

DOTCONF_CB(if_insul_loss_tangent)
{
    IF_INSUL_loss_tangent = atof(cmd->data.list[0]);
    return NULL;
}

DOTCONF_CB(if_insul_thickness)
{
    IF_INSUL_thickness = atof(cmd->data.list[0]);
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


