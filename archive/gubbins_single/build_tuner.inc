/*******************************************************************************
** build_tuner.inc - file included in simple.c, containing code to build the  **
**                   correct RF circuit for the mixer                         **
** ========================================================================== **
** Author - Paul Grimes                                                       **
** Date   - 22/07/03                                                          **
** ========================================================================== **
**  The tapered radial stub class written previously appears to be missing.   **
**  Therefore, stub type is currently ignored.                                **
** ========================================================================== **
** Changes:                                                                   **
*******************************************************************************/

printf("# building tuner...\n");

circuit Rf;
circuit If;

sdata_interp External(2, device::f);
External.touchstone(TOUCHSTONE_FILE.c_str());

sdata_interp If_Connect(2, device::f);
If_Connect.touchstone(IF_CONNECTION.c_str());

microstrip If_Trans1;
  If_Trans1.ground_plane(If_Ground);
  If_Trans1.substrate(If_Board);
  If_Trans1.top_strip(If_Top);
  If_Trans1.superstrate(Cover);
  // Physical dimensions
  If_Trans1.length = & IF_TRANS1_length;
  If_Trans1.width = & IF_TRANS1_width;
  If_Trans1.sub_thick = & IF_BOARD_THICKNESS;

microstrip If_Trans2 = If_Trans1;
  If_Trans2.length = & IF_TRANS2_length;
  If_Trans2.width = & IF_TRANS2_width;

microstrip If_Trans3 = If_Trans1;
  If_Trans3.length = & IF_TRANS3_length;
  If_Trans3.width = & IF_TRANS3_width;

microstrip If_Trans4 = If_Trans1;
  If_Trans4.length = & IF_TRANS4_length;
  If_Trans4.width = & IF_TRANS4_width;

microstrip If_Trans5 = If_Trans1;
  If_Trans5.length = & IF_TRANS5_length;
  If_Trans5.width = & IF_TRANS5_width;

// If_Connection has it's ports backwards for compatibility with HFSS models
circuit If_Connection;
  If_Connection.connect(If_Connect, 1, If_Trans1, 1);
  If_Connection.connect(If_Trans1, 2, If_Trans2, 1);
  If_Connection.connect(If_Trans2, 2, If_Trans3, 1);
  If_Connection.connect(If_Trans3, 2, If_Trans4, 1);
  If_Connection.connect(If_Trans4, 2, If_Trans5, 1);
  
  If_Connection.add_port(If_Trans5, 2);
  If_Connection.add_port(If_Connect, 2);
  

transformer Rf_Input;
  Rf_Input.Z2 = & Z0;

transformer If_Load;
  If_Load.Z1 = & IF_Z0;
  
transformer If_Dummy;
  If_Dummy.Z1 = IF_Z0;
  If_Dummy.Z2 = IF_Z0;
  
circuit Rf_Choke;
open_term Rf_Choke_Term;

radial_stub Radial_Choke;
microstrip Radial_Choke_Output;

circuit Filter_Choke;
microstrip Filter_Choke_Input;
microstrip Filter_Choke_Line1;
microstrip Filter_Choke_Line2;
microstrip Filter_Choke_Line3;
microstrip Filter_Choke_Line4;
microstrip Filter_Choke_Line5;
microstrip Filter_Choke_Line6;
microstrip Filter_Choke_Line7;
microstrip Filter_Choke_Output;

circuit If_Choke;

microstrip If_Out_Strip;
    If_Out_Strip.ground_plane(Ground);
    If_Out_Strip.substrate(Insulation);
    If_Out_Strip.top_strip(Top);
    If_Out_Strip.superstrate(Cover);
    // Physical dimensions
    If_Out_Strip.length = & IF_OUT_STRIP_length;
    If_Out_Strip.width = & IF_OUT_STRIP_width;
    If_Out_Strip.sub_thick = & INSUL_THICKNESS;

