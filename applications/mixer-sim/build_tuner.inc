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
circuit Bias;

branch Bias_Branch;

transformer If_Dummy;
If_Dummy.Z1 = & IF_Z0;
If_Dummy.Z2 = & IF_Z0;

transformer If_Load;
If_Load.Z1 = & IF_Z0;

transformer Rf_Input;
Rf_Input.Z2 = & Z0;

sdata_interp External(2, device::f);
External.touchstone(TOUCHSTONE_FILE.c_str());

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
microstrip Filter_Choke_Output;

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
    Radial_Choke.width = & RADIAL_CHOKE_INPUT_width;
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

    Filter_Choke_Output.ground_plane(Ground);
    Filter_Choke_Output.substrate(Insulation);
    Filter_Choke_Output.top_strip(Top);
    Filter_Choke_Output.superstrate(Cover);
    // Physical dimensions
    Filter_Choke_Output.length = & FILTER_CHOKE_LINE6_length;
    Filter_Choke_Output.width = & FILTER_CHOKE_LINE6_width;
    Filter_Choke_Output.sub_thick = & INSUL_THICKNESS;


    Filter_Choke.connect( Filter_Choke_Input, 2, Filter_Choke_Line1, 1);
    Filter_Choke.connect( Filter_Choke_Line1, 2, Filter_Choke_Line2, 1);
    Filter_Choke.connect( Filter_Choke_Line2, 2, Filter_Choke_Line3, 1);
    Filter_Choke.connect( Filter_Choke_Line3, 2, Filter_Choke_Line4, 1);
    Filter_Choke.connect( Filter_Choke_Line4, 2, Filter_Choke_Line5, 1);
    Filter_Choke.connect( Filter_Choke_Line5, 2, Filter_Choke_Line6, 1);
    Filter_Choke.connect( Filter_Choke_Line6, 2, Filter_Choke_Output, 1);
    Filter_Choke.add_port( Filter_Choke_Input, 1);
    Filter_Choke.add_port( Filter_Choke_Output, 2);

    Rf_Choke.connect( Filter_Choke, 2, Rf_Choke_Term, 1 );
    Rf_Choke.add_port( Filter_Choke, 1 );

    If_Choke = Filter_Choke;
}

branch Rf_Branch;
branch Rf_Branch2;

series_tee Series_Tee_1;
series_tee Series_Tee_2;
series_tee Series_Tee_3;

branch If_Branch;

radial_stub Tuning_Stub_1;
radial_stub Tuning_Stub_2;
radial_stub Tuning_Stub_3;

open_term Tuning_Term_1;
open_term Tuning_Term_2;
open_term Tuning_Term_3;

radial_stub If_Tuning_Stub_1;

trl_base * Input_Strip;
trl_base * Tuning_Strip;
trl_base * Tuning_Strip_2;
trl_base * Tuning_Strip_3;

microstrip Input_Strip_MS;
cpw Input_Strip_CPW;
microstrip Tuning_Strip_MS;
cpw Tuning_Strip_CPW;
microstrip Tuning_Strip_2_MS;
cpw Tuning_Strip_2_CPW;
microstrip Tuning_Strip_3_MS;
cpw Tuning_Strip_3_CPW;

if (TUNER_TYPE == "EXTERNAL")
{

    Rf.connect(Rf_Input, 2, External, 1);
    Rf.add_port(External, 2); // Sis goes here
    Rf.add_port(Rf_Input, 1);    // LO goes here

    If.connect(If_Dummy, 2, If_Load, 1);
    If.add_port(If_Dummy, 1);    // SIS goes here
    If.add_port(If_Load, 2);     // LO goes here

    Sis_1.set_iv(Iv);
    Sis_1.Vn = JUNCTION_VGAP;
    Sis_1.Rn = JUNCTION_RNA/JUNCTION_AREA;
    Sis_1.Cap = JUNCTION_SCAP*JUNCTION_AREA;
}

if (TUNER_TYPE == "EXT_WITH_RFCHOKE")
{
    // Create RF choke
    Rf.connect(Rf_Input, 2, External, 1);
    Rf.connect(External, 2, Rf_Branch, 1);
    Rf.connect(Rf_Branch, 2, Rf_Choke, 1);
    Rf.add_port(Rf_Branch, 3); // SIS goes here
    Rf.add_port(Rf_Input, 1);  // LO goes here

    // Connect circuit and add ports
    If.connect(If_Choke, 2, If_Load, 1);
    If.add_port(If_Choke, 1);  // SIS
    If.add_port(If_Load, 2);  // If output

    Sis_1.set_iv(Iv);
    Sis_1.Vn = JUNCTION_VGAP;
    Sis_1.Rn = JUNCTION_RNA/JUNCTION_AREA;
    Sis_1.Cap = JUNCTION_SCAP*JUNCTION_AREA;
}

