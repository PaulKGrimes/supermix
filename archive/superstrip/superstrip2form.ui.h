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



void SuperStrip2Form::setDielectric()
{
    // Get the values from the input widgets
    substrate.eps = diEpsilonDoubleSpinBox->value();
    substrate.tand = diLossDoubleSpinBox->value() * 1.0e-4;
}


void SuperStrip2Form::setSuperconductor()
{
    superconductor.Vgap = scGapDoubleSpinBox->value() * mVolt;
    superconductor.Tc = scTcDoubleSpinBox->value() * Kelvin;
    superconductor.rho_normal = scRhoDoubleSpinBox->value() * Micro*Ohm*Centi*Meter;
}


void SuperStrip2Form::setDimensions()
{
    strip.width = widthDoubleSpinBox->value() * Micron;
    strip.sub_thick = heightDoubleSpinBox->value() * Micron;
    superconductor.Thick = thicknessDoubleSpinBox->value() * Micron;
    conductor.Thick = thicknessDoubleSpinBox->value() * Micron;
}


void SuperStrip2Form::setOperatingPoint()
{
    device::f = freqDoubleSpinBox->value() * GHz;
    device::T = temperatureDoubleSpinBox->value() * Kelvin;
}


void SuperStrip2Form::calculate()
{
    // This line required to get a perfectly  matched section of microstrip
    device::Z0 = strip.Zchar(device::f, device::T).real;

    // Get SData from the microstrip
    sdata s = strip.get_data();

    // Set the attenuation (dB/mm) and the noise temperature
    resLossLineEdit->setText( QString::number(s.SdB(2,1), 'f', 4) );
    resTnLineEdit->setText( QString::number(s.tn(2,1)/Kelvin, 'f', 4) );

    // Set the characteristic impedance (Ohms)
    resImpLineEdit->setText( QString::number( strip.Zchar(device::f, device::T).real, 'f', 4 ) );

    // Set the inductance per mm
    resBetaLineEdit->setText( QString::number( strip.Kprop(device::f, device::T).imaginary , 'f', 5) );

    redrawPlot();
}


