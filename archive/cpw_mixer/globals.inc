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

std::string TRAFO1_TYPE;
parameter TRAFO1_length;
parameter TRAFO1_length_unit;
std::string TRAFO1_length_unitStr;
parameter TRAFO1_width;
parameter TRAFO1_width_unit;
std::string TRAFO1_width_unitStr;
parameter TRAFO1_space;
parameter TRAFO1_space_unit;
std::string TRAFO1_space_unitStr;

std::string TRAFO2_TYPE;
parameter TRAFO2_length;
parameter TRAFO2_length_unit;
std::string TRAFO2_length_unitStr;
parameter TRAFO2_width;
parameter TRAFO2_width_unit;
std::string TRAFO2_width_unitStr;
parameter TRAFO2_space;
parameter TRAFO2_space_unit;
std::string TRAFO2_space_unitStr;

std::string TRAFO3_TYPE;
parameter TRAFO3_length;
parameter TRAFO3_length_unit;
std::string TRAFO3_length_unitStr;
parameter TRAFO3_width;
parameter TRAFO3_width_unit;
std::string TRAFO3_width_unitStr;
parameter TRAFO3_space;
parameter TRAFO3_space_unit;
std::string TRAFO3_space_unitStr;


  // Radial Stub RF Choke
std::string STUBIN_TYPE;
parameter STUBIN_length;
parameter STUBIN_length_unit;
std::string STUBIN_length_unitStr;
parameter STUBIN_width;
parameter STUBIN_width_unit;
std::string STUBIN_width_unitStr;
parameter STUBIN_space;
parameter STUBIN_space_unit;
std::string STUBIN_space_unitStr;

parameter RADIAL_STUB_INPUT_length;
parameter RADIAL_STUB_INPUT_length_unit;
std::string RADIAL_STUB_INPUT_length_unitStr;
parameter RADIAL_STUB_INPUT_width;
parameter RADIAL_STUB_INPUT_width_unit;
std::string RADIAL_STUB_INPUT_width_unitStr;
parameter RADIAL_STUB_FAN_radius;
parameter RADIAL_STUB_FAN_radius_unit;
std::string RADIAL_STUB_FAN_radius_unitStr;
parameter RADIAL_STUB_FAN_angle;
parameter RADIAL_STUB_FAN_angle_unit;
std::string RADIAL_STUB_FAN_angle_unitStr;



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

// SUBSTRATE metallisation
std::string SUBSTRATE_DEFAULT;
parameter SUBSTRATE_THICKNESS;
parameter SUBSTRATE_THICKNESS_unit;
std::string SUBSTRATE_THICKNESS_unitStr;

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

// Si
parameter SI_epsilon;
parameter SI_loss_tangent;

// Air
parameter AIR_epsilon = 1.0;
parameter AIR_loss_tangent = 0.0;

/*************************************************
* Other useful globals, not defined in conf file *
*************************************************/

int USB_PORT;
int LSB_PORT;
int IF_PORT;