if (TUNER_TYPE == "RFCHOKE")
{
    // Connect RF choke to Rf_Input and junction
    Rf.connect(Rf_Input, 2, Rf_Branch, 2);
    Rf.connect(Rf_Branch, 3, Rf_Choke, 1);
    Rf.add_port(Rf_Branch, 1);
    Rf.add_port(Rf_Input, 1);

    If.connect(If_Choke, 2, If_Load, 1);
    If.add_port(If_Choke, 1);
    If.add_port(If_Load, 2);

    Sis_1.set_iv(Iv);
    Sis_1.Vn = JUNCTION_VGAP;
    Sis_1.Rn = JUNCTION_RNA/JUNCTION_AREA;
    Sis_1.Cap = JUNCTION_SCAP*JUNCTION_AREA;
}

if (TUNER_TYPE == "PARALLEL_STUB")
{
    Tuning_Stub_1.width = & SINGLE_STUB_INPUT_width;
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

    Rf.connect(Rf_Input, 2, Rf_Branch, 2);
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
    If_Tuning_Stub_1.width = & SINGLE_STUB_INPUT_width;
    If_Tuning_Stub_1.sub_thick = & INSUL_THICKNESS;
    If_Tuning_Stub_1.radius = & SINGLE_STUB_FAN_radius;
    If_Tuning_Stub_1.angle = & SINGLE_STUB_FAN_angle;

    If.connect(If_Tuning_Stub_1, 2, Tuning_Term_1, 1);
    If.connect(If_Branch, 3, If_Tuning_Stub_1, 1);
    If.connect(If_Branch, 2, If_Choke, 1);
    If.connect(If_Choke, 2, If_Load, 1);
    If.add_port(If_Branch, 1);
    If.add_port(If_Load, 2);

    Sis_1.set_iv(Iv);
    Sis_1.Vn = JUNCTION_VGAP;
    Sis_1.Rn = JUNCTION_RNA/JUNCTION_AREA;
    Sis_1.Cap = JUNCTION_SCAP*JUNCTION_AREA;

}