if (RF_CHOKE_TYPE == "RADIAL")
{
    // Material specs
    Radial_Choke.ground_plane(Ground);
    Radial_Choke.substrate(Insulation);
    Radial_Choke.top_strip(Top);
    Radial_Choke.superstrate(Cover);
    // Physical dimensions
    Radial_Choke.length = & RADIAL_CHOKE_INPUT_length;
    Radial_Choke.width = & RADIAL_CHOKE_INPUT_width1;
    Radial_Choke.sub_thick = & INSUL_THICKNESS;
    Radial_Choke.radius = & RADIAL_CHOKE_FAN_radius;
    Radial_Choke.angle = & RADIAL_CHOKE_FAN_angle;

    // Material specs
    Radial_Choke_Output.ground_plane(Ground);
    Radial_Choke_Output.substrate(Insulation);
    Radial_Choke_Output.top_strip(Top);
    Radial_Choke_Output.superstrate(Cover);
    // Physical dimensions
    Radial_Choke_Output.length = & RADIAL_CHOKE_OUTPUT_length;
    Radial_Choke_Output.width = & RADIAL_CHOKE_OUTPUT_width;
    Radial_Choke_Output.sub_thick = & INSUL_THICKNESS;
    
    Rf_Choke.connect( Radial_Choke, 2, Radial_Choke_Output, 1);
    Rf_Choke.connect( Radial_Choke_Output, 2, Rf_Choke_Term, 1);
    Rf_Choke.add_port( Radial_Choke, 1);
    
    If_Choke.connect( Radial_Choke, 2, Radial_Choke_Output, 1);
    If_Choke.add_port( Radial_Choke, 1);
    If_Choke.add_port( Radial_Choke_Output, 2);
}
else if (RF_CHOKE_TYPE == "FILTER")
{
    // Setup Rf Choke microstrip sections
    Filter_Choke_Input.ground_plane(Ground);
    Filter_Choke_Input.substrate(Insulation);
    Filter_Choke_Input.top_strip(Top);
    Filter_Choke_Input.superstrate(Cover);
    // Physical dimensions
    Filter_Choke_Input.length = & FILTER_CHOKE_INPUT_length;
    Filter_Choke_Input.width = & FILTER_CHOKE_INPUT_width;
    Filter_Choke_Input.sub_thick = & INSUL_THICKNESS;
    
    Filter_Choke_Line1.ground_plane(Ground);
    Filter_Choke_Line1.substrate(Insulation);
    Filter_Choke_Line1.top_strip(Top);
    Filter_Choke_Line1.superstrate(Cover);
    // Physical dimensions
    Filter_Choke_Line1.length = & FILTER_CHOKE_LINE1_length;
    Filter_Choke_Line1.width = & FILTER_CHOKE_LINE1_width;
    Filter_Choke_Line1.sub_thick = & INSUL_THICKNESS;
    
    Filter_Choke_Line2.ground_plane(Ground);
    Filter_Choke_Line2.substrate(Insulation);
    Filter_Choke_Line2.top_strip(Top);
    Filter_Choke_Line2.superstrate(Cover);
    // Physical dimensions
    Filter_Choke_Line2.length = & FILTER_CHOKE_LINE2_length;
    Filter_Choke_Line2.width = & FILTER_CHOKE_LINE2_width;
    Filter_Choke_Line2.sub_thick = & INSUL_THICKNESS;
    
    Filter_Choke_Line3.ground_plane(Ground);
    Filter_Choke_Line3.substrate(Insulation);
    Filter_Choke_Line3.top_strip(Top);
    Filter_Choke_Line3.superstrate(Cover);
    // Physical dimensions
    Filter_Choke_Line3.length = & FILTER_CHOKE_LINE3_length;
    Filter_Choke_Line3.width = & FILTER_CHOKE_LINE3_width;
    Filter_Choke_Line3.sub_thick = & INSUL_THICKNESS;
    
    Filter_Choke_Line4.ground_plane(Ground);
    Filter_Choke_Line4.substrate(Insulation);
    Filter_Choke_Line4.top_strip(Top);
    Filter_Choke_Line4.superstrate(Cover);
    // Physical dimensions
    Filter_Choke_Line4.length = & FILTER_CHOKE_LINE4_length;
    Filter_Choke_Line4.width = & FILTER_CHOKE_LINE4_width;
    Filter_Choke_Line4.sub_thick = & INSUL_THICKNESS;
    
    Filter_Choke_Line5.ground_plane(Ground);
    Filter_Choke_Line5.substrate(Insulation);
    Filter_Choke_Line5.top_strip(Top);
    Filter_Choke_Line5.superstrate(Cover);
    // Physical dimensions
    Filter_Choke_Line5.length = & FILTER_CHOKE_LINE5_length;
    Filter_Choke_Line5.width = & FILTER_CHOKE_LINE5_width;
    Filter_Choke_Line5.sub_thick = & INSUL_THICKNESS;
    
    Filter_Choke_Line6.ground_plane(Ground);
    Filter_Choke_Line6.substrate(Insulation);
    Filter_Choke_Line6.top_strip(Top);
    Filter_Choke_Line6.superstrate(Cover);
    // Physical dimensions
    Filter_Choke_Line6.length = & FILTER_CHOKE_LINE6_length;
    Filter_Choke_Line6.width = & FILTER_CHOKE_LINE6_width;
    Filter_Choke_Line6.sub_thick = & INSUL_THICKNESS;
    
    Filter_Choke_Line7.ground_plane(Ground);
    Filter_Choke_Line7.substrate(Insulation);
    Filter_Choke_Line7.top_strip(Top);
    Filter_Choke_Line7.superstrate(Cover);
    // Physical dimensions
    Filter_Choke_Line7.length = & FILTER_CHOKE_LINE7_length;
    Filter_Choke_Line7.width = & FILTER_CHOKE_LINE7_width;
    Filter_Choke_Line7.sub_thick = & INSUL_THICKNESS;
    
    
    Filter_Choke_Output.ground_plane(Ground);
    Filter_Choke_Output.substrate(Insulation);
    Filter_Choke_Output.top_strip(Top);
    Filter_Choke_Output.superstrate(Cover);
    // Physical dimensions
    Filter_Choke_Output.length = & FILTER_CHOKE_OUTPUT_length;
    Filter_Choke_Output.width = & FILTER_CHOKE_OUTPUT_width;
    Filter_Choke_Output.sub_thick = & INSUL_THICKNESS;
    
    
    Filter_Choke.connect( Filter_Choke_Input, 2, Filter_Choke_Line1, 1);
    Filter_Choke.connect( Filter_Choke_Line1, 2, Filter_Choke_Line2, 1);
    Filter_Choke.connect( Filter_Choke_Line2, 2, Filter_Choke_Line3, 1);
    Filter_Choke.connect( Filter_Choke_Line3, 2, Filter_Choke_Line4, 1);
    Filter_Choke.connect( Filter_Choke_Line4, 2, Filter_Choke_Line5, 1);
    Filter_Choke.connect( Filter_Choke_Line5, 2, Filter_Choke_Line6, 1);
    Filter_Choke.connect( Filter_Choke_Line6, 2, Filter_Choke_Line7, 1);
    Filter_Choke.connect( Filter_Choke_Line7, 2, Filter_Choke_Output, 1);
    Filter_Choke.add_port( Filter_Choke_Input, 1);
    Filter_Choke.add_port( Filter_Choke_Output, 2);
    
    Rf_Choke.connect( Filter_Choke, 2, Rf_Choke_Term, 1 );
    Rf_Choke.add_port( Filter_Choke, 1 );
    
    If_Choke = Filter_Choke;
}

