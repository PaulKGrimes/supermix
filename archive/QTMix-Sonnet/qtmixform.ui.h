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
	idcFileChooser->setFileName( "/data/simulations/QTMix-Belitsky/idc.dat");
	ikkFileChooser->setFileName( "/data/simulations/QTMix-Belitsky/ikk.dat");
		
	sis.set_iv(iv);

	// Needed for Complex impedance calculation
	TwoPiI = 2*Pi*Complex(0.0, 1.0);
	
	// Set up operating point devices (parts that don't change)
	// Set the LO voltage_source
	lo.R = & device::Z0;
	lo.source_f = 650*GHz;

	// Set the bias source
	bias.R = 0.0;
	bias.f = 0.0;
		
	// Set up the non RF Choke circuit
	rfNoChoke.nport(sonnetDevice);
	rfNoChoke.add_port(2);
	rfNoChoke.add_port(1);
	
	// Set up the non RF Choke Zemb calculation circuit
	juncCap.parallel();
	juncCap.C = & sis.Cap;
	rfNoChokeZemb.connect( rfNoChoke, 1, juncCap, 1 );
	rfNoChokeZemb.add_port( juncCap, 2 ); // SIS 1 port
	rfNoChokeZemb.add_port( rfNoChoke, 2 ); // LO port
	
	// Set up the terminator for the RF Choke
	rfTerm.Z = & ifNoChokeTrans.Z1;

	// Set up the RF choke
	rfChoke.substrate( substrate );
	rfChoke.superstrate( superstrate );
	
	// Set up the RF with choke circuit
	rfWithChoke.connect( sonnetDevice, 2, rfBranch, 1);
	rfWithChoke.connect( rfBranch, 2, rfChoke, 1);
	rfWithChoke.connect( rfChoke, 2, rfTerm, 1 );
	rfWithChoke.add_port( rfBranch, 3 ); // SIS 1 goes here
	rfWithChoke.add_port( sonnetDevice, 1 ); // LO here
    
	// Set up the RF with choke Zemb calculating circuit
	rfWithChokeZemb.connect( rfWithChoke, 1, juncCap, 1 );
	rfWithChokeZemb.add_port( juncCap, 2 ); // SIS 1 port
	rfWithChokeZemb.add_port( rfWithChoke, 2 ); // LO port
	
	// Set up IF circuit with RF Choke
	ifWithChoke.connect( rfChoke, 2, ifWithChokeTrans, 1 );
	ifWithChoke.add_port( rfChoke, 1 ); // SIS 1 goes here
	ifWithChoke.add_port( ifWithChokeTrans, 2 ); // IF Output
	
	//Set up tue mixer
	mix.set_rf( rfNoChoke );
	mix.set_if( ifNoChokeTrans );
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
        
    cout << "Setting up RF choke...\n";
    // Set up RF Choke
    rfChoke.top_strip( superconductor );
    rfChoke.ground_plane( superconductor );
    rfChoke.superstrate( superstrate );
    rfChoke.substrate( substrate );
    
    rfChoke.width = 3.0 * Micron;
    rfChoke.length = 39.0 * Micron;
    rfChoke.sub_thick = 0.42 * Micron;
    rfChoke.angle = 180 * Degree;
    rfChoke.radius = 30.0 * Micron;
        
    // Set up microstrip for shadowing rfChoke input strip
    rfChokeStrip.top_strip( superconductor );
    rfChokeStrip.ground_plane( superconductor );
    rfChokeStrip.superstrate( superstrate );
    rfChokeStrip.substrate( substrate );
    rfChokeStrip.width = & rfChoke.width;
    rfChokeStrip.length = & rfChoke.length;
    rfChokeStrip.sub_thick = & rfChoke.sub_thick;

    sonnetDevice.touchstone("/data/simulations/QTMix-Sonnet/dut.d");
    
    cout << "Setting operating point...\n";
    // Operating point
    lo.source_f = 650*GHz;
    device::f = 5*GHz;
    bias.source_voltage = 2.0*mVolt;
    lo.source_power = 100*Nano*Watt;
    lo.Temp = 0.0*Kelvin;
    
    cout << "Setting IF transformer...\n";
    // Set up if transformer - must be done after device::f is set above
    ifNoChokeTrans.Z1 = 50*Ohm;
    ifWithChokeTrans.Z1 = 50*Ohm;
    
    cout << "Resetting Mixer...\n";
    // Create the mixer
    mix.set_rf( rfNoChoke );
    mix.set_if( ifNoChokeTrans );
    mix.set_LO( & lo.source_f ); // Shadow LO frequency
    mix.set_bias( bias );
    mix.harmonics( 1 );
    mix.set_balance_terminator( lo, 2 );
    
    if (mix.flag_mixer_incomplete())
    {
	cout << "Mixer incomplete...\n";
	cout << "\tNum Junctions = " << mix.junctions() << endl;
	cout << "\tSize of RF circuit = " << rfNoChoke.size() << endl;
	cout << "\tSize of IF circuit = " << ifNoChokeTrans.size() << endl;
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
    if ( !topRfChokeComboBox->count() )
    {
	topRfChokeComboBox->insertItem( "Superconductor" );
	topRfChokeComboBox->insertItem( "Normal Metal" );
    }
    topRfChokeComboBox->setCurrentItem( 0 );
    if ( !groundRfChokeComboBox->count() )
    {
	groundRfChokeComboBox->insertItem( "Superconductor" );
	groundRfChokeComboBox->insertItem( "Normal Metal" );
    }
    groundRfChokeComboBox->setCurrentItem( 0 );    

    cout << "Setting RF Choke controls...\n";
    // Set RF Choke controls    
    rfChokeCheckBox->setChecked( FALSE );
    widthRfChokeDoubleSpinBox->setValue( 3.0 );
    lengthRfChokeDoubleSpinBox->setValue( 39.0 );
    heightRfChokeDoubleSpinBox->setValue( 0.4 );
    angleRfChokeDoubleSpinBox->setValue( 180 );
    radiusRfChokeDoubleSpinBox->setValue( 30.0 );
    
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
	case 9: // Need to fix many things to get embedding impedance to work
	    {            
		device::f = lo.source_f; // set device::f so we get the correct frequency from the stub
		if ( rfChokeCheckBox->isChecked() )
		{
		    Zstub = ( ( 1.0 + rfWithChokeZemb.S(1,1) ) / ( 1.0 - rfWithChokeZemb.S(1,1) ) );
		}
		else
		{
		    Zstub = ( ( 1.0 + rfNoChokeZemb.S(1,1) ) / ( 1.0-rfNoChokeZemb.S(1,1) ) );
		}
		device::f = IFfreqDoubleSpinBox->value()*GHz; // reset device::f
		
		left[i] = Zstub.real;
		break;
	    }
	case 10:
	    {
		device::f = lo.source_f;  // set device::f so we get the correct frequency from the stub
		if ( rfChokeCheckBox->isChecked() )
		{
		    Zstub = ( ( 1.0 + rfWithChokeZemb.S(1,1) ) / ( 1.0-rfWithChokeZemb.S(1,1) ) );
		}
		else
		{
		    Zstub = ( ( 1.0 + rfNoChokeZemb.S(1,1)) / ( 1.0-rfNoChokeZemb.S(1,1) ) );
		}
		device::f = IFfreqDoubleSpinBox->value()*GHz; // reset device::f
		
		left[i] = Zstub.imaginary;
		break;
	    }
	case 11:
	    {
		device::f = lo.source_f; // set device::f so we get the correct frequency from the stub
		if ( rfChokeCheckBox->isChecked() )
		{
		    left[i] = 10*log10( norm( rfWithChokeZemb.S( 1,2 ) ) );
		}
		else
		{
		    left[i] = 10*log10( norm( rfNoChokeZemb.S( 1,2 ) ) );
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
	case 9: // Need to fix lots of things to get this to work
	    {            
		device::f = lo.source_f; // set device::f so we get the correct frequency from the stub
		if ( rfChokeCheckBox->isChecked() )
		{
		    Zstub = ( ( 1.0 + rfWithChokeZemb.S(1,1) ) / ( 1.0-rfWithChokeZemb.S(1,1) ) );
		}
		else
		{
		    Zstub = ( ( 1.0 + rfNoChokeZemb.S(1,1) ) / ( 1.0-rfNoChokeZemb.S(1,1) ) );
		}
		device::f = IFfreqDoubleSpinBox->value()*GHz; // reset device::f
		
		right[i] = Zstub.real;
		break;
	    }
	case 10:
	    {
		device::f = lo.source_f;  // set device::f so we get the correct frequency from the stub
		if ( rfChokeCheckBox->isChecked() )
		{
		    Zstub = ( ( 1.0 + rfWithChokeZemb.S(1,1) ) / ( 1.0-rfWithChokeZemb.S(1,1) ) );
		}
		else
		{
		    Zstub = ( ( 1.0 + rfNoChokeZemb.S(1,1) ) / ( 1.0-rfNoChokeZemb.S(1,1) ) );
		}
		device::f = IFfreqDoubleSpinBox->value()*GHz; // reset device::f
		
		right[i] = Zstub.imaginary;
		break;
	    }
	case 11:
	    {
		device::f = lo.source_f; // set device::f so we get the correct frequency from the stub
		if ( rfChokeCheckBox->isChecked() )
		{
		    right[i] = 10*log10( norm( rfWithChokeZemb.S( 1,2 ) ) );
		}
		else
		{
		    right[i] = 10*log10( norm( rfNoChokeZemb.S( 1,2 ) ) );
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
    ts << "#    IF Z0           : " << abs(ifNoChokeTrans.Z2)/Ohm << " Ohms\n";
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
    ts << "# Tuner: " << sonnetFileChooser->fileName() << endl;
    ts << "# \n";
    ts << "# RF Choke:\n";
    ts << "#    Enabled : ";
    if (rfChokeCheckBox->isChecked())
    {
        ts << "Yes\n";
    } else {
        ts << "No\n";
    }
    ts << "#    Top conductor   : " << topRfChokeComboBox->currentText() << endl;
    ts << "#    Ground plane    : " << groundRfChokeComboBox->currentText() << endl;
    ts << "#    Width           : " << rfChoke.width/Micron << " um\n";
    ts << "#    Length          : " << rfChoke.length/Micron << " um\n";
    ts << "#    Height          : " << rfChoke.sub_thick/Micron << " um\n";
    ts << "#    Radius          : " << rfChoke.radius/Micron << " um\n";
    ts << "#    Angle           : " << rfChoke.angle/Degree << " deg\n";
    ts << "# \n";
    ts << "# Scan variable : " << plotXAxisComboBox->currentText() << "\n";
    ts << "# \n";
    ts << "# X Axis | Unpump DC | Unpump KK | Pump DC | LSB Gain | USB Gain | IF Output | Noise Temp | LO Fundamental Current | Junction Zemb\n";
    
    int nPoints = 100;
    double x = 0.0; // X value
    cout << "Starting loop over data to be written to file\n";
    ts.precision( 6 );
    
    parameter coupling;
    
    // Loop of points
    for (int i = 0; i <= nPoints; i++)
    {
        x = plotLow + (plotHigh - plotLow)*double(i)/double(nPoints);
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
		if ( rfChokeCheckBox->isChecked() )
		{
		    coupling = 10*log10( norm( rfWithChokeZemb.S( 1,2 ) ) );
		}
		else
		{
		    coupling = 10*log10( norm( rfNoChokeZemb.S( 1,2 ) ) );
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
        
        if (rfChokeCheckBox->isChecked())
        {
            Zstub = ( ( 1.0 + rfWithChokeZemb.S(1,1) ) / ( 1.0-rfWithChokeZemb.S(1,1) ) );
        } else {
            Zstub = ( ( 1.0 + rfNoChokeZemb.S(1,1) ) / ( 1.0-rfNoChokeZemb.S(1,1) ) );
        }
        ts << "\t" << Zstub.real << " " << Zstub.imaginary; 
        ts << "\t" << coupling;
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
    ifNoChokeTrans.Z1 = Complex( ifImpDoubleSpinBox->value() * Ohm );
    ifWithChokeTrans.Z1 = Complex( ifImpDoubleSpinBox->value() * Ohm );
    
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
   
    redrawPlot();
}


void QTMixForm::setSimParameters()
{
    device::T = tDoubleSpinBox->value() * Kelvin;
    device::Z0 = z0DoubleSpinBox->value() * Ohm;
    
    redrawPlot();
}


void QTMixForm::setRfChoke()
{
    if( rfChokeCheckBox->isChecked() )
    {
	mix.set_rf( rfWithChoke );
	mix.set_if( ifWithChoke );
	mix.set_balance_terminator( lo, 2 );	
	rfChokeGroupBox->setEnabled( TRUE );
    }
    else
    {
	mix.set_rf( rfNoChoke );
	mix.set_if( ifNoChokeTrans );
	mix.set_balance_terminator( lo, 2 );	
	rfChokeGroupBox->setEnabled( FALSE );
    }
    
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

	    plotLowDoubleSpinBox->setValue( 50.0 );
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


void QTMixForm::setRfChokeDim()
{
    // Set Materials
    switch ( topRfChokeComboBox->currentItem() )
    {
    case 0:
	{
	    rfChoke.top_strip( superconductor );
	    rfChokeStrip.top_strip( superconductor );
	    break;
	}
    case 1:
	{
	    rfChoke.top_strip( normalconductor );
	    rfChokeStrip.top_strip( normalconductor );
	    break;
	}
    }
    switch ( groundRfChokeComboBox->currentItem() )
    {
    case 0:
	{
	    rfChoke.ground_plane( superconductor );
	    rfChokeStrip.ground_plane( superconductor );
	    break;
	}
    case 1:
	{
	    rfChoke.ground_plane( normalconductor );
	    rfChokeStrip.ground_plane( normalconductor );
	    break;
	}
    }
    
    // Set dimensions
    rfChoke.angle = angleRfChokeDoubleSpinBox->value() * Degree;
    rfChoke.radius = radiusRfChokeDoubleSpinBox->value() * Micron;
    rfChoke.length = lengthRfChokeDoubleSpinBox->value() * Micron;
    rfChoke.width = widthRfChokeDoubleSpinBox->value() * Micron;
    rfChoke.sub_thick = heightRfChokeDoubleSpinBox->value() * Micron;
    
    calcStripProperties();
    
    redrawPlot();
}




void QTMixForm::setTuner()
{
    // prepend part is a dodgy hack to find the true root /
    QString sonnetFile = sonnetFileChooser->fileName().prepend("/../..");
    
    // Check to see if these exist, and if not use these useful defaults
    if ( sonnetFile == "/../.." )
    {
	sonnetFile = "dut.dat";
    }
    
    sonnetDevice.interpolator().clear();
    sonnetDevice.touchstone( sonnetFile.ascii() );
    
    calcStripProperties();
    
    redrawPlot();
}


void QTMixForm::calcStripProperties()
{
    double phase;
    
    // Set Z0 and phase output for Radial stub input strip
    z0RadialLineEdit->setText(  QString::number( rfChokeStrip.Zchar(lo.source_f, device::T).real, 'f', 2 ).append( " ?" ) );
    phase = rfChokeStrip.Kprop( lo.source_f, device::T ).imaginary*rfChokeStrip.length/Degree;
    phaseRadialLineEdit->setText(  QString::number( phase, 'f', 2 ).append( " °" ) );    
}

