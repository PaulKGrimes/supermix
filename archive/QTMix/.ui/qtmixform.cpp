/****************************************************************************
** Form implementation generated from reading ui file 'qtmixform.ui'
**
** Created: Fri Feb 23 17:02:44 2007
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "qtmixform.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qsplitter.h>
#include <qframe.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "supermix.h"
#include "../qtmixform.ui.h"
/*
 *  Constructs a QTMixForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
QTMixForm::QTMixForm( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "QTMixForm" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, sizePolicy().hasHeightForWidth() ) );
    QTMixFormLayout = new QVBoxLayout( this, 2, 6, "QTMixFormLayout"); 

    splitter2 = new QSplitter( this, "splitter2" );
    splitter2->setOrientation( QSplitter::Horizontal );

    frame4 = new QFrame( splitter2, "frame4" );
    frame4->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)5, 0, 0, frame4->sizePolicy().hasHeightForWidth() ) );
    frame4->setFrameShape( QFrame::StyledPanel );
    frame4->setFrameShadow( QFrame::Raised );
    frame4Layout = new QVBoxLayout( frame4, 6, 6, "frame4Layout"); 

    junctionGroupBox = new QGroupBox( frame4, "junctionGroupBox" );
    junctionGroupBox->setColumnLayout(0, Qt::Vertical );
    junctionGroupBox->layout()->setSpacing( 6 );
    junctionGroupBox->layout()->setMargin( 6 );
    junctionGroupBoxLayout = new QGridLayout( junctionGroupBox->layout() );
    junctionGroupBoxLayout->setAlignment( Qt::AlignTop );

    idcFileChooser = new FileChooser( junctionGroupBox, "idcFileChooser" );
    idcFileChooser->setEnabled( TRUE );

    junctionGroupBoxLayout->addMultiCellWidget( idcFileChooser, 0, 0, 2, 3 );
    spacer18_3 = new QSpacerItem( 60, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    junctionGroupBoxLayout->addItem( spacer18_3, 0, 1 );

    textLabel1_2 = new QLabel( junctionGroupBox, "textLabel1_2" );

    junctionGroupBoxLayout->addWidget( textLabel1_2, 0, 0 );

    textLabel2_4 = new QLabel( junctionGroupBox, "textLabel2_4" );

    junctionGroupBoxLayout->addWidget( textLabel2_4, 1, 0 );
    spacer17_3 = new QSpacerItem( 60, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    junctionGroupBoxLayout->addItem( spacer17_3, 1, 1 );

    ikkFileChooser = new FileChooser( junctionGroupBox, "ikkFileChooser" );
    ikkFileChooser->setEnabled( TRUE );

    junctionGroupBoxLayout->addMultiCellWidget( ikkFileChooser, 1, 1, 2, 3 );

    textLabel2_4_2 = new QLabel( junctionGroupBox, "textLabel2_4_2" );

    junctionGroupBoxLayout->addMultiCellWidget( textLabel2_4_2, 2, 3, 0, 1 );
    spacer17_3_2 = new QSpacerItem( 30, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    junctionGroupBoxLayout->addItem( spacer17_3_2, 2, 2 );
    spacer17_3_2_2 = new QSpacerItem( 30, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    junctionGroupBoxLayout->addMultiCell( spacer17_3_2_2, 3, 4, 2, 2 );

    RnSpinBox = new QDoubleSpinBox( junctionGroupBox, "RnSpinBox" );
    RnSpinBox->setMaxValue( 100 );
    RnSpinBox->setLineStep( 0.1 );
    RnSpinBox->setValue( 1 );
    RnSpinBox->setDisplayPrecision( 1 );

    junctionGroupBoxLayout->addMultiCellWidget( RnSpinBox, 2, 3, 3, 3 );

    CapSpinBox = new QDoubleSpinBox( junctionGroupBox, "CapSpinBox" );
    CapSpinBox->setMaxValue( 1000 );
    CapSpinBox->setLineStep( 5 );
    CapSpinBox->setValue( 65 );
    CapSpinBox->setDisplayPrecision( 1 );

    junctionGroupBoxLayout->addWidget( CapSpinBox, 4, 3 );

    textLabel2_4_2_2 = new QLabel( junctionGroupBox, "textLabel2_4_2_2" );

    junctionGroupBoxLayout->addMultiCellWidget( textLabel2_4_2_2, 4, 4, 0, 1 );
    frame4Layout->addWidget( junctionGroupBox );

    embImpGroupBox = new QGroupBox( frame4, "embImpGroupBox" );
    embImpGroupBox->setColumnLayout(0, Qt::Vertical );
    embImpGroupBox->layout()->setSpacing( 6 );
    embImpGroupBox->layout()->setMargin( 6 );
    embImpGroupBoxLayout = new QVBoxLayout( embImpGroupBox->layout() );
    embImpGroupBoxLayout->setAlignment( Qt::AlignTop );

    layout18 = new QHBoxLayout( 0, 0, 6, "layout18"); 

    textLabel2_2_4 = new QLabel( embImpGroupBox, "textLabel2_2_4" );
    layout18->addWidget( textLabel2_2_4 );
    spacer18_2_4 = new QSpacerItem( 130, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout18->addItem( spacer18_2_4 );

    layout14 = new QHBoxLayout( 0, 0, 6, "layout14"); 

    realIfEmbImpDoubleSpinBox = new QDoubleSpinBox( embImpGroupBox, "realIfEmbImpDoubleSpinBox" );
    realIfEmbImpDoubleSpinBox->setMaxValue( 20 );
    realIfEmbImpDoubleSpinBox->setLineStep( 0.1 );
    realIfEmbImpDoubleSpinBox->setValue( 0 );
    realIfEmbImpDoubleSpinBox->setDisplayPrecision( 2 );
    layout14->addWidget( realIfEmbImpDoubleSpinBox );

    textLabel3_2_4 = new QLabel( embImpGroupBox, "textLabel3_2_4" );
    layout14->addWidget( textLabel3_2_4 );

    imagIfEmbImpDoubleSpinBox = new QDoubleSpinBox( embImpGroupBox, "imagIfEmbImpDoubleSpinBox" );
    imagIfEmbImpDoubleSpinBox->setMaxValue( 10 );
    imagIfEmbImpDoubleSpinBox->setMinValue( -10 );
    imagIfEmbImpDoubleSpinBox->setLineStep( 0.1 );
    imagIfEmbImpDoubleSpinBox->setValue( 0 );
    imagIfEmbImpDoubleSpinBox->setDisplayPrecision( 2 );
    layout14->addWidget( imagIfEmbImpDoubleSpinBox );
    layout18->addLayout( layout14 );
    embImpGroupBoxLayout->addLayout( layout18 );

    layout16 = new QHBoxLayout( 0, 0, 6, "layout16"); 

    textLabel2_2 = new QLabel( embImpGroupBox, "textLabel2_2" );
    layout16->addWidget( textLabel2_2 );
    spacer18_2 = new QSpacerItem( 74, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout16->addItem( spacer18_2 );

    layout15 = new QHBoxLayout( 0, 0, 6, "layout15"); 

    real1EmbImpDoubleSpinBox = new QDoubleSpinBox( embImpGroupBox, "real1EmbImpDoubleSpinBox" );
    real1EmbImpDoubleSpinBox->setMaxValue( 10 );
    real1EmbImpDoubleSpinBox->setLineStep( 0.1 );
    real1EmbImpDoubleSpinBox->setValue( 0 );
    real1EmbImpDoubleSpinBox->setDisplayPrecision( 2 );
    layout15->addWidget( real1EmbImpDoubleSpinBox );

    textLabel3_2 = new QLabel( embImpGroupBox, "textLabel3_2" );
    layout15->addWidget( textLabel3_2 );

    imag1EmbImpDoubleSpinBox = new QDoubleSpinBox( embImpGroupBox, "imag1EmbImpDoubleSpinBox" );
    imag1EmbImpDoubleSpinBox->setMaxValue( 10 );
    imag1EmbImpDoubleSpinBox->setMinValue( -10 );
    imag1EmbImpDoubleSpinBox->setLineStep( 0.1 );
    imag1EmbImpDoubleSpinBox->setValue( 0 );
    imag1EmbImpDoubleSpinBox->setDisplayPrecision( 2 );
    layout15->addWidget( imag1EmbImpDoubleSpinBox );
    layout16->addLayout( layout15 );
    embImpGroupBoxLayout->addLayout( layout16 );

    layout7_2 = new QHBoxLayout( 0, 0, 6, "layout7_2"); 

    textLabel2_2_2 = new QLabel( embImpGroupBox, "textLabel2_2_2" );
    textLabel2_2_2->setEnabled( FALSE );
    layout7_2->addWidget( textLabel2_2_2 );
    spacer18_2_2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout7_2->addItem( spacer18_2_2 );

    layout6_2 = new QHBoxLayout( 0, 0, 6, "layout6_2"); 

    real2EmbImpDoubleSpinBox = new QDoubleSpinBox( embImpGroupBox, "real2EmbImpDoubleSpinBox" );
    real2EmbImpDoubleSpinBox->setEnabled( FALSE );
    real2EmbImpDoubleSpinBox->setMaxValue( 2 );
    real2EmbImpDoubleSpinBox->setLineStep( 0.1 );
    real2EmbImpDoubleSpinBox->setValue( 1 );
    real2EmbImpDoubleSpinBox->setDisplayPrecision( 2 );
    layout6_2->addWidget( real2EmbImpDoubleSpinBox );

    textLabel3_2_2 = new QLabel( embImpGroupBox, "textLabel3_2_2" );
    textLabel3_2_2->setEnabled( FALSE );
    layout6_2->addWidget( textLabel3_2_2 );

    imag2EmbImpDoubleSpinBox = new QDoubleSpinBox( embImpGroupBox, "imag2EmbImpDoubleSpinBox" );
    imag2EmbImpDoubleSpinBox->setEnabled( FALSE );
    imag2EmbImpDoubleSpinBox->setMaxValue( 2 );
    imag2EmbImpDoubleSpinBox->setMinValue( -2 );
    imag2EmbImpDoubleSpinBox->setLineStep( 0.1 );
    imag2EmbImpDoubleSpinBox->setValue( 0 );
    imag2EmbImpDoubleSpinBox->setDisplayPrecision( 2 );
    layout6_2->addWidget( imag2EmbImpDoubleSpinBox );
    layout7_2->addLayout( layout6_2 );
    embImpGroupBoxLayout->addLayout( layout7_2 );

    layout7_3 = new QHBoxLayout( 0, 0, 6, "layout7_3"); 

    textLabel2_2_3 = new QLabel( embImpGroupBox, "textLabel2_2_3" );
    textLabel2_2_3->setEnabled( FALSE );
    layout7_3->addWidget( textLabel2_2_3 );
    spacer18_2_3 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout7_3->addItem( spacer18_2_3 );

    layout6_3 = new QHBoxLayout( 0, 0, 6, "layout6_3"); 

    real3EmbImpDoubleSpinBox = new QDoubleSpinBox( embImpGroupBox, "real3EmbImpDoubleSpinBox" );
    real3EmbImpDoubleSpinBox->setEnabled( FALSE );
    real3EmbImpDoubleSpinBox->setMaxValue( 2 );
    real3EmbImpDoubleSpinBox->setLineStep( 0.1 );
    real3EmbImpDoubleSpinBox->setValue( 1 );
    real3EmbImpDoubleSpinBox->setDisplayPrecision( 2 );
    layout6_3->addWidget( real3EmbImpDoubleSpinBox );

    textLabel3_2_3 = new QLabel( embImpGroupBox, "textLabel3_2_3" );
    textLabel3_2_3->setEnabled( FALSE );
    layout6_3->addWidget( textLabel3_2_3 );

    imag3EmbImpDoubleSpinBox = new QDoubleSpinBox( embImpGroupBox, "imag3EmbImpDoubleSpinBox" );
    imag3EmbImpDoubleSpinBox->setEnabled( FALSE );
    imag3EmbImpDoubleSpinBox->setMaxValue( 2 );
    imag3EmbImpDoubleSpinBox->setMinValue( -2 );
    imag3EmbImpDoubleSpinBox->setLineStep( 0.1 );
    imag3EmbImpDoubleSpinBox->setValue( 0 );
    imag3EmbImpDoubleSpinBox->setDisplayPrecision( 2 );
    layout6_3->addWidget( imag3EmbImpDoubleSpinBox );
    layout7_3->addLayout( layout6_3 );
    embImpGroupBoxLayout->addLayout( layout7_3 );
    frame4Layout->addWidget( embImpGroupBox );

    opPointGroupBox = new QGroupBox( frame4, "opPointGroupBox" );
    opPointGroupBox->setColumnLayout(0, Qt::Vertical );
    opPointGroupBox->layout()->setSpacing( 6 );
    opPointGroupBox->layout()->setMargin( 6 );
    opPointGroupBoxLayout = new QGridLayout( opPointGroupBox->layout() );
    opPointGroupBoxLayout->setAlignment( Qt::AlignTop );

    textLabel5 = new QLabel( opPointGroupBox, "textLabel5" );

    opPointGroupBoxLayout->addWidget( textLabel5, 0, 0 );

    textLabel4_2 = new QLabel( opPointGroupBox, "textLabel4_2" );

    opPointGroupBoxLayout->addWidget( textLabel4_2, 1, 0 );

    textLabel6 = new QLabel( opPointGroupBox, "textLabel6" );

    opPointGroupBoxLayout->addWidget( textLabel6, 2, 0 );
    spacer22 = new QSpacerItem( 70, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    opPointGroupBoxLayout->addMultiCell( spacer22, 0, 0, 1, 2 );
    spacer21 = new QSpacerItem( 70, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    opPointGroupBoxLayout->addMultiCell( spacer21, 1, 1, 1, 2 );
    spacer23 = new QSpacerItem( 80, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    opPointGroupBoxLayout->addMultiCell( spacer23, 2, 2, 1, 2 );

    loVoltDoubleSpinBox = new QDoubleSpinBox( opPointGroupBox, "loVoltDoubleSpinBox" );
    loVoltDoubleSpinBox->setLineStep( 0.1 );
    loVoltDoubleSpinBox->setValue( 1 );
    loVoltDoubleSpinBox->setDisplayPrecision( 2 );

    opPointGroupBoxLayout->addWidget( loVoltDoubleSpinBox, 2, 3 );

    freqDoubleSpinBox = new QDoubleSpinBox( opPointGroupBox, "freqDoubleSpinBox" );
    freqDoubleSpinBox->setMaxValue( 2 );
    freqDoubleSpinBox->setMinValue( 0.01 );
    freqDoubleSpinBox->setLineStep( 0.1 );
    freqDoubleSpinBox->setValue( 0.9 );
    freqDoubleSpinBox->setDisplayPrecision( 2 );

    opPointGroupBoxLayout->addWidget( freqDoubleSpinBox, 1, 3 );

    biasDoubleSpinBox = new QDoubleSpinBox( opPointGroupBox, "biasDoubleSpinBox" );
    biasDoubleSpinBox->setMaxValue( 10 );
    biasDoubleSpinBox->setLineStep( 0.1 );
    biasDoubleSpinBox->setValue( 0.5 );
    biasDoubleSpinBox->setDisplayPrecision( 2 );

    opPointGroupBoxLayout->addWidget( biasDoubleSpinBox, 0, 3 );
    spacer24 = new QSpacerItem( 90, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    opPointGroupBoxLayout->addMultiCell( spacer24, 4, 4, 1, 2 );

    IFfreqDoubleSpinBox = new QDoubleSpinBox( opPointGroupBox, "IFfreqDoubleSpinBox" );
    IFfreqDoubleSpinBox->setMaxValue( 1 );
    IFfreqDoubleSpinBox->setLineStep( 0.01 );
    IFfreqDoubleSpinBox->setValue( 0.01 );
    IFfreqDoubleSpinBox->setDisplayPrecision( 6 );

    opPointGroupBoxLayout->addWidget( IFfreqDoubleSpinBox, 4, 3 );

    textLabel8 = new QLabel( opPointGroupBox, "textLabel8" );

    opPointGroupBoxLayout->addWidget( textLabel8, 4, 0 );
    frame4Layout->addWidget( opPointGroupBox );

    simulationGroupBox = new QGroupBox( frame4, "simulationGroupBox" );
    simulationGroupBox->setColumnLayout(0, Qt::Vertical );
    simulationGroupBox->layout()->setSpacing( 6 );
    simulationGroupBox->layout()->setMargin( 6 );
    simulationGroupBoxLayout = new QVBoxLayout( simulationGroupBox->layout() );
    simulationGroupBoxLayout->setAlignment( Qt::AlignTop );

    layout21 = new QHBoxLayout( 0, 0, 6, "layout21"); 

    textLabel11 = new QLabel( simulationGroupBox, "textLabel11" );
    layout21->addWidget( textLabel11 );
    spacer27 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout21->addItem( spacer27 );

    harmonicsSpinBox = new QSpinBox( simulationGroupBox, "harmonicsSpinBox" );
    harmonicsSpinBox->setMaxValue( 5 );
    harmonicsSpinBox->setMinValue( 1 );
    layout21->addWidget( harmonicsSpinBox );
    simulationGroupBoxLayout->addLayout( layout21 );
    frame4Layout->addWidget( simulationGroupBox );

    resultsGroupBox = new QGroupBox( frame4, "resultsGroupBox" );
    resultsGroupBox->setColumnLayout(0, Qt::Vertical );
    resultsGroupBox->layout()->setSpacing( 6 );
    resultsGroupBox->layout()->setMargin( 6 );
    resultsGroupBoxLayout = new QGridLayout( resultsGroupBox->layout() );
    resultsGroupBoxLayout->setAlignment( Qt::AlignTop );
    spacer13 = new QSpacerItem( 21, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    resultsGroupBoxLayout->addItem( spacer13, 0, 1 );
    spacer14 = new QSpacerItem( 21, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    resultsGroupBoxLayout->addItem( spacer14, 1, 1 );

    unpumpLineEdit = new QLineEdit( resultsGroupBox, "unpumpLineEdit" );
    unpumpLineEdit->setReadOnly( TRUE );

    resultsGroupBoxLayout->addWidget( unpumpLineEdit, 0, 3 );

    textLabel1_3 = new QLabel( resultsGroupBox, "textLabel1_3" );

    resultsGroupBoxLayout->addWidget( textLabel1_3, 0, 0 );

    textLabel2_3 = new QLabel( resultsGroupBox, "textLabel2_3" );

    resultsGroupBoxLayout->addWidget( textLabel2_3, 1, 0 );

    pumpedLineEdit = new QLineEdit( resultsGroupBox, "pumpedLineEdit" );
    pumpedLineEdit->setReadOnly( TRUE );

    resultsGroupBoxLayout->addWidget( pumpedLineEdit, 1, 3 );

    textLabel3_3 = new QLabel( resultsGroupBox, "textLabel3_3" );

    resultsGroupBoxLayout->addWidget( textLabel3_3, 3, 0 );
    spacer15 = new QSpacerItem( 21, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    resultsGroupBoxLayout->addMultiCell( spacer15, 3, 3, 1, 2 );

    textLabel3_3_2 = new QLabel( resultsGroupBox, "textLabel3_3_2" );

    resultsGroupBoxLayout->addWidget( textLabel3_3_2, 2, 0 );

    lsbGainLineEdit = new QLineEdit( resultsGroupBox, "lsbGainLineEdit" );
    lsbGainLineEdit->setReadOnly( TRUE );

    resultsGroupBoxLayout->addWidget( lsbGainLineEdit, 2, 3 );

    usbGainLineEdit = new QLineEdit( resultsGroupBox, "usbGainLineEdit" );
    usbGainLineEdit->setReadOnly( TRUE );

    resultsGroupBoxLayout->addWidget( usbGainLineEdit, 3, 3 );

    textLabel4_3 = new QLabel( resultsGroupBox, "textLabel4_3" );

    resultsGroupBoxLayout->addWidget( textLabel4_3, 4, 0 );
    spacer16_3 = new QSpacerItem( 35, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    resultsGroupBoxLayout->addMultiCell( spacer16_3, 6, 6, 1, 2 );
    spacer16_2 = new QSpacerItem( 35, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    resultsGroupBoxLayout->addItem( spacer16_2, 5, 1 );

    textLabel4_3_2 = new QLabel( resultsGroupBox, "textLabel4_3_2" );

    resultsGroupBoxLayout->addWidget( textLabel4_3_2, 5, 0 );

    textLabel4_3_3 = new QLabel( resultsGroupBox, "textLabel4_3_3" );

    resultsGroupBoxLayout->addWidget( textLabel4_3_3, 6, 0 );

    loCurrentLineEdit = new QLineEdit( resultsGroupBox, "loCurrentLineEdit" );
    loCurrentLineEdit->setReadOnly( TRUE );

    resultsGroupBoxLayout->addWidget( loCurrentLineEdit, 6, 3 );
    spacer15_2 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    resultsGroupBoxLayout->addMultiCell( spacer15_2, 2, 2, 1, 2 );

    noiseLineEdit = new QLineEdit( resultsGroupBox, "noiseLineEdit" );
    noiseLineEdit->setReadOnly( TRUE );

    resultsGroupBoxLayout->addWidget( noiseLineEdit, 5, 3 );

    ifOutputLineEdit = new QLineEdit( resultsGroupBox, "ifOutputLineEdit" );
    ifOutputLineEdit->setReadOnly( TRUE );

    resultsGroupBoxLayout->addMultiCellWidget( ifOutputLineEdit, 4, 4, 2, 3 );
    spacer16 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    resultsGroupBoxLayout->addItem( spacer16, 4, 1 );
    frame4Layout->addWidget( resultsGroupBox );
    spacer17_2 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    frame4Layout->addItem( spacer17_2 );

    buttonLayout = new QHBoxLayout( 0, 0, 6, "buttonLayout"); 

    resetPushButton = new QPushButton( frame4, "resetPushButton" );
    buttonLayout->addWidget( resetPushButton );

    calculatePushButton = new QPushButton( frame4, "calculatePushButton" );
    calculatePushButton->setDefault( TRUE );
    buttonLayout->addWidget( calculatePushButton );

    quitPushButton = new QPushButton( frame4, "quitPushButton" );
    buttonLayout->addWidget( quitPushButton );
    frame4Layout->addLayout( buttonLayout );

    frame3 = new QFrame( splitter2, "frame3" );
    frame3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)3, 0, 0, frame3->sizePolicy().hasHeightForWidth() ) );
    frame3->setFrameShape( QFrame::StyledPanel );
    frame3->setFrameShadow( QFrame::Raised );
    frame3Layout = new QVBoxLayout( frame3, 6, 6, "frame3Layout"); 

    plotControlsGroupBox = new QGroupBox( frame3, "plotControlsGroupBox" );
    plotControlsGroupBox->setColumnLayout(0, Qt::Vertical );
    plotControlsGroupBox->layout()->setSpacing( 6 );
    plotControlsGroupBox->layout()->setMargin( 6 );
    plotControlsGroupBoxLayout = new QVBoxLayout( plotControlsGroupBox->layout() );
    plotControlsGroupBoxLayout->setAlignment( Qt::AlignTop );

    layout6 = new QHBoxLayout( 0, 0, 6, "layout6"); 

    textLabel1 = new QLabel( plotControlsGroupBox, "textLabel1" );
    layout6->addWidget( textLabel1 );

    plotLowDoubleSpinBox = new QDoubleSpinBox( plotControlsGroupBox, "plotLowDoubleSpinBox" );
    plotLowDoubleSpinBox->setMaxValue( 10 );
    plotLowDoubleSpinBox->setMinValue( 0 );
    plotLowDoubleSpinBox->setLineStep( 0.1 );
    plotLowDoubleSpinBox->setValue( 0 );
    plotLowDoubleSpinBox->setDisplayPrecision( 2 );
    layout6->addWidget( plotLowDoubleSpinBox );

    textLabel2 = new QLabel( plotControlsGroupBox, "textLabel2" );
    layout6->addWidget( textLabel2 );

    plotHighDoubleSpinBox = new QDoubleSpinBox( plotControlsGroupBox, "plotHighDoubleSpinBox" );
    plotHighDoubleSpinBox->setMaxValue( 10 );
    plotHighDoubleSpinBox->setMinValue( 0 );
    plotHighDoubleSpinBox->setLineStep( 0.1 );
    plotHighDoubleSpinBox->setValue( 2 );
    plotHighDoubleSpinBox->setDisplayPrecision( 2 );
    layout6->addWidget( plotHighDoubleSpinBox );
    spacer18 = new QSpacerItem( 41, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout6->addItem( spacer18 );

    plotRedrawPushButton = new QPushButton( plotControlsGroupBox, "plotRedrawPushButton" );
    layout6->addWidget( plotRedrawPushButton );

    saveDataPushButton = new QPushButton( plotControlsGroupBox, "saveDataPushButton" );
    layout6->addWidget( saveDataPushButton );
    plotControlsGroupBoxLayout->addLayout( layout6 );

    layout19 = new QHBoxLayout( 0, 0, 6, "layout19"); 

    textLabel7 = new QLabel( plotControlsGroupBox, "textLabel7" );
    layout19->addWidget( textLabel7 );

    plotXAxisComboBox = new QComboBox( FALSE, plotControlsGroupBox, "plotXAxisComboBox" );
    plotXAxisComboBox->setMaxCount( 10 );
    layout19->addWidget( plotXAxisComboBox );
    spacer26 = new QSpacerItem( 71, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout19->addItem( spacer26 );
    plotControlsGroupBoxLayout->addLayout( layout19 );

    layout3 = new QHBoxLayout( 0, 0, 6, "layout3"); 

    textLabel3 = new QLabel( plotControlsGroupBox, "textLabel3" );
    layout3->addWidget( textLabel3 );

    plotLeftComboBox = new QComboBox( FALSE, plotControlsGroupBox, "plotLeftComboBox" );
    plotLeftComboBox->setAutoCompletion( TRUE );
    plotLeftComboBox->setDuplicatesEnabled( FALSE );
    layout3->addWidget( plotLeftComboBox );
    spacer17 = new QSpacerItem( 21, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout3->addItem( spacer17 );

    textLabel4 = new QLabel( plotControlsGroupBox, "textLabel4" );
    layout3->addWidget( textLabel4 );

    plotRightComboBox = new QComboBox( FALSE, plotControlsGroupBox, "plotRightComboBox" );
    plotRightComboBox->setAutoCompletion( TRUE );
    plotRightComboBox->setDuplicatesEnabled( FALSE );
    layout3->addWidget( plotRightComboBox );
    plotControlsGroupBoxLayout->addLayout( layout3 );
    frame3Layout->addWidget( plotControlsGroupBox );

    plot = new QwtPlot( frame3, "plot" );
    plot->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)3, 0, 0, plot->sizePolicy().hasHeightForWidth() ) );
    plot->setProperty( "yRightAxis", QVariant( TRUE, 0 ) );
    frame3Layout->addWidget( plot );
    QTMixFormLayout->addWidget( splitter2 );
    languageChange();
    resize( QSize(810, 803).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( quitPushButton, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( calculatePushButton, SIGNAL( clicked() ), this, SLOT( calculate() ) );
    connect( resetPushButton, SIGNAL( clicked() ), this, SLOT( init() ) );
    connect( plotLowDoubleSpinBox, SIGNAL( valueChanged(const QString&) ), this, SLOT( setPlotLimits() ) );
    connect( plotHighDoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setPlotLimits() ) );
    connect( plotRedrawPushButton, SIGNAL( clicked() ), this, SLOT( redrawPlot() ) );
    connect( plotLeftComboBox, SIGNAL( activated(int) ), this, SLOT( redrawPlot() ) );
    connect( plotRightComboBox, SIGNAL( activated(int) ), this, SLOT( redrawPlot() ) );
    connect( plot, SIGNAL( plotMousePressed(const QMouseEvent&) ), this, SLOT( calcOnMouse(const QMouseEvent&) ) );
    connect( saveDataPushButton, SIGNAL( clicked() ), this, SLOT( fileSaveAs() ) );
    connect( freqDoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setOperatingPoint() ) );
    connect( IFfreqDoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setOperatingPoint() ) );
    connect( biasDoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setOperatingPoint() ) );
    connect( loVoltDoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setOperatingPoint() ) );
    connect( real1EmbImpDoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setEmbImpedance() ) );
    connect( imag1EmbImpDoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setEmbImpedance() ) );
    connect( real2EmbImpDoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setEmbImpedance() ) );
    connect( imag2EmbImpDoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setEmbImpedance() ) );
    connect( real3EmbImpDoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setEmbImpedance() ) );
    connect( imag3EmbImpDoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setEmbImpedance() ) );
    connect( harmonicsSpinBox, SIGNAL( valueChanged(int) ), this, SLOT( setHarmonics(int) ) );
    connect( idcFileChooser, SIGNAL( fileNameChanged(const QString&) ), this, SLOT( setIVdata() ) );
    connect( ikkFileChooser, SIGNAL( fileNameChanged(const QString&) ), this, SLOT( setIVdata() ) );
    connect( realIfEmbImpDoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setEmbImpedance() ) );
    connect( imagIfEmbImpDoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setEmbImpedance() ) );
    connect( plotXAxisComboBox, SIGNAL( activated(const QString&) ), this, SLOT( setPlotXaxis() ) );
    connect( CapSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setJunction() ) );
    connect( RnSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setJunction() ) );

    // tab order
    setTabOrder( resetPushButton, calculatePushButton );
    setTabOrder( calculatePushButton, quitPushButton );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
QTMixForm::~QTMixForm()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void QTMixForm::languageChange()
{
    setCaption( tr( "QT Mix" ) );
    junctionGroupBox->setTitle( tr( "Junction" ) );
    textLabel1_2->setText( tr( "IV data file:" ) );
    textLabel2_4->setText( tr( "KK data file:" ) );
    textLabel2_4_2->setText( tr( "Normal Resistance:" ) );
    RnSpinBox->setSuffix( tr( " Ohm" ) );
    CapSpinBox->setSuffix( tr( " fF" ) );
    textLabel2_4_2_2->setText( tr( "Junction Capacitance" ) );
    embImpGroupBox->setTitle( tr( "Embedding Impedance" ) );
    textLabel2_2_4->setText( tr( "IF:" ) );
    textLabel3_2_4->setText( tr( "+" ) );
    imagIfEmbImpDoubleSpinBox->setPrefix( QString::null );
    imagIfEmbImpDoubleSpinBox->setSuffix( tr( "i" ) );
    textLabel2_2->setText( tr( "Fundamental:" ) );
    textLabel3_2->setText( tr( "+" ) );
    imag1EmbImpDoubleSpinBox->setPrefix( QString::null );
    imag1EmbImpDoubleSpinBox->setSuffix( tr( "i" ) );
    textLabel2_2_2->setText( tr( "1st Harmonic:" ) );
    textLabel3_2_2->setText( tr( "+" ) );
    imag2EmbImpDoubleSpinBox->setPrefix( QString::null );
    imag2EmbImpDoubleSpinBox->setSuffix( tr( "i" ) );
    textLabel2_2_3->setText( tr( "2nd Harmonic:" ) );
    textLabel3_2_3->setText( tr( "+" ) );
    imag3EmbImpDoubleSpinBox->setPrefix( QString::null );
    imag3EmbImpDoubleSpinBox->setSuffix( tr( "i" ) );
    opPointGroupBox->setTitle( tr( "Operating Point" ) );
    textLabel5->setText( tr( "Bias:" ) );
    textLabel4_2->setText( tr( "LO Frequency:" ) );
    textLabel6->setText( tr( "LO Source Voltage:" ) );
    textLabel8->setText( tr( "IF Frequency:" ) );
    simulationGroupBox->setTitle( tr( "Simulation" ) );
    textLabel11->setText( tr( "No. of harmonics to include:" ) );
    resultsGroupBox->setTitle( tr( "Results" ) );
    textLabel1_3->setText( tr( "Unpumped DC Current:" ) );
    textLabel2_3->setText( tr( "Pumped DC Current:" ) );
    textLabel3_3->setText( tr( "Mixer Gain (USB):" ) );
    textLabel3_3_2->setText( tr( "Mixer Gain (LSB):" ) );
    textLabel4_3->setText( tr( "IF Output:" ) );
    textLabel4_3_2->setText( tr( "Mixer Noise (K):" ) );
    textLabel4_3_3->setText( tr( "LO Current:" ) );
    resetPushButton->setText( tr( "&Reset" ) );
    calculatePushButton->setText( tr( "Calculate" ) );
    quitPushButton->setText( tr( "&Quit" ) );
    plotControlsGroupBox->setTitle( tr( "Plot Controls" ) );
    textLabel1->setText( tr( "Plot from:" ) );
    plotLowDoubleSpinBox->setSuffix( QString::null );
    textLabel2->setText( tr( "to:" ) );
    plotHighDoubleSpinBox->setSuffix( QString::null );
    plotRedrawPushButton->setText( tr( "Re&draw" ) );
    saveDataPushButton->setText( tr( "&Save" ) );
    textLabel7->setText( tr( "X axis:" ) );
    textLabel3->setText( tr( "Left axis:" ) );
    textLabel4->setText( tr( "Right axis:" ) );
}