void SuperStrip2Form::init()
{
    // Set up operating point
    device::f = 700*GHz;
    device::T = 4.2*Kelvin;
    device::Z0 = 20*Ohm;

    freqDoubleSpinBox->setValue( 700.0 );
    temperatureDoubleSpinBox->setValue( 4.2 );
    
    // Set up Metal Type combobox
    if (!metalTypeComboBox->count() ) 
    {
	metalTypeComboBox->insertItem( "Superconductor" );
	metalTypeComboBox->insertItem( "Normal Metal" );
    }
    metalTypeComboBox->setCurrentItem( 0 );

    // Set up superconductor
    superconductor.Vgap = 2.81*mVolt;
    superconductor.Tc = 9.23*Kelvin;
    superconductor.rho_normal = 5.0*Micro*Ohm*Centi*Meter;
    superconductor.Thick = 0.4*Micron;

    top = & superconductor;
    
    scGapDoubleSpinBox->setValue( 2.81 );
    scTcDoubleSpinBox->setValue( 9.23 );
    scRhoDoubleSpinBox->setValue( 5.0 );
    thicknessDoubleSpinBox->setValue( 0.4 );
    
    // Set up normal metal
    conductor.rho = 3.18*Micro*Ohm*Centi*Meter;
    conductor.Thick = 0.4*Micron;
    
    nmRhoDoubleSpinBox->setValue( 3.18 );

    // Set up dielectric

    substrate.eps = 5.8;
    substrate.tand = 4.0e-4;

    diEpsilonDoubleSpinBox->setValue( 5.8 );
    diLossDoubleSpinBox->setValue( 4.0 );

    // Set up air
    air.eps = 1.0;
    air.tand = 0.0;

    // Set up microstrip
    strip.ground_plane( superconductor );
    strip.substrate ( substrate );
    strip.top_strip( superconductor );
    strip.superstrate( air );

    strip.length = 1000.0*Micron;
    strip.width = 3.0*Micron;
    strip.sub_thick = 0.4*Micron;

    widthDoubleSpinBox->setValue( 3.0 );
    heightDoubleSpinBox->setValue( 0.4 );

    // Set up plot controls
    plotLow = 10.0;
    plotHigh = 1000.0;

    plotLowDoubleSpinBox->setValue( 10.0 );
    plotHighDoubleSpinBox->setValue( 1000.0 );

    if (!plotLeftComboBox->count())
    {
	plotLeftComboBox->insertItem( "Impedance" );
	plotLeftComboBox->insertItem( "Beta" );
	plotLeftComboBox->insertItem( "Attenuation" );
	plotLeftComboBox->insertItem( "Noise" );
	plotLeftComboBox->insertItem( "Re(Zsurf)" );
	plotLeftComboBox->insertItem( "Im(Zsurf)" );
	plotLeftComboBox->insertItem( "Re(Skin Depth)" );
	plotLeftComboBox->insertItem( "Im(Skin Depth)" );
    }
    plotLeftComboBox->setCurrentItem( 0 );

    if (!plotRightComboBox->count())
    {
	plotRightComboBox->insertItem( "Impedance" );
	plotRightComboBox->insertItem( "Beta" );
	plotRightComboBox->insertItem( "Attenuation" );
	plotRightComboBox->insertItem( "Noise" );
	plotRightComboBox->insertItem( "Re(Zsurf)" );
	plotRightComboBox->insertItem( "Im(Zsurf)" );
	plotRightComboBox->insertItem( "Re(Skin Depth)" );
	plotRightComboBox->insertItem( "Im(Skin Depth)" );
    }
    plotRightComboBox->setCurrentItem( 2 );

    // Set up plot
    plot->setAutoLegend(TRUE);
    plot->setLegendPos(Qwt::Bottom);
    plot->setGridPen( QPen(darkGray, 1, SolidLine) );

    plot->setAxisTitle(plot->xBottom, "Frequency (GHz)");
    plot->setAxisTitle(plot->yLeft, "Impedance (Ohms)");
    plot->setAxisTitle(plot->yRight, "Attenuation (dB/mm)");

    plot->removeCurves();
    plotLeftCurve = plot->insertCurve("Impedance");
    plotRightCurve = plot->insertCurve("Losses", plot->xBottom, plot->yRight);

    plot->setCurvePen(plotLeftCurve, QPen(red));
    plot->setCurvePen(plotRightCurve, QPen(blue));

    redrawPlot();

}


