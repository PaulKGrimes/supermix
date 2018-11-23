/*******************************************************************************
** build_mixer.inc - file included in balanced.c, containing code to build    **
**                   the correct RF circuit for the mixer                     **
** ========================================================================== **
** Author - Paul Grimes                                                       **
** Date   - 15/10/04                                                          **
** ========================================================================== **
** To allow for the use of NbTiN or other metals, two wiring and ground plane **
** materials are assumed to exist. Components before the DC/IF blocks are     **
** assumed to be made from Wiring2 and Ground2, with components between the   **
** DC/IF blocks and the junctions made from Wiring1 and Ground1               **
** ========================================================================== **
** Changes:                                                                   **
*******************************************************************************/

circuit RF;
circuit IF;
circuit Bias;


/******************************
* RF components on mixer chip *
******************************/

// Finline components are currently lengths of microstrip, as we don't
// have a finline class
tapered_mstrip Finline1;
    Finline1.top_strip(Wiring2);
    Finline1.ground_plane(Ground2);
    Finline1.superstrate(Cover);
    Finline1.substrate(Insulation);
    Finline1.width1 = 0.8*Micron;
    Finline1.width2 = & FINLINE_WIDTH;
    Finline1.length = & FINLINE_LENGTH;
    Finline1.sub_thick = & INSUL_THICKNESS;

tapered_mstrip Finline2(Finline1);


// Various hybrid components: externally calculated, ideal and microstrip based
sdata_interp External_RF_Hybrid(4);

hybrid90 Ideal_RF_Hybrid;

circuit SuperMix_RF_Hybrid;
// This needs finishing

// RF_Hybrid as a generalisation of the various hybrids that could be used
// Port 1 is input, 2 is through (phi), 3 is coupled (phi+90 degrees) and 4 is isolated
circuit RF_pre_tuner;

circuit RF_Hybrid;
    if (RF_HYBRID=="EXTERNAL")
    {
        External_RF_Hybrid.Z0 = RF_HYBRID_Z0;
        External_RF_Hybrid.touchstone(EXTERNAL_RF_HYBRID_FILENAME.c_str());
        RF_Hybrid.add_port(External_RF_Hybrid, 1);
        RF_Hybrid.add_port(External_RF_Hybrid, 4);
        RF_Hybrid.add_port(External_RF_Hybrid, 3);
        RF_Hybrid.add_port(External_RF_Hybrid, 2);
        device::Z0 = & Z0;
    } else if (RF_HYBRID=="SUPERMIX")
    {
        cerr << "SuperMix Hybrid Not Implemented\n";
        return 0;
    } else
    {
        RF_Hybrid.add_port(Ideal_RF_Hybrid, 1);
        RF_Hybrid.add_port(Ideal_RF_Hybrid, 2);
        RF_Hybrid.add_port(Ideal_RF_Hybrid, 3);
        RF_Hybrid.add_port(Ideal_RF_Hybrid, 4);
    }

// Connection microstrips
microstrip Connection_Strip1a;
    Connection_Strip1a.top_strip(Wiring2);
    Connection_Strip1a.ground_plane(Ground2);
    Connection_Strip1a.superstrate(Cover);
    Connection_Strip1a.substrate(Insulation);
    Connection_Strip1a.width = & CONNECT_STRIP1_WIDTH;
    Connection_Strip1a.length = & CONNECT_STRIP1_LENGTH;
    Connection_Strip1a.sub_thick = & INSUL_THICKNESS;
    
microstrip Connection_Strip1b(Connection_Strip1a);
    
// DC/IF blocking capacitors
capacitor DCBlock1;
    DCBlock1.series();
    DCBlock1.set(DC_BLOCK_CAP);
    
capacitor DCBlock2(DCBlock1);

// Connection microstrips
microstrip Connection_Strip2a;
    Connection_Strip2a.top_strip(Wiring1);
    Connection_Strip2a.ground_plane(Ground1);
    Connection_Strip2a.superstrate(Cover);
    Connection_Strip2a.substrate(Insulation);
    Connection_Strip2a.width = & CONNECT_STRIP2_WIDTH;
    Connection_Strip2a.length = & CONNECT_STRIP2_LENGTH;
    Connection_Strip2a.sub_thick = & INSUL_THICKNESS;


microstrip Connection_Strip2b(Connection_Strip2a);

RF_pre_tuner.connect(RF_Hybrid, 2, Connection_Strip1a, 1);
RF_pre_tuner.connect(RF_Hybrid, 3, Connection_Strip1b, 1);
RF_pre_tuner.connect(Connection_Strip1a, 2, DCBlock1, 1);
RF_pre_tuner.connect(Connection_Strip1b, 2, DCBlock2, 1);
RF_pre_tuner.connect(DCBlock1, 2, Connection_Strip2a, 1);
RF_pre_tuner.connect(DCBlock2, 2, Connection_Strip2b, 1);

RF_pre_tuner.add_port(RF_Hybrid, 1);
RF_pre_tuner.add_port(RF_Hybrid, 4);
RF_pre_tuner.add_port(Connection_Strip2a, 2);
RF_pre_tuner.add_port(Connection_Strip2b, 2);

