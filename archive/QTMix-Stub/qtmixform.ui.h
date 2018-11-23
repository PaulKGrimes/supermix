/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
#include "supermix.h"
#include "qfiledialog.h"
#include "qfile.h"
#include "qmessagebox.h"
#include "qtextstream.h"
#include "filechooser.h"


void QTMixForm::calculate()
{
    // Balance the mixer
    mix.balance();
    
    // Set the results boxes    
    unpumpLineEdit->setText( QString::number( iv.I( bias.source_voltage / (sis.Vn*mVolt) ).imaginary *sis.Vn/sis.Rn / (Micro*Amp), 'f', 4 ).append( " uA" ) );
    pumpedLineEdit->setText( QString::number( (mix.I_junc(0)[1].real)/ (Micro*Amp), 'f', 4 ).append( " uA" ) );
    lsbGainLineEdit->setText( QString::number( 10*log10(norm(mix.get_data().S[IF_PORT][LSB_PORT])), 'f', 4 ).append( " dB" ) );
    usbGainLineEdit->setText( QString::number( 10*log10(norm(mix.get_data().S[IF_PORT][USB_PORT])), 'f', 4 ).append( " dB" ) );
    ifOutputLineEdit->setText( QString::number( mix.get_term_data().C[IF_PORT][IF_PORT].real / Kelvin, 'f', 4 ).append( " K" ) );
    noiseLineEdit->setText( QString::number( mix.get_term_data().C[IF_PORT][IF_PORT].real/(norm(mix.get_data().S[IF_PORT][LSB_PORT])+norm(mix.get_data().S[IF_PORT][USB_PORT])) / Kelvin ).append( " K" ) );    
    
    // Insert marker onto plot
    double x = 0.0;
    
    switch ( plotXAxisComboBox->currentItem() )
    {
    case 0:
	{
	    x = biasDoubleSpinBox->value();
	    break;
	}
    case 1:
	{
	    x = loPowerDoubleSpinBox->value();
	    break;
	}
    case 2:
	{
	    x = freqDoubleSpinBox->value();
	    break;
	}
    case 3:
	{
	    x = IFfreqDoubleSpinBox->value();
	    break;
	}
    }
		
    plot->removeMarkers();
    long mark = plot->insertMarker( );
    plot->setMarkerLineStyle( mark, QwtMarker::VLine );
    plot->setMarkerXPos( mark, x );
    
    plot->replot();
    
}