if (TUNER_TYPE == "SINGLE_STUB")
{
    Tuning_Stub_1.width = & SINGLE_STUB_INPUT_width;
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

    Rf.connect(Rf_Input, 2, Rf_Branch, 2);
    Rf.connect(Rf_Branch, 3, Tuning_Stub_1, 1);
    Rf.connect(Tuning_Stub_1, 2, Tuning_Term_1, 1);
    Rf.add_port(Rf_Branch, 1); // SIS here
    Rf.add_port(Rf_Input, 1); // LO here

    // Material specs
    If_Tuning_Stub_1.ground_plane(Ground);
    If_Tuning_Stub_1.substrate(Insulation);
    If_Tuning_Stub_1.top_strip(Top);
    If_Tuning_Stub_1.superstrate(Cover);
    // Physical dimensions
    If_Tuning_Stub_1.length = & SINGLE_STUB_INPUT_length;
    If_Tuning_Stub_1.width = & SINGLE_STUB_INPUT_width;
    If_Tuning_Stub_1.sub_thick = & INSUL_THICKNESS;
    If_Tuning_Stub_1.radius = & SINGLE_STUB_FAN_radius;
    If_Tuning_Stub_1.angle = & SINGLE_STUB_FAN_angle;

    If.connect(If_Tuning_Stub_1, 2, If_Load, 1);
    If.add_port(If_Tuning_Stub_1, 1);
    If.add_port(If_Load, 2);

    Sis_1.set_iv(Iv);
    Sis_1.Vn = JUNCTION_VGAP;
    Sis_1.Rn = JUNCTION_RNA/JUNCTION_AREA;
    Sis_1.Cap = JUNCTION_SCAP*JUNCTION_AREA;

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
    Tuning_Stub_1.width = & DOUBLE_STUB_1_INPUT_width;
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
    Tuning_Stub_2.width = & DOUBLE_STUB_2_INPUT_width;
    Tuning_Stub_2.sub_thick = & INSUL_THICKNESS;
    Tuning_Stub_2.radius = & DOUBLE_STUB_2_FAN_radius;
    Tuning_Stub_2.angle = & DOUBLE_STUB_2_FAN_angle;

    Rf_Branch.set_branches(4);

    Rf.connect(Rf_Input, 2, Rf_Branch, 2);
    Rf.connect(Rf_Branch, 3, Tuning_Stub_1, 1);
    Rf.connect(Tuning_Stub_1, 2, Tuning_Term_1, 1);
    Rf.connect(Rf_Branch, 4, Tuning_Stub_2, 1);
    Rf.connect(Tuning_Stub_2, 2, Tuning_Term_2, 1);
    Rf.add_port(Rf_Branch, 1); // SIS here
    Rf.add_port(Rf_Input, 1); // LO here

    // Material specs
    If_Tuning_Stub_1.ground_plane(Ground);
    If_Tuning_Stub_1.substrate(Insulation);
    If_Tuning_Stub_1.top_strip(Top);
    If_Tuning_Stub_1.superstrate(Cover);
    // Physical dimensions
    If_Tuning_Stub_1.length = & DOUBLE_STUB_1_INPUT_length;
    If_Tuning_Stub_1.width = & DOUBLE_STUB_1_INPUT_width;
    If_Tuning_Stub_1.sub_thick = & INSUL_THICKNESS;
    If_Tuning_Stub_1.radius = & DOUBLE_STUB_1_FAN_radius;
    If_Tuning_Stub_1.angle = & DOUBLE_STUB_1_FAN_angle;

    If.connect(If_Tuning_Stub_1, 2, If_Load, 1);
    If.add_port(If_Tuning_Stub_1, 1);
    If.add_port(If_Load, 2);

    Sis_1.set_iv(Iv);
    Sis_1.Vn = JUNCTION_VGAP;
    Sis_1.Rn = JUNCTION_RNA/JUNCTION_AREA;
    Sis_1.Cap = JUNCTION_SCAP*JUNCTION_AREA;

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
    Tuning_Stub_1.width = & TRIPLE_STUB_1_INPUT_width;
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
    Tuning_Stub_2.width = & TRIPLE_STUB_2_INPUT_width;
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
    Tuning_Stub_3.width = & TRIPLE_STUB_3_INPUT_width;
    Tuning_Stub_3.sub_thick = & INSUL_THICKNESS;
    Tuning_Stub_3.radius = & TRIPLE_STUB_3_FAN_radius;
    Tuning_Stub_3.angle = & TRIPLE_STUB_3_FAN_angle;


    Rf_Branch.set_branches(5);

    Rf.connect(Rf_Input, 2, Rf_Branch, 2);
    Rf.connect(Rf_Branch, 3, Tuning_Stub_1, 1);
    Rf.connect(Tuning_Stub_1, 2, Tuning_Term_1, 1);
    Rf.connect(Rf_Branch, 4, Tuning_Stub_2, 1);
    Rf.connect(Tuning_Stub_2, 2, Tuning_Term_2, 1);
    Rf.connect(Rf_Branch, 5, Tuning_Stub_3, 1);
    Rf.connect(Tuning_Stub_3, 2, Tuning_Term_3, 1);
    Rf.add_port(Rf_Branch, 1); // SIS here
    Rf.add_port(Rf_Input, 1); // LO here

    // Material specs
    If_Tuning_Stub_1.ground_plane(Ground);
    If_Tuning_Stub_1.substrate(Insulation);
    If_Tuning_Stub_1.top_strip(Top);
    If_Tuning_Stub_1.superstrate(Cover);
    // Physical dimensions
    If_Tuning_Stub_1.length = & TRIPLE_STUB_1_INPUT_length;
    If_Tuning_Stub_1.width = & TRIPLE_STUB_1_INPUT_width;
    If_Tuning_Stub_1.sub_thick = & INSUL_THICKNESS;
    If_Tuning_Stub_1.radius = & TRIPLE_STUB_1_FAN_radius;
    If_Tuning_Stub_1.angle = & TRIPLE_STUB_1_FAN_angle;

    If.connect(If_Tuning_Stub_1, 2, If_Load, 1);
    If.add_port(If_Tuning_Stub_1, 1);
    If.add_port(If_Load, 2);

    Sis_1.set_iv(Iv);
    Sis_1.Vn = JUNCTION_VGAP;
    Sis_1.Rn = JUNCTION_RNA/JUNCTION_AREA;
    Sis_1.Cap = JUNCTION_SCAP*JUNCTION_AREA;

}

if (TUNER_TYPE == "BELITSKY")
{
    // RF Circuit
    // Create microstrip - add switch here for CPW options
    // Material specs
    Input_Strip_MS.ground_plane(Ground);
    Input_Strip_MS.substrate(Insulation);
    Input_Strip_MS.top_strip(Top);
    Input_Strip_MS.superstrate(Cover);
    // Physical dimensions
    Input_Strip_MS.length = & BELITSKY_INPUT_STRIP_length;
    Input_Strip_MS.width = & BELITSKY_INPUT_STRIP_width;
    Input_Strip_MS.sub_thick = & BELITSKY_INPUT_STRIP_thick;

    // Material specs
    Tuning_Strip_MS.ground_plane(Ground);
    Tuning_Strip_MS.substrate(Insulation);
    Tuning_Strip_MS.top_strip(Top);
    Tuning_Strip_MS.superstrate(Cover);
    // Physical dimensions
    Tuning_Strip_MS.length = & BELITSKY_TUNING_STRIP_length;
    Tuning_Strip_MS.width = & BELITSKY_TUNING_STRIP_width;
    Tuning_Strip_MS.sub_thick = & BELITSKY_TUNING_STRIP_thick;

    Input_Strip = &Input_Strip_MS;
    Tuning_Strip = &Tuning_Strip_MS;

    Rf_Branch.set_branches(3);

    // Connect circuit
    Rf.connect(Rf_Input, 2, *Input_Strip, 1);
    Rf.connect(*Input_Strip, 2, Rf_Branch, 2);
    Rf.connect(Rf_Branch, 3, *Tuning_Strip, 1);

    // add ports
    Rf.add_port(Rf_Branch, 1); // SIS 1
    Rf.add_port(*Tuning_Strip, 2);  // SIS 2
    Rf.add_port(Rf_Input, 1);   // RF Input

    // IF Circuit

    // Connect circuit and add ports
    If.connect(*Tuning_Strip, 2, If_Branch, 1);
    If.connect(If_Branch, 3, If_Load, 1);
    If.add_port(*Tuning_Strip, 1);   // SIS_1
    If.add_port(If_Branch, 2);  // SIS_2
    If.add_port(If_Load, 2);  // If output

    // Create the two SIS junctions
    JUNCTION_AREA = BELITSKY_JUNCTION_1_AREA;

    Sis_1.set_iv(Iv);
    Sis_1.Vn = JUNCTION_VGAP;
    Sis_1.Rn = JUNCTION_RNA/JUNCTION_AREA;
    Sis_1.Cap = JUNCTION_SCAP*JUNCTION_AREA;

    JUNCTION_AREA = BELITSKY_JUNCTION_2_AREA;

    Sis_2.set_iv(Iv);
    Sis_2.Vn = JUNCTION_VGAP;
    Sis_2.Rn = JUNCTION_RNA/JUNCTION_AREA;
    Sis_2.Cap = JUNCTION_SCAP*JUNCTION_AREA;

    // Create the 2 junction bias circuit
    Bias_Source.source_f = 0.0;
    Bias_Source.source_voltage = & Bias_Voltage;
    Bias.connect(Bias_Source, 1, Bias_Branch, 1);
    Bias.add_port(Bias_Branch, 2);
    Bias.add_port(Bias_Branch, 3);
}