// Tuning components
microstrip Input_Strip1;
    Input_Strip1.top_strip(Wiring1);
    Input_Strip1.ground_plane(Ground1);
    Input_Strip1.superstrate(Cover);
    Input_Strip1.substrate(Insulation);
    Input_Strip1.width = & INPUT_STRIP_WIDTH;
    Input_Strip1.length = & INPUT_STRIP_LENGTH;
    Input_Strip1.sub_thick = & INSUL_THICKNESS;

microstrip Input_Strip2(Input_Strip1);

branch Branch_1a, Branch_1b, Branch_2a, Branch_2b;

microstrip Tuning_Strip1;
    Tuning_Strip1.top_strip(Wiring1);
    Tuning_Strip1.ground_plane(Ground1);
    Tuning_Strip1.superstrate(Cover);
    Tuning_Strip1.substrate(Insulation);
    Tuning_Strip1.width = & TUNING_STRIP_WIDTH;
    Tuning_Strip1.length = & TUNING_STRIP_LENGTH;
    Tuning_Strip1.sub_thick = & TUNING_STRIP_THICKNESS;

microstrip Tuning_Strip2(Tuning_Strip1);

radial_stub RF_Choke1;
    RF_Choke1.top_strip(Wiring1);
    RF_Choke1.ground_plane(Ground1);
    RF_Choke1.superstrate(Cover);
    RF_Choke1.substrate(Insulation);
    RF_Choke1.width  = & RF_CHOKE_WIDTH;
    RF_Choke1.length = & RF_CHOKE_LENGTH;
    RF_Choke1.radius = & RF_CHOKE_RADIUS;
    RF_Choke1.angle  = & RF_CHOKE_ANGLE;
    RF_Choke1.sub_thick  = & INSUL_THICKNESS;
    
radial_stub RF_Choke2(RF_Choke1);
    
// Eventually these will be set to the impedance of the IF_output_strip
zterm RF_Term1;
zterm RF_Term2;


/*******************
* Build RF Circuit *
*******************/
/*
RF.connect(Finline1, 2, RF_Hybrid, 1);
RF.connect(Finline2, 2, RF_Hybrid, 4);

RF.connect(RF_Hybrid, 2, Connection_Strip1a, 1);
RF.connect(RF_Hybrid, 3, Connection_Strip1b, 1);

RF.connect(Connection_Strip1a, 2, DCBlock1, 1);
RF.connect(Connection_Strip1b, 2, DCBlock2, 1);

RF.connect(DCBlock1, 2, Connection_Strip2a, 1);
RF.connect(DCBlock2, 2, Connection_Strip2b, 1);

RF.connect(Connection_Strip2a, 2, Input_Strip1, 1);
RF.connect(Connection_Strip2b, 2, Input_Strip2, 1);
*/
RF.connect(Finline1, 2, RF_pre_tuner, 1);
RF.connect(Finline2, 2, RF_pre_tuner, 2);

RF.connect(RF_pre_tuner, 3, Input_Strip1, 1);
RF.connect(RF_pre_tuner, 4, Input_Strip2, 1);

RF.connect(Input_Strip1, 2, Branch_1a, 1);
RF.connect(Input_Strip2, 2, Branch_2a, 1);

RF.connect(Branch_1a, 2, Tuning_Strip1, 1);
RF.connect(Branch_2a, 2, Tuning_Strip2, 1);

RF.connect(Tuning_Strip1, 2, Branch_1b, 1);
RF.connect(Tuning_Strip2, 2, Branch_2b, 1);

RF.connect(Branch_1b, 2, RF_Choke1, 1);
RF.connect(Branch_2b, 2, RF_Choke2, 1);

RF.connect(RF_Choke1, 2, RF_Term1, 1);
RF.connect(RF_Choke2, 2, RF_Term2, 1);

RF.add_port(Branch_1a, 3); // SIS 1a
RF.add_port(Branch_1b, 3); // SIS 1b
RF.add_port(Branch_2a, 3); // SIS 2a
RF.add_port(Branch_2b, 3); // SIS 2b

RF.add_port(Finline1, 1); // RF signal input
RF.add_port(Finline2, 1); // LO signal input


/****************
* IF components *
****************/

microstrip IF_Tuning_Strip1(Tuning_Strip1);
microstrip IF_Tuning_Strip2(Tuning_Strip2);

branch IF_Branch_1;
branch IF_Branch_2;

radial_stub IF_Choke_1(RF_Choke1);
radial_stub IF_Choke_2(RF_Choke2);

microstrip IF_Output_Strip1;
    IF_Output_Strip1.top_strip(Wiring1);
    IF_Output_Strip1.ground_plane(Ground1);
    IF_Output_Strip1.superstrate(Cover);
    IF_Output_Strip1.substrate(Insulation);
    IF_Output_Strip1.width = & IF_OUTPUT_WIDTH;
    IF_Output_Strip1.length = & IF_OUTPUT_LENGTH;
    IF_Output_Strip1.sub_thick = & INSUL_THICKNESS;
    