void QTMixForm::init()
{    
    //=======================
    // Things that should only be done once
    if ( !(mix.junctions()==2) )
    {
	// This code will call setIVdata() automagically
	idcFileChooser->setFileName( "/data/simulations/QTMix-Stub/idc.dat");
	ikkFileChooser->setFileName( "/data/simulations/QTMix-Stub/ikk.dat");
		
	sis.set_iv(iv);
	
	// Set up operating point devices (parts that don't change)
	// Set the LO voltage_source
	lo.R = & device::Z0;
	lo.source_f = 650*GHz;

	// Set the bias source
	bias.R = 0.0;
	bias.f = 0.0;
		
	// Set up the stubs
	stub1.substrate( substrate );
	stub1.superstrate( superstrate );
	stub1Strip.substrate( substrate );
	stub1Strip.superstrate( superstrate );
	
	stub2.substrate( substrate );
	stub2.superstrate( superstrate );
	stub2Strip.substrate( substrate );
	stub2Strip.superstrate( superstrate );
	
	stub3.substrate( substrate );
	stub3.superstrate( superstrate );
	stub3Strip.substrate( substrate );
	stub3Strip.superstrate( superstrate );
	
	// Set up the three branches we will use
	rf1StubBranch.set_branches(3);
	rf2StubBranch.set_branches(4);
	rf3StubBranch.set_branches(5);

	// Set up the RF circuits
	rf1Stub.connect(rf1StubBranch, 2, stub1, 1);
	rf1Stub.connect(stub1, 2, rfStub1Term, 1);
	rf1Stub.add_port(rf1StubBranch, 3);
	rf1Stub.add_port(rf1StubBranch, 1);
	
	rf2Stub.connect(rf2StubBranch, 2, stub1, 1);
	rf2Stub.connect(stub1, 2, rfStub1Term, 1);
	rf2Stub.connect(rf2StubBranch, 3, stub2, 1);
	rf2Stub.connect(stub2, 2, rfStub2Term, 1);
	rf2Stub.add_port(rf2StubBranch, 4);
	rf2Stub.add_port(rf2StubBranch, 1);
	
	rf3Stub.connect(rf3StubBranch, 2, stub1, 1);
	rf3Stub.connect(stub1, 2, rfStub1Term, 1);
	rf3Stub.connect(rf3StubBranch, 3, stub2, 1);
	rf3Stub.connect(stub2, 2, rfStub2Term, 1);
	rf3Stub.connect(rf3StubBranch, 4, stub3, 1);
	rf3Stub.connect(stub3, 2, rfStub3Term, 1);
	rf3Stub.add_port(rf3StubBranch, 5);
	rf3Stub.add_port(rf3StubBranch, 1);
	
	// Setup IF circuit
	ifCircuit.connect(stub1, 2, ifTrans, 1);
	ifCircuit.add_port( stub1, 1);
	ifCircuit.add_port( ifTrans, 2);
	
	juncCap.parallel();
	
	// Set up the  Zemb calculation circuits
	rf1StubZemb.connect(rf1StubBranch, 2, stub1, 1);
	rf1StubZemb.connect(stub1, 2, rfStub1Term, 1);
	rf1StubZemb.connect(rf1StubBranch, 3, juncCap, 1);
	rf1StubZemb.add_port(rf1StubBranch, 1);
	rf1StubZemb.add_port(juncCap, 2);
	
	rf2StubZemb.connect(rf2StubBranch, 2, stub1, 1);
	rf2StubZemb.connect(stub1, 2, rfStub1Term, 1);
	rf2StubZemb.connect(rf2StubBranch, 3, stub2, 1);
	rf2StubZemb.connect(stub2, 2, rfStub2Term, 1);
	rf2StubZemb.connect(rf2StubBranch, 4, juncCap, 1);
	rf2StubZemb.add_port(rf2StubBranch, 1);
	rf2StubZemb.add_port(juncCap, 2);
	
	rf3StubZemb.connect(rf3StubBranch, 2, stub1, 1);
	rf3StubZemb.connect(stub1, 2, rfStub1Term, 1);
	rf3StubZemb.connect(rf3StubBranch, 3, stub2, 1);
	rf3StubZemb.connect(stub2, 2, rfStub2Term, 1);
	rf3StubZemb.connect(rf3StubBranch, 4, stub3, 1);
	rf3StubZemb.connect(stub3, 2, rfStub3Term, 1);
	rf3StubZemb.connect(rf3StubBranch, 5, juncCap, 1);
	rf3StubZemb.add_port(rf3StubBranch, 1);
	rf3StubZemb.add_port(juncCap, 2);
		
	//Set up tue mixer
	mix.set_rf( rf1Stub );
	mix.set_if( ifCircuit );
	mix.set_LO( & lo.source_f ); // Shadow LO frequency
	mix.set_bias( bias );
	mix.add_junction( sis );
	mix.set_balance_terminator( lo, 2 );
	cout << "Completed mixer build...\n";
    }   
    //========================
    // Reset things that might have changed
    cout << "Setting globals...\n";
    
    // Simulation settings
    device::T = 4.2*Kelvin;
    device::Z0 = 20.0*Ohm;
        
    cout << "Setting junction parameters...\n";
    // Junction settings
    sis.Rn = device::Z0;
    sis.Vn = 2.80*mVolt;
    sis.Cap = 65.0*fFarad;
                
    cout << "Setting up materials...\n";
    // For SiO and air
    substrate.eps = 5.8;
    substrate.tand = 4e-4;
    superstrate.eps = 1.0;
    superstrate.tand = 0.0;
    
    // For Niobium
    superconductor.Vgap = sis.Vn;
    superconductor.Tc = 9.23*Kelvin;
    superconductor.rho_normal = 5.0*Micro*Ohm*Centi*Meter;
    superconductor.Thick = 0.4*Micron;
    
    // For Aluminium
    normalconductor.sigma( 2.688/(Micro*Ohm*Centi*Meter), 0.0 );
        
    cout << "Setting up Stubs...\n";
    // Set up Stubs
    stub1.top_strip( superconductor );
    stub1.ground_plane( superconductor );
    stub1.width = 3.0 * Micron;
    stub1.length = 5.0 * Micron;
    stub1.sub_thick = 0.42 * Micron;
    stub1.angle = 180 * Degree;
    stub1.radius = 30.0 * Micron;

    stub2.top_strip( superconductor );
    stub2.ground_plane( superconductor );
    stub2.width = 3.0 * Micron;
    stub2.length = 5.0 * Micron;
    stub2.sub_thick = 0.42 * Micron;
    stub2.angle = 180 * Degree;
    stub2.radius = 30.0 * Micron;

    stub3.top_strip( superconductor );
    stub3.ground_plane( superconductor );
    stub3.width = 3.0 * Micron;
    stub3.length = 5.0 * Micron;
    stub3.sub_thick = 0.42 * Micron;
    stub3.angle = 180 * Degree;
    stub3.radius = 30.0 * Micron;

    // Set up microstrips for shadowing  stub input strips
    stub1Strip.top_strip( superconductor );
    stub1Strip.ground_plane( superconductor );
    stub1Strip.superstrate( superstrate );
    stub1Strip.substrate( substrate );
    stub1Strip.width = & stub1.width;
    stub1Strip.length = & stub1.length;
    stub1Strip.sub_thick = & stub1.sub_thick;
    
    stub1Strip.top_strip( superconductor );
    stub1Strip.ground_plane( superconductor );
    stub1Strip.superstrate( superstrate );
    stub1Strip.substrate( substrate );
    stub2Strip.width = & stub2.width;
    stub2Strip.length = & stub2.length;
    stub2Strip.sub_thick = & stub2.sub_thick;
    
    stub3Strip.top_strip( superconductor );
    stub3Strip.ground_plane( superconductor );
    stub3Strip.superstrate( superstrate );
    stub3Strip.substrate( substrate );
    stub3Strip.width = & stub3.width;
    stub3Strip.length = & stub3.length;
    stub3Strip.sub_thick = & stub3.sub_thick;
    
    cout << "Setting IF transformer...\n";
    // Set up if transformer 
    ifTrans.Z1 = 50*Ohm;

    cout << "Setting operating point...\n";
    // Operating point
    lo.source_f = 650*GHz;
    device::f = 5*GHz;
    bias.source_voltage = 2.0*mVolt;
    lo.source_power = 100*Nano*Watt;
    lo.Temp = 0.0*Kelvin;
        
    cout << "Resetting Mixer...\n";
    // Create the mixer
    mix.set_rf( rf1Stub );
    mix.harmonics( 1 );
    mix.set_balance_terminator( lo, 2 );
    
    if (mix.flag_mixer_incomplete())
    {
	cout << "Mixer incomplete...\n";
	cout << "\tNum Junctions = " << mix.junctions() << endl;
	cout << "\tSize of RF circuit = " << rf1Stub.size() << endl;
	cout << "\tSize of IF circuit = " << ifCircuit.size() << endl;
	cout << "\tSize of Bias circuit = " << bias.size() << endl;
    }
    
    IF_PORT = mix.port(2, 0);
    USB_PORT = mix.port(2, 1);
    LSB_PORT = mix.port(2, -1);
    
    cout << "Balancing Mixer...\n";
    mix.initialize_operating_state();
    mix.balance();
    
    cout << "Setting plot controls...\n";
    // Plot controls
    plotLow = 0.0;
    plotHigh = 5.0;

    plotLowDoubleSpinBox->setValue( 0.0 );
    plotHighDoubleSpinBox->setValue( 5.0 );

    if(!plotXAxisComboBox->count())
    {
	plotXAxisComboBox->insertItem("Bias");
	plotXAxisComboBox->insertItem("LO Power");
	plotXAxisComboBox->insertItem("LO Frequency");
	plotXAxisComboBox->insertItem("IF Frequency");
    }
    plotXAxisComboBox->setCurrentItem( 0 );
    
    if (!plotLeftComboBox->count())
    {
	plotLeftComboBox->insertItem( "DC Current (unpump)" );
	plotLeftComboBox->insertItem( "KK Transform" );
	plotLeftComboBox->insertItem( "DC Current" );
	plotLeftComboBox->insertItem( "Mixer Gain (LSB)" );
	plotLeftComboBox->insertItem( "Mixer Gain (USB)" );
	plotLeftComboBox->insertItem( "Mixer Gain (DSB)" );
	plotLeftComboBox->insertItem( "Mixer Noise (log K)" );
	plotLeftComboBox->insertItem( "IF Output (K)" );
	plotLeftComboBox->insertItem( "LO Fundamental Current" );
	plotLeftComboBox->insertItem( "LO Impedance" );
	plotLeftComboBox->insertItem( "Re(Zemb) Junction" );
	plotLeftComboBox->insertItem( "Im(Zemb) Junction" );
	plotLeftComboBox->insertItem( "Coupling" );
    }
    plotLeftComboBox->setCurrentItem( 0 );

    if (!plotRightComboBox->count())
    {
	plotRightComboBox->insertItem( "DC Current (unpump)" );
	plotRightComboBox->insertItem( "KK Transform" );
	plotRightComboBox->insertItem( "DC Current" );
	plotRightComboBox->insertItem( "Mixer Gain (LSB)" );
	plotRightComboBox->insertItem( "Mixer Gain (USB)" );
	plotRightComboBox->insertItem( "Mixer Gain (DSB)" );
	plotRightComboBox->insertItem( "Mixer Noise (K)" );
	plotRightComboBox->insertItem( "IF Output" );
	plotRightComboBox->insertItem( "LO Fundamental Current" );
	plotRightComboBox->insertItem( "LO Impedance" );	
	plotRightComboBox->insertItem( "Re(Zemb) Junction" );
	plotRightComboBox->insertItem( "Im(Zemb) Junction" );
	plotRightComboBox->insertItem( "Coupling" );
    }
    plotRightComboBox->setCurrentItem( 2 );

    // Set up plot
    plot->setAutoLegend(TRUE);
    plot->setLegendPos(Qwt::Bottom);
    plot->setGridPen( QPen(darkGray, 1, SolidLine) );

    plot->setAxisTitle(plot->xBottom, "Bias");
    plot->setAxisTitle(plot->yLeft, "Unpumped IV");
    plot->setAxisTitle(plot->yRight, "DC Current");

    plot->removeCurves();
    plotLeftCurve = plot->insertCurve( plotLeftComboBox->currentText() );
    plotRightCurve = plot->insertCurve( plotLeftComboBox->currentText(), plot->xBottom, plot->yRight );

    plot->setCurvePen(plotLeftCurve, QPen(red));
    plot->setCurvePen(plotRightCurve, QPen(blue));
    
    // Set global spin boxes
    tDoubleSpinBox->setValue( 4.2 );
    z0DoubleSpinBox->setValue( 20.0 );
    ifImpDoubleSpinBox->setValue( 50.0 );
    harmonicsSpinBox->setValue( 1 );
        
    cout << "Setting junction control spin boxes...\n";
    // Set junction controls
    rnJuncDoubleSpinBox->setValue( device::Z0 / Ohm );
    vGapJuncDoubleSpinBox->setValue( 2.80 );
    capJuncDoubleSpinBox->setValue( 65.0 );
    
    // Set materials controls
    if ( !topStub1ComboBox->count() )
    {
	topStub1ComboBox->insertItem( "Superconductor" );
	topStub1ComboBox->insertItem( "Normal Metal" );
    }
    topStub1ComboBox->setCurrentItem( 0 );
    if ( !groundStub1ComboBox->count() )
    {
	groundStub1ComboBox->insertItem( "Superconductor" );
	groundStub1ComboBox->insertItem( "Normal Metal" );
    }
    groundStub1ComboBox->setCurrentItem( 0 );    

    if ( !topStub2ComboBox->count() )
    {
	topStub2ComboBox->insertItem( "Superconductor" );
	topStub2ComboBox->insertItem( "Normal Metal" );
    }
    topStub2ComboBox->setCurrentItem( 0 );
    if ( !groundStub2ComboBox->count() )
    {
	groundStub2ComboBox->insertItem( "Superconductor" );
	groundStub2ComboBox->insertItem( "Normal Metal" );
    }
    groundStub2ComboBox->setCurrentItem( 0 );    

    if ( !topStub3ComboBox->count() )
    {
	topStub3ComboBox->insertItem( "Superconductor" );
	topStub3ComboBox->insertItem( "Normal Metal" );
    }
    topStub3ComboBox->setCurrentItem( 0 );
    if ( !groundStub3ComboBox->count() )
    {
	groundStub3ComboBox->insertItem( "Superconductor" );
	groundStub3ComboBox->insertItem( "Normal Metal" );
    }
    groundStub3ComboBox->setCurrentItem( 0 );    

    
    cout << "Setting Stub controls...\n";
    // Set Stub  controls    
    widthStub1DoubleSpinBox->setValue( 3.0 );
    lengthStub1DoubleSpinBox->setValue( 5.0 );
    heightStub1DoubleSpinBox->setValue( 0.42 );
    angleStub1DoubleSpinBox->setValue( 180 );
    radiusStub1DoubleSpinBox->setValue( 30.0 );
    
    stub2CheckBox->setChecked( FALSE );
    widthStub2DoubleSpinBox->setValue( 3.0 );
    lengthStub2DoubleSpinBox->setValue( 5.0 );
    heightStub2DoubleSpinBox->setValue( 0.42 );
    angleStub2DoubleSpinBox->setValue( 180 );
    radiusStub2DoubleSpinBox->setValue( 30.0 );
    
    stub3CheckBox->setChecked( FALSE );
    widthStub3DoubleSpinBox->setValue( 3.0 );
    lengthStub3DoubleSpinBox->setValue( 5.0 );
    heightStub3DoubleSpinBox->setValue( 0.42 );
    angleStub3DoubleSpinBox->setValue( 180 );
    radiusStub3DoubleSpinBox->setValue( 30.0 );

    // Set operating point controls
    freqDoubleSpinBox->setValue( 650.0 );
    IFfreqDoubleSpinBox->setValue( 5.0 );
    biasDoubleSpinBox->setValue( 2.0 );
    loPowerDoubleSpinBox->setValue( 100 );
    loTempSpinBox->setValue( 0 );
    
    calcStripProperties();
    
    redrawPlot();
}


