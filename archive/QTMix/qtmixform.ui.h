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
    unpumpLineEdit->setText( QString::number( iv.I( bias.source_voltage ).imaginary, 'f', 4 ) );
    pumpedLineEdit->setText( QString::number( mix.I_junc(0)[1].real, 'f', 4 ) );
    lsbGainLineEdit->setText( QString::number( abs(mix.get_data().S[IF_PORT][LSB_PORT]), 'f', 4 ) );
    usbGainLineEdit->setText( QString::number( abs(mix.get_data().S[IF_PORT][USB_PORT]), 'f', 4 ) );
    ifOutputLineEdit->setText( QString::number( mix.get_term_data().C[IF_PORT][IF_PORT].real, 'f', 4 ) );
    noiseLineEdit->setText( QString::number( mix.get_term_data().C[IF_PORT][IF_PORT].real/norm(mix.get_data().S[IF_PORT][LSB_PORT]) ) );
    loCurrentLineEdit->setText( QString::number( abs( mix.I_junc(1)[1] ), 'f', 4 ) );
    
    
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
     x = loVoltDoubleSpinBox->value();
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
    cout << "# Init called\n";
    // Something that has to be done first
    device::Z0 = 1.0*Ohm;
    
    cout << "# Init : setting operating point\n";
    // Operating point
    lo.source_f = VoltToFreq*1.0;
    // IF Frequency is the required value of device::f for a mixer
    device::f = 0.01*VoltToFreq;
    bias.source_voltage = 0.5;
    lo.source_voltage = 1.0/pow(2.0,0.5);
    lo.Temp = 0.0;

    
    //=======================
    // Things that should only be done once
    if (!(mix.junctions() == 1)) 
    {
 device::T = 1.0; // arbitary as we are not interested in superconducting devices or noise temperatures
 
 // This code will call setIVdata() automagically
 idcFileChooser->setFileName( "/data/simulations/QTMix/idc.dat");
 ikkFileChooser->setFileName( "/data/simulations/QTMix/ikk.dat");
  
  
 sis.set_iv(iv);
 sis.Rn = 1.0;
 sis.Vn = 1.0;
 sis.Cap = 65.0;
    
 embImp.Z = 1.0;
    
 // Set up operating point devices (parts that don't change)
 // Set the LO voltage_source
 lo.R = 1.0;
 lo.T = 1.0;


 // Set the bias source
 bias.R = 0.0*device::Z0;
 bias.f = 0.0;
 
 // Create the mixer
 mix.set_rf( embImp );
 mix.set_if( dummyIf );
 mix.set_LO( & lo.source_f ); // Shadow LO frequency
 mix.set_bias( bias );
 mix.add_junction( sis );
 mix.set_balance_terminator( lo, 2 );
 
 IF_PORT = mix.port(2, 0);
 USB_PORT = mix.port(2, 1);
 LSB_PORT = mix.port(2, -1);        
    }   
    //========================
    // Reset things that might have changed
    RnSpinBox->setValue( 1.0 );
    CapSpinBox->setValue( 65 );
    
    // Embedding impedance(s)
    dummyIf.Z1 = 1.0*device::Z0;
    
    realIfEmbImpDoubleSpinBox->setValue( 1.0 );
    imagIfEmbImpDoubleSpinBox->setValue( 0.0 );
    
    embImp.Z = 1.0*device::Z0;
    
    real1EmbImpDoubleSpinBox->setValue( 1.0 );
    imag1EmbImpDoubleSpinBox->setValue( 0.0 );
    real2EmbImpDoubleSpinBox->setValue( 1.0 );
    imag2EmbImpDoubleSpinBox->setValue( 0.0 );
    real3EmbImpDoubleSpinBox->setValue( 1.0 );
    imag3EmbImpDoubleSpinBox->setValue( 0.0 );
        
    freqDoubleSpinBox->setValue( 1.0 );
    IFfreqDoubleSpinBox->setValue( 0.01 );
    biasDoubleSpinBox->setValue( 0.5 );
    loVoltDoubleSpinBox->setValue( 1.0 );
    
    // Simulation parameters
    mix.harmonics(1);
    harmonicsSpinBox->setValue(1);
    
    mix.initialize_operating_state();
    mix.balance();
    
    // Plot controls
    plotLow = 0.0;
    plotHigh = 2.0;

    plotLowDoubleSpinBox->setValue( 0.0 );
    plotHighDoubleSpinBox->setValue( 2.0 );

    if(!plotXAxisComboBox->count())
    {
 plotXAxisComboBox->insertItem("Bias");
 plotXAxisComboBox->insertItem("LO Voltage");
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
 plotLeftComboBox->insertItem( "Mixer Noise (K)" );
 plotLeftComboBox->insertItem( "IF Output" );
 plotLeftComboBox->insertItem( "LO Fundamental Current" );
 plotLeftComboBox->insertItem( "LO 1st Harmonic Current" );
 plotLeftComboBox->insertItem( "LO 2nd Harmonic Current" );
 plotLeftComboBox->insertItem( "LO Impedance" );  
    }
    plotLeftComboBox->setCurrentItem( 0 );

    if (!plotRightComboBox->count())
    {
 plotRightComboBox->insertItem( "DC Current (unpump)" );
 plotRightComboBox->insertItem( "KK Transform" );
 plotRightComboBox->insertItem( "DC Current" );
 plotRightComboBox->insertItem( "Mixer Gain (LSB)" );
 plotRightComboBox->insertItem( "Mixer Gain (USB)" );
 plotRightComboBox->insertItem( "Mixer Noise (K)" );
 plotRightComboBox->insertItem( "IF Output" );
 plotRightComboBox->insertItem( "LO Fundamental Current" );
 plotRightComboBox->insertItem( "LO 1st Harmonic Current" );
 plotRightComboBox->insertItem( "LO 2nd Harmonic Current" );
 plotRightComboBox->insertItem( "LO Impedance" );  
    }
    plotRightComboBox->setCurrentItem( 2 );

    // Set up plot
    plot->setAutoLegend(TRUE);
    plot->setLegendPos(Qwt::Bottom);
    plot->setGridPen( QPen(darkGray, 1, SolidLine) );

    plot->setAxisTitle(plot->xBottom, "Bias");
    plot->setAxisTitle(plot->yLeft, "Pumped IV");
    plot->setAxisTitle(plot->yRight, "IF Current");

    plot->removeCurves();
    plotLeftCurve = plot->insertCurve( plotLeftComboBox->currentText() );
    plotRightCurve = plot->insertCurve( plotLeftComboBox->currentText(), plot->xBottom, plot->yRight );

    plot->setCurvePen(plotLeftCurve, QPen(red));
    plot->setCurvePen(plotRightCurve, QPen(blue));
    
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
 double low = device::f/VoltToFreq + 0.01;
 if (plotLowDoubleSpinBox->value() < low)
 {
     plotLowDoubleSpinBox->setValue( low );
     plotLow = low;
 }
    }
    
    if (plotXAxisComboBox->currentItem() == 3 ) // IF Freq scan
    {
 double high = lo.source_f/VoltToFreq - 0.01;
 if (plotHighDoubleSpinBox->value() < high)
 {
     plotHighDoubleSpinBox->setValue( high );
     plotHigh = high;
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
 bottom[i] = plotLow + (plotHigh - plotLow) * double(i) / double(nPoints-1);
 
 // Calculate the operating point for this datum
 switch ( plotXAxisComboBox->currentItem() )
 {
 case 0:
     {
  bias.source_voltage = bottom[i];
  break;
     }
 case 1:
     {
  lo.source_voltage = bottom[i]/pow(2.0,0.5);
  break;
     }
 case 2:
     {
  lo.source_f = VoltToFreq*bottom[i];
  break;
     }
 case 3:
     {
  device::f = bottom[i]*VoltToFreq;
  break;
     }
 }
 
 // This bit is important!!!
 mix.initialize_operating_state();
 mix.balance();
 
 Complex low_V, low_I, hi_V, hi_I;
 
 // Calculate the LH axis and data
 switch ( plotLeftComboBox->currentItem() )
 {
 case 0:
     {
  left[i] = iv.I( bias.source_voltage ).imaginary;
  break;
     }
 case 1:
     {
  left[i] = iv.I( bias.source_voltage ).real;
  break;
     }
 case 2:
     {
  left[i] = abs( mix.I_junc( 0 )[1] );
  break;
     }
 case 3:
     {
  left[i] = abs(mix.get_data().S[IF_PORT][LSB_PORT]);
  break;
     }
 case 4:
     {
  left[i] = abs(mix.get_data().S[IF_PORT][USB_PORT]);
  break;
     }
 case 5:
     {
  left[i] = mix.get_term_data().C[IF_PORT][IF_PORT].real/norm(mix.get_data().S[IF_PORT][USB_PORT]);
  break;
     }
 case 6:
     {
  left[i] = mix.get_term_data().C[IF_PORT][IF_PORT].real;
  break;
     }
 case 7:
     {
  left[i] = abs( mix.I_junc( 1 )[1] );
  break;
     }
 case 8:
     {
  left[i] = abs( mix.I_junc( 2 )[1] );
  break;
     }
 case 9:
     {
  left[i] = abs( mix.I_junc( 3 )[1] );
  break;
     }
 case 10:
     {
		low_V = mix.V_junc( 1 )[1];
        low_I = mix.I_junc( 1 )[1];
        lo.source_voltage += 0.0001;
        mix.balance();
        hi_V = mix.V_junc( 1 )[1];
        hi_I = mix.I_junc( 1 )[1];
        lo.source_voltage -= 0.0001;
        mix.balance();
        left[i] = abs(( hi_V - low_V )/( hi_I - low_I ));
  break;
     }
 } // End of LH switch
 
 // Calculate the RH axis and data
 switch ( plotRightComboBox->currentItem() ) 
 {
 case 0:
     {
  right[i] = iv.I( bias.source_voltage ).imaginary;
  break;
     }
 case 1:
     {
  right[i] = iv.I( bias.source_voltage ).real;
  break;
     }
 case 2:
     {
  right[i] = abs( mix.I_junc(0)[1] );
  break;
     }
 case 3:
     {
  right[i] = abs(mix.get_data().S[IF_PORT][LSB_PORT]);
  break;
     }
 case 4:
     {
  right[i] = abs(mix.get_data().S[IF_PORT][USB_PORT]);
  break;
     }
 case 5:
     {
  right[i] = mix.get_term_data().C[IF_PORT][IF_PORT].real/norm(mix.get_data().S[IF_PORT][USB_PORT]);
  break;
     }
 case 6:
     {
  right[i] = mix.get_term_data().C[IF_PORT][IF_PORT].real;
  break;
     }
 case 7:
	    {
		right[i] = abs(mix.I_junc(1)[1]);
		break;
	    }
	case 8:
	    {
		right[i] = abs(mix.I_junc(2)[1]);
		break;
	    }
	case 9:
	    {
		right[i] = abs(mix.I_junc(3)[1]);
		break;
	    }
	case 10:
	    {
		low_V = mix.V_junc( 1 )[1];
        low_I = mix.I_junc( 1 )[1];
        lo.source_voltage += 0.0001;
        mix.balance();
        hi_V = mix.V_junc( 1 )[1];
        hi_I = mix.I_junc( 1 )[1];
        lo.source_voltage -= 0.0001;
        mix.balance();
        right[i] = abs(( hi_V - low_V )/( hi_I - low_I ));
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
	    bias.source_voltage = biasDoubleSpinBox->value();
	    break;
	}
    case 1:
	{
	    lo.source_voltage = loVoltDoubleSpinBox->value()/pow(2.0,0.5);
	    break;
	}	    
    case 2:
	{
	    lo.source_f = freqDoubleSpinBox->value()*VoltToFreq;
	    break;
	}
    case 3:
	{
	    device::f = IFfreqDoubleSpinBox->value()*VoltToFreq;
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
     loVoltDoubleSpinBox->setValue( x );
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
    
    //This code is for debugging purposes
    sdata if_data = dummyIf.get_data();
    complex TwoPiI(0.0, 2*Pi);
    complex Zstub = ((1.0 + if_data.S[1][1])/(1.0-if_data.S[1][1]))*device::Z0;
    complex ZembIF  = (1.0/((TwoPiI*device::f/GHz*1.0e9*sis.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/sis.Rn;
    
    device::f = lo.source_f;
    sdata rf_data = embImp.get_data();
    Zstub = ((1.0 + rf_data.S[1][1])/(1.0-rf_data.S[1][1]))*device::Z0;
    complex ZembRF  = (1.0/((TwoPiI*device::f/GHz*1.0e9*sis.Cap/fFarad*1.0e-15)+(1.0/Zstub)))/sis.Rn;
    
    device::f = IFfreqDoubleSpinBox->value()*VoltToFreq;    
    // End of debug code
    
    // Output Settings
    ts << "# QTMix Mixer Data" << endl;
    ts << "# \n# IV data file: "<< idcFileChooser->fileName() << endl;
    ts << "# KK data file: " << ikkFileChooser->fileName() << endl;
    ts << "# Junction Resistance: "<< RnSpinBox->value() << endl;
    ts << "# Junction Capacitance: " << CapSpinBox->value() << " fF\n";
    ts << "# " << endl;
    ts << "# Impedances:" << endl;
    ts << "#  IF:  " << realIfEmbImpDoubleSpinBox->value() << " + " << imagIfEmbImpDoubleSpinBox->value() << "i\n";
    ts << "#  Fundamental:  " << real1EmbImpDoubleSpinBox->value() << " + " << imag1EmbImpDoubleSpinBox->value() << "i\n";
    ts << "#  Harmonic 1 :  " << real2EmbImpDoubleSpinBox->value() << " + " << imag2EmbImpDoubleSpinBox->value() << "i\n";
    ts << "#  Harmonic 2 :   " << real3EmbImpDoubleSpinBox->value() << " + " << imag3EmbImpDoubleSpinBox->value() << "i\n";
    ts << "# \n";
    // This next line is for debugging purposes
    ts << "# Actual Impedances:" <<endl;
    ts << "#  IF:  " << ZembIF.real << " + " << ZembIF.imaginary << "i\n";
    ts << "#  EmbImp.Z = " << embImp.Z.get().real << " + " << embImp.Z.get().imaginary <<"i\n";
    ts << "#  LO:  " << ZembRF.real << " + " << ZembRF.imaginary <<"i\n";
    // End of debug code
    ts << "# Operating Point:\n";
    ts << "# Bias       : " << bias.source_voltage << endl;
    ts << "# LO Freq    : " << lo.source_f/VoltToFreq << endl;
    ts << "# LO Voltage : " << lo.source_voltage*pow(2.0,0.5) << endl;
    ts << "# IF Freq    : " << device::f/VoltToFreq << endl;
    ts << "# \n";
    ts << "# Harmonics : " << mix.harmonics() << "\n# \n";
    ts << "# Scan variable : " << plotXAxisComboBox->currentText() << "\n# \n";
    ts << "# X Axis | Unpump DC | Unpump KK | Pump DC | LSB Gain | USB Gain | IF Output | Noise Temp | LO Fundamental Current ";
    if ( mix.harmonics() == 1 )
    {
 ts << endl;
    }
    if ( mix.harmonics() == 2 )
    {
 ts << "| LO 1st Harmonic \n";
    }
    if ( mix.harmonics() == 3 )
    {
 ts << "| LO 1st Harmonic | LO 2nd Harmonic\n";
    }
    if (mix.harmonics() == 4 )
    {
     ts << "| LO 1st Harmonic | LO 2nd Harmonic | LO 3rd Harmonic \n";
    }
    if (mix.harmonics() == 5 )
    {
     ts << "| LO 1st Harmonic | LO 2nd Harmonic | LO 3rd Harmonic | LO 4th Harmonic\n";
    }
    
    int nPoints = 101;
    double x = 0.0; // X value
    
    ts.precision( 6 );
    
    // Loop of points
    for (int i = 0; i < nPoints; i++)
    {
 x = plotLow + (plotHigh - plotLow)*double(i)/double(nPoints-1);
 switch ( plotXAxisComboBox->currentItem() )
 {
 case 0:
     {
  bias.source_voltage = x;
  break;
     }
 case 1:
     {
  lo.source_voltage = x/pow(2.0,0.5);
  break;
     }     
 case 2:
     {
  lo.source_f = VoltToFreq * x;
  break;
     }
 case 3:
     {
  device::f = x * VoltToFreq;
  break;
     }
 }
 
 // Important bit
 mix.balance();
 
 ts << x 
  << "\t" << iv.I( x ).imaginary 
  << "\t" << iv.I( x ).real 
  << "\t" << abs( mix.I_junc(0)[1] ) 
  << "\t" << abs(mix.get_data().S[IF_PORT][LSB_PORT])
  << "\t" << abs(mix.get_data().S[IF_PORT][USB_PORT])
  << "\t" << mix.get_term_data().C[IF_PORT][IF_PORT].real
  << "\t" << mix.get_term_data().C[IF_PORT][IF_PORT].real/norm(mix.get_data().S[IF_PORT][USB_PORT])
  << "\t" << abs(mix.I_junc(1)[1]);
 if ( mix.harmonics() == 2 )
 {
     ts << "\t" << abs(mix.I_junc(2)[1]);
 }
 if ( mix.harmonics() ==3 )
 {
     ts << "\t" << abs(mix.I_junc(2)[1])
      << "\t" << abs(mix.I_junc(3)[1]);
 }
 if ( mix.harmonics() ==4 )
 {
     ts << "\t" << abs(mix.I_junc(2)[1])
      << "\t" << abs(mix.I_junc(3)[1])
      << "\t" << abs(mix.I_junc(4)[1]);
 }
 if ( mix.harmonics() ==5 )
 {
     ts << "\t" << abs(mix.I_junc(2)[1])
      << "\t" << abs(mix.I_junc(3)[1])
      << "\t" << abs(mix.I_junc(4)[1])
      << "\t" << abs(mix.I_junc(5)[1]);
 }
 ts << endl;
 
    } // End of for loop
 
    // Reset scan variable
    switch ( plotXAxisComboBox->currentItem() )
    {
    case 0:
 {
     bias.source_voltage = biasDoubleSpinBox->value();
     break;
 }
    case 1:
 {
     lo.source_voltage = loVoltDoubleSpinBox->value();
     break;
 }     
    case 2:
 {
     lo.source_f = freqDoubleSpinBox->value()*VoltToFreq;
     break;
 }
    case 3:
 {
     device::f = IFfreqDoubleSpinBox->value()*VoltToFreq;
     break;
 }
    }

    file.close();
}


void QTMixForm::setOperatingPoint()
{
    cout << "# setOperatingPoint called\n";
    // Set IF freq
    device::f = IFfreqDoubleSpinBox->value()*VoltToFreq;
    
    // Set LO freq
    lo.source_f = VoltToFreq * freqDoubleSpinBox->value();
    
    // Set LO volts
    lo.source_voltage = loVoltDoubleSpinBox->value()/pow(2.0,0.5);
        
    // Set Bias
    bias.source_voltage = biasDoubleSpinBox->value();
    
    // Redraw plot? Breaks on mouse event if we do.  
    // This won't happen if we set the scan variable value properly in on MouseEvent
    redrawPlot();
}

void QTMixForm::setEmbImpedance()
{
    dummyIf.Z1 = Complex(realIfEmbImpDoubleSpinBox->value(), imagIfEmbImpDoubleSpinBox->value() )*Ohm;;
    embImp.Z = Complex( real1EmbImpDoubleSpinBox->value(), imag1EmbImpDoubleSpinBox->value() )*Ohm;
    
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


void QTMixForm::setPlotXaxis()
{
    // Disable signals to setPlotLimits()
    //plotLowDoubleSpinBox->blockSignals( TRUE );
    //plotHighDoubleSpinBox->blockSignals( TRUE );

    
    switch ( plotXAxisComboBox->currentItem() )
    {
    case 0: // Bias
 {
     plotLowDoubleSpinBox->setValue( 0.0 );
     plotHighDoubleSpinBox->setValue( 2.0 );
     
        plotLowDoubleSpinBox->setMinValue( 0.0 );
	    plotHighDoubleSpinBox->setMinValue( 0.01 );
	    
   	    plotLowDoubleSpinBox->setMaxValue( 2.0 );
	    plotHighDoubleSpinBox->setMaxValue( 5.0 );
	    break;
	}
    case 1: // LO Voltage
	{
	    plotLowDoubleSpinBox->setValue( 0.0 );
	    plotHighDoubleSpinBox->setValue( 2.0 );
	    
	    plotLowDoubleSpinBox->setMinValue( 0.00 );
	    plotHighDoubleSpinBox->setMinValue( 0.00 );
	    
   	    plotLowDoubleSpinBox->setMaxValue( 10.0 );
	    plotHighDoubleSpinBox->setMaxValue( 10.0 );
	    
	    break;
	}	    
    case 2: // LO Frequency
	{
	    plotLowDoubleSpinBox->setValue( 0.0 );
	    plotHighDoubleSpinBox->setValue( 2.0 );
	    
	    plotLowDoubleSpinBox->setMinValue( 0.051 );
	    plotHighDoubleSpinBox->setMinValue( 0.051 );
	    
   	    plotLowDoubleSpinBox->setMaxValue( 2.0 );
	    plotHighDoubleSpinBox->setMaxValue( 5.0 );

	    break;
	}
    case 3: // IF Frequency
	{
	    plotLowDoubleSpinBox->setValue( 0.0 );
	    plotHighDoubleSpinBox->setValue( 0.05 );
	    	    
	    plotLowDoubleSpinBox->setMinValue( 0.0 );
	    plotHighDoubleSpinBox->setMinValue( 0.001 );
	    
   	    plotLowDoubleSpinBox->setMaxValue( 0.05 );
	    plotHighDoubleSpinBox->setMaxValue( 0.05 );

	    break;
	}
    }
    
    // Reenable signals to setPlotLimits()
    //plotLowDoubleSpinBox->blockSignals( FALSE );
    //plotHighDoubleSpinBox->blockSignals( FALSE );
    
    redrawPlot();
}


void QTMixForm::setJunction()
{
    sis.Rn = RnSpinBox->value();
    sis.Cap = CapSpinBox->value()*fFarad;
    
    redrawPlot();
}
