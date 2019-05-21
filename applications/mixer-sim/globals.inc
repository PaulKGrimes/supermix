/*******************************************************************************
** globals.inc - file included in simple.c, containing definitions of globals **
**               required to conrol SuperMix parameters                       **
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

// Set in simple.conf by default

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

parameter BIAS;
parameter BIAS_unit;
std::string BIAS_unitStr;

parameter LO_POWER;
parameter LO_POWER_unit;
std::string LO_POWER_unitStr;

parameter Lo_Temp = 0.0*Kelvin;

// End of simple.conf settings

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

parameter IF_FREQ;      // The actual parameter
parameter IF_FREQ_unit; // The value of the parameter's unit
std::string IF_FREQ_unitStr; // The name of the parameter's unit

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
parameter BELITSKY_INPUT_STRIP_thick;
parameter BELITSKY_INPUT_STRIP_thick_unit;
std::string BELITSKY_INPUT_STRIP_thick_unitStr;
parameter BELITSKY_TUNING_STRIP_length;
parameter BELITSKY_TUNING_STRIP_length_unit;
std::string BELITSKY_TUNING_STRIP_length_unitStr;
parameter BELITSKY_TUNING_STRIP_width;
parameter BELITSKY_TUNING_STRIP_width_unit;
std::string BELITSKY_TUNING_STRIP_width_unitStr;
parameter BELITSKY_TUNING_STRIP_thick;
parameter BELITSKY_TUNING_STRIP_thick_unit;
std::string BELITSKY_TUNING_STRIP_thick_unitStr;

// Series Distributed Mixer
std::string SDM_INPUT_TYPE;
parameter SDM_INPUT_length;
parameter SDM_INPUT_length_unit;
std::string SDM_INPUT_length_unitStr;
parameter SDM_INPUT_width1;
parameter SDM_INPUT_width1_unit;
std::string SDM_INPUT_width1_unitStr;
parameter SDM_INPUT_width2;
parameter SDM_INPUT_width2_unit;
std::string SDM_INPUT_width2_unitStr;
// Junction 1
parameter JUNCTION_1_AREA;
parameter JUNCTION_1_AREA_unit;
std::string JUNCTION_1_AREA_unitStr;
// Line from junction 1 to 2
std::string SDM_TUNER_12_TYPE;
parameter SDM_TUNER_12_length;
parameter SDM_TUNER_12_length_unit;
std::string SDM_TUNER_12_length_unitStr;
parameter SDM_TUNER_12_width1;
parameter SDM_TUNER_12_width1_unit;
std::string SDM_TUNER_12_width1_unitStr;
parameter SDM_TUNER_12_width2;
parameter SDM_TUNER_12_width2_unit;
std::string SDM_TUNER_12_width2_unitStr;
// Junction 2
parameter JUNCTION_2_AREA;
parameter JUNCTION_2_AREA_unit;
std::string JUNCTION_2_AREA_unitStr;
// Line from junction 2 to 3
std::string SDM_TUNER_23_TYPE;
parameter SDM_TUNER_23_length;
parameter SDM_TUNER_23_length_unit;
std::string SDM_TUNER_23_length_unitStr;
parameter SDM_TUNER_23_width1;
parameter SDM_TUNER_23_width1_unit;
std::string SDM_TUNER_23_width1_unitStr;
parameter SDM_TUNER_23_width2;
parameter SDM_TUNER_23_width2_unit;
std::string SDM_TUNER_23_width2_unitStr;
// Junction 3
parameter JUNCTION_3_AREA;
parameter JUNCTION_3_AREA_unit;
std::string JUNCTION_3_AREA_unitStr;
// Line from junction 3 to 4
std::string SDM_TUNER_34_TYPE;
parameter SDM_TUNER_34_length;
parameter SDM_TUNER_34_length_unit;
std::string SDM_TUNER_34_length_unitStr;
parameter SDM_TUNER_34_width1;
parameter SDM_TUNER_34_width1_unit;
std::string SDM_TUNER_34_width1_unitStr;
parameter SDM_TUNER_34_width2;
parameter SDM_TUNER_34_width2_unit;
std::string SDM_TUNER_34_width2_unitStr;
// Junction 4
parameter JUNCTION_4_AREA;
parameter JUNCTION_4_AREA_unit;
std::string JUNCTION_4_AREA_unitStr;

// RF Choke
std::string RF_CHOKE_TYPE;

  // Radial Stub RF Choke
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

  // Filter type RF Choke
int FILTER_CHOKE_SECTIONS;
parameter FILTER_CHOKE_INPUT_length;
parameter FILTER_CHOKE_INPUT_length_unit;
std::string FILTER_CHOKE_INPUT_length_unitStr;
parameter FILTER_CHOKE_INPUT_width;
parameter FILTER_CHOKE_INPUT_width_unit;
std::string FILTER_CHOKE_INPUT_width_unitStr;
parameter FILTER_CHOKE_LINE1_length;
parameter FILTER_CHOKE_LINE1_length_unit;
std::string FILTER_CHOKE_LINE1_length_unitStr;
parameter FILTER_CHOKE_LINE1_width;
parameter FILTER_CHOKE_LINE1_width_unit;
std::string FILTER_CHOKE_LINE1_width_unitStr;
parameter FILTER_CHOKE_LINE2_length;
parameter FILTER_CHOKE_LINE2_length_unit;
std::string FILTER_CHOKE_LINE2_length_unitStr;
parameter FILTER_CHOKE_LINE2_width;
parameter FILTER_CHOKE_LINE2_width_unit;
std::string FILTER_CHOKE_LINE2_width_unitStr;
parameter FILTER_CHOKE_LINE3_length;
parameter FILTER_CHOKE_LINE3_length_unit;
std::string FILTER_CHOKE_LINE3_length_unitStr;
parameter FILTER_CHOKE_LINE3_width;
parameter FILTER_CHOKE_LINE3_width_unit;
std::string FILTER_CHOKE_LINE3_width_unitStr;
parameter FILTER_CHOKE_LINE4_length;
parameter FILTER_CHOKE_LINE4_length_unit;
std::string FILTER_CHOKE_LINE4_length_unitStr;
parameter FILTER_CHOKE_LINE4_width;
parameter FILTER_CHOKE_LINE4_width_unit;
std::string FILTER_CHOKE_LINE4_width_unitStr;
parameter FILTER_CHOKE_LINE5_length;
parameter FILTER_CHOKE_LINE5_length_unit;
std::string FILTER_CHOKE_LINE5_length_unitStr;
parameter FILTER_CHOKE_LINE5_width;
parameter FILTER_CHOKE_LINE5_width_unit;
std::string FILTER_CHOKE_LINE5_width_unitStr;
parameter FILTER_CHOKE_LINE6_length;
parameter FILTER_CHOKE_LINE6_length_unit;
std::string FILTER_CHOKE_LINE6_length_unitStr;
parameter FILTER_CHOKE_LINE6_width;
parameter FILTER_CHOKE_LINE6_width_unit;
std::string FILTER_CHOKE_LINE6_width_unitStr;
parameter FILTER_CHOKE_OUTPUT_length;
parameter FILTER_CHOKE_OUTPUT_length_unit;
std::string FILTER_CHOKE_OUTPUT_length_unitStr;
parameter FILTER_CHOKE_OUTPUT_width;
parameter FILTER_CHOKE_OUTPUT_width_unit;
std::string FILTER_CHOKE_OUTPUT_width_unitStr;


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