void QTMixForm::redrawPlot()
{
    // set mouse cursor to prevent more events while we do this
    setCursor( Qt::WaitCursor );
    
    plot->removeCurves();
    plotLeftCurve = plot->insertCurve( plotLeftComboBox->currentText() );
    plotRightCurve = plot->insertCurve( plotRightComboBox->currentText(), plot->xBottom, plot->yRight );

    plot->setCurvePen( plotLeftCurve, QPen(red) );
    plot->setCurvePen( plotRightCurve, QPen(blue) );

    plot->setAxisTitle( plot->xBottom, plotXAxisComboBox->currentText() );
    plot->setAxisTitle( plot->yLeft, plotLeftComboBox->currentText() );
    plot->setAxisTitle( plot->yRight, plotRightComboBox->currentText() );
    
    // Sanity checks on IF frequency (which must be lower than LO freq)
    if (plotXAxisComboBox->currentItem() == 2) // LO Freq scan
    {
	double low = device::f + 0.01;
	if (plotLowDoubleSpinBox->value()*GHz < low)
	{
	    plotLowDoubleSpinBox->setValue( low/GHz );
	    plotLow = low/GHz;
	}
    }
    
    if (plotXAxisComboBox->currentItem() == 3 ) // IF Freq scan
    {
	double high = lo.source_f - 0.01;
	if (plotHighDoubleSpinBox->value()*GHz > high)
	{
	    plotHighDoubleSpinBox->setValue( high/GHz );
	    plotHigh = high/GHz;
	}
    }
    
    // Sanity checks on Harmonics
    if ( (plotLeftComboBox->currentItem() == 8) or (plotRightComboBox->currentItem() == 8) )
    {
	if (mix.harmonics() < 2)
	{
	    mix.harmonics(2);
	    harmonicsSpinBox->setValue( 2 );
	}
    }
    if ( (plotLeftComboBox->currentItem() == 9) or (plotRightComboBox->currentItem() == 9) )
    {
	if (mix.harmonics() < 3)
	{
	    mix.harmonics(3);
	    harmonicsSpinBox->setValue( 3 );
	}
    }
    
    int nPoints = 101;

    // Declare arrays for holding results
    double bottom[nPoints];
    double left[nPoints];
    double right[nPoints];

    double low_V, low_I, hi_V, hi_I;
    
    // Do the calculation
    for (int i=0; i < nPoints; i++)
    {
	bottom[i] = plotLow + (plotHigh - plotLow) * double(i) / double(nPoints);
	
	// Calculate the operating point for this datum
	switch ( plotXAxisComboBox->currentItem() )
	{
	case 0:
	    {
		bias.source_voltage = bottom[i]*mVolt;
		break;
	    }
	case 1:
	    {
		lo.source_power = bottom[i]*Nano*Watt;
		break;
	    }
	case 2:
	    {
		lo.source_f = bottom[i] * GHz;
		break;
	    }
	case 3:
	    {
		device::f = bottom[i] * GHz;
		break;
	    }
	}
	
	// This bit is important!!!
	mix.initialize_operating_state();
	mix.balance();
	
	// Calculate the LH axis and data
	switch ( plotLeftComboBox->currentItem() )
	{
	case 0:
	    {
		left[i] = iv.I( bias.source_voltage / (sis.Vn*mVolt) ).imaginary * sis.Vn/sis.Rn / (Micro*Amp);
		break;
	    }
	case 1:
	    {
		left[i] = iv.I( bias.source_voltage / (sis.Vn*mVolt) ).real;
		break;
	    }
	case 2:
	    {
		left[i] = (abs( mix.I_junc( 0 )[1] )) / (Micro*Amp);
		break;
	    }
	case 3:
	    {
		left[i] = 10*log10(norm(mix.get_data().S[IF_PORT][LSB_PORT]));
		break;
	    }
	case 4:
	    {
		left[i] = 10*log10(norm(mix.get_data().S[IF_PORT][USB_PORT]));
		break;
	    }
	case 5:
	    {
		left[i] = 10*log10(norm(mix.get_data().S[IF_PORT][LSB_PORT])+norm(mix.get_data().S[IF_PORT][USB_PORT]));
		break;
	    }
	case 6:
	    {
		left[i] = log10( mix.get_term_data().C[IF_PORT][IF_PORT].real/(norm(mix.get_data().S[IF_PORT][LSB_PORT])+norm(mix.get_data().S[IF_PORT][USB_PORT])) / Kelvin );
		break;
	    }
	case 7:
	    {
		left[i] = mix.get_term_data().C[IF_PORT][IF_PORT].real / Kelvin;
		break;
	    }
	case 8:
	    {
		left[i] = abs( mix.I_junc( 1 )[1] ) / (Micro*Amp);
		break;
	    }
	case 9:
	    {
		low_V = abs(mix.V_junc( 1 )[1]);
		low_I = abs(mix.I_junc( 1 )[1]);
		lo.source_power += 0.1;
		mix.balance();
		hi_V = abs(mix.V_junc( 1 )[1]);
		hi_I = abs(mix.I_junc( 1 )[1]);
		lo.source_power -= 0.1;
		mix.balance();
		left[i] = abs(( hi_V - low_V )/( hi_I - low_I ));
	    }
	case 10: // Need to fix many things to get embedding impedance to work
	    {            
		device::f = lo.source_f; // set device::f so we get the correct frequency from the stub
		// Check to see which RF circuit to use
		// we assume that enabling stub 3 implies stub2 is enabled
		if ( stub2CheckBox->isChecked() )
		{
		    if( stub3CheckBox->isChecked() )
		    {
			Zstub = ( ( 1.0 + rf3StubZemb.S(1,1) ) / ( 1.0 - rf3StubZemb.S(1,1) ) );
		    }
		    else
		    {
			Zstub = ( ( 1.0 + rf2StubZemb.S(1,1) ) / ( 1.0-rf2StubZemb.S(1,1) ) );
		    }
		} 
		else
		{
		    Zstub = ( ( 1.0 + rf1StubZemb.S(1,1) ) / ( 1.0-rf1StubZemb.S(1,1) ) );
		}

		device::f = IFfreqDoubleSpinBox->value()*GHz; // reset device::f
		
		left[i] = Zstub.real;
		break;
	    }
	case 11:
	    {
		device::f = lo.source_f;  // set device::f so we get the correct frequency from the stub
		// Check to see which RF circuit to use
		// we assume that enabling stub 3 implies stub2 is enabled
		if ( stub2CheckBox->isChecked() )
		{
		    if( stub3CheckBox->isChecked() )
		    {
			Zstub = ( ( 1.0 + rf3StubZemb.S(1,1) ) / ( 1.0 - rf3StubZemb.S(1,1) ) );
		    }
		    else
		    {
			Zstub = ( ( 1.0 + rf2StubZemb.S(1,1) ) / ( 1.0-rf2StubZemb.S(1,1) ) );
		    }
		} 
		else
		{
		    Zstub = ( ( 1.0 + rf1StubZemb.S(1,1) ) / ( 1.0-rf1StubZemb.S(1,1) ) );
		}
		device::f = IFfreqDoubleSpinBox->value()*GHz; // reset device::f
		
		left[i] = Zstub.imaginary;
		break;
	    }
	case 12:
	    {
		device::f = lo.source_f; // set device::f so we get the correct frequency from the stub
		// Check to see which RF circuit to use
		// we assume that enabling stub 3 implies stub2 is enabled
		if ( stub2CheckBox->isChecked() )
		{
		    if( stub3CheckBox->isChecked() )
		    {
			left[i] = 10*log10( norm( rf3StubZemb.S( 1,2 ) ) );
		    }
		    else
		    {
			left[i] = 10*log10( norm( rf2StubZemb.S( 1,2 ) ) );
		    }
		} 
		else
		{
		    left[i] = 10*log10( norm( rf1StubZemb.S( 1,2 ) ) );
		}
		device::f = IFfreqDoubleSpinBox->value()*GHz; // reset device::f
        
		break;
	    }
	} // End of LH switch
	
	// Calculate the RH axis and data
	switch ( plotRightComboBox->currentItem() ) 
	{
	case 0:
	    {
		right[i] = iv.I( bias.source_voltage / (sis.Vn*mVolt) ).imaginary * sis.Vn/sis.Rn / (Micro*Amp);
		break;
	    }
	case 1:
	    {
		right[i] = iv.I( bias.source_voltage / (sis.Vn*mVolt) ).real / (Micro*Amp);
		break;
	    }
	case 2:
	    {
		right[i] = abs( mix.I_junc(0)[1] ) / (Micro*Amp);
		break;
	    }
	case 3:
	    {
		right[i] = 10*log10(norm(mix.get_data().S[IF_PORT][LSB_PORT]));
		break;
	    }
	case 4:
	    {
		right[i] = 10*log10(norm(mix.get_data().S[IF_PORT][USB_PORT]));
		break;
	    }
	case 5:
	    {
		right[i] = 10*log10(norm(mix.get_data().S[IF_PORT][LSB_PORT])+norm(mix.get_data().S[IF_PORT][USB_PORT]));
		break;
	    }
	case 6:
	    {
		right[i] = log10( mix.get_term_data().C[IF_PORT][IF_PORT].real/(norm(mix.get_data().S[IF_PORT][LSB_PORT])+norm(mix.get_data().S[IF_PORT][USB_PORT])) / Kelvin );
		break;
	    }
	case 7:
	    {
		right[i] = mix.get_term_data().C[IF_PORT][IF_PORT].real / Kelvin;
		break;
	    }
	case 8:
	    {
		right[i] = abs( mix.I_junc(1)[1] ) / (Micro*Amp);
		break;
	    }
    	case 9:
	    {
		low_V = abs(mix.V_junc( 1 )[1]);
		low_I = abs(mix.I_junc( 1 )[1]);
		lo.source_power += 1;
		mix.balance();
		hi_V = abs(mix.V_junc( 1 )[1]);
		hi_I = abs(mix.I_junc( 1 )[1]);
		lo.source_power -= 1;
		mix.balance();
		right[i] = (( hi_V - low_V )/( hi_I - low_I ));
	    }

	case 10: // Need to fix lots of things to get this to work
	    {            
		device::f = lo.source_f; // set device::f so we get the correct frequency from the stub
		if ( stub2CheckBox->isChecked() )
		{
		    if( stub3CheckBox->isChecked() )
		    {
			Zstub = ( ( 1.0 + rf3StubZemb.S(1,1) ) / ( 1.0 - rf3StubZemb.S(1,1) ) );
		    }
		    else
		    {
			Zstub = ( ( 1.0 + rf2StubZemb.S(1,1) ) / ( 1.0-rf2StubZemb.S(1,1) ) );
		    }
		} 
		else
		{
		    Zstub = ( ( 1.0 + rf1StubZemb.S(1,1) ) / ( 1.0-rf1StubZemb.S(1,1) ) );
		}
		device::f = IFfreqDoubleSpinBox->value()*GHz; // reset device::f
		
		right[i] = Zstub.real;
		break;
	    }
	case 11:
	    {
		device::f = lo.source_f;  // set device::f so we get the correct frequency from the stub
		if ( stub2CheckBox->isChecked() )
		{
		    if( stub3CheckBox->isChecked() )
		    {
			Zstub = ( ( 1.0 + rf3StubZemb.S(1,1) ) / ( 1.0 - rf3StubZemb.S(1,1) ) );
		    }
		    else
		    {
			Zstub = ( ( 1.0 + rf2StubZemb.S(1,1) ) / ( 1.0-rf2StubZemb.S(1,1) ) );
		    }
		} 
		else
		{
		    Zstub = ( ( 1.0 + rf1StubZemb.S(1,1) ) / ( 1.0-rf1StubZemb.S(1,1) ) );
		}
		device::f = IFfreqDoubleSpinBox->value()*GHz; // reset device::f
		
		right[i] = Zstub.imaginary;
		break;
	    }
	case 12:
	    {
		device::f = lo.source_f; // set device::f so we get the correct frequency from the stub
		if ( stub2CheckBox->isChecked() )
		{
		    if( stub3CheckBox->isChecked() )
		    {
			right[i] = 10*log10( norm( rf3StubZemb.S( 1,2 ) ) );
		    }
		    else
		    {
			right[i] = 10*log10( norm( rf2StubZemb.S( 1,2 ) ) );
		    }
		} 
		else
		{
		    right[i] = 10*log10( norm( rf1StubZemb.S( 1,2 ) ) );
		}
		device::f = IFfreqDoubleSpinBox->value()*GHz; // reset device::f
        
		break;
	    }
	} // End of RH switch
	
    } // end of for loop
    
    /*// For test purposes
    for (int j = 0; j < nPoints; j++)
    {
	cout << bottom[j] << "\t" << left[j] << "\t" << right[j] << endl;
    }*/
    
    // Reset scan variable
    switch ( plotXAxisComboBox->currentItem() )
    {
    case 0:
	{
	    bias.source_voltage = biasDoubleSpinBox->value() * mVolt;
	    break;
	}
    case 1:
	{
	    lo.source_power = loPowerDoubleSpinBox->value() * Nano*Watt;
	    break;
	}	    
    case 2:
	{
	    lo.source_f = freqDoubleSpinBox->value() * GHz;
	    break;
	}
    case 3:
	{
	    device::f = IFfreqDoubleSpinBox->value() * GHz;
	    break;
	}
    }
	     
    plot->setCurveData(plotLeftCurve, bottom, left, nPoints);
    plot->setCurveData(plotRightCurve, bottom, right, nPoints);
        
    plot->replot();
    
    // Reset the mouse cursor
    setCursor( Qt::ArrowCursor );
	    
    calculate();
}


