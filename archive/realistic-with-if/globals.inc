/*******************************************************************************
** globals.inc - file included in realistic.c, containing definitions of      **
**               globals required to conrol SuperMix parameters               **
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
**          01/09/03 - Altered from SuperMix_simple version for use with      **
**          SuperMix_realistic.
*******************************************************************************/

// Set in realistic.conf by default

// The mixer configuration file to use
std::string MIXER_CONFIG;

// The output file
std::string OUTPUT;

/*******************************
* Simulation control paramters *
*******************************/

// Operating point variable to scan
//  These will override fixed points below
std::string SCANNED_VARIABLE;
parameter SCAN_LOW;
parameter SCAN_HIGH;
parameter SCAN_STEP;
parameter SCAN_unit;
std::string SCAN_unitStr;

// Fixed operating point parameters
parameter FREQUENCY;
parameter FREQUENCY_unit;
std::string FREQUENCY_unitStr;

parameter IF_FREQ;
parameter IF_FREQ_unit;
std::string IF_FREQ_unitStr;

parameter BIAS;
parameter BIAS_unit;
std::string BIAS_unitStr;

parameter LO_POWER;
parameter LO_POWER_unit;
std::string LO_POWER_unitStr;

parameter Lo_Temp = 0.0*Kelvin;
parameter Load_Temp = 0.0*Kelvin;
/*****************************
* RF Loss control parameters *
*****************************/

// Beamsplitter
parameter BS_EPSILON;
parameter BS_LOSS_TAN;
parameter BS_THICKNESS;
parameter BS_ANGLE;
parameter BS_TEMP;

// Dewar window
parameter DW_EPSILON;
parameter DW_LOSS_TAN;
parameter DW_THICKNESS;
parameter DW_ANGLE;
parameter DW_TEMP;

// Lossy microstrip
parameter LOSSY_INPUT_length;
parameter LOSSY_INPUT_length_unit;
std::string LOSSY_INPUT_length_unitStr;
parameter LOSSY_INPUT_width;
parameter LOSSY_INPUT_width_unit;
std::string LOSSY_INPUT_width_unitStr;

// Misc RF Losses
parameter MISC_ATTEN;
parameter MISC_TEMP;


// End of realistic.conf settings

// Set in mixer.conf by default

/*********************************
* General environment parameters *
*********************************/
parameter TEMP;
parameter TEMP_unit;
std::string TEMP_unitStr;

parameter Z0;
parameter Z0_unit;
std::string Z0_unitStr;

parameter IF_Z0;
parameter IF_Z0_unit;
std::string IF_Z0_unitStr;

int HARMONICS;

/**********************
* Junction parameters *
**********************/
std::string IV_CURVE;
std::string KK_CURVE;

parameter RNA;
parameter RNA_unit;
std::string RNA_unitStr;

parameter SCAP;
parameter SCAP_unit;
std::string SCAP_unitStr;

parameter AREA;
parameter AREA_unit;
std::string AREA_unitStr;

parameter VGAP;
parameter VGAP_unit;
std::string VGAP_unitStr;

/*******************
* Tuner Parameters *
*******************/

std::string TUNER_TYPE;

std::string TOUCHSTONE_FILE;
std::string IF_TOUCHSTONE_FILE;

// Single stub tuner
std::string SINGLE_STUB_TYPE;
parameter SINGLE_STUB_INPUT_length;
parameter SINGLE_STUB_INPUT_length_unit;
std::string SINGLE_STUB_INPUT_length_unitStr;
parameter SINGLE_STUB_INPUT_width1;
parameter SINGLE_STUB_INPUT_width1_unit;
std::string SINGLE_STUB_INPUT_width1_unitStr;
parameter SINGLE_STUB_INPUT_width2;
parameter SINGLE_STUB_INPUT_width2_unit;
std::string SINGLE_STUB_INPUT_width2_unitStr;
parameter SINGLE_STUB_FAN_radius;
parameter SINGLE_STUB_FAN_radius_unit;
std::string SINGLE_STUB_FAN_radius_unitStr;
parameter SINGLE_STUB_FAN_angle;
parameter SINGLE_STUB_FAN_angle_unit;
std::string SINGLE_STUB_FAN_angle_unitStr;