if (TUNER_TYPE == "BEL_WITH_RFCHOKE")
{
    // RF Circuit
    // Material specs
    Input_Strip_MS.ground_plane(Ground);
    Input_Strip_MS.substrate(Insulation);
    Input_Strip_MS.top_strip(Top);
    Input_Strip_MS.superstrate(Cover);
    // Physical dimensions
    Input_Strip_MS.length = & BELITSKY_INPUT_STRIP_length;
    Input_Strip_MS.width = & BELITSKY_INPUT_STRIP_width;
    Input_Strip_MS.sub_thick = & BELITSKY_INPUT_STRIP_thick;

    // Material specs
    Tuning_Strip_MS.ground_plane(Ground);
    Tuning_Strip_MS.substrate(Insulation);
    Tuning_Strip_MS.top_strip(Top);
    Tuning_Strip_MS.superstrate(Cover);
    // Physical dimensions
    Tuning_Strip_MS.length = & BELITSKY_TUNING_STRIP_length;
    Tuning_Strip_MS.width = & BELITSKY_TUNING_STRIP_width;
    Tuning_Strip_MS.sub_thick = & BELITSKY_TUNING_STRIP_thick;

    Input_Strip = & Input_Strip_MS;
    Tuning_Strip = & Tuning_Strip_MS;

    // Connect circuit
    Rf.connect(Rf_Input, 2, *Input_Strip, 1);
    Rf.connect(*Input_Strip, 2, Rf_Branch, 1);
    Rf.connect(Rf_Branch, 2, *Tuning_Strip, 1);
    Rf.connect(*Tuning_Strip, 2, Rf_Branch2, 1);
    Rf.connect(Rf_Branch2, 2, Rf_Choke, 1);

    // add ports
    Rf.add_port(Rf_Branch, 3);   // SIS 1
    Rf.add_port(Rf_Branch2, 3);  // SIS 2
    Rf.add_port(Rf_Input, 1);     // RF Input

    // IF Circuit
    // Connect circuit and add ports
    If.connect(*Tuning_Strip, 2, If_Branch, 1);
    If.connect(If_Branch, 3, If_Choke, 1);
    If.connect(If_Choke, 2, If_Load, 1);
    If.add_port(*Tuning_Strip, 1);   // SIS_1
    If.add_port(If_Branch, 2);  // SIS_2
    If.add_port(If_Load, 2);  // If output

    // Create the two SIS junctions
    JUNCTION_AREA = BELITSKY_JUNCTION_1_AREA;

    Sis_1.set_iv(Iv);
    Sis_1.Vn = JUNCTION_VGAP;
    Sis_1.Rn = JUNCTION_RNA/JUNCTION_AREA;
    Sis_1.Cap = JUNCTION_SCAP*JUNCTION_AREA;

    JUNCTION_AREA = BELITSKY_JUNCTION_2_AREA;

    Sis_2.set_iv(Iv);
    Sis_2.Vn = JUNCTION_VGAP;
    Sis_2.Rn = JUNCTION_RNA/JUNCTION_AREA;
    Sis_2.Cap = JUNCTION_SCAP*JUNCTION_AREA;

    // Create the 2 junction bias circuit
    Bias_Source.source_f = 0.0;
    Bias_Source.source_voltage = & Bias_Voltage;
    Bias.connect(Bias_Source, 1, Bias_Branch, 1);
    Bias.add_port(Bias_Branch, 2);
    Bias.add_port(Bias_Branch, 3);
}