void QTMixForm::setPlotLimits()
{
    plotLow = plotLowDoubleSpinBox->value();
    plotHigh = plotHighDoubleSpinBox->value();
    
    redrawPlot();
}

// Calculates the microstrip properties at the point where the mouse event occured on the plot
void QTMixForm::calcOnMouse( const QMouseEvent &e )
{
    double x = plot->invTransform( plot->xBottom, e.x() );
    
    //Chose correct box to set, and set it.  Need to calc at the same point
    switch(plotXAxisComboBox->currentItem())
    {
    case 0:
	{
	    biasDoubleSpinBox->setValue( x );
	    break;
	}
    case 1:
	{
	    loPowerDoubleSpinBox->setValue( x );
	    break;
	}
    case 2:
	{
	    freqDoubleSpinBox->setValue( x );
	    break;
	}
    case 3:
	{
	    IFfreqDoubleSpinBox->setValue( x );
	    break;
	}
    }
    
    plot->removeMarkers();
    long mark = plot->insertMarker( );
    plot->setMarkerLineStyle( mark, QwtMarker::VLine );
    plot->setMarkerXPos( mark, x );
    
    plot->replot();
    
    setOperatingPoint();
    calcStripProperties();
    calculate();
}


void QTMixForm::fileSaveAs()
{
    QString filename = QFileDialog::getSaveFileName( QString::null, 
		"Data (*.dat)", this, "file save as", "QT Mix -- Data Save As" );
    if ( !filename.isEmpty() ) {
	int answer = 0;
	if ( QFile::exists( filename ) )
	    answer = QMessageBox::warning(
		    this, "QT Mix -- Overwrite File",
		    QString( "Overwrite\n\'%1\'?" ).
		    arg( filename ),
		    "&Yes", "&No", QString::null, 1, 1 );
	if ( answer == 0 ) 
	{
	    m_filename = filename;
	    fileSave();
	    return;
            }
    }
}