branch Rf_Branch;
branch Rf_Branch2;

branch If_Branch;
branch If_Branch2;

radial_stub Tuning_Stub_1;
radial_stub Tuning_Stub_2;
radial_stub Tuning_Stub_3;

open_term Tuning_Term_1;
open_term Tuning_Term_2;
open_term Tuning_Term_3;

radial_stub If_Tuning_Stub_1;

microstrip Input_Strip;
microstrip Tuning_Strip;
microstrip If_Tuning_Strip;

circuit filter;

microstrip filter_input;
  filter_input.ground_plane(Ground);
	filter_input.substrate(Insulation);
	filter_input.top_strip(Top);
	filter_input.superstrate(Cover);

	filter_input.length = & FILTER_INPUT_length;
	filter_input.width = & FILTER_INPUT_width;
	filter_input.sub_thick = & INSUL_THICKNESS;
  

microstrip filter_input1;
	filter_input1.ground_plane(Ground);
	filter_input1.substrate(Insulation);
	filter_input1.top_strip(Top);
	filter_input1.superstrate(Cover);

	filter_input1.length = & FILT_INPUT_length;
	filter_input1.width = & FILT_INPUT_width;
	filter_input1.sub_thick = & INSUL_THICKNESS;

microstrip filter_input2;
	filter_input2.ground_plane(Ground);
	filter_input2.substrate(Insulation);
	filter_input2.top_strip(Top);
	filter_input2.superstrate(Cover);

	filter_input2.length = & FILT_INPUT_length;
	filter_input2.width = & FILT_INPUT_width;
	filter_input2.sub_thick = & INSUL_THICKNESS;

microstrip filter_centre;
	filter_centre.ground_plane(Ground);
	filter_centre.substrate(Insulation);
	filter_centre.top_strip(Top);
	filter_centre.superstrate(Cover);

	filter_centre.length = & FILT_CENTRE_length;
	filter_centre.width = & FILT_CENTRE_width;
	filter_centre.sub_thick = & INSUL_THICKNESS;

capacitor filter_cap1;
	filter_cap1.C = & FILT_CAP;
	filter_cap1.series();

capacitor filter_cap2;
	filter_cap2.C = & FILT_CAP;
	filter_cap2.series();


filter.connect(filter_input, 2, filter_input1, 1);
filter.connect(filter_input1, 2, filter_cap1, 1);
filter.connect(filter_cap1, 2, filter_centre, 1);
filter.connect(filter_centre, 2, filter_cap2, 1);
filter.connect(filter_cap2, 2, filter_input2, 1);