microstrip IF_Output_Strip2(IF_Output_Strip1);

RF_Term1.Z = IF_Output_Strip1.Zchar(Frequency, device::T);
RF_Term2.Z = IF_Output_Strip2.Zchar(Frequency, device::T);


microstrip IF_Connection_Strip1;
    IF_Connection_Strip1.top_strip(IF_Wiring);
    IF_Connection_Strip1.ground_plane(IF_Ground);
    IF_Connection_Strip1.superstrate(IF_Cover);
    IF_Connection_Strip1.substrate(IF_Insulation);
    IF_Connection_Strip1.width = & IF_CONNECTION_WIDTH;
    IF_Connection_Strip1.sub_thick = & IF_INSUL_thickness;
    IF_Connection_Strip1.length = & IF_CONNECTION_LENGTH_1;

microstrip IF_Connection_Strip2(IF_Connection_Strip1);
    IF_Connection_Strip2.length = & IF_CONNECTION_LENGTH_2;

// Various hybrid components: externally calculated, ideal and microstrip based
sdata_interp External_IF_Hybrid(4);
    
hybrid180 Ideal_IF_Hybrid;

circuit SuperMix_IF_Hybrid;
// This needs finishing

// General IF_Hybrid that selects correct circuit.
// Port 1 is sum output, Port 2 and 3 are inputs, and Port 4 is difference output
circuit IF_Hybrid;
    if (IF_HYBRID=="EXTERNAL")
    {
        External_IF_Hybrid.touchstone(EXTERNAL_IF_HYBRID_FILENAME.c_str());
        IF_Hybrid.add_port(External_IF_Hybrid, 1);
        IF_Hybrid.add_port(External_IF_Hybrid, 2);
        IF_Hybrid.add_port(External_IF_Hybrid, 3);
        IF_Hybrid.add_port(External_IF_Hybrid, 4);
    } else if (IF_HYBRID=="SUPERMIX")
    {
        cerr << "SuperMix Hybrid Not Implemented\n";
        return 0;
    } else
    {
        IF_Hybrid.add_port(Ideal_IF_Hybrid, 1);
        IF_Hybrid.add_port(Ideal_IF_Hybrid, 2);
        IF_Hybrid.add_port(Ideal_IF_Hybrid, 3);
        IF_Hybrid.add_port(Ideal_IF_Hybrid, 4);
    }
    

transformer IF_Load1;
    IF_Load1.Z2 = & IF_AMP_Z0;
    IF_Load1.Z1 = & IF_HYBRID_OUT_Z0;
    
transformer IF_Load2(IF_Load1);
  

/*******************
* Build IF Circuit *
*******************/

IF.connect(IF_Tuning_Strip1, 2, IF_Branch_1, 1);
IF.connect(IF_Tuning_Strip2, 2, IF_Branch_2, 1);
    
IF.connect(IF_Branch_1, 2, IF_Choke_1, 1);
IF.connect(IF_Branch_2, 2, IF_Choke_2, 1);
    
IF.connect(IF_Choke_1, 2, IF_Output_Strip1, 1);
IF.connect(IF_Choke_2, 2, IF_Output_Strip2, 1);
    
IF.connect(IF_Output_Strip1, 2, IF_Connection_Strip1, 1);
IF.connect(IF_Output_Strip2, 2, IF_Connection_Strip2, 1);

IF.connect(IF_Connection_Strip1, 2, IF_Hybrid, 2);
IF.connect(IF_Connection_Strip2, 2, IF_Hybrid, 3);

IF.connect(IF_Hybrid, 1, IF_Load1, 1);
IF.connect(IF_Hybrid, 4, IF_Load2, 1);

IF.add_port(IF_Tuning_Strip1, 1); // SIS 1a
IF.add_port(IF_Branch_1, 3);      // SIS 1b
IF.add_port(IF_Tuning_Strip2, 1); // SIS 2a
IF.add_port(IF_Branch_2, 3);      // SIS 2b

IF.add_port(IF_Load1, 2);         // Sum output
IF.add_port(IF_Load2, 2);         // Difference output



/******************
* Bias Components *
******************/

branch Bias_Branch_1;
branch Bias_Branch_2;

voltage_source Bias_1;
    Bias_1.R = & BIAS_R;
    Bias_1.Temp = 0*Kelvin;
    Bias_1.source_f = 0.0;
    Bias_1.source_voltage = & Bias_Voltage_1;

voltage_source Bias_2;
    Bias_2.R = & BIAS_R;
    Bias_2.Temp = 0*Kelvin;
    Bias_2.source_f = 0.0;
    Bias_2.source_voltage = & Bias_Voltage_2;


/*********************
* Build Bias Circuit *
*********************/

Bias.connect(Bias_1, 1, Bias_Branch_1, 1);
Bias.connect(Bias_2, 1, Bias_Branch_2, 1);

Bias.add_port(Bias_Branch_1, 2); // SIS 1a
Bias.add_port(Bias_Branch_1, 3); // SIS 1b
Bias.add_port(Bias_Branch_2, 2); // SIS 2a
Bias.add_port(Bias_Branch_2, 3); // SIS 2b