void SuperStrip2Form::redrawPlot()
{
    plot->removeCurves();
    plotLeftCurve = plot->insertCurve( plotLeftComboBox->currentText() );
    plotRightCurve = plot->insertCurve( plotRightComboBox->currentText(), plot->xBottom, plot->yRight );

    plot->setCurvePen(plotLeftCurve, QPen(red));
    plot->setCurvePen(plotRightCurve, QPen(blue));

    int nPoints = 100;

    // Declare arrays for holding results
    double bottom[nPoints];
    double left[nPoints];
    double right[nPoints];

    sdata s;

    // Do the calculation
    for (int i=0; i < nPoints; i++)
    {
	bottom[i] = plotLow + (plotHigh - plotLow) * double(i) / double(nPoints);
	device::f = bottom[i] * GHz;

	device::Z0 = strip.Zchar(device::f, device::T).real;
	s = strip.get_data();

	// Calculate the LH axis and data
	switch ( plotLeftComboBox->currentItem() )
	{
	case 0:
	{
	    plot->setAxisTitle(plot->yLeft, "Impedance (Ohms)");
	    left[i] = strip.Zchar(device::f, device::T).real / Ohm;
	    break;
	}
	case 1:
	    {
	    plot->setAxisTitle(plot->yLeft, "Beta");
	    left[i] = strip.Kprop(device::f, device::T).imaginary;
	    break;
	}
	case 2:
	    {
	    plot->setAxisTitle(plot->yLeft, "Attenuation (dB/mm)");
	    left[i] = s.SdB(2,1);
	    break;
	}
	case 3:
	    {
	    plot->setAxisTitle(plot->yLeft, "Noise Temperature (K)");
	    left[i] = s.tn(2,1)/Kelvin;
	    break;
	}
	case 4:
	    {
	    plot->setAxisTitle(plot->yLeft, "Re(Surface Impedance)");
	    left[i] = top->Zsurf(device::f, device::T).real;
	    break;
	}
	case 5:
	    {
	    plot->setAxisTitle(plot->yLeft, "Im(Surface Impedance)");
	    left[i] = top->Zsurf(device::f, device::T).imaginary;
	    break;
	}
	case 6:
	    {
	    plot->setAxisTitle(plot->yLeft, "Re(Skin Depth) (nm)");
	    left[i] = ( top->Zsurf(device::f, device::T).real / (muVacuum * 2 * Pi * device::f) ) / (Nano*Meter);
	    break;
	}
	case 7:
	    {
	    plot->setAxisTitle(plot->yLeft, "Im(Skin Depth) (nm)");
	    left[i] = ( top->Zsurf(device::f, device::T).imaginary / (muVacuum * 2 * Pi * device::f) ) / (Nano*Meter);
	    break;
	}   
	}
	
	// Calculate the RH axis and data
	switch ( plotRightComboBox->currentItem() ) 
	{
	case 0:
	{
	    plot->setAxisTitle( plot->yRight, "Impedance (Ohms)" );
	    plot->setAxisLabelFormat( plot->yRight, 'f', 5 );
	    right[i] = strip.Zchar(device::f, device::T).real / Ohm;
	    break;
	}
	case 1:
	    {
	    plot->setAxisTitle(plot->yRight, "Beta");
	    right[i] = strip.Kprop(device::f, device::T).imaginary;
	    break;
	}
	case 2:
	    {
	    plot->setAxisTitle(plot->yRight, "Attenuation (dB/mm)");
	    right[i] = s.SdB(2,1);
	    break;
	}
	case 3:
	    {
	    plot->setAxisTitle(plot->yRight, "Noise Temperature (K)");
	    right[i] = s.tn(2,1)/Kelvin;
	    break;
	}
	case 4:
	    {
	    plot->setAxisTitle(plot->yRight, "Re(Surface Impedance)");
	    right[i] = top->Zsurf(device::f, device::T).real;
	    break;
	}
	case 5:
	    {
	    plot->setAxisTitle(plot->yRight, "Im(Surface Impedance)");
	    right[i] = top->Zsurf(device::f, device::T).imaginary;
	    break;
	}
	case 6:
	    {
	    plot->setAxisTitle(plot->yRight, "Re(Skin Depth) (nm)");
	    right[i] = ( top->Zsurf(device::f, device::T).real / (muVacuum * 2 * Pi * device::f) ) / (Nano*Meter);
	    break;
	}
	case 7:
	    {
	    plot->setAxisTitle(plot->yRight, "Im(Skin Depth) (nm)");
	    right[i] = ( top->Zsurf(device::f, device::T).imaginary / (muVacuum * 2 * Pi * device::f) ) / (Nano*Meter);
	    break;
	}
	}
	
    }
    
    // Reset device::f
    device::f = freqDoubleSpinBox->value() * GHz;
    
    plot->setCurveData(plotLeftCurve, bottom, left, nPoints);
    plot->setCurveData(plotRightCurve, bottom, right, nPoints);
    
    plot->replot();
}


void SuperStrip2Form::setPlotLimits()
{
    plotLow = plotLowDoubleSpinBox->value();
    plotHigh = plotHighDoubleSpinBox->value();
}

// Calculates the microstrip properties at the point where the mouse event occured on the plot
void SuperStrip2Form::calcOnMouse( const QMouseEvent &e )
{
    double freq = plot->invTransform( plot->xBottom, e.x() );
    freqDoubleSpinBox->setValue( freq );
    
    plot->removeMarkers();
    long mark = plot->insertMarker( );
    plot->setMarkerLineStyle( mark, QwtMarker::VLine );
    plot->setMarkerXPos( mark, freq );
    
    setOperatingPoint();
    calculate();
}