// Double stub tuner, stub 1
std::string DOUBLE_STUB_1_TYPE;
parameter DOUBLE_STUB_1_INPUT_length;
parameter DOUBLE_STUB_1_INPUT_length_unit;
std::string DOUBLE_STUB_1_INPUT_length_unitStr;
parameter DOUBLE_STUB_1_INPUT_width1;
parameter DOUBLE_STUB_1_INPUT_width1_unit;
std::string DOUBLE_STUB_1_INPUT_width1_unitStr;
parameter DOUBLE_STUB_1_INPUT_width2;
parameter DOUBLE_STUB_1_INPUT_width2_unit;
std::string DOUBLE_STUB_1_INPUT_width2_unitStr;
parameter DOUBLE_STUB_1_FAN_radius;
parameter DOUBLE_STUB_1_FAN_radius_unit;
std::string DOUBLE_STUB_1_FAN_radius_unitStr;
parameter DOUBLE_STUB_1_FAN_angle;
parameter DOUBLE_STUB_1_FAN_angle_unit;
std::string DOUBLE_STUB_1_FAN_angle_unitStr;
// Double stub tuner, stub 2
std::string DOUBLE_STUB_2_TYPE;
parameter DOUBLE_STUB_2_INPUT_length;
parameter DOUBLE_STUB_2_INPUT_length_unit;
std::string DOUBLE_STUB_2_INPUT_length_unitStr;
parameter DOUBLE_STUB_2_INPUT_width1;
parameter DOUBLE_STUB_2_INPUT_width1_unit;
std::string DOUBLE_STUB_2_INPUT_width1_unitStr;
parameter DOUBLE_STUB_2_INPUT_width2;
parameter DOUBLE_STUB_2_INPUT_width2_unit;
std::string DOUBLE_STUB_2_INPUT_width2_unitStr;
parameter DOUBLE_STUB_2_FAN_radius;
parameter DOUBLE_STUB_2_FAN_radius_unit;
std::string DOUBLE_STUB_2_FAN_radius_unitStr;
parameter DOUBLE_STUB_2_FAN_angle;
parameter DOUBLE_STUB_2_FAN_angle_unit;
std::string DOUBLE_STUB_2_FAN_angle_unitStr;

// Triple stub tuner, stub 1
std::string TRIPLE_STUB_1_TYPE;
parameter TRIPLE_STUB_1_INPUT_length;
parameter TRIPLE_STUB_1_INPUT_length_unit;
std::string TRIPLE_STUB_1_INPUT_length_unitStr;
parameter TRIPLE_STUB_1_INPUT_width1;
parameter TRIPLE_STUB_1_INPUT_width1_unit;
std::string TRIPLE_STUB_1_INPUT_width1_unitStr;
parameter TRIPLE_STUB_1_INPUT_width2;
parameter TRIPLE_STUB_1_INPUT_width2_unit;
std::string TRIPLE_STUB_1_INPUT_width2_unitStr;
parameter TRIPLE_STUB_1_FAN_radius;
parameter TRIPLE_STUB_1_FAN_radius_unit;
std::string TRIPLE_STUB_1_FAN_radius_unitStr;
parameter TRIPLE_STUB_1_FAN_angle;
parameter TRIPLE_STUB_1_FAN_angle_unit;
std::string TRIPLE_STUB_1_FAN_angle_unitStr;
// Triple stub tuner, stub 2
std::string TRIPLE_STUB_2_TYPE;
parameter TRIPLE_STUB_2_INPUT_length;
parameter TRIPLE_STUB_2_INPUT_length_unit;
std::string TRIPLE_STUB_2_INPUT_length_unitStr;
parameter TRIPLE_STUB_2_INPUT_width1;
parameter TRIPLE_STUB_2_INPUT_width1_unit;
std::string TRIPLE_STUB_2_INPUT_width1_unitStr;
parameter TRIPLE_STUB_2_INPUT_width2;
parameter TRIPLE_STUB_2_INPUT_width2_unit;
std::string TRIPLE_STUB_2_INPUT_width2_unitStr;
parameter TRIPLE_STUB_2_FAN_radius;
parameter TRIPLE_STUB_2_FAN_radius_unit;
std::string TRIPLE_STUB_2_FAN_radius_unitStr;
parameter TRIPLE_STUB_2_FAN_angle;
parameter TRIPLE_STUB_2_FAN_angle_unit;
std::string TRIPLE_STUB_2_FAN_angle_unitStr;
// Triple stub tuner, stub 3
std::string TRIPLE_STUB_3_TYPE;
parameter TRIPLE_STUB_3_INPUT_length;
parameter TRIPLE_STUB_3_INPUT_length_unit;
std::string TRIPLE_STUB_3_INPUT_length_unitStr;
parameter TRIPLE_STUB_3_INPUT_width1;
parameter TRIPLE_STUB_3_INPUT_width1_unit;
std::string TRIPLE_STUB_3_INPUT_width1_unitStr;
parameter TRIPLE_STUB_3_INPUT_width2;
parameter TRIPLE_STUB_3_INPUT_width2_unit;
std::string TRIPLE_STUB_3_INPUT_width2_unitStr;
parameter TRIPLE_STUB_3_FAN_radius;
parameter TRIPLE_STUB_3_FAN_radius_unit;
std::string TRIPLE_STUB_3_FAN_radius_unitStr;
parameter TRIPLE_STUB_3_FAN_angle;
parameter TRIPLE_STUB_3_FAN_angle_unit;
std::string TRIPLE_STUB_3_FAN_angle_unitStr;