filter.add_port(filter_input, 1);
filter.add_port(filter_input2, 2);

microstrip tuner_input;
  tuner_input.ground_plane(Ground);
  tuner_input.substrate(Insulation);
  tuner_input.top_strip(Top);
  tuner_input.superstrate(Cover);
  
  tuner_input.length = & TUNER_INPUT_length;
  tuner_input.width = & TUNER_INPUT_width;
  tuner_input.sub_thick = & INSUL_THICKNESS;

zterm if_term(Complex(1000000,0));

if (TUNER_TYPE == "EXTERNAL")
{
    Rf.connect(Rf_Input, 2, filter, 1);
    Rf.connect(filter, 2, tuner_input, 1);
    Rf.connect(tuner_input, 2, External, 1);
    Rf.add_port(External, 2); // Sis goes here
    Rf.add_port(Rf_Input, 1);    // LO goes here

    If.connect(if_term, 1, filter, 1);
    If.connect(filter, 2, tuner_input, 1);
    If.connect(tuner_input, 2, If_Branch, 2);
    If.connect(If_Branch, 3, If_Out_Strip, 1);
    If.connect(If_Out_Strip, 2, If_Connection, 2);
    If.connect(If_Connection, 1, If_Load, 1);
    If.add_port(If_Branch, 1);    // SIS goes here
    If.add_port(If_Load, 2); 
}

if (TUNER_TYPE == "EXT_WITH_RFCHOKE")
{
    // Create RF choke
    Rf.connect(Rf_Input, 2, filter, 1);
    Rf.connect(filter, 2, tuner_input, 1);
    Rf.connect(tuner_input, 2, External, 1);
    Rf.connect(External, 2, Rf_Branch, 1);
    Rf.connect(Rf_Branch, 2, Rf_Choke, 1);
    Rf.add_port(Rf_Branch, 3); // SIS goes here
    Rf.add_port(Rf_Input, 1);  // LO goes here

    // Connect circuit and add ports
    If.connect(if_term, 1, filter, 1);
    If.connect(filter, 2, tuner_input, 1);
    If.connect(tuner_input, 2, External, 1);
    If.connect(External, 2, If_Branch, 2);
    If.connect(If_Branch, 3, If_Choke, 1);
    If.connect(If_Choke, 2, If_Out_Strip, 1);
    If.connect(If_Out_Strip, 2, If_Connection, 2);
    If.connect(If_Connection, 1, If_Load, 1);
    If.add_port(If_Branch, 1);  // SIS
    If.add_port(If_Load, 2);  // If output
}

if (TUNER_TYPE == "RFCHOKE")
{
    // Connect RF choke to Rf_Input and junction
  
    Rf.connect(Rf_Input, 2, filter, 1);
    Rf.connect(filter, 2, tuner_input, 1);
    Rf.connect(tuner_input, 2, Rf_Branch, 2);
    Rf.connect(Rf_Branch, 3, Rf_Choke, 1);
    Rf.add_port(Rf_Branch, 1);
    Rf.add_port(Rf_Input, 1);
    
    // Connect circuit and add ports
    If.connect(if_term, 1, filter, 1);
    If.connect(filter, 2, tuner_input, 1);
    If.connect(tuner_input, 2, If_Branch, 2);
    If.connect(If_Branch, 3, If_Choke, 1);
    If.connect(If_Choke, 2, If_Out_Strip, 1);
    If.connect(If_Out_Strip, 2, If_Connection, 2);
    If.connect(If_Connection, 1, If_Load, 1);
    If.add_port(If_Branch, 1);  // SIS
    If.add_port(If_Load, 2);  // If output
}

