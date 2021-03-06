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

circuit Rf;
circuit If;

// Experimental RF lossy components

// beamsplitter's port 1 is transmitted, port 2 reflected
beamsplitter Splitter;
    Splitter.epsilon = & BS_EPSILON;
    Splitter.loss_tangent = & BS_LOSS_TAN;
    Splitter.thickness = & BS_THICKNESS;
    Splitter.angle = & BS_ANGLE;
    Splitter.Temp = & BS_TEMP;

beamsplitter_red Window;
    Window.epsilon = & DW_EPSILON;
    Window.loss_tangent = & DW_LOSS_TAN;
    Window.thickness = & DW_THICKNESS;
    Window.angle = & DW_ANGLE;
    Window.Temp = & DW_TEMP;

microstrip Lossy_input;
    Lossy_input.ground_plane(Ground);
    Lossy_input.substrate(Insulation);
    Lossy_input.top_strip(Top);
    Lossy_input.superstrate(Cover);
    // Physical dimensions
    Lossy_input.length = & LOSSY_INPUT_length;
    Lossy_input.width = & LOSSY_INPUT_width;
    Lossy_input.sub_thick = & INSUL_THICKNESS;

attenuator Atten;
    Atten.dB = & MISC_ATTEN;
    Atten.Temp = & MISC_TEMP;

transformer If_Load;
If_Load.Z2 = & IF_Z0;
If_Load.Z1 = & IF_Z0;

sdata_interp External(2, device::f);
External.touchstone(TOUCHSTONE_FILE.c_str());
sdata_interp If_External(2, device::f);
If_External.touchstone(IF_TOUCHSTONE_FILE.c_str());

transformer Dummy;
transformer If_Dummy;

radial_stub Rf_Choke;
open_term Rf_Choke_Term;

branch Rf_Branch;
branch Rf_Branch2;

radial_stub If_Choke;
microstrip If_Choke_Output;
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

sdata_interp If_Finline(2, device::f);
If_Finline.touchstone(IF_FINLINE_S2P.c_str());
sdata_interp If_Connection(2, device::f);
If_Connection.touchstone(IF_CONNECT_S2P.c_str());

microstrip If_Finline_Out;
  If_Finline_Out.ground_plane(Ground);
  If_Finline_Out.substrate(Insulation);
  If_Finline_Out.top_strip(Top);
  If_Finline_Out.superstrate(Cover);
  // Physical dimensions
  If_Finline_Out.length = & IF_FINLINE_OUT_length;
  If_Finline_Out.width = & IF_FINLINE_OUT_width;
  If_Finline_Out.sub_thick = & INSUL_THICKNESS;
  
zterm If_Feed_Term;
  If_Feed_Term.Z = & IF_FEED_TERM;
  

// Create an isolator with perfectly matched 0K load, 
// to prevent standing waves between mixer circuit and optics
circulator isolator;
zterm iso_load;
  iso_load.Z = & device::Z0;

if (TUNER_TYPE == "EXTERNAL")
{
    Rf.connect(Splitter, 3, Window, 1);
    Rf.connect(Window, 2, Atten, 1);
    Rf.connect(Atten, 2, isolator, 1);
    Rf.connect(isolator, 2, Lossy_input, 1);
    Rf.connect(isolator, 3, iso_load, 1);
    Rf.connect(Lossy_input, 2, External, 1);
    Rf.add_port(External, 2); // Sis goes here
    Rf.add_port(Splitter, 1); // Load goes here
    Rf.add_port(Splitter, 2); // LO goes here

    If.connect(If_Connection, 1, If_Load, 1);
    If.connect(If_Branch, 2, If_Connection, 2);
    If.connect(If_Branch, 1, If_External, 2);
    If.connect(If_External, 1, If_Finline_Out, 2);
    If.connect(If_Finline_Out, 1, If_Finline, 2);
    If.connect(If_Finline, 1, If_Feed_Term, 1);
    If.add_port(If_Branch, 3);    // SIS goes here
    If.add_port(If_Load, 2);
}

