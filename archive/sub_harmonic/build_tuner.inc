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

transformer If_Load;
If_Load.Z2 = & IF_Z0;
If_Load.Z1 = & Z0;

sdata_interp External(2, device::f);
External.touchstone(TOUCHSTONE_FILE.c_str());

transformer Dummy;
transformer If_Dummy;

circuit Rf_Choke;
open_term Rf_Choke_Term;
radial_stub Radial_Choke;
microstrip Radial_Choke_Output;

circuit Filter_Choke;
microstrip Filter_Choke_Input;
microstrip Filter_Choke_Wide1;
microstrip Filter_Choke_Narrow1;
microstrip Filter_Choke_Wide2;
microstrip Filter_Choke_Narrow2;
microstrip Filter_Choke_Wide3;
microstrip Filter_Choke_Narrow3;

circuit If_Choke;

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
    If_Choke.connect( Radial_Choke_Output, 2, If_Dummy, 1);
    If_Choke.add_port( Radial_Choke, 1);
    If_Choke.add_port( If_Dummy, 2);
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
    
    Filter_Choke_Narrow1.ground_plane(Ground);
    Filter_Choke_Narrow1.substrate(Insulation);
    Filter_Choke_Narrow1.top_strip(Top);
    Filter_Choke_Narrow1.superstrate(Cover);
    // Physical dimensions
    Filter_Choke_Narrow1.length = & FILTER_CHOKE_NARROW_length;
    Filter_Choke_Narrow1.width = & FILTER_CHOKE_NARROW_width;
    Filter_Choke_Narrow1.sub_thick = & INSUL_THICKNESS;
    
    Filter_Choke_Narrow2 = Filter_Choke_Narrow1;
    Filter_Choke_Narrow3 = Filter_Choke_Narrow1;

    Filter_Choke_Wide1.ground_plane(Ground);
    Filter_Choke_Wide1.substrate(Insulation);
    Filter_Choke_Wide1.top_strip(Top);
    Filter_Choke_Wide1.superstrate(Cover);
    // Physical dimensions
    Filter_Choke_Wide1.length = & FILTER_CHOKE_WIDE_length;
    Filter_Choke_Wide1.width = & FILTER_CHOKE_WIDE_width;
    Filter_Choke_Wide1.sub_thick = & INSUL_THICKNESS;
    
    Filter_Choke_Wide2 = Filter_Choke_Wide1;
    Filter_Choke_Wide3 = Filter_Choke_Wide1;
    
    Filter_Choke.connect( Filter_Choke_Input, 2, Filter_Choke_Wide1, 1);
    Filter_Choke.connect( Filter_Choke_Wide1, 2, Filter_Choke_Narrow1, 1);
    Filter_Choke.connect( Filter_Choke_Narrow1, 2, Filter_Choke_Wide2, 1);
    Filter_Choke.connect( Filter_Choke_Wide2, 2, Filter_Choke_Narrow2, 1);
    Filter_Choke.connect( Filter_Choke_Narrow2, 2, Filter_Choke_Wide3, 1);
    Filter_Choke.connect( Filter_Choke_Wide3, 2, Filter_Choke_Narrow3, 1);
    Filter_Choke.add_port( Filter_Choke_Input, 1);
    Filter_Choke.add_port( Filter_Choke_Narrow3, 2);
    
    Rf_Choke.connect( Filter_Choke, 2, Rf_Choke_Term, 1 );
    Rf_Choke.add_port( Filter_Choke, 1 );
    
    If_Choke.connect( Filter_Choke, 2, If_Dummy, 1 );
    If_Choke.add_port( Filter_Choke, 1 );
    If_Choke.add_port( If_Dummy, 2 );
}

branch Rf_Branch;
branch Rf_Branch2;

branch If_Branch;

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


if (TUNER_TYPE == "EXTERNAL")
{
    Dummy.Z1 = & Z0;
    Dummy.Z2 = & Z0;

    Rf.connect(Dummy, 2, External, 1);
    Rf.add_port(External, 2); // Sis goes here
    Rf.add_port(Dummy, 1);    // LO goes here

    If_Dummy.Z1 = & Z0;
    If_Dummy.Z2 = & Z0;

    If.connect(If_Dummy, 2, If_Load, 1);
    If.add_port(If_Dummy, 1);    // SIS goes here
    If.add_port(If_Load, 2);     // LO goes here
}