void QTMixForm::fileSave()
{
    QFile file( m_filename );
    if ( !file.open( IO_WriteOnly ) ) {
	return;
    }
    QTextStream ts( &file );
    
    ts.width( 6 );
    
    // Output Settings
    ts << "# QTMix Mixer Data" << endl;
    ts << "# \n";
    ts << "# Globals:\n";
    ts << "#    Temperature     : " << device::T/Kelvin << " K\n";
    ts << "#    Z0              : " << device::Z0/Ohm << " Ohms\n";
    ts << "#    IF Z0           : " << abs(ifTrans.Z1)/Ohm << " Ohms\n";
    ts << "# \n";
    ts << "# Operating Point:\n";
    ts << "#    Bias            : " << bias.source_voltage/mVolt << " mV" << endl;
    ts << "#    LO Freq         : " << lo.source_f/GHz << " GHz" << endl;
    ts << "#    LO Voltage      : " << lo.source_power/(Nano*Watt) << " nW" << endl;
    ts << "#    LO Temperature  : " << lo.Temp/Kelvin << " K\n";
    ts << "#    IF Freq         : " << device::f/GHz << " GHz" << endl;
    ts << "#    Harmonics : " << mix.harmonics() << "\n";
    ts << "# \n";
    ts << "# Junction Properties:\n";
    ts << "#    IV data file    : " << idcFileChooser->fileName() << endl;
    ts << "#    KK data file    : " << ikkFileChooser->fileName() << endl;
    ts << "#    Rn              : " << sis.Rn/Ohm << " Ohm\n";
    ts << "#    Capacitance     : " << sis.Cap/fFarad << " fF\n";
    ts << "# \n";
    ts << "# Material Properties:\n";
    ts << "#    Dielectric constant         : " << substrate.eps << endl;
    ts << "#    Dielectric loss             : " << substrate.tand/1e-4 << " 10e-4\n";
    ts << "#    Superconductor Tc           : " << superconductor.Tc/Kelvin << " K\n";
    ts << "#    Superconductor Vgap         : " << superconductor.Vgap/mVolt << " mV\n";
    ts << "#    Superconductor RhoN         : " << superconductor.rho_normal/(Micro*Ohm*Centi*Meter) << " uOhm cm\n";
    ts << "#    Superconductor thickness    : " << superconductor.Thick/Micron << " um\n";
    ts << "#    Metal Resistivity           : " << normalconductor.rho/(Micro*Ohm*Centi*Meter) << " uOhm cm\n";
    ts << "#    Metal thickness             : " << normalconductor.Thick/Micron << " um\n";
    ts << "# \n";
    ts << "# Stub 1:\n";
    ts << "#    Top conductor   : " << topStub1ComboBox->currentText() << endl;
    ts << "#    Ground plane    : " << groundStub1ComboBox->currentText() << endl;
    ts << "#    Width           : " << stub1.width/Micron << " um\n";
    ts << "#    Length          : " << stub1.length/Micron << " um\n";
    ts << "#    Height          : " << stub1.sub_thick/Micron << " um\n";
    ts << "#    Radius          : " << stub1.radius/Micron << " um\n";
    ts << "#    Angle           : " << stub1.angle/Degree << " deg\n";
    ts << "# \n";
    ts << "# Stub 2:\n";
    ts << "#    Enabled : ";
    if (stub2CheckBox->isChecked())
    {
        ts << "Yes\n";
    } else {
        ts << "No\n";
    }
    ts << "#    Top conductor   : " << topStub2ComboBox->currentText() << endl;
    ts << "#    Ground plane    : " << groundStub2ComboBox->currentText() << endl;
    ts << "#    Width           : " << stub2.width/Micron << " um\n";
    ts << "#    Length          : " << stub2.length/Micron << " um\n";
    ts << "#    Height          : " << stub2.sub_thick/Micron << " um\n";
    ts << "#    Radius          : " << stub2.radius/Micron << " um\n";
    ts << "#    Angle           : " << stub2.angle/Degree << " deg\n";
    ts << "# \n";
    ts << "# Stub 3:\n";
    ts << "#    Enabled : ";
    if (stub3CheckBox->isChecked())
    {
        ts << "Yes\n";
    } else {
        ts << "No\n";
    }
    ts << "#    Top conductor   : " << topStub3ComboBox->currentText() << endl;
    ts << "#    Ground plane    : " << groundStub3ComboBox->currentText() << endl;
    ts << "#    Width           : " << stub3.width/Micron << " um\n";
    ts << "#    Length          : " << stub3.length/Micron << " um\n";
    ts << "#    Height          : " << stub3.sub_thick/Micron << " um\n";
    ts << "#    Radius          : " << stub3.radius/Micron << " um\n";
    ts << "#    Angle           : " << stub3.angle/Degree << " deg\n";
    ts << "# \n";
    ts << "# Scan variable : " << plotXAxisComboBox->currentText() << "\n";
    ts << "# \n";
    ts << "# X Axis | Unpump DC | Unpump KK | Pump DC | LSB Gain | USB Gain | IF Output | Noise Temp | LO Fundamental Current | Junction Zemb | Coupling\n";
    
    int nPoints = 101;
    double x = 0.0; // X value
    cout << "Starting loop over data to be written to file\n";
    ts.precision( 6 );
    
    parameter coupling;
    
    // Loop of points
    for (int i = 0; i <= nPoints; i++)
    {
        x = plotLow + (plotHigh - plotLow)*double(i-1)/double(nPoints-1);
        switch ( plotXAxisComboBox->currentItem() )
        {
        case 0:
            {
                bias.source_voltage = x*mVolt;
                break;
            }
        case 1:
            {
                lo.source_power = x*Nano*Watt;
                break;
            }	    
        case 2:
            {
                lo.source_f = x*GHz;
                break;
            }
        case 3:
            {
                device::f = x*GHz;
                break;
            }
        }
        
        device::f = lo.source_f; // set device::f so we get the correct frequency from the stub
		if ( stub2CheckBox->isChecked() )
		{
		    if( stub3CheckBox->isChecked() )
		    {
			coupling = 10*log10( norm( rf3StubZemb.S( 1,2 ) ) );
		    }
		    else
		    {
			coupling = 10*log10( norm( rf2StubZemb.S( 1,2 ) ) );
		    }
		} 
		else
		{
		    coupling = 10*log10( norm( rf1StubZemb.S( 1,2 ) ) );
		}
		device::f = IFfreqDoubleSpinBox->value()*GHz; // reset device::f
	
        // Important bit
        mix.balance();
	
        ts << x 
            << "\t" << iv.I( x ).imaginary * 2.0 
            << "\t" << iv.I( x ).real 
            << "\t" << abs( mix.I_junc(0)[1] )
            << "\t" << norm(mix.get_data().S[IF_PORT][LSB_PORT])
            << "\t" << norm(mix.get_data().S[IF_PORT][USB_PORT])
            << "\t" << mix.get_term_data().C[IF_PORT][IF_PORT].real
            << "\t" << mix.get_term_data().C[IF_PORT][IF_PORT].real/norm(mix.get_data().S[IF_PORT][USB_PORT])
            << "\t" << abs(mix.I_junc(1)[1]);
        
	device::f = lo.source_f;
	
	if( stub2CheckBox->isChecked() )
	{
	    if( stub3CheckBox->isChecked() )
	    {
		Zstub = ( ( 1.0 + rf3StubZemb.S(1,1) ) / ( 1.0 - rf3StubZemb.S(1,1) ) );
	    }
	    else
	    {
		Zstub = ( ( 1.0 + rf2StubZemb.S(1,1) ) / ( 1.0-rf2StubZemb.S(1,1) ) );
	    }
	} 
	else
	{
	    Zstub = ( ( 1.0 + rf1StubZemb.S(1,1) ) / ( 1.0-rf1StubZemb.S(1,1) ) );
	}
	ts << "\t" << Zstub.real << " " << Zstub.imaginary;
    ts << "\t" << coupling; // Calculated above.
	
	ts << endl;
        
	device::f = IFfreqDoubleSpinBox->value() * GHz;
	
    } // End of for loop
	
    // Reset scan variable
    switch ( plotXAxisComboBox->currentItem() )
    {
    case 0:
        {
            bias.source_voltage = biasDoubleSpinBox->value() * mVolt;
            break;
        }
    case 1:
        {
            lo.source_power = loPowerDoubleSpinBox->value() * Nano*Watt;
            break;
        }	    
    case 2:
        {
            lo.source_f = freqDoubleSpinBox->value() * GHz;
            break;
        }
    case 3:
        {
            device::f = IFfreqDoubleSpinBox->value() * GHz;
            break;
        }
    }

    file.close();
}