void SuperStrip2Form::fileSaveAs()
{
    QString filename = QFileDialog::getSaveFileName( QString::null, 
		"Data (*.dat)", this, "file save as", "SuperStrip -- Data Save As" );
    if ( !filename.isEmpty() ) {
	int answer = 0;
	if ( QFile::exists( filename ) )
	    answer = QMessageBox::warning(
		    this, "SuperStrip -- Overwrite File",
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



void SuperStrip2Form::fileSave()
{
    QFile file( m_filename );
    if ( !file.open( IO_WriteOnly ) ) {
	return;
    }
    QTextStream ts( &file );
    
    ts.width( 6 );
    
    // Output Settings
    ts << "# SuperStrip2 Microstrip Data" << endl;
    ts << "# " << endl;
    switch ( metalTypeComboBox->currentItem() )
    {
    case 0:
	{
	    ts << "# Metal Type:		Superconductor" << endl;
	    ts << "# Superconductor Properties:" << endl;
	    ts << "# 	Gap Voltage: 		" << superconductor.Vgap/mVolt << " mV" << endl;
	    ts << "# 	Critical Temp:		" << superconductor.Tc/Kelvin << " K" << endl;
	    ts << "# 	Normal Resistivity:	" << superconductor.rho_normal/( Micro*Ohm*Centi*Meter ) << " uOhm cm" << endl;
	}
    case 1:
	{
	    ts << "# Metal Type:		Normal Metal" << endl;
	    ts << "# Conductor Properties:" << endl;
	    ts << "# 	Resistivity:		" << conductor.rho/( Micro*Ohm*Centi*Meter ) << "uOhm cm" << endl;
	}
    }	
    ts << "# Dielectric Properties:" << endl;
    ts << "# 	Permittivity:		" << substrate.eps << endl;
    ts << "# 	Loss Tangent:		" << substrate.tand << endl;
    ts << "# Microstrip Dimensions:" << endl;
    ts << "# 	Width:			" << strip.width/Micron << " um" << endl;
    ts << "# 	Length:			1.0 mm" << endl;
    ts << "# 	Height:			" << strip.sub_thick/Micron << " um" << endl;
    ts << "# 	Conductor Thickness:	" << top->Thick/Micron << " um" << endl;
    ts << "# Temperature:			" << device::T/Kelvin << " K" << endl;
    ts << "# " << endl;
    ts << "# Freq (GHz) | Z0 (Ohms) | Beta | Attenuation (dB/mm) | Noise Figure (K/mm) | Re(Zsurf) | Im(Zsurf)" << endl;
    
    // Calculate data and output to ts
    ts.precision ( 6 );    
    int nPoints = 99;
    sdata s;
    
    for (int i=0; i < nPoints; i++)
    {
	device::f = (plotLow + (plotHigh - plotLow) * double(i) / double(nPoints)) * GHz;
	ts << device::f/GHz << " ";
	
	device::Z0 = strip.Zchar(device::f, device::T).real;
	ts << device::Z0/Ohm << " ";
	ts << strip.Kprop(device::f, device::T).imaginary << " ";
	
	s = strip.get_data();
	ts << s.SdB(2,1) << " ";
	ts << s.tn(2,1)/Kelvin << " ";
	ts << top->Zsurf(device::f, device::T).real << " ";
	ts << top->Zsurf(device::f, device::T).imaginary << endl;
    }
    file.close();
}


void SuperStrip2Form::setMetal() 
{
    switch ( metalTypeComboBox->currentItem() )
    {
    case 0:
	{
	    // Superconductor Case
	    strip.top_strip( superconductor );
	    strip.ground_plane( superconductor );
	    top = &superconductor;
	    metalWidgetStack->raiseWidget( 0 );
	    break;
	}
    case 1:
	{
	    // Normal Metal Case
	    strip.top_strip( conductor );
	    strip.ground_plane( conductor );
	    top = &conductor;
	    metalWidgetStack->raiseWidget( 1 );
	    break;
	}
    }	  
}


void SuperStrip2Form::setConductor()
{
    conductor.rho = nmRhoDoubleSpinBox->value() * Micro*Ohm*Centi*Meter;
}
