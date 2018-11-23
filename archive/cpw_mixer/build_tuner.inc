/******************************************************************************
** build_tuner.inc - file included in cpw_trafo.cc, containing code to build **
**                   the RF and IF circuits for the mixer                    **
** ========================================================================= **
** Author - Paul Grimes                                                      **
** Date   - 22/07/03                                                         **
** ========================================================================= **
** The tuning circuit consists of:                                           **
**     |--------|  |--------|  |--------|       |--------|  |----------|     **
** RF--| Trafo1 |--| Trafo2 |--| Trafo3 |--(X)--| StubIn |--| Rad_Stub |--IF **
**     |--------|  |--------|  |--------|   |   |--------|  |----------|     **
**                                          E                                **
** where Trafo1, 2 and 3 and StubIn may be cpw_new or microstrip objects     **
** ========================================================================= **
** Changes:                                                                  **
** 27/05/09 - Converted to CPW/microstrip hybrid Trafo tuning or external    **
**            Touchstone file tuning only.                                   **
******************************************************************************/

printf("# building tuner...\n");

circuit Rf;
circuit If;

transformer If_Dummy;
If_Dummy.Z1 = & IF_Z0;
If_Dummy.Z2 = & IF_Z0;

transformer If_Load;
If_Load.Z1 = & IF_Z0;

transformer Rf_Input;
Rf_Input.Z2 = & Z0;
Rf_Input.Z1 = & Z0;

sdata_interp External(2, device::f);
External.touchstone(TOUCHSTONE_FILE.c_str());

zterm Rf_Choke_Term;
    Rf_Choke_Term.Z = IF_Z0;
open_term If_Term;

// Define the trafo tuning components
cpw_new Trafo1_cpw;
    Trafo1_cpw.top_strip(Top);
    Trafo1_cpw.substrate(Substrate);
    Trafo1_cpw.width = & TRAFO1_width;
    Trafo1_cpw.space = & TRAFO1_space;
    Trafo1_cpw.length = & TRAFO1_length;
    Trafo1_cpw.sub_thick = & SUBSTRATE_THICKNESS;

cpw_new If_Trafo1_cpw = Trafo1_cpw;

cpw_new Trafo2_cpw;
    Trafo2_cpw.top_strip(Top);
    Trafo2_cpw.substrate(Substrate);
    Trafo2_cpw.width = & TRAFO2_width;
    Trafo2_cpw.space = & TRAFO2_space;
    Trafo2_cpw.length = & TRAFO2_length;
    Trafo2_cpw.sub_thick = & SUBSTRATE_THICKNESS;

cpw_new If_Trafo2_cpw = Trafo2_cpw;

cpw_new Trafo3_cpw;
    Trafo3_cpw.top_strip(Top);
    Trafo3_cpw.substrate(Substrate);
    Trafo3_cpw.width = & TRAFO3_width;
    Trafo3_cpw.space = & TRAFO3_space;
    Trafo3_cpw.length = & TRAFO3_length;
    Trafo3_cpw.sub_thick = & SUBSTRATE_THICKNESS;

cpw_new If_Trafo3_cpw = Trafo3_cpw;

cpw_new StubIn_cpw;
    StubIn_cpw.top_strip(Top);
    StubIn_cpw.substrate(Substrate);
    StubIn_cpw.width = & STUBIN_width;
    StubIn_cpw.space = & STUBIN_space;
    StubIn_cpw.length = & STUBIN_length;
    StubIn_cpw.sub_thick = & SUBSTRATE_THICKNESS;

cpw_new If_StubIn_cpw = StubIn_cpw;

microstrip Trafo1_ms;
    Trafo1_ms.superstrate(Cover);
    Trafo1_ms.top_strip(Top);
    Trafo1_ms.substrate(Insulation);
    Trafo1_ms.ground_plane(Ground);
    Trafo1_ms.sub_thick = & INSUL_THICKNESS;
    Trafo1_ms.width = & TRAFO1_width;
    Trafo1_ms.length = & TRAFO1_length;

microstrip If_Trafo1_ms = Trafo1_ms;

microstrip Trafo2_ms;
    Trafo2_ms.superstrate(Cover);
    Trafo2_ms.top_strip(Top);
    Trafo2_ms.substrate(Insulation);
    Trafo2_ms.ground_plane(Ground);
    Trafo2_ms.sub_thick = & INSUL_THICKNESS;
    Trafo2_ms.width = & TRAFO2_width;
    Trafo2_ms.length = & TRAFO2_length;

microstrip If_Trafo2_ms = Trafo2_ms;

microstrip Trafo3_ms;
    Trafo3_ms.superstrate(Cover);
    Trafo3_ms.top_strip(Top);
    Trafo3_ms.substrate(Insulation);
    Trafo3_ms.ground_plane(Ground);
    Trafo3_ms.sub_thick = & INSUL_THICKNESS;
    Trafo3_ms.width = & TRAFO3_width;
    Trafo3_ms.length = & TRAFO3_length;

microstrip If_Trafo3_ms = Trafo3_ms;

microstrip StubIn_ms;
    StubIn_ms.superstrate(Cover);
    StubIn_ms.top_strip(Top);
    StubIn_ms.substrate(Insulation);
    StubIn_ms.ground_plane(Ground);
    StubIn_ms.sub_thick = & INSUL_THICKNESS;
    StubIn_ms.width = & STUBIN_width;
    StubIn_ms.length = & STUBIN_length;

microstrip If_StubIn_ms = StubIn_ms;