void QTMixForm::setOperatingPoint()
{
    // Set IF freq
    device::f = IFfreqDoubleSpinBox->value() * GHz;
    
    // Set LO freq
    lo.source_f = freqDoubleSpinBox->value() * GHz;
    
    // Set LO volts
    lo.source_power = loPowerDoubleSpinBox->value() * Nano*Watt;
    
    // Set LO temp
    lo.Temp = double( loTempSpinBox->value() ) * Kelvin;
    
    // Set Bias
    bias.source_voltage = biasDoubleSpinBox->value() * mVolt;
    
    // Redraw plot? Breaks on mouse event if we do.  
    // This won't happen if we set the scan variable value properly in on MouseEvent
    
    calcStripProperties();
    
    redrawPlot();
}


void QTMixForm::setIFImpedance()
{
    ifTrans.Z1 = Complex( ifImpDoubleSpinBox->value() * Ohm );
    
    redrawPlot();
}


void QTMixForm::setHarmonics( int h )
{
    mix.harmonics(h);
    
    redrawPlot();
}


void QTMixForm::setIVdata()
{
    // prepend part is a dodgy hack to find the true root /
    QString idcFile =  idcFileChooser->fileName().prepend("/../..");

    QString ikkFile = ikkFileChooser->fileName().prepend("/../..");
    
    // Check to see if these exist, and if not use these useful defaults
    if ( ikkFile == "/../.." )
    {
	ikkFile = "ikk.dat";
    }
    if ( idcFile == "/../.." )
    {
	idcFile = "idc.dat";
    }
    
    iv.data( idcFile.ascii(), ikkFile.ascii() );
}