if (TUNER_TYPE == "EXT_WITH_RFCHOKE")
{
    // Create RF choke
    // Material specs
    Rf_Choke.ground_plane(Ground);
    Rf_Choke.substrate(Insulation);
    Rf_Choke.top_strip(Top);
    Rf_Choke.superstrate(Cover);
    // Physical dimensions
    Rf_Choke.length = & RADIAL_CHOKE_INPUT_length;
    Rf_Choke.width = & RADIAL_CHOKE_INPUT_width1;
    Rf_Choke.sub_thick = & INSUL_THICKNESS;
    Rf_Choke.radius = & RADIAL_CHOKE_FAN_radius;
    Rf_Choke.angle = & RADIAL_CHOKE_FAN_angle;

    Rf.connect(Splitter, 3, Window, 1);
    Rf.connect(Window, 2, Atten, 1);
    Rf.connect(Atten, 2, isolator, 1);
    Rf.connect(isolator, 2, Lossy_input, 1);
    Rf.connect(isolator, 3, iso_load, 1);
    Rf.connect(Lossy_input, 2, External, 1);
    Rf.connect(External, 2, Rf_Branch, 1);
    Rf.connect(Rf_Branch, 2, Rf_Choke, 1);
    Rf.connect(Rf_Choke, 2, Rf_Choke_Term, 1);
    Rf.add_port(Rf_Branch, 3); // SIS goes here
    Rf.add_port(Splitter, 1);  // Load goes here
    Rf.add_port(Splitter, 2);  // LO goes here

    // Create RF chokes IF representation
    // Material specs
    If_Choke.ground_plane(Ground);
    If_Choke.substrate(Insulation);
    If_Choke.top_strip(Top);
    If_Choke.superstrate(Cover);
    // Physical dimensions
    If_Choke.length = & RADIAL_CHOKE_INPUT_length;
    If_Choke.width = & RADIAL_CHOKE_INPUT_width1;
    If_Choke.sub_thick = & INSUL_THICKNESS;
    If_Choke.radius = & RADIAL_CHOKE_FAN_radius;
    If_Choke.angle = & RADIAL_CHOKE_FAN_angle;

    // Material specs
    If_Choke_Output.ground_plane(Ground);
    If_Choke_Output.substrate(Insulation);
    If_Choke_Output.top_strip(Top);
    If_Choke_Output.superstrate(Cover);
    // Physical dimensions
    If_Choke_Output.length = & RADIAL_CHOKE_OUTPUT_length;
    If_Choke_Output.width = & RADIAL_CHOKE_OUTPUT_width;
    If_Choke_Output.sub_thick = & INSUL_THICKNESS;
    
    // Connect circuit and add ports
    If.connect(If_Connection, 2, If_Load, 1);
    If.connect(If_Choke_Output, 2, If_Connection, 1);
    If.connect(If_Choke, 2, If_Choke_Output, 1);
    If.connect(If_Branch, 2, If_Choke, 1);
    If.connect(If_Branch, 1, If_External, 2);
    If.connect(If_External, 1, If_Finline_Out, 2);
    If.connect(If_Finline_Out, 1, If_Finline, 2);
    If.connect(If_Finline, 1, If_Feed_Term, 1);
    If.add_port(If_Branch, 3);    // SIS goes here
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

    Rf.connect(Splitter, 3, Window, 1);
    Rf.connect(Window, 2, Atten, 1);
    Rf.connect(Atten, 2, isolator, 1);
    Rf.connect(isolator, 2, Lossy_input, 1);
    Rf.connect(isolator, 3, iso_load, 1);
    Rf.connect(Lossy_input, 2, Rf_Branch, 1);
    Rf.connect(Rf_Branch, 3, Tuning_Stub_1, 1);
    Rf.connect(Tuning_Stub_1, 2, Tuning_Term_1, 1);
    Rf.add_port(Rf_Branch, 2); // SIS here
    Rf.add_port(Splitter, 1);  // Load goes here
    Rf.add_port(Splitter, 2);  // LO goes here

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

    // Material specs
    If_Choke_Output.ground_plane(Ground);
    If_Choke_Output.substrate(Insulation);
    If_Choke_Output.top_strip(Top);
    If_Choke_Output.superstrate(Cover);
    // Physical dimensions
    If_Choke_Output.length = & RADIAL_CHOKE_OUTPUT_length;
    If_Choke_Output.width = & RADIAL_CHOKE_OUTPUT_width;
    If_Choke_Output.sub_thick = & INSUL_THICKNESS;
    
    If.connect(If_Connection, 2, If_Load, 1);
    If.connect(If_Choke_Output, 2, If_Connection, 1);
    If.connect(If_Tuning_Stub_1, 2, If_Choke_Output, 1);
    If.connect(If_Branch, 2, If_Tuning_Stub_1, 1);
    If.connect(If_Branch, 1, If_Finline_Out, 2);
    If.connect(If_Finline_Out, 1, If_Finline, 2);
    If.connect(If_Finline, 1, If_Feed_Term, 1);
    If.add_port(If_Branch, 3);    // SIS goes here
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

    Rf.connect(Splitter, 3, Window, 1);
    Rf.connect(Window, 2, Atten, 1);
    Rf.connect(Atten, 2, isolator, 1);
    Rf.connect(isolator, 2, Lossy_input, 1);
    Rf.connect(isolator, 3, iso_load, 1);
    Rf.connect(Lossy_input, 2, Rf_Branch, 1);
    Rf.connect(Rf_Branch, 3, Tuning_Stub_1, 1);
    Rf.connect(Tuning_Stub_1, 2, Tuning_Term_1, 1);
    Rf.connect(Rf_Branch, 4, Tuning_Stub_2, 1);
    Rf.connect(Tuning_Stub_2, 2, Tuning_Term_2, 1);
    Rf.add_port(Rf_Branch, 2); // SIS here
    Rf.add_port(Splitter, 1);  // Load goes here
    Rf.add_port(Splitter, 2);  // LO goes here

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

    // Material specs
    If_Choke_Output.ground_plane(Ground);
    If_Choke_Output.substrate(Insulation);
    If_Choke_Output.top_strip(Top);
    If_Choke_Output.superstrate(Cover);
    // Physical dimensions
    If_Choke_Output.length = & RADIAL_CHOKE_OUTPUT_length;
    If_Choke_Output.width = & RADIAL_CHOKE_OUTPUT_width;
    If_Choke_Output.sub_thick = & INSUL_THICKNESS;
    
    If.connect(If_Connection, 1, If_Load, 1);
    If.connect(If_Choke_Output, 2, If_Connection, 1);
    If.connect(If_Tuning_Stub_1, 2, If_Choke_Output, 1);
    If.connect(If_Branch, 2, If_Tuning_Stub_1, 1);
    If.connect(If_Branch, 1, If_Finline_Out, 1);
    If.connect(If_Finline_Out, 2, If_Finline, 1);
    If.connect(If_Finline, 2, If_Feed_Term, 1);
    If.add_port(If_Branch, 3);    // SIS goes here
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

    Rf.connect(Splitter, 3, Window, 1);
    Rf.connect(Window, 2, Atten, 1);
    Rf.connect(Atten, 2, isolator, 1);
    Rf.connect(isolator, 2, Lossy_input, 1);
    Rf.connect(isolator, 3, iso_load, 1);
    Rf.connect(Lossy_input, 2, Rf_Branch, 1);
    Rf.connect(Rf_Branch, 3, Tuning_Stub_1, 1);
    Rf.connect(Tuning_Stub_1, 2, Tuning_Term_1, 1);
    Rf.connect(Rf_Branch, 4, Tuning_Stub_2, 1);
    Rf.connect(Tuning_Stub_2, 2, Tuning_Term_2, 1);
    Rf.connect(Rf_Branch, 5, Tuning_Stub_3, 1);
    Rf.connect(Tuning_Stub_3, 2, Tuning_Term_3, 1);
    Rf.add_port(Rf_Branch, 2); // SIS here
    Rf.add_port(Splitter, 1);  // Load goes here
    Rf.add_port(Splitter, 2);  // LO goes here

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

    
    // Material specs
    If_Choke_Output.ground_plane(Ground);
    If_Choke_Output.substrate(Insulation);
    If_Choke_Output.top_strip(Top);
    If_Choke_Output.superstrate(Cover);
    // Physical dimensions
    If_Choke_Output.length = & RADIAL_CHOKE_OUTPUT_length;
    If_Choke_Output.width = & RADIAL_CHOKE_OUTPUT_width;
    If_Choke_Output.sub_thick = & INSUL_THICKNESS;
    
    If.connect(If_Connection, 1, If_Load, 1);
    If.connect(If_Choke_Output, 2, If_Connection, 2);
    If.connect(If_Tuning_Stub_1, 2, If_Choke_Output, 1);
    If.connect(If_Branch, 2, If_Tuning_Stub_1, 1);
    If.connect(If_Branch, 1, If_Finline_Out, 1);
    If.connect(If_Finline_Out, 2, If_Finline, 1);
    If.connect(If_Finline, 2, If_Feed_Term, 1);
    If.add_port(If_Branch, 3);    // SIS goes here
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
    Input_Strip.sub_thick = & INSUL_THICKNESS;

    // Material specs
    Tuning_Strip.ground_plane(Ground);
    Tuning_Strip.substrate(Insulation);
    Tuning_Strip.top_strip(Top);
    Tuning_Strip.superstrate(Cover);
    // Physical dimensions
    Tuning_Strip.length = & BELITSKY_TUNING_STRIP_length;
    Tuning_Strip.width = & BELITSKY_TUNING_STRIP_width;
    Tuning_Strip.sub_thick = 0.2*Micron;

    Rf_Branch.set_branches(3);

    // Connect circuit
    Rf.connect(Splitter, 3, Window, 1);
    Rf.connect(Window, 2, Atten, 1);
    Rf.connect(Atten, 2, isolator, 1);
    Rf.connect(isolator, 2, Lossy_input, 1);
    Rf.connect(isolator, 3, iso_load, 1);
    Rf.connect(Lossy_input, 2, Input_Strip, 1);
    Rf.connect(Input_Strip, 2, Rf_Branch, 1);
    Rf.connect(Rf_Branch, 2, Tuning_Strip, 1);

    // add ports
    Rf.add_port(Rf_Branch, 3); // SIS 1
    Rf.add_port(Tuning_Strip, 2);  // SIS 2
    Rf.add_port(Splitter, 1);  // Load goes here
    Rf.add_port(Splitter, 2);  // LO goes here

    // IF Circuit
    // Material specs
    If_Tuning_Strip.ground_plane(Ground);
    If_Tuning_Strip.substrate(Insulation);
    If_Tuning_Strip.top_strip(Top);
    If_Tuning_Strip.superstrate(Cover);
    // Physical dimensions
    If_Tuning_Strip.length = & BELITSKY_TUNING_STRIP_length;
    If_Tuning_Strip.width = & BELITSKY_TUNING_STRIP_width;
    If_Tuning_Strip.sub_thick = & INSUL_THICKNESS;

    // Connect circuit and add ports
    If.connect(If_Feed_Term, 1, If_Finline, 2);
    If.connect(If_Finline, 1, If_Finline_Out, 1);
    If.connect(If_Finline_Out, 2, If_Branch2, 1);
    If.connect(If_Branch2, 2, If_Tuning_Strip, 1);
    If.connect(If_Tuning_Strip, 2, If_Branch, 1);
    If.connect(If_Branch, 2, If_Connection, 2);
    If.connect(If_Connection, 1, If_Load, 1);
    If.add_port(If_Branch2, 3);   // SIS_1
    If.add_port(If_Branch, 3);  // SIS_2
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
    Input_Strip.sub_thick = & INSUL_THICKNESS;

    // Material specs
    Tuning_Strip.ground_plane(Ground);
    Tuning_Strip.substrate(Insulation);
    Tuning_Strip.top_strip(Top);
    Tuning_Strip.superstrate(Cover);
    // Physical dimensions
    Tuning_Strip.length = & BELITSKY_TUNING_STRIP_length;
    Tuning_Strip.width = & BELITSKY_TUNING_STRIP_width;
    Tuning_Strip.sub_thick = & INSUL_THICKNESS;

    // Create RF choke
    // Material specs
    Rf_Choke.ground_plane(Ground);
    Rf_Choke.substrate(Insulation);
    Rf_Choke.top_strip(Top);
    Rf_Choke.superstrate(Cover);
    // Physical dimensions
    Rf_Choke.length = & RADIAL_CHOKE_INPUT_length;
    Rf_Choke.width = & RADIAL_CHOKE_INPUT_width1;
    Rf_Choke.sub_thick = & INSUL_THICKNESS;
    Rf_Choke.radius = & RADIAL_CHOKE_FAN_radius;
    Rf_Choke.angle = & RADIAL_CHOKE_FAN_angle;

    // Connect circuit
    Rf.connect(Splitter, 3, Window, 1);
    Rf.connect(Window, 2, Atten, 1);
    Rf.connect(Atten, 2, isolator, 1);
    Rf.connect(isolator, 2, Lossy_input, 1);
    Rf.connect(isolator, 3, iso_load, 1);
    Rf.connect(Lossy_input, 2, Input_Strip, 1);
    Rf.connect(Input_Strip, 2, Rf_Branch, 1);
    Rf.connect(Rf_Branch, 2, Tuning_Strip, 1);
    Rf.connect(Tuning_Strip, 2, Rf_Branch2, 1);
    Rf.connect(Rf_Branch2, 2, Rf_Choke, 1);
    Rf.connect(Rf_Choke, 2, Rf_Choke_Term, 1);

    // add ports
    Rf.add_port(Rf_Branch, 3);   // SIS 1
    Rf.add_port(Rf_Branch2, 3);  // SIS 2
    Rf.add_port(Splitter, 1);  // Load goes here
    Rf.add_port(Splitter, 2);  // LO goes here

    // IF Circuit
    // Material specs
    If_Tuning_Strip.ground_plane(Ground);
    If_Tuning_Strip.substrate(Insulation);
    If_Tuning_Strip.top_strip(Top);
    If_Tuning_Strip.superstrate(Cover);
    // Physical dimensions
    If_Tuning_Strip.length = & BELITSKY_TUNING_STRIP_length;
    If_Tuning_Strip.width = & BELITSKY_TUNING_STRIP_width;
    If_Tuning_Strip.sub_thick = & INSUL_THICKNESS;

    // Material specs
    If_Choke.ground_plane(Ground);
    If_Choke.substrate(Insulation);
    If_Choke.top_strip(Top);
    If_Choke.superstrate(Cover);
    // Physical dimensions
    If_Choke.length = & RADIAL_CHOKE_INPUT_length;
    If_Choke.width = & RADIAL_CHOKE_INPUT_width1;
    If_Choke.sub_thick = & INSUL_THICKNESS;
    If_Choke.radius = & RADIAL_CHOKE_FAN_radius;
    If_Choke.angle = & RADIAL_CHOKE_FAN_angle;

    // Material specs
    If_Choke_Output.ground_plane(Ground);
    If_Choke_Output.substrate(Insulation);
    If_Choke_Output.top_strip(Top);
    If_Choke_Output.superstrate(Cover);
    // Physical dimensions
    If_Choke_Output.length = & RADIAL_CHOKE_OUTPUT_length;
    If_Choke_Output.width = & RADIAL_CHOKE_OUTPUT_width;
    If_Choke_Output.sub_thick = & INSUL_THICKNESS;
    
    // Connect circuit and add ports
    If.connect(If_Feed_Term, 1, If_Finline, 1);
    If.connect(If_Finline, 2, If_Finline_Out, 1);
    //If.connect(If_Feed_Term, 1, If_Finline_Out, 1);
    If.connect(If_Finline_Out, 2, If_Branch2, 1);
    If.connect(If_Branch2, 2, If_Tuning_Strip, 1);
    If.connect(If_Tuning_Strip, 2, If_Branch, 1);
    If.connect(If_Branch, 2, If_Choke, 1);
    If.connect(If_Choke, 2, If_Choke_Output, 1);
    If.connect(If_Choke_Output, 2, If_Connection, 2);
    If.connect(If_Connection, 1, If_Load, 1);
    If.add_port(If_Branch2, 3);   // SIS_1
    If.add_port(If_Branch, 3);  // SIS_2
    If.add_port(If_Load, 2);  // If output

}