if (TUNER_TYPE == "SERIES_DISTRIBUTED_2")
{
    // RF Circuit
    // Create microstrip - add switch here for CPW options
    // Material specs
    if (SDM_INPUT_TYPE == "COPLANAR")
    {

        Input_Strip_CPW.substrate(Insulation);
        Input_Strip_CPW.top_strip(Top);

        // Physical dimensions
        Input_Strip_CPW.length = & SDM_INPUT_length;
        Input_Strip_CPW.width = & SDM_INPUT_width;
        Input_Strip_CPW.space = & SDM_INPUT_width2;

        Input_Strip = &Input_Strip_CPW;
    }
    else // assume microstrip
    {
        Input_Strip_MS.ground_plane(Ground);
        Input_Strip_MS.substrate(Insulation);
        Input_Strip_MS.top_strip(Top);
        Input_Strip_MS.superstrate(Cover);
        // Physical dimensions
        Input_Strip_MS.length = & SDM_INPUT_length;
        Input_Strip_MS.width = & SDM_INPUT_width;
        Input_Strip_MS.sub_thick = & SDM_INPUT_width2;

        Input_Strip = &Input_Strip_MS;
    }

    if (SDM_TUNER_12_TYPE == "COPLANAR")
    {

        Tuning_Strip_CPW.substrate(Insulation);
        Tuning_Strip_CPW.top_strip(Top);

        // Physical dimensions
        Tuning_Strip_CPW.length = & SDM_TUNER_12_length;
        Tuning_Strip_CPW.width = & SDM_TUNER_12_width;
        Tuning_Strip_CPW.space = & SDM_TUNER_12_width2;

        Tuning_Strip = &Tuning_Strip_CPW;
    }
    else // assume microstrip
    {
        Tuning_Strip_MS.ground_plane(Ground);
        Tuning_Strip_MS.substrate(Insulation);
        Tuning_Strip_MS.top_strip(Top);
        Tuning_Strip_MS.superstrate(Cover);
        // Physical dimensions
        Tuning_Strip_MS.length = & SDM_TUNER_12_length;
        Tuning_Strip_MS.width = & SDM_TUNER_12_width;
        Tuning_Strip_MS.sub_thick = & SDM_TUNER_12_width2;

        Tuning_Strip = &Tuning_Strip_MS;
    }


    Rf_Branch.set_branches(3);

    // Connect circuit
    Rf.connect(Rf_Input, 2, *Input_Strip, 1);
    Rf.connect(*Input_Strip, 2, Series_Tee_1, 1);
    Rf.connect(Series_Tee_1, 3, *Tuning_Strip, 1);

    // add ports
    Rf.add_port(Series_Tee_1, 2); // SIS 1
    Rf.add_port(*Tuning_Strip, 2);  // SIS 2
    Rf.add_port(Rf_Input, 1);   // RF Input

    // IF Circuit

    // Connect circuit and add ports
    If.connect(*Tuning_Strip, 2, Series_Tee_1, 1);
    If.connect(Series_Tee_1, 3, If_Load, 1);
    If.add_port(*Tuning_Strip, 1);   // SIS_1
    If.add_port(Series_Tee_1, 2);  // SIS_2
    If.add_port(If_Load, 2);  // If output

    // Create the two SIS junctions
    JUNCTION_AREA = SDM_JUNCTION_1_AREA;

    Sis_1.set_iv(Iv);
    Sis_1.Vn = JUNCTION_VGAP;
    Sis_1.Rn = JUNCTION_RNA/JUNCTION_AREA;
    Sis_1.Cap = JUNCTION_SCAP*JUNCTION_AREA;

    JUNCTION_AREA = SDM_JUNCTION_2_AREA;

    Sis_2.set_iv(Iv);
    Sis_2.Vn = JUNCTION_VGAP;
    Sis_2.Rn = JUNCTION_RNA/JUNCTION_AREA;
    Sis_2.Cap = JUNCTION_SCAP*JUNCTION_AREA;

    // Create the 2 junction bias circuit
    Bias_Source.source_f = 0.0;
    Bias_Source.source_voltage = & Bias_Voltage;
    Bias.connect(Bias_Source, 1, Series_Tee_1, 1);
    Bias.add_port(Series_Tee_1, 2);
    Bias.add_port(Series_Tee_1, 3);
}