void QTMixForm::setJunction()
{
    sis.Rn = rnJuncDoubleSpinBox->value()*Ohm;
    sis.Vn = vGapJuncDoubleSpinBox->value()*mVolt;
    sis.Cap = capJuncDoubleSpinBox->value()*fFarad;
 
    juncCap.C = capJuncDoubleSpinBox->value()*fFarad;
    
    redrawPlot();
}


void QTMixForm::setSimParameters()
{
    device::T = tDoubleSpinBox->value() * Kelvin;
    device::Z0 = z0DoubleSpinBox->value() * Ohm;
    
    redrawPlot();
}


void QTMixForm::setRfCircuit()
{
    if( stub3CheckBox->isChecked() )
    {
	mix.set_rf( rf3Stub );
	stub2GroupBox->setEnabled( TRUE );
	stub3GroupBox->setEnabled( TRUE );
	stub2CheckBox->setChecked( TRUE ); // Switch on stub 2 if stub 3 is on
    }
    else 
    {
	if ( stub2CheckBox->isChecked() )
	{
	    mix.set_rf( rf2Stub );
	    stub2GroupBox->setEnabled( TRUE );
	    stub3GroupBox->setEnabled( FALSE );
	} 
	else
	{
	    mix.set_rf( rf1Stub );
	    stub2GroupBox->setEnabled( FALSE );
	    stub3GroupBox->setEnabled( FALSE );
	    stub3CheckBox->setChecked( FALSE ); // Switch off stub3 if stub 2 is off
	}
    }
    mix.set_balance_terminator( lo, 2 );
    mix.initialize_operating_state();
    
    redrawPlot();
}


void QTMixForm::setPlotXAxis()
{
    // This function sets up some stuff for various plot controls, based on
    // the parameter plotted against
    
    plotLowDoubleSpinBox->blockSignals( TRUE );
    plotHighDoubleSpinBox->blockSignals( TRUE );
    
    switch( plotXAxisComboBox->currentItem() )
    {
    case 0: // Bias Scan
        {
	    // Set up the plot limit controls
	    plotLowDoubleSpinBox->setSuffix( " mV" );
	    plotHighDoubleSpinBox->setSuffix( " mV" );
	    
	    plotLowDoubleSpinBox->setMinValue( 0.0 );
	    plotHighDoubleSpinBox->setMinValue( 0.0 );
	    
	    plotLowDoubleSpinBox->setMaxValue( 10.0 );
	    plotHighDoubleSpinBox->setMaxValue( 10.0 );
	    
	    plotLowDoubleSpinBox->setValue( 0.1 );
	    plotHighDoubleSpinBox->setValue( 5.0 );

	    plotLowDoubleSpinBox->setDisplayPrecision( 2 );
	    plotHighDoubleSpinBox->setDisplayPrecision( 2 );

	    break;
        }
    case 1: // LO Power Scan
        {
	    // Set up the plot limit controls
	    plotLowDoubleSpinBox->setSuffix( " nW" );
	    plotHighDoubleSpinBox->setSuffix( " nW" );

	    plotLowDoubleSpinBox->setMinValue( 0.0 );
	    plotHighDoubleSpinBox->setMinValue( 0.0 );

	    plotLowDoubleSpinBox->setMaxValue( 10000.0 );
	    plotHighDoubleSpinBox->setMaxValue( 10000.0 );

	    plotLowDoubleSpinBox->setValue( 5.0 );
	    plotHighDoubleSpinBox->setValue( 1000.0 );
	 
	    plotLowDoubleSpinBox->setDisplayPrecision( 0 );
	    plotHighDoubleSpinBox->setDisplayPrecision( 0 );

	    break;
        }
    case 2: // LO freq scan
        {
	    // Set up the plot limit controls
	    plotLowDoubleSpinBox->setSuffix( " GHz" );
	    plotHighDoubleSpinBox->setSuffix( " GHz" );

	    plotLowDoubleSpinBox->setMinValue( 10.0 );
	    plotHighDoubleSpinBox->setMinValue( 10.0 );

	    plotLowDoubleSpinBox->setMaxValue( 1500.0 );
	    plotHighDoubleSpinBox->setMaxValue( 3000.0 );
	    
	    plotLowDoubleSpinBox->setValue( 500.0 );
	    plotHighDoubleSpinBox->setValue( 800.0 );
	
	    plotLowDoubleSpinBox->setDisplayPrecision( 0 );
	    plotHighDoubleSpinBox->setDisplayPrecision( 0 );

	    break;
	}
    case 3:
	{
	    // Set up the plot limit controls
	    plotLowDoubleSpinBox->setSuffix( " GHz" );
	    plotHighDoubleSpinBox->setSuffix( " GHz" );

	    plotLowDoubleSpinBox->setMinValue( 0.1 );
	    plotHighDoubleSpinBox->setMinValue( 0.1 );
	    
	    plotLowDoubleSpinBox->setMaxValue( 50.0 );
	    plotHighDoubleSpinBox->setMaxValue( 50.0 );
	    
	    plotLowDoubleSpinBox->setValue( 0.1 );
	    plotHighDoubleSpinBox->setValue( 20.0 );

	    plotLowDoubleSpinBox->setDisplayPrecision( 1 );
	    plotHighDoubleSpinBox->setDisplayPrecision( 1 );

	    break;
	}
    }
    
    plotLowDoubleSpinBox->blockSignals( FALSE );
    plotHighDoubleSpinBox->blockSignals( FALSE );
    
    // since we blocked signals, we need to call setPlotLimits explicitly
    setPlotLimits();
    
    plotLowDoubleSpinBox->repaint();
    plotHighDoubleSpinBox->repaint();

    redrawPlot();
}