if (TUNER_TYPE == "PARALLEL_STUB")
{
    Tuning_Stub_1.width = & SINGLE_STUB_INPUT_width1;
    // Material specs
    Tuning_Stub_1.ground_plane(Ground);
    Tuning_Stub_1.substrate(Insulation);
    Tuning_Stub_1.top_strip(Top);
    Tuning_Stub_1.superstrate(Cover);
    // Physical dimensions
    Tuning_Stub_1.length = & SINGLE_STUB_INPUT_length;
    Tuning_Stub_1.sub_thick = & INSUL_THICKNESS;
    Tuning_Stub_1.radius = & SINGLE_STUB_FAN_radius;
    Tuning_Stub_1.angle = & SINGLE_STUB_FAN_angle;

    Rf_Branch.set_branches(4);

    Rf.connect(Rf_Input, 2, filter, 1);
    Rf.connect(filter, 2, tuner_input, 1);
    Rf.connect(tuner_input, 2, Rf_Branch, 2);
    Rf.connect(Rf_Branch, 3, Tuning_Stub_1, 1);
    Rf.connect(Tuning_Stub_1, 2, Tuning_Term_1, 1);
    Rf.connect(Rf_Branch, 4, Rf_Choke, 1);
    Rf.add_port(Rf_Branch, 1); // SIS here
    Rf.add_port(Rf_Input, 1); // LO here

    // Material specs
    If_Tuning_Stub_1.ground_plane(Ground);
    If_Tuning_Stub_1.substrate(Insulation);
    If_Tuning_Stub_1.top_strip(Top);
    If_Tuning_Stub_1.superstrate(Cover);
    // Physical dimensions
    If_Tuning_Stub_1.length = & SINGLE_STUB_INPUT_length;
    If_Tuning_Stub_1.width = & SINGLE_STUB_INPUT_width1;
    If_Tuning_Stub_1.sub_thick = & INSUL_THICKNESS;
    If_Tuning_Stub_1.radius = & SINGLE_STUB_FAN_radius;
    If_Tuning_Stub_1.angle = & SINGLE_STUB_FAN_angle;

    If_Branch.set_branches(4);

    If.connect(If_Tuning_Stub_1, 2, Tuning_Term_1, 1);
    If.connect(if_term, 1, filter, 1);
    If.connect(filter, 2, tuner_input, 1);
    If.connect(tuner_input, 2, If_Branch, 4);
    If.connect(If_Branch, 3, If_Tuning_Stub_1, 1);
    If.connect(If_Branch, 2, If_Choke, 1);
    If.connect(If_Choke, 2, If_Out_Strip, 1);
    If.connect(If_Out_Strip, 2, If_Connection, 2);
    If.connect(If_Connection, 1, If_Load, 1);
    If.add_port(If_Branch, 1);
    If.add_port(If_Load, 2);

}

if (TUNER_TYPE == "SINGLE_STUB")
{
    Tuning_Stub_1.width = & SINGLE_STUB_INPUT_width1;
    // Material specs
    Tuning_Stub_1.ground_plane(Ground);
    Tuning_Stub_1.substrate(Insulation);
    Tuning_Stub_1.top_strip(Top);
    Tuning_Stub_1.superstrate(Cover);
    // Physical dimensions
    Tuning_Stub_1.length = & SINGLE_STUB_INPUT_length;
    Tuning_Stub_1.sub_thick = & INSUL_THICKNESS;
    Tuning_Stub_1.radius = & SINGLE_STUB_FAN_radius;
    Tuning_Stub_1.angle = & SINGLE_STUB_FAN_angle;

    Rf.connect(Rf_Input, 2, filter, 1);
    Rf.connect(filter, 2, tuner_input, 1);
    Rf.connect(tuner_input, 2, Rf_Branch, 1);
    Rf.connect(Rf_Branch, 3, Tuning_Stub_1, 1);
    Rf.connect(Tuning_Stub_1, 2, Tuning_Term_1, 1);
    Rf.add_port(Rf_Branch, 2); // SIS here
    Rf.add_port(Rf_Input, 1); // LO here

    // Material specs
    If_Tuning_Stub_1.ground_plane(Ground);
    If_Tuning_Stub_1.substrate(Insulation);
    If_Tuning_Stub_1.top_strip(Top);
    If_Tuning_Stub_1.superstrate(Cover);
    // Physical dimensions
    If_Tuning_Stub_1.length = & SINGLE_STUB_INPUT_length;
    If_Tuning_Stub_1.width = & SINGLE_STUB_INPUT_width1;
    If_Tuning_Stub_1.sub_thick = & INSUL_THICKNESS;
    If_Tuning_Stub_1.radius = & SINGLE_STUB_FAN_radius;
    If_Tuning_Stub_1.angle = & SINGLE_STUB_FAN_angle;

    If.connect(if_term, 1, filter, 1);
    If.connect(filter, 2, tuner_input, 1);
    If.connect(tuner_input, 2, If_Branch, 2);
    If.connect(If_Branch, 3, If_Tuning_Stub_1, 1);
    If.connect(If_Tuning_Stub_1, 2, If_Out_Strip, 1);
    If.connect(If_Out_Strip, 2, If_Connection, 2);
    If.connect(If_Connection, 1, If_Load, 1);
    If.add_port(If_Branch, 1);
    If.add_port(If_Load, 2);

}