if (TUNER_TYPE == "SERIES_DISTRIBUTED_3")
{
    // RF Circuit
    // Create microstrip - add switch here for CPW options
    // Material specs
    if (SDM_INPUT_TYPE == "COPLANAR")
    {
        Input_Strip_CPW.substrate(Insulation);
        Input_Strip_CPW.top_strip(Top);

        // Physical dimensions
        Input_Strip_CPW.length = & SDM_INPUT_length;
        Input_Strip_CPW.width = & SDM_INPUT_width;
        Input_Strip_CPW.space = & SDM_INPUT_width2;

        Input_Strip = &Input_Strip_CPW;
    }
    else // assume microstrip
    {
        Input_Strip_MS.ground_plane(Ground);
        Input_Strip_MS.substrate(Insulation);
        Input_Strip_MS.top_strip(Top);
        Input_Strip_MS.superstrate(Cover);
        // Physical dimensions
        Input_Strip_MS.length = & SDM_INPUT_length;
        Input_Strip_MS.width = & SDM_INPUT_width;
        Input_Strip_MS.sub_thick = & SDM_INPUT_width2;

        Input_Strip = &Input_Strip_MS;
    }

    if (SDM_TUNER_12_TYPE == "COPLANAR")
    {

        Tuning_Strip_CPW.substrate(Insulation);
        Tuning_Strip_CPW.top_strip(Top);

        // Physical dimensions
        Tuning_Strip_CPW.length = & SDM_TUNER_12_length;
        Tuning_Strip_CPW.width = & SDM_TUNER_12_width;
        Tuning_Strip_CPW.space = & SDM_TUNER_12_width2;

        Tuning_Strip = &Tuning_Strip_CPW;
    }
    else // assume microstrip
    {
        Tuning_Strip_MS.ground_plane(Ground);
        Tuning_Strip_MS.substrate(Insulation);
        Tuning_Strip_MS.top_strip(Top);
        Tuning_Strip_MS.superstrate(Cover);
        // Physical dimensions
        Tuning_Strip_MS.length = & SDM_TUNER_12_length;
        Tuning_Strip_MS.width = & SDM_TUNER_12_width;
        Tuning_Strip_MS.sub_thick = & SDM_TUNER_12_width2;

        Tuning_Strip = &Tuning_Strip_MS;
    }

    if (SDM_TUNER_23_TYPE == "COPLANAR")
    {

        Tuning_Strip_2_CPW.substrate(Insulation);
        Tuning_Strip_2_CPW.top_strip(Top);

        // Physical dimensions
        Tuning_Strip_2_CPW.length = & SDM_TUNER_23_length;
        Tuning_Strip_2_CPW.width = & SDM_TUNER_23_width;
        Tuning_Strip_2_CPW.space = & SDM_TUNER_23_width2;

        Tuning_Strip_2 = &Tuning_Strip_2_CPW;
    }
    else // assume microstrip
    {
        Tuning_Strip_2_MS.ground_plane(Ground);
        Tuning_Strip_2_MS.substrate(Insulation);
        Tuning_Strip_2_MS.top_strip(Top);
        Tuning_Strip_2_MS.superstrate(Cover);
        // Physical dimensions
        Tuning_Strip_2_MS.length = & SDM_TUNER_23_length;
        Tuning_Strip_2_MS.width = & SDM_TUNER_23_width;
        Tuning_Strip_2_MS.sub_thick = & SDM_TUNER_23_width2;

        Tuning_Strip_2 = &Tuning_Strip_2_MS;
    }

    // Connect circuit
    Rf.connect(Rf_Input, 2, *Input_Strip, 1);
    Rf.connect(*Input_Strip, 2, Series_Tee_1, 1);
    Rf.connect(Series_Tee_1, 3, *Tuning_Strip, 1);
    Rf.connect(*Tuning_Strip, 2, Series_Tee_2, 1);
    Rf.connect(Series_Tee_2, 3, *Tuning_Strip_2, 1);


    // add ports
    Rf.add_port(Series_Tee_1, 2); // SIS 1
    Rf.add_port(Series_Tee_2, 2);  // SIS 2
    Rf.add_port(*Tuning_Strip_2, 2);  // SIS 3
    Rf.add_port(Rf_Input, 1);   // RF Input

    // IF Circuit

    // Connect circuit and add ports
    If.connect(If_Load, 1, *Input_Strip, 1);
    If.connect(*Input_Strip, 2, Series_Tee_1, 1);
    If.connect(Series_Tee_1, 3, *Tuning_Strip, 1);
    If.connect(*Tuning_Strip, 2, Series_Tee_2, 1);
    If.connect(Series_Tee_2, 3, *Tuning_Strip_2, 1);


    // add ports
    If.add_port(Series_Tee_1, 2); // SIS 1
    If.add_port(Series_Tee_2, 2);  // SIS 2
    If.add_port(*Tuning_Strip_2, 2);  // SIS 3
    If.add_port(If_Load, 2);   // RF Input

    // Create the three SIS junctions
    JUNCTION_AREA = SDM_JUNCTION_1_AREA;

    Sis_1.set_iv(Iv);
    Sis_1.Vn = JUNCTION_VGAP;
    Sis_1.Rn = JUNCTION_RNA/JUNCTION_AREA;
    Sis_1.Cap = JUNCTION_SCAP*JUNCTION_AREA;

    JUNCTION_AREA = SDM_JUNCTION_2_AREA;

    Sis_2.set_iv(Iv);
    Sis_2.Vn = JUNCTION_VGAP;
    Sis_2.Rn = JUNCTION_RNA/JUNCTION_AREA;
    Sis_2.Cap = JUNCTION_SCAP*JUNCTION_AREA;

    JUNCTION_AREA = SDM_JUNCTION_3_AREA;

    Sis_3.set_iv(Iv);
    Sis_3.Vn = JUNCTION_VGAP;
    Sis_3.Rn = JUNCTION_RNA/JUNCTION_AREA;
    Sis_3.Cap = JUNCTION_SCAP*JUNCTION_AREA;

    // Create the 2 junction bias circuit
    Bias_Source.source_f = 0.0;
    Bias_Source.source_voltage = & Bias_Voltage;
    Bias.connect(Bias_Source, 1, *Input_Strip, 1);
    Bias.connect(*Input_Strip, 2, Series_Tee_1, 1);
    Bias.connect(Series_Tee_1, 3, *Tuning_Strip, 1);
    Bias.connect(*Tuning_Strip, 2, Series_Tee_2, 1);
    Bias.connect(Series_Tee_2, 3, *Tuning_Strip_2, 1);

    Bias.add_port(Series_Tee_1, 2);
    Bias.add_port(Series_Tee_2, 2);
    Bias.add_port(*Tuning_Strip_2, 2);
    Bias.add_port(*Input_Strip, 1);
}