void QTMixForm::setNormMetal()
{
    normalconductor.rho = rhoNormMetalDoubleSpinBox->value() * Micro*Ohm*Centi*Meter;
    normalconductor.Thick = thickNormMetalDoubleSpinBox->value() * Micron;
 
    calcStripProperties();
    
    redrawPlot();    
}


void QTMixForm::setScMetal()
{
    superconductor.Tc = tcScMetalDoubleSpinBox->value() * Kelvin;
    superconductor.rho_normal = rhoScMetalDoubleSpinBox->value() * Micro*Ohm*Centi*Meter;
    superconductor.Vgap = vGapScMetalDoubleSpinBox->value() * mVolt;
    superconductor.Thick = thickScMetalDoubleSpinBox->value() * Micron;
 
    calcStripProperties();
    
    redrawPlot();
}


void QTMixForm::setDielectric()
{
    substrate.eps = epsSubstrateDoubleSpinBox->value();
    substrate.tand = tandSubstrateDoubleSpinBox->value() * 1.0e-4;
    
    calcStripProperties();
    
    redrawPlot();
}


void QTMixForm::setStub1Dim()
{
    // Set Materials
    switch ( topStub1ComboBox->currentItem() )
    {
    case 0:
	{
	    stub1.top_strip( superconductor );
	    stub1Strip.top_strip( superconductor );
	    break;
	}
    case 1:
	{
	    stub1.top_strip( normalconductor );
	    stub1Strip.top_strip( normalconductor );
	    break;
	}
    }
    switch ( groundStub1ComboBox->currentItem() )
    {
    case 0:
	{
	    stub1.ground_plane( superconductor );
	    stub1Strip.ground_plane( superconductor );
	    break;
	}
    case 1:
	{
	    stub1.ground_plane( normalconductor );
	    stub1Strip.ground_plane( normalconductor );
	    break;
	}
    }
    
    // Set dimensions
    stub1.angle = angleStub1DoubleSpinBox->value() * Degree;
    stub1.radius = radiusStub1DoubleSpinBox->value() * Micron;
    stub1.length = lengthStub1DoubleSpinBox->value() * Micron;
    stub1.width = widthStub1DoubleSpinBox->value() * Micron;
    stub1.sub_thick = heightStub1DoubleSpinBox->value() * Micron;
    
    calcStripProperties();
    
    redrawPlot();
}


void QTMixForm::setStub2Dim()
{
    // Set Materials
    switch ( topStub2ComboBox->currentItem() )
    {
    case 0:
	{
	    stub2.top_strip( superconductor );
	    stub2Strip.top_strip( superconductor );
	    break;
	}
    case 1:
	{
	    stub2.top_strip( normalconductor );
	    stub2Strip.top_strip( normalconductor );
	    break;
	}
    }
    switch ( groundStub2ComboBox->currentItem() )
    {
    case 0:
	{
	    stub2.ground_plane( superconductor );
	    stub2Strip.ground_plane( superconductor );
	    break;
	}
    case 1:
	{
	    stub2.ground_plane( normalconductor );
	    stub2Strip.ground_plane( normalconductor );
	    break;
	}
    }
    
    // Set dimensions
    stub2.angle = angleStub2DoubleSpinBox->value() * Degree;
    stub2.radius = radiusStub2DoubleSpinBox->value() * Micron;
    stub2.length = lengthStub2DoubleSpinBox->value() * Micron;
    stub2.width = widthStub2DoubleSpinBox->value() * Micron;
    stub2.sub_thick = heightStub2DoubleSpinBox->value() * Micron;
    
    calcStripProperties();
    
    redrawPlot();
}


void QTMixForm::setStub3Dim()
{
    // Set Materials
    switch ( topStub3ComboBox->currentItem() )
    {
    case 0:
	{
	    stub3.top_strip( superconductor );
	    stub3Strip.top_strip( superconductor );
	    break;
	}
    case 1:
	{
	    stub3.top_strip( normalconductor );
	    stub3Strip.top_strip( normalconductor );
	    break;
	}
    }
    switch ( groundStub3ComboBox->currentItem() )
    {
    case 0:
	{
	    stub3.ground_plane( superconductor );
	    stub3Strip.ground_plane( superconductor );
	    break;
	}
    case 1:
	{
	    stub3.ground_plane( normalconductor );
	    stub3Strip.ground_plane( normalconductor );
	    break;
	}
    }
    
    // Set dimensions
    stub3.angle = angleStub3DoubleSpinBox->value() * Degree;
    stub3.radius = radiusStub3DoubleSpinBox->value() * Micron;
    stub3.length = lengthStub3DoubleSpinBox->value() * Micron;
    stub3.width = widthStub3DoubleSpinBox->value() * Micron;
    stub3.sub_thick = heightStub3DoubleSpinBox->value() * Micron;
    
    calcStripProperties();
    
    redrawPlot();
}


void QTMixForm::calcStripProperties()
{
    double phase;
    
    // Set Z0 and phase output for Stub 1 input strip
    z0Stub1LineEdit->setText(  QString::number( stub1Strip.Zchar(lo.source_f, device::T).real, 'f', 2 ).append( " ?" ) );
    phase = stub1Strip.Kprop( lo.source_f, device::T ).imaginary*stub1Strip.length/Degree;
    phaseStub1LineEdit->setText(  QString::number( phase, 'f', 2 ).append( " °" ) );    
    
        // Set Z0 and phase output for Stub 1 input strip
    z0Stub2LineEdit->setText(  QString::number( stub2Strip.Zchar(lo.source_f, device::T).real, 'f', 2 ).append( " ?" ) );
    phase = stub2Strip.Kprop( lo.source_f, device::T ).imaginary*stub2Strip.length/Degree;
    phaseStub2LineEdit->setText(  QString::number( phase, 'f', 2 ).append( " °" ) );    
    
        // Set Z0 and phase output for Stub 1 input strip
    z0Stub3LineEdit->setText(  QString::number( stub3Strip.Zchar(lo.source_f, device::T).real, 'f', 2 ).append( " ?" ) );
    phase = stub3Strip.Kprop( lo.source_f, device::T ).imaginary*stub3Strip.length/Degree;
    phaseStub3LineEdit->setText(  QString::number( phase, 'f', 2 ).append( " °" ) );    
}