if (TUNER_TYPE == "DOUBLE_STUB")
{
    // Material specs
    Tuning_Stub_1.ground_plane(Ground);
    Tuning_Stub_1.substrate(Insulation);
    Tuning_Stub_1.top_strip(Top);
    Tuning_Stub_1.superstrate(Cover);
    // Physical dimensions
    Tuning_Stub_1.length = & DOUBLE_STUB_1_INPUT_length;
    Tuning_Stub_1.width = & DOUBLE_STUB_1_INPUT_width1;
    Tuning_Stub_1.sub_thick = & INSUL_THICKNESS;
    Tuning_Stub_1.radius = & DOUBLE_STUB_1_FAN_radius;
    Tuning_Stub_1.angle = & DOUBLE_STUB_1_FAN_angle;


    // Material specs
    Tuning_Stub_2.ground_plane(Ground);
    Tuning_Stub_2.substrate(Insulation);
    Tuning_Stub_2.top_strip(Top);
    Tuning_Stub_2.superstrate(Cover);
    // Physical dimensions
    Tuning_Stub_2.length = & DOUBLE_STUB_2_INPUT_length;
    Tuning_Stub_2.width = & DOUBLE_STUB_2_INPUT_width1;
    Tuning_Stub_2.sub_thick = & INSUL_THICKNESS;
    Tuning_Stub_2.radius = & DOUBLE_STUB_2_FAN_radius;
    Tuning_Stub_2.angle = & DOUBLE_STUB_2_FAN_angle;

    Rf_Branch.set_branches(4);

    Rf.connect(Rf_Input, 2, filter, 1);
    Rf.connect(filter, 2, tuner_input, 1);
    Rf.connect(tuner_input, 2, Rf_Branch, 1);
    Rf.connect(Rf_Branch, 3, Tuning_Stub_1, 1);
    Rf.connect(Tuning_Stub_1, 2, Tuning_Term_1, 1);
    Rf.connect(Rf_Branch, 4, Tuning_Stub_2, 1);
    Rf.connect(Tuning_Stub_2, 2, Tuning_Term_2, 1);
    Rf.add_port(Rf_Branch, 2); // SIS here
    Rf.add_port(Rf_Input, 1); // LO here

    // Material specs
    If_Tuning_Stub_1.ground_plane(Ground);
    If_Tuning_Stub_1.substrate(Insulation);
    If_Tuning_Stub_1.top_strip(Top);
    If_Tuning_Stub_1.superstrate(Cover);
    // Physical dimensions
    If_Tuning_Stub_1.length = & DOUBLE_STUB_1_INPUT_length;
    If_Tuning_Stub_1.width = & DOUBLE_STUB_1_INPUT_width1;
    If_Tuning_Stub_1.sub_thick = & INSUL_THICKNESS;
    If_Tuning_Stub_1.radius = & DOUBLE_STUB_1_FAN_radius;
    If_Tuning_Stub_1.angle = & DOUBLE_STUB_1_FAN_angle;

    If.connect(if_term, 1, filter, 1);
    If.connect(filter, 2, tuner_input, 1);
    If.connect(tuner_input, 2, If_Branch, 2);
    If.connect(If_Branch, 3, If_Tuning_Stub_1, 1);
    If.connect(If_Tuning_Stub_1, 2, If_Out_Strip, 1);
    If.connect(If_Out_Strip, 2, If_Connection, 2);
    If.connect(If_Connection, 1, If_Load, 1);
    If.add_port(If_Branch, 1);
    If.add_port(If_Load, 2);

}

