/*******************************************************************************
** globals.h -   file included in balanced.c, containing definitions of       **
**               globals required to control SuperMix parameters with dotconf **
** ========================================================================== **
** Author - Paul Grimes                                                       **
** Date   - 15/10/04                                                          **
** ========================================================================== **
** SuperMix parameters are currently created as globals, so that dotconf      **
** functions that interpret each line in the .conf file can alter them as     **
** side effects.                                                              **
**                                                                            **
** We intend to use all caps for global parameter names, and camelback for    **
** SuperMix internal parameter names                                          **
** ========================================================================== **
** Changes:                                                                   **
**          01/09/03 - Created from SuperMix-realistic/globals.inc            **
**          21/10/04 - Altered to fit balanced mixer simulations              **
*******************************************************************************/

// Set in balanced.conf by default

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

// Fixed operating point parameters
parameter FREQUENCY;

parameter BIAS_1;
parameter BIAS_2;

parameter LO_POWER;

parameter LO_NOISE_TEMP = 0.0*Kelvin;
parameter LOAD_NOISE_TEMP = 0.0*Kelvin;

// End of realistic.conf settings

// Set in mixer.conf by default

/*********************************
* General environment parameters *
*********************************/
parameter TEMP;

parameter Z0;

parameter IF_FREQ;

parameter IF_Z0;

int HARMONICS;

/**********************
* Junction parameters *
**********************/
std::string IV_CURVE;
std::string KK_CURVE;

parameter RNA;
parameter SCAP;
parameter VGAP;

parameter AREA_1a;
parameter AREA_1b;
parameter AREA_2a;
parameter AREA_2b;


/************************
* RF Circuit Parameters *
************************/

parameter FINLINE_WIDTH;
parameter FINLINE_LENGTH;

std::string RF_HYBRID;
std::string EXTERNAL_RF_HYBRID_FILENAME;
parameter RF_HYBRID_Z0;

parameter CONNECT_STRIP1_WIDTH;
parameter CONNECT_STRIP1_LENGTH;

parameter DC_BLOCK_CAP;

parameter CONNECT_STRIP2_WIDTH;
parameter CONNECT_STRIP2_LENGTH;

parameter INPUT_STRIP_WIDTH;
parameter INPUT_STRIP_LENGTH;

parameter TUNING_STRIP_WIDTH;
parameter TUNING_STRIP_LENGTH;
parameter TUNING_STRIP_THICKNESS;

parameter RF_CHOKE_WIDTH;
parameter RF_CHOKE_LENGTH;
parameter RF_CHOKE_RADIUS;
parameter RF_CHOKE_ANGLE;


/************************
* IF Circuit Parameters *
************************/

parameter IF_OUTPUT_WIDTH;
parameter IF_OUTPUT_LENGTH;

parameter IF_CONNECTION_WIDTH;
parameter IF_CONNECTION_LENGTH_1;
parameter IF_CONNECTION_LENGTH_2;

std::string IF_HYBRID;
std::string EXTERNAL_IF_HYBRID_FILENAME;

parameter IF_HYBRID_OUT_Z0;
parameter IF_AMP_Z0;


/**************************
* Bias Circuit Parameters *
**************************/

parameter BIAS_R;


/********************
* Default Materials *
********************/

// WIRING1 metallisation
std::string WIRING_1;
parameter WIRING_1_THICKNESS;


// GROUND1 metallisation
std::string GROUND_1;
parameter GROUND_1_THICKNESS;

// WIRING2 metallisation
std::string WIRING_2;
parameter WIRING_2_THICKNESS;


// GROUND2 metallisation
std::string GROUND_2;
parameter GROUND_2_THICKNESS;

// INSUL dielectric
std::string INSULATION;
parameter INSUL_THICKNESS;

// COVER dielectric
std::string COVER;
parameter COVER_THICKNESS;

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


/*******************************
* IF Board Material Properties *
*******************************/

parameter IF_WIRING_rho;
parameter IF_WIRING_thick;

parameter IF_INSUL_epsilon;
parameter IF_INSUL_loss_tangent;
parameter IF_INSUL_thickness;

/*************************************************
* Other useful globals, not defined in conf file *
*************************************************/

int RF_USB_PORT;
int RF_LSB_PORT;
int LO_USB_PORT;
int LO_LSB_PORT;
int SUM_IF_PORT;
int DIFF_IF_PORT;