if (TUNER_TYPE == "EXT_WITH_RFCHOKE")
{
    // Create RF choke
    Rf.connect(External, 2, Rf_Branch, 1);
    Rf.connect(Rf_Branch, 2, Rf_Choke, 1);
    Rf.add_port(Rf_Branch, 3); // SIS goes here
    Rf.add_port(External, 1);  // LO goes here

    // Connect circuit and add ports
    If.connect(If_Choke, 2, If_Load, 1);
    If.add_port(If_Choke, 1);  // SIS
    If.add_port(If_Load, 2);  // If output
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


    Rf.connect(Rf_Branch, 3, Tuning_Stub_1, 1);
    Rf.connect(Tuning_Stub_1, 2, Tuning_Term_1, 1);
    Rf.add_port(Rf_Branch, 2); // SIS here
    Rf.add_port(Rf_Branch, 1); // LO here

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

    If.connect(If_Tuning_Stub_1, 2, If_Load, 1);
    If.add_port(If_Tuning_Stub_1, 1);
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

    Rf.connect(Rf_Branch, 3, Tuning_Stub_1, 1);
    Rf.connect(Tuning_Stub_1, 2, Tuning_Term_1, 1);
    Rf.connect(Rf_Branch, 4, Tuning_Stub_2, 1);
    Rf.connect(Tuning_Stub_2, 2, Tuning_Term_2, 1);
    Rf.add_port(Rf_Branch, 2); // SIS here
    Rf.add_port(Rf_Branch, 1); // LO here

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

    If.connect(If_Tuning_Stub_1, 2, If_Load, 1);
    If.add_port(If_Tuning_Stub_1, 1);
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

    Rf.connect(Rf_Branch, 3, Tuning_Stub_1, 1);
    Rf.connect(Tuning_Stub_1, 2, Tuning_Term_1, 1);
    Rf.connect(Rf_Branch, 4, Tuning_Stub_2, 1);
    Rf.connect(Tuning_Stub_2, 2, Tuning_Term_2, 1);
    Rf.connect(Rf_Branch, 5, Tuning_Stub_3, 1);
    Rf.connect(Tuning_Stub_3, 2, Tuning_Term_3, 1);
    Rf.add_port(Rf_Branch, 2); // SIS here
    Rf.add_port(Rf_Branch, 1); // LO here
    
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
    
    If.connect(If_Tuning_Stub_1, 2, If_Load, 1);
    If.add_port(If_Tuning_Stub_1, 1);
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
    Rf.connect(Input_Strip, 2, Rf_Branch, 1);
    Rf.connect(Rf_Branch, 2, Tuning_Strip, 1);
    
    // add ports
    Rf.add_port(Rf_Branch, 3); // SIS 1
    Rf.add_port(Tuning_Strip, 2);  // SIS 2
    Rf.add_port(Input_Strip, 1);   // RF Input
    
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
    If.connect(If_Tuning_Strip, 2, If_Branch, 1);
    If.connect(If_Branch, 3, If_Load, 1);
    If.add_port(If_Tuning_Strip, 1);   // SIS_1
    If.add_port(If_Branch, 2);  // SIS_2
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
    Rf.connect(Input_Strip, 2, Rf_Branch, 1);
    Rf.connect(Rf_Branch, 2, Tuning_Strip, 1);
    Rf.connect(Tuning_Strip, 2, Rf_Branch2, 1);
    Rf.connect(Rf_Branch2, 2, Rf_Choke, 1);
        
    // add ports
    Rf.add_port(Rf_Branch, 3);   // SIS 1
    Rf.add_port(Rf_Branch2, 3);  // SIS 2
    Rf.add_port(Input_Strip, 1);     // RF Input
    
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
    If.connect(If_Tuning_Strip, 2, If_Branch, 1);
    If.connect(If_Branch, 3, If_Choke, 1);
    If.connect(If_Choke, 2, If_Load, 1);
    If.add_port(If_Tuning_Strip, 1);   // SIS_1
    If.add_port(If_Branch, 2);  // SIS_2
    If.add_port(If_Load, 2);  // If output

}

printf("# tuner complete\n");