if (TUNER_TYPE == "TRIPLE_STUB")
{
    // Material specs
    Tuning_Stub_1.ground_plane(Ground);
    Tuning_Stub_1.substrate(Insulation);
    Tuning_Stub_1.top_strip(Top);
    Tuning_Stub_1.superstrate(Cover);
    // Physical dimensions
    Tuning_Stub_1.length = & TRIPLE_STUB_1_INPUT_length;
    Tuning_Stub_1.width = & TRIPLE_STUB_1_INPUT_width1;
    Tuning_Stub_1.sub_thick = & INSUL_THICKNESS;
    Tuning_Stub_1.radius = & TRIPLE_STUB_1_FAN_radius;
    Tuning_Stub_1.angle = & TRIPLE_STUB_1_FAN_angle;

    // Material specs
    Tuning_Stub_2.ground_plane(Ground);
    Tuning_Stub_2.substrate(Insulation);
    Tuning_Stub_2.top_strip(Top);
    Tuning_Stub_2.superstrate(Cover);
    // Physical dimensions
    Tuning_Stub_2.length = & TRIPLE_STUB_2_INPUT_length;
    Tuning_Stub_2.width = & TRIPLE_STUB_2_INPUT_width1;
    Tuning_Stub_2.sub_thick = & INSUL_THICKNESS;
    Tuning_Stub_2.radius = & TRIPLE_STUB_2_FAN_radius;
    Tuning_Stub_2.angle = & TRIPLE_STUB_2_FAN_angle;

    // Material specs
    Tuning_Stub_3.ground_plane(Ground);
    Tuning_Stub_3.substrate(Insulation);
    Tuning_Stub_3.top_strip(Top);
    Tuning_Stub_3.superstrate(Cover);
    // Physical dimensions
    Tuning_Stub_3.length = & TRIPLE_STUB_3_INPUT_length;
    Tuning_Stub_3.width = & TRIPLE_STUB_3_INPUT_width1;
    Tuning_Stub_3.sub_thick = & INSUL_THICKNESS;
    Tuning_Stub_3.radius = & TRIPLE_STUB_3_FAN_radius;
    Tuning_Stub_3.angle = & TRIPLE_STUB_3_FAN_angle;


    Rf_Branch.set_branches(5);

    Rf.connect(Rf_Input, 2, filter, 1);
    Rf.connect(filter, 2, tuner_input, 1);
    Rf.connect(tuner_input, 2, Rf_Branch, 1);
    Rf.connect(Rf_Branch, 3, Tuning_Stub_1, 1);
    Rf.connect(Tuning_Stub_1, 2, Tuning_Term_1, 1);
    Rf.connect(Rf_Branch, 4, Tuning_Stub_2, 1);
    Rf.connect(Tuning_Stub_2, 2, Tuning_Term_2, 1);
    Rf.connect(Rf_Branch, 5, Tuning_Stub_3, 1);
    Rf.connect(Tuning_Stub_3, 2, Tuning_Term_3, 1);
    Rf.add_port(Rf_Branch, 2); // SIS here
    Rf.add_port(Rf_Input, 1); // LO here
    
    // Material specs
    If_Tuning_Stub_1.ground_plane(Ground);
    If_Tuning_Stub_1.substrate(Insulation);
    If_Tuning_Stub_1.top_strip(Top);
    If_Tuning_Stub_1.superstrate(Cover);
    // Physical dimensions
    If_Tuning_Stub_1.length = & TRIPLE_STUB_1_INPUT_length;
    If_Tuning_Stub_1.width = & TRIPLE_STUB_1_INPUT_width1;
    If_Tuning_Stub_1.sub_thick = & INSUL_THICKNESS;
    If_Tuning_Stub_1.radius = & TRIPLE_STUB_1_FAN_radius;
    If_Tuning_Stub_1.angle = & TRIPLE_STUB_1_FAN_angle;
    
    If.connect(if_term, 1, filter, 1);
    If.connect(filter, 2, tuner_input, 1);
    If.connect(tuner_input, 2, If_Branch, 2);
    If.connect(If_Branch, 3, If_Tuning_Stub_1, 1);
    If.connect(If_Tuning_Stub_1, 2, If_Out_Strip, 1);
    If.connect(If_Out_Strip, 2, If_Connection, 2);
    If.connect(If_Connection, 1, If_Load, 1);
    If.add_port(If_Branch, 1);
    If.add_port(If_Load, 2);

}


if (TUNER_TYPE == "BELITSKY")
{
    // RF Circuit
    // Create microstrip
    // Material specs
    Input_Strip.ground_plane(Ground);
    Input_Strip.substrate(Insulation);
    Input_Strip.top_strip(Top);
    Input_Strip.superstrate(Cover);
    // Physical dimensions
    Input_Strip.length = & BELITSKY_INPUT_STRIP_length;
    Input_Strip.width = & BELITSKY_INPUT_STRIP_width;
    Input_Strip.sub_thick = & BELITSKY_INPUT_STRIP_thick;

    // Material specs
    Tuning_Strip.ground_plane(Ground);
    Tuning_Strip.substrate(Insulation);
    Tuning_Strip.top_strip(Top);
    Tuning_Strip.superstrate(Cover);
    // Physical dimensions
    Tuning_Strip.length = & BELITSKY_TUNING_STRIP_length;
    Tuning_Strip.width = & BELITSKY_TUNING_STRIP_width;
    Tuning_Strip.sub_thick = & BELITSKY_TUNING_STRIP_thick;

    Rf_Branch.set_branches(3);
    
    // Connect circuit
    Rf.connect(Rf_Input, 2, filter, 1);
    Rf.connect(filter, 2, tuner_input, 1);
    Rf.connect(tuner_input, 2, Input_Strip, 1);
    Rf.connect(Input_Strip, 2, Rf_Branch, 1);
    Rf.connect(Rf_Branch, 2, Tuning_Strip, 1);
    
    // add ports
    Rf.add_port(Rf_Branch, 3); // SIS 1
    Rf.add_port(Tuning_Strip, 2);  // SIS 2
    Rf.add_port(Rf_Input, 1);   // RF Input
    
    // IF Circuit
    // Material specs
    If_Tuning_Strip.ground_plane(Ground);
    If_Tuning_Strip.substrate(Insulation);
    If_Tuning_Strip.top_strip(Top);
    If_Tuning_Strip.superstrate(Cover);
    // Physical dimensions
    If_Tuning_Strip.length = & BELITSKY_TUNING_STRIP_length;
    If_Tuning_Strip.width = & BELITSKY_TUNING_STRIP_width;
    If_Tuning_Strip.sub_thick = & BELITSKY_TUNING_STRIP_thick;

    // Connect circuit and add ports
    If.connect(if_term, 1, filter, 1);
    If.connect(filter, 2, tuner_input, 1);
    If.connect(tuner_input, 2, Input_Strip, 1);
    If.connect(Input_Strip, 2, If_Branch, 2);
    If.connect(If_Branch, 3, If_Tuning_Strip, 1);
    If.connect(If_Tuning_Strip, 2, If_Branch2, 2);
    If.connect(If_Branch2, 3, If_Out_Strip, 1);
    If.connect(If_Out_Strip, 2, If_Connection, 2);
    If.connect(If_Connection, 1, If_Load, 1);
    
    If.add_port(If_Branch, 1);   // SIS_1
    If.add_port(If_Branch2, 1);  // SIS_2
    If.add_port(If_Load, 2);  // If output
}