circuit Trafo1, Trafo2, Trafo3, StubIn;
circuit If_Trafo1, If_Trafo2, If_Trafo3, If_StubIn;

// Decide what type of each line should be
if (TRAFO1_TYPE == "cpw")
{
    Trafo1.add_port(Trafo1_cpw, 1);
    Trafo1.add_port(Trafo1_cpw, 2);

    If_Trafo1.add_port(If_Trafo1_cpw, 1);
    If_Trafo1.add_port(If_Trafo1_cpw, 2);
}
else
{
    Trafo1.add_port(Trafo1_ms, 1);
    Trafo1.add_port(Trafo1_ms, 2);

    If_Trafo1.add_port(If_Trafo1_ms, 1);
    If_Trafo1.add_port(If_Trafo1_ms, 2);

}

if (TRAFO2_TYPE == "cpw")
{
    Trafo2.add_port(Trafo2_cpw, 1);
    Trafo2.add_port(Trafo2_cpw, 2);

    If_Trafo2.add_port(If_Trafo2_cpw, 1);
    If_Trafo2.add_port(If_Trafo2_cpw, 2);
}
else
{
    Trafo2.add_port(Trafo2_ms, 1);
    Trafo2.add_port(Trafo2_ms, 2);

    If_Trafo2.add_port(If_Trafo2_ms, 1);
    If_Trafo2.add_port(If_Trafo2_ms, 2);

}

if (TRAFO3_TYPE == "cpw")
{
    Trafo3.add_port(Trafo3_cpw, 1);
    Trafo3.add_port(Trafo3_cpw, 2);

    If_Trafo3.add_port(If_Trafo3_cpw, 1);
    If_Trafo3.add_port(If_Trafo3_cpw, 2);
}
else
{
    Trafo3.add_port(Trafo3_ms, 1);
    Trafo3.add_port(Trafo3_ms, 2);

    If_Trafo3.add_port(If_Trafo3_ms, 1);
    If_Trafo3.add_port(If_Trafo3_ms, 2);

}

if (STUBIN_TYPE == "cpw")
{
    StubIn.add_port(StubIn_cpw, 1);
    StubIn.add_port(StubIn_cpw, 2);

    If_StubIn.add_port(If_StubIn_cpw, 1);
    If_StubIn.add_port(If_StubIn_cpw, 2);
}
else
{
    StubIn.add_port(StubIn_ms, 1);
    StubIn.add_port(StubIn_ms, 2);

    If_StubIn.add_port(If_StubIn_ms, 1);
    If_StubIn.add_port(If_StubIn_ms, 2);

}




// Radial stub used to terminate the Trafo tuning.
// The stub has a short section of input microstrip used to connect the StubIn
// CPW/microstrip to the half moon section.  This should nominally by about
// 3 micron long.
radial_stub RadialStub;
    RadialStub.superstrate(Cover);
    RadialStub.top_strip(Top);
    RadialStub.substrate(Insulation);
    RadialStub.ground_plane(Ground);
    RadialStub.radius = & RADIAL_STUB_FAN_radius;
    RadialStub.angle = & RADIAL_STUB_FAN_angle;
    RadialStub.sub_thick = & INSUL_THICKNESS;
    RadialStub.width = & RADIAL_STUB_INPUT_width;
    RadialStub.length = & RADIAL_STUB_INPUT_length;

radial_stub If_RadialStub = RadialStub;

branch Rf_Branch;
branch If_Branch;

if (TUNER_TYPE == "EXTERNAL") // We are using an external simulation
{
    Rf.connect(Rf_Input, 2, External, 1);
    Rf.add_port(External, 2); // Sis goes here
    Rf.add_port(Rf_Input, 1);    // LO goes here

    If.connect(If_Dummy, 2, If_Load, 1);
    If.add_port(If_Dummy, 1);    // SIS goes here
    If.add_port(If_Load, 2);     // LO goes here
}
else // We are building the tuner from internal components
{

    Rf.connect(Rf_Input, 2, Trafo1, 1);
    Rf.connect(Trafo1, 2, Trafo2, 1);
    Rf.connect(Trafo2, 2, Trafo3, 1);
    Rf.connect(Trafo3, 2, Rf_Branch, 1);
    Rf.connect(Rf_Branch, 3, StubIn, 1);
    Rf.connect(StubIn, 2, RadialStub, 1);
    Rf.connect(RadialStub, 2, Rf_Choke_Term, 1);

    Rf.add_port(Rf_Branch, 2); // Sis goes here
    Rf.add_port(Rf_Input, 1);  // LO goes here

    //printf("# RF Circuit check: %f %f %f %f\n", Substrate.eps/1.0, Substrate.tand/1.0, Trafo1.sub_thick/Micron, Trafo1.get_data().SdB(1,1));

    If.connect(If_Term, 1, If_Trafo1, 1);
    If.connect(If_Trafo1, 2, If_Trafo2, 1);
    If.connect(If_Trafo2, 2, If_Trafo3, 1);
    If.connect(If_Trafo3, 2, If_Branch, 1);
    If.connect(If_Branch, 3, If_StubIn, 1);
    If.connect(If_StubIn, 2, If_RadialStub, 1);
    If.connect(If_RadialStub, 2, If_Dummy, 1);
    If.connect(If_Dummy, 2, If_Load, 1);

    If.add_port(If_Branch, 2);    // SIS goes here
    //If.add_port(If_StubIn, 1);   // SIS goes here, no IF on input
    If.add_port(If_Load, 2);     // IF Output goes here
}


printf("# tuner complete\n");