// Belitsky tuner
parameter BELITSKY_INPUT_STRIP_length;
parameter BELITSKY_INPUT_STRIP_length_unit;
std::string BELITSKY_INPUT_STRIP_length_unitStr;
parameter BELITSKY_INPUT_STRIP_width;
parameter BELITSKY_INPUT_STRIP_width_unit;
std::string BELITSKY_INPUT_STRIP_width_unitStr;
parameter BELITSKY_TUNING_STRIP_length;
parameter BELITSKY_TUNING_STRIP_length_unit;
std::string BELITSKY_TUNING_STRIP_length_unitStr;
parameter BELITSKY_TUNING_STRIP_width;
parameter BELITSKY_TUNING_STRIP_width_unit;
std::string BELITSKY_TUNING_STRIP_width_unitStr;

// RF Choke
std::string RF_CHOKE_TYPE;
std::string RADIAL_CHOKE_TYPE;
parameter RADIAL_CHOKE_INPUT_length;
parameter RADIAL_CHOKE_INPUT_length_unit;
std::string RADIAL_CHOKE_INPUT_length_unitStr;
parameter RADIAL_CHOKE_INPUT_width1;
parameter RADIAL_CHOKE_INPUT_width1_unit;
std::string RADIAL_CHOKE_INPUT_width1_unitStr;
parameter RADIAL_CHOKE_INPUT_width2;
parameter RADIAL_CHOKE_INPUT_width2_unit;
std::string RADIAL_CHOKE_INPUT_width2_unitStr;
parameter RADIAL_CHOKE_FAN_radius;
parameter RADIAL_CHOKE_FAN_radius_unit;
std::string RADIAL_CHOKE_FAN_radius_unitStr;
parameter RADIAL_CHOKE_FAN_angle;
parameter RADIAL_CHOKE_FAN_angle_unit;
std::string RADIAL_CHOKE_FAN_angle_unitStr;
std::string RADIAL_CHOKE_OUTPUT_type;
parameter RADIAL_CHOKE_OUTPUT_length;
parameter RADIAL_CHOKE_OUTPUT_length_unit;
std::string RADIAL_CHOKE_OUTPUT_length_unitStr;
parameter RADIAL_CHOKE_OUTPUT_width;
parameter RADIAL_CHOKE_OUTPUT_width_unit;
std::string RADIAL_CHOKE_OUTPUT_width_unitStr;

std::string IF_FINLINE_S2P;
std::string IF_CONNECT_S2P;
parameter IF_FINLINE_OUT_width;
parameter IF_FINLINE_OUT_width_unit;
std::string IF_FINLINE_OUT_width_unitStr;
parameter IF_FINLINE_OUT_length;
parameter IF_FINLINE_OUT_length_unit;
std::string IF_FINLINE_OUT_length_unitStr;

parameter IF_FEED_TERM;
parameter IF_FEED_TERM_unit;
std::string IF_FEED_TERM_unitStr;


/********************
* Default Materials *
********************/

// Top metallisation
std::string TOP_DEFAULT;
parameter TOP_THICKNESS;
parameter TOP_THICKNESS_unit;
std::string TOP_THICKNESS_unitStr;

// GROUND metallisation
std::string GROUND_DEFAULT;
parameter GROUND_THICKNESS;
parameter GROUND_THICKNESS_unit;
std::string GROUND_THICKNESS_unitStr;

// INSUL metallisation
std::string INSUL_DEFAULT;
parameter INSUL_THICKNESS;
parameter INSUL_THICKNESS_unit;
std::string INSUL_THICKNESS_unitStr;

// COVER metallisation
std::string COVER_DEFAULT;
parameter COVER_THICKNESS;
parameter COVER_THICKNESS_unit;
std::string COVER_THICKNESS_unitStr;

/**********************
* Material Properties *
**********************/

// Niobium
parameter NB_Vgap;
parameter NB_Tc;
parameter NB_Rho_normal;

// Niobium Titanium Nitride
parameter NBTIN_Vgap;
parameter NBTIN_Tc;
parameter NBTIN_Rho_normal;

// SiO
parameter SIO_epsilon;
parameter SIO_loss_tangent;

// Air
parameter AIR_epsilon = 1.0;
parameter AIR_loss_tangent = 0.0;

/*************************************************
* Other useful globals, not defined in conf file *
*************************************************/

int USB_PORT;
int LSB_PORT;
int IF_PORT;