if (TUNER_TYPE == "BEL_WITH_RFCHOKE")
{
    // RF Circuit
    // Material specs
    Input_Strip.ground_plane(Ground);
    Input_Strip.substrate(Insulation);
    Input_Strip.top_strip(Top);
    Input_Strip.superstrate(Cover);
    // Physical dimensions
    Input_Strip.length = & BELITSKY_INPUT_STRIP_length;
    Input_Strip.width = & BELITSKY_INPUT_STRIP_width;
    Input_Strip.sub_thick = & BELITSKY_INPUT_STRIP_thick;

    // Material specs
    Tuning_Strip.ground_plane(Ground);
    Tuning_Strip.substrate(Insulation);
    Tuning_Strip.top_strip(Top);
    Tuning_Strip.superstrate(Cover);
    // Physical dimensions
    Tuning_Strip.length = & BELITSKY_TUNING_STRIP_length;
    Tuning_Strip.width = & BELITSKY_TUNING_STRIP_width;
    Tuning_Strip.sub_thick = & BELITSKY_TUNING_STRIP_thick;
    
    // Connect circuit
    Rf.connect(Rf_Input, 2, filter, 1);
    Rf.connect(filter, 2, tuner_input, 1);
    Rf.connect(tuner_input, 2, Input_Strip, 1);
    Rf.connect(Input_Strip, 2, Rf_Branch, 1);
    Rf.connect(Rf_Branch, 2, Tuning_Strip, 1);
    Rf.connect(Tuning_Strip, 2, Rf_Branch2, 1);
    Rf.connect(Rf_Branch2, 2, Rf_Choke, 1);
        
    // add ports
    Rf.add_port(Rf_Branch, 3);   // SIS 1
    Rf.add_port(Rf_Branch2, 3);  // SIS 2
    Rf.add_port(Rf_Input, 1);     // RF Input
    
    // IF Circuit
    // Material specs
    If_Tuning_Strip.ground_plane(Ground);
    If_Tuning_Strip.substrate(Insulation);
    If_Tuning_Strip.top_strip(Top);
    If_Tuning_Strip.superstrate(Cover);
    // Physical dimensions
    If_Tuning_Strip.length = & BELITSKY_TUNING_STRIP_length;
    If_Tuning_Strip.width = & BELITSKY_TUNING_STRIP_width;
    If_Tuning_Strip.sub_thick = & BELITSKY_TUNING_STRIP_thick;
       
    // Connect circuit and add ports
    If.connect(if_term, 1, filter, 1);
    If.connect(filter, 2, tuner_input, 1);
    If.connect(tuner_input, 2, Input_Strip, 1);
    If.connect(Input_Strip, 2, If_Branch, 1);
    If.connect(If_Branch, 2, If_Tuning_Strip, 1);
    If.connect(If_Tuning_Strip, 2, If_Branch2, 1);
    If.connect(If_Branch2, 2, If_Choke, 1);
    If.connect(If_Choke, 2, If_Out_Strip, 1);
    If.connect(If_Out_Strip, 2, If_Connection, 2);
    If.connect(If_Connection, 1, If_Load, 1);
    If.add_port(If_Branch, 3);   // SIS_1
    If.add_port(If_Branch2, 3);  // SIS_2
    If.add_port(If_Load, 2);  // If output

}  
  
printf("# tuner complete\n");