if (TUNER_TYPE == "SERIES_DISTRIBUTED_4")
{
    // RF Circuit
    // Create microstrip - add switch here for CPW options
    // Material specs
    if (SDM_INPUT_TYPE == "COPLANAR")
    {

        Input_Strip_CPW.substrate(Insulation);
        Input_Strip_CPW.top_strip(Top);

        // Physical dimensions
        Input_Strip_CPW.length = & SDM_INPUT_length;
        Input_Strip_CPW.width = & SDM_INPUT_width;
        Input_Strip_CPW.space = & SDM_INPUT_width2;

        Input_Strip = &Input_Strip_CPW;
    }
    else // assume microstrip
    {
        Input_Strip_MS.ground_plane(Ground);
        Input_Strip_MS.substrate(Insulation);
        Input_Strip_MS.top_strip(Top);
        Input_Strip_MS.superstrate(Cover);
        // Physical dimensions
        Input_Strip_MS.length = & SDM_INPUT_length;
        Input_Strip_MS.width = & SDM_INPUT_width;
        Input_Strip_MS.sub_thick = & SDM_INPUT_width2;

        Input_Strip = &Input_Strip_MS;
    }

    if (SDM_TUNER_12_TYPE == "COPLANAR")
    {

        Tuning_Strip_CPW.substrate(Insulation);
        Tuning_Strip_CPW.top_strip(Top);

        // Physical dimensions
        Tuning_Strip_CPW.length = & SDM_TUNER_12_length;
        Tuning_Strip_CPW.width = & SDM_TUNER_12_width;
        Tuning_Strip_CPW.space = & SDM_TUNER_12_width2;

        Tuning_Strip = &Tuning_Strip_CPW;
    }
    else // assume microstrip
    {
        Tuning_Strip_MS.ground_plane(Ground);
        Tuning_Strip_MS.substrate(Insulation);
        Tuning_Strip_MS.top_strip(Top);
        Tuning_Strip_MS.superstrate(Cover);
        // Physical dimensions
        Tuning_Strip_MS.length = & SDM_TUNER_12_length;
        Tuning_Strip_MS.width = & SDM_TUNER_12_width;
        Tuning_Strip_MS.sub_thick = & SDM_TUNER_12_width2;

        Tuning_Strip = &Tuning_Strip_MS;
    }

    if (SDM_TUNER_23_TYPE == "COPLANAR")
    {

        Tuning_Strip_2_CPW.substrate(Insulation);
        Tuning_Strip_2_CPW.top_strip(Top);

        // Physical dimensions
        Tuning_Strip_2_CPW.length = & SDM_TUNER_23_length;
        Tuning_Strip_2_CPW.width = & SDM_TUNER_23_width;
        Tuning_Strip_2_CPW.space = & SDM_TUNER_23_width2;

        Tuning_Strip_2 = &Tuning_Strip_2_CPW;
    }
    else // assume microstrip
    {
        Tuning_Strip_2_MS.ground_plane(Ground);
        Tuning_Strip_2_MS.substrate(Insulation);
        Tuning_Strip_2_MS.top_strip(Top);
        Tuning_Strip_2_MS.superstrate(Cover);
        // Physical dimensions
        Tuning_Strip_2_MS.length = & SDM_TUNER_23_length;
        Tuning_Strip_2_MS.width = & SDM_TUNER_23_width;
        Tuning_Strip_2_MS.sub_thick = & SDM_TUNER_23_width2;

        Tuning_Strip_2 = &Tuning_Strip_2_MS;
    }

    if (SDM_TUNER_34_TYPE == "COPLANAR")
    {

        Tuning_Strip_3_CPW.substrate(Insulation);
        Tuning_Strip_3_CPW.top_strip(Top);

        // Physical dimensions
        Tuning_Strip_3_CPW.length = & SDM_TUNER_34_length;
        Tuning_Strip_3_CPW.width = & SDM_TUNER_34_width;
        Tuning_Strip_3_CPW.space = & SDM_TUNER_34_width2;

        Tuning_Strip_3 = &Tuning_Strip_3_CPW;
    }
    else // assume microstrip
    {
        Tuning_Strip_3_MS.ground_plane(Ground);
        Tuning_Strip_3_MS.substrate(Insulation);
        Tuning_Strip_3_MS.top_strip(Top);
        Tuning_Strip_3_MS.superstrate(Cover);
        // Physical dimensions
        Tuning_Strip_3_MS.length = & SDM_TUNER_34_length;
        Tuning_Strip_3_MS.width = & SDM_TUNER_34_width;
        Tuning_Strip_3_MS.sub_thick = & SDM_TUNER_34_width2;

        Tuning_Strip_3 = &Tuning_Strip_3_MS;
    }

    // Connect circuit
    Rf.connect(Rf_Input, 2, *Input_Strip, 1);
    Rf.connect(*Input_Strip, 2, Series_Tee_1, 1);
    Rf.connect(Series_Tee_1, 3, *Tuning_Strip, 1);
    Rf.connect(*Tuning_Strip, 2, Series_Tee_2, 1);
    Rf.connect(Series_Tee_2, 3, *Tuning_Strip_2, 1);
    Rf.connect(*Tuning_Strip_2, 2, Series_Tee_3, 1);
    Rf.connect(Series_Tee_3, 3, *Tuning_Strip_3, 1);


    // add ports
    Rf.add_port(Series_Tee_1, 2); // SIS 1
    Rf.add_port(Series_Tee_2, 2);  // SIS 2
    Rf.add_port(Series_Tee_3, 2);  // SIS 3
    Rf.add_port(*Tuning_Strip_3, 2);  // SIS 4
    Rf.add_port(Rf_Input, 1);   // RF Input

    // IF Circuit

    // Connect circuit and add ports
    If.connect(If_Load, 1, *Input_Strip, 1);
    If.connect(*Input_Strip, 2, Series_Tee_1, 1);
    If.connect(Series_Tee_1, 3, *Tuning_Strip, 1);
    If.connect(*Tuning_Strip, 2, Series_Tee_2, 1);
    If.connect(Series_Tee_2, 3, *Tuning_Strip_2, 1);
    If.connect(*Tuning_Strip_2, 2, Series_Tee_3, 1);
    If.connect(Series_Tee_3, 3, *Tuning_Strip_3, 1);


    // add ports
    If.add_port(Series_Tee_1, 2); // SIS 1
    If.add_port(Series_Tee_2, 2);  // SIS 2
    If.add_port(Series_Tee_3, 2);  // SIS 3
    If.add_port(*Tuning_Strip_3, 2);  // SIS 4
    If.add_port(If_Load, 2);   // RF Input

    // Create the four SIS junctions
    JUNCTION_AREA = SDM_JUNCTION_1_AREA;

    Sis_1.set_iv(Iv);
    Sis_1.Vn = JUNCTION_VGAP;
    Sis_1.Rn = JUNCTION_RNA/JUNCTION_AREA;
    Sis_1.Cap = JUNCTION_SCAP*JUNCTION_AREA;

    JUNCTION_AREA = SDM_JUNCTION_2_AREA;

    Sis_2.set_iv(Iv);
    Sis_2.Vn = JUNCTION_VGAP;
    Sis_2.Rn = JUNCTION_RNA/JUNCTION_AREA;
    Sis_2.Cap = JUNCTION_SCAP*JUNCTION_AREA;

    JUNCTION_AREA = SDM_JUNCTION_3_AREA;

    Sis_3.set_iv(Iv);
    Sis_3.Vn = JUNCTION_VGAP;
    Sis_3.Rn = JUNCTION_RNA/JUNCTION_AREA;
    Sis_3.Cap = JUNCTION_SCAP*JUNCTION_AREA;

    JUNCTION_AREA = SDM_JUNCTION_4_AREA;

    Sis_4.set_iv(Iv);
    Sis_4.Vn = JUNCTION_VGAP;
    Sis_4.Rn = JUNCTION_RNA/JUNCTION_AREA;
    Sis_4.Cap = JUNCTION_SCAP*JUNCTION_AREA;

    // Create the 2 junction bias circuit
    Bias_Source.source_f = 0.0;
    Bias_Source.source_voltage = & Bias_Voltage;
    Bias.connect(Bias_Source, 1, *Input_Strip, 1);
    Bias.connect(*Input_Strip, 2, Series_Tee_1, 1);
    Bias.connect(Series_Tee_1, 3, *Tuning_Strip, 1);
    Bias.connect(*Tuning_Strip, 2, Series_Tee_2, 1);
    Bias.connect(Series_Tee_2, 3, *Tuning_Strip_2, 1);
    Bias.connect(*Tuning_Strip_2, 2, Series_Tee_3, 1);
    Bias.connect(Series_Tee_3, 3, *Tuning_Strip_3, 1);

    Bias.add_port(Series_Tee_1, 2);
    Bias.add_port(Series_Tee_2, 2);
    Bias.add_port(Series_Tee_3, 2);
    Bias.add_port(*Tuning_Strip_3, 2);
    Bias.add_port(*Input_Strip, 1);
}


printf("# tuner complete\n");
