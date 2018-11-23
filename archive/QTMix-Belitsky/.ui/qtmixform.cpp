/****************************************************************************
** Form implementation generated from reading ui file 'qtmixform.ui'
**
** Created: Wed Feb 9 12:53:30 2005
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.3   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "qtmixform.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qsplitter.h>
#include <qframe.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <qgroupbox.h>
#include <qdoublespinbox.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <filechooser.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qwt_plot.h>
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
    QTMixFormLayout = new QHBoxLayout( this, 2, 6, "QTMixFormLayout"); 

    splitter3 = new QSplitter( this, "splitter3" );
    splitter3->setOrientation( QSplitter::Horizontal );

    frame4 = new QFrame( splitter3, "frame4" );
    frame4->setMinimumSize( QSize( 0, 0 ) );
    frame4->setFrameShape( QFrame::StyledPanel );
    frame4->setFrameShadow( QFrame::Raised );
    frame4Layout = new QVBoxLayout( frame4, 6, 6, "frame4Layout"); 

    tabWidget2 = new QTabWidget( frame4, "tabWidget2" );

    globalsTab = new QWidget( tabWidget2, "globalsTab" );
    globalsTabLayout = new QVBoxLayout( globalsTab, 6, 6, "globalsTabLayout"); 

    simulationGroupBox = new QGroupBox( globalsTab, "simulationGroupBox" );
    simulationGroupBox->setColumnLayout(0, Qt::Vertical );
    simulationGroupBox->layout()->setSpacing( 6 );
    simulationGroupBox->layout()->setMargin( 6 );
    simulationGroupBoxLayout = new QGridLayout( simulationGroupBox->layout() );
    simulationGroupBoxLayout->setAlignment( Qt::AlignTop );
    spacer29 = new QSpacerItem( 111, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    simulationGroupBoxLayout->addItem( spacer29, 1, 2 );
    spacer30 = new QSpacerItem( 51, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    simulationGroupBoxLayout->addMultiCell( spacer30, 0, 0, 1, 2 );

    tDoubleSpinBox = new QDoubleSpinBox( simulationGroupBox, "tDoubleSpinBox" );
    tDoubleSpinBox->setLineStep( 0.1 );
    tDoubleSpinBox->setValue( 4.2 );
    tDoubleSpinBox->setDisplayPrecision( 2 );

    simulationGroupBoxLayout->addMultiCellWidget( tDoubleSpinBox, 0, 0, 3, 4 );

    textLabel6_2 = new QLabel( simulationGroupBox, "textLabel6_2" );

    simulationGroupBoxLayout->addWidget( textLabel6_2, 0, 0 );

    textLabel5_2 = new QLabel( simulationGroupBox, "textLabel5_2" );

    simulationGroupBoxLayout->addWidget( textLabel5_2, 1, 0 );

    harmonicsSpinBox = new QSpinBox( simulationGroupBox, "harmonicsSpinBox" );
    harmonicsSpinBox->setMaxValue( 3 );
    harmonicsSpinBox->setMinValue( 1 );

    simulationGroupBoxLayout->addWidget( harmonicsSpinBox, 3, 4 );
    spacer27 = new QSpacerItem( 134, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    simulationGroupBoxLayout->addMultiCell( spacer27, 3, 3, 2, 3 );

    textLabel11 = new QLabel( simulationGroupBox, "textLabel11" );

    simulationGroupBoxLayout->addMultiCellWidget( textLabel11, 3, 3, 0, 1 );

    textLabel1_5 = new QLabel( simulationGroupBox, "textLabel1_5" );

    simulationGroupBoxLayout->addWidget( textLabel1_5, 2, 0 );
    spacer26_2 = new QSpacerItem( 101, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    simulationGroupBoxLayout->addMultiCell( spacer26_2, 2, 2, 1, 2 );

    ifImpDoubleSpinBox = new QDoubleSpinBox( simulationGroupBox, "ifImpDoubleSpinBox" );
    ifImpDoubleSpinBox->setMaxValue( 999 );
    ifImpDoubleSpinBox->setMinValue( 1 );
    ifImpDoubleSpinBox->setLineStep( 10 );
    ifImpDoubleSpinBox->setValue( 50 );
    ifImpDoubleSpinBox->setDisplayPrecision( 1 );

    simulationGroupBoxLayout->addMultiCellWidget( ifImpDoubleSpinBox, 2, 2, 3, 4 );

    z0DoubleSpinBox = new QDoubleSpinBox( simulationGroupBox, "z0DoubleSpinBox" );
    z0DoubleSpinBox->setMaxValue( 999 );
    z0DoubleSpinBox->setMinValue( 1 );
    z0DoubleSpinBox->setValue( 20 );
    z0DoubleSpinBox->setDisplayPrecision( 1 );

    simulationGroupBoxLayout->addMultiCellWidget( z0DoubleSpinBox, 1, 1, 3, 4 );
    globalsTabLayout->addWidget( simulationGroupBox );

    opPointGroupBox = new QGroupBox( globalsTab, "opPointGroupBox" );
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

    biasDoubleSpinBox = new QDoubleSpinBox( opPointGroupBox, "biasDoubleSpinBox" );
    biasDoubleSpinBox->setMaxValue( 10 );
    biasDoubleSpinBox->setLineStep( 0.1 );
    biasDoubleSpinBox->setValue( 2 );
    biasDoubleSpinBox->setDisplayPrecision( 2 );

    opPointGroupBoxLayout->addWidget( biasDoubleSpinBox, 0, 3 );
    spacer24 = new QSpacerItem( 90, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    opPointGroupBoxLayout->addMultiCell( spacer24, 5, 5, 1, 2 );

    textLabel8 = new QLabel( opPointGroupBox, "textLabel8" );

    opPointGroupBoxLayout->addWidget( textLabel8, 5, 0 );

    textLabel3_4 = new QLabel( opPointGroupBox, "textLabel3_4" );

    opPointGroupBoxLayout->addMultiCellWidget( textLabel3_4, 3, 4, 0, 1 );
    spacer19 = new QSpacerItem( 16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    opPointGroupBoxLayout->addItem( spacer19, 3, 2 );

    freqDoubleSpinBox = new QDoubleSpinBox( opPointGroupBox, "freqDoubleSpinBox" );
    freqDoubleSpinBox->setMaxValue( 2000 );
    freqDoubleSpinBox->setMinValue( 50 );
    freqDoubleSpinBox->setLineStep( 5 );
    freqDoubleSpinBox->setValue( 650 );
    freqDoubleSpinBox->setDisplayPrecision( 0 );

    opPointGroupBoxLayout->addWidget( freqDoubleSpinBox, 1, 3 );

    loPowerDoubleSpinBox = new QDoubleSpinBox( opPointGroupBox, "loPowerDoubleSpinBox" );
    loPowerDoubleSpinBox->setMaxValue( 9999 );
    loPowerDoubleSpinBox->setLineStep( 1 );
    loPowerDoubleSpinBox->setValue( 100 );
    loPowerDoubleSpinBox->setDisplayPrecision( 0 );

    opPointGroupBoxLayout->addWidget( loPowerDoubleSpinBox, 2, 3 );

    loTempSpinBox = new QSpinBox( opPointGroupBox, "loTempSpinBox" );
    loTempSpinBox->setMaxValue( 999 );

    opPointGroupBoxLayout->addWidget( loTempSpinBox, 3, 3 );

    IFfreqDoubleSpinBox = new QDoubleSpinBox( opPointGroupBox, "IFfreqDoubleSpinBox" );
    IFfreqDoubleSpinBox->setMaxValue( 49.99 );
    IFfreqDoubleSpinBox->setLineStep( 0.5 );
    IFfreqDoubleSpinBox->setValue( 4 );
    IFfreqDoubleSpinBox->setDisplayPrecision( 2 );

    opPointGroupBoxLayout->addMultiCellWidget( IFfreqDoubleSpinBox, 4, 5, 3, 3 );
    globalsTabLayout->addWidget( opPointGroupBox );
    tabWidget2->insertTab( globalsTab, QString("") );

    junctionTab = new QWidget( tabWidget2, "junctionTab" );
    junctionTabLayout = new QVBoxLayout( junctionTab, 6, 6, "junctionTabLayout"); 

    junctionGroupBox = new QGroupBox( junctionTab, "junctionGroupBox" );
    junctionGroupBox->setColumnLayout(0, Qt::Vertical );
    junctionGroupBox->layout()->setSpacing( 6 );
    junctionGroupBox->layout()->setMargin( 6 );
    junctionGroupBoxLayout = new QGridLayout( junctionGroupBox->layout() );
    junctionGroupBoxLayout->setAlignment( Qt::AlignTop );

    textLabel1_2 = new QLabel( junctionGroupBox, "textLabel1_2" );

    junctionGroupBoxLayout->addWidget( textLabel1_2, 0, 0 );

    textLabel2_4 = new QLabel( junctionGroupBox, "textLabel2_4" );

    junctionGroupBoxLayout->addWidget( textLabel2_4, 1, 0 );

    idcFileChooser = new FileChooser( junctionGroupBox, "idcFileChooser" );
    idcFileChooser->setEnabled( TRUE );

    junctionGroupBoxLayout->addWidget( idcFileChooser, 0, 3 );
    spacer18_3 = new QSpacerItem( 60, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    junctionGroupBoxLayout->addMultiCell( spacer18_3, 0, 0, 1, 2 );
    spacer17_3 = new QSpacerItem( 60, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    junctionGroupBoxLayout->addMultiCell( spacer17_3, 1, 1, 1, 2 );

    ikkFileChooser = new FileChooser( junctionGroupBox, "ikkFileChooser" );
    ikkFileChooser->setEnabled( TRUE );

    junctionGroupBoxLayout->addWidget( ikkFileChooser, 1, 3 );

    textLabel1_4 = new QLabel( junctionGroupBox, "textLabel1_4" );

    junctionGroupBoxLayout->addMultiCellWidget( textLabel1_4, 2, 2, 0, 1 );
    spacer23_2 = new QSpacerItem( 31, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    junctionGroupBoxLayout->addItem( spacer23_2, 2, 2 );
    spacer24_2 = new QSpacerItem( 31, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    junctionGroupBoxLayout->addItem( spacer24_2, 3, 2 );

    textLabel2_2 = new QLabel( junctionGroupBox, "textLabel2_2" );

    junctionGroupBoxLayout->addMultiCellWidget( textLabel2_2, 3, 3, 0, 1 );

    textLabel3_2 = new QLabel( junctionGroupBox, "textLabel3_2" );

    junctionGroupBoxLayout->addWidget( textLabel3_2, 4, 0 );
    spacer25 = new QSpacerItem( 41, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    junctionGroupBoxLayout->addItem( spacer25, 4, 2 );

    rnJuncDoubleSpinBox = new QDoubleSpinBox( junctionGroupBox, "rnJuncDoubleSpinBox" );
    rnJuncDoubleSpinBox->setMinValue( 1 );
    rnJuncDoubleSpinBox->setValue( 10 );
    rnJuncDoubleSpinBox->setDisplayPrecision( 1 );

    junctionGroupBoxLayout->addWidget( rnJuncDoubleSpinBox, 2, 3 );

    capJuncDoubleSpinBox = new QDoubleSpinBox( junctionGroupBox, "capJuncDoubleSpinBox" );
    capJuncDoubleSpinBox->setAcceptDrops( FALSE );
    capJuncDoubleSpinBox->setMaxValue( 1999 );
    capJuncDoubleSpinBox->setValue( 65 );
    capJuncDoubleSpinBox->setDisplayPrecision( 1 );

    junctionGroupBoxLayout->addWidget( capJuncDoubleSpinBox, 3, 3 );

    vGapJuncDoubleSpinBox = new QDoubleSpinBox( junctionGroupBox, "vGapJuncDoubleSpinBox" );
    vGapJuncDoubleSpinBox->setMaxValue( 9 );
    vGapJuncDoubleSpinBox->setMinValue( 0.01 );
    vGapJuncDoubleSpinBox->setLineStep( 0.01 );
    vGapJuncDoubleSpinBox->setValue( 2.81 );
    vGapJuncDoubleSpinBox->setDisplayPrecision( 2 );

    junctionGroupBoxLayout->addWidget( vGapJuncDoubleSpinBox, 4, 3 );
    junctionTabLayout->addWidget( junctionGroupBox );
    spacer28_2 = new QSpacerItem( 20, 101, QSizePolicy::Minimum, QSizePolicy::Expanding );
    junctionTabLayout->addItem( spacer28_2 );
    tabWidget2->insertTab( junctionTab, QString("") );

    materialsTab = new QWidget( tabWidget2, "materialsTab" );
    materialsTabLayout = new QVBoxLayout( materialsTab, 6, 6, "materialsTabLayout"); 

    substrateGroupBox = new QGroupBox( materialsTab, "substrateGroupBox" );
    substrateGroupBox->setColumnLayout(0, Qt::Vertical );
    substrateGroupBox->layout()->setSpacing( 6 );
    substrateGroupBox->layout()->setMargin( 6 );
    substrateGroupBoxLayout = new QGridLayout( substrateGroupBox->layout() );
    substrateGroupBoxLayout->setAlignment( Qt::AlignTop );

    textLabel1_6 = new QLabel( substrateGroupBox, "textLabel1_6" );

    substrateGroupBoxLayout->addWidget( textLabel1_6, 0, 0 );

    textLabel2_5 = new QLabel( substrateGroupBox, "textLabel2_5" );

    substrateGroupBoxLayout->addWidget( textLabel2_5, 1, 0 );
    spacer29_2 = new QSpacerItem( 71, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    substrateGroupBoxLayout->addItem( spacer29_2, 0, 1 );
    spacer30_2 = new QSpacerItem( 71, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    substrateGroupBoxLayout->addItem( spacer30_2, 1, 1 );

    epsSubstrateDoubleSpinBox = new QDoubleSpinBox( substrateGroupBox, "epsSubstrateDoubleSpinBox" );
    epsSubstrateDoubleSpinBox->setMinValue( 1 );
    epsSubstrateDoubleSpinBox->setLineStep( 0.1 );
    epsSubstrateDoubleSpinBox->setValue( 5.8 );
    epsSubstrateDoubleSpinBox->setDisplayPrecision( 1 );

    substrateGroupBoxLayout->addWidget( epsSubstrateDoubleSpinBox, 0, 2 );

    tandSubstrateDoubleSpinBox = new QDoubleSpinBox( substrateGroupBox, "tandSubstrateDoubleSpinBox" );
    tandSubstrateDoubleSpinBox->setMaxValue( 9999 );
    tandSubstrateDoubleSpinBox->setValue( 4 );
    tandSubstrateDoubleSpinBox->setDisplayPrecision( 1 );

    substrateGroupBoxLayout->addWidget( tandSubstrateDoubleSpinBox, 1, 2 );
    materialsTabLayout->addWidget( substrateGroupBox );

    scMaterialGroupBox = new QGroupBox( materialsTab, "scMaterialGroupBox" );
    scMaterialGroupBox->setColumnLayout(0, Qt::Vertical );
    scMaterialGroupBox->layout()->setSpacing( 6 );
    scMaterialGroupBox->layout()->setMargin( 6 );
    scMaterialGroupBoxLayout = new QGridLayout( scMaterialGroupBox->layout() );
    scMaterialGroupBoxLayout->setAlignment( Qt::AlignTop );

    textLabel3_5 = new QLabel( scMaterialGroupBox, "textLabel3_5" );

    scMaterialGroupBoxLayout->addMultiCellWidget( textLabel3_5, 0, 0, 0, 1 );

    textLabel4_5 = new QLabel( scMaterialGroupBox, "textLabel4_5" );

    scMaterialGroupBoxLayout->addMultiCellWidget( textLabel4_5, 1, 1, 0, 1 );

    textLabel5_3 = new QLabel( scMaterialGroupBox, "textLabel5_3" );

    scMaterialGroupBoxLayout->addMultiCellWidget( textLabel5_3, 2, 2, 0, 1 );
    spacer31 = new QSpacerItem( 41, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    scMaterialGroupBoxLayout->addItem( spacer31, 0, 2 );
    spacer32 = new QSpacerItem( 41, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    scMaterialGroupBoxLayout->addItem( spacer32, 1, 2 );
    spacer33 = new QSpacerItem( 41, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    scMaterialGroupBoxLayout->addItem( spacer33, 2, 2 );

    rhoScMetalDoubleSpinBox = new QDoubleSpinBox( scMaterialGroupBox, "rhoScMetalDoubleSpinBox" );
    rhoScMetalDoubleSpinBox->setValue( 5 );
    rhoScMetalDoubleSpinBox->setDisplayPrecision( 2 );

    scMaterialGroupBoxLayout->addWidget( rhoScMetalDoubleSpinBox, 2, 3 );

    vGapScMetalDoubleSpinBox = new QDoubleSpinBox( scMaterialGroupBox, "vGapScMetalDoubleSpinBox" );
    vGapScMetalDoubleSpinBox->setLineStep( 0.1 );
    vGapScMetalDoubleSpinBox->setValue( 2.85 );
    vGapScMetalDoubleSpinBox->setDisplayPrecision( 2 );

    scMaterialGroupBoxLayout->addWidget( vGapScMetalDoubleSpinBox, 1, 3 );

    tcScMetalDoubleSpinBox = new QDoubleSpinBox( scMaterialGroupBox, "tcScMetalDoubleSpinBox" );
    tcScMetalDoubleSpinBox->setLineStep( 0.1 );
    tcScMetalDoubleSpinBox->setValue( 9.23 );
    tcScMetalDoubleSpinBox->setDisplayPrecision( 2 );

    scMaterialGroupBoxLayout->addWidget( tcScMetalDoubleSpinBox, 0, 3 );

    textLabel12_2 = new QLabel( scMaterialGroupBox, "textLabel12_2" );

    scMaterialGroupBoxLayout->addWidget( textLabel12_2, 3, 0 );
    spacer38_2 = new QSpacerItem( 140, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    scMaterialGroupBoxLayout->addMultiCell( spacer38_2, 3, 3, 1, 2 );

    thickScMetalDoubleSpinBox = new QDoubleSpinBox( scMaterialGroupBox, "thickScMetalDoubleSpinBox" );
    thickScMetalDoubleSpinBox->setLineStep( 0.1 );
    thickScMetalDoubleSpinBox->setValue( 0.4 );
    thickScMetalDoubleSpinBox->setDisplayPrecision( 2 );

    scMaterialGroupBoxLayout->addWidget( thickScMetalDoubleSpinBox, 3, 3 );
    materialsTabLayout->addWidget( scMaterialGroupBox );

    normMetalGroupBox = new QGroupBox( materialsTab, "normMetalGroupBox" );
    normMetalGroupBox->setColumnLayout(0, Qt::Vertical );
    normMetalGroupBox->layout()->setSpacing( 6 );
    normMetalGroupBox->layout()->setMargin( 6 );
    normMetalGroupBoxLayout = new QGridLayout( normMetalGroupBox->layout() );
    normMetalGroupBoxLayout->setAlignment( Qt::AlignTop );

    textLabel6_3 = new QLabel( normMetalGroupBox, "textLabel6_3" );

    normMetalGroupBoxLayout->addWidget( textLabel6_3, 0, 0 );

    textLabel12 = new QLabel( normMetalGroupBox, "textLabel12" );

    normMetalGroupBoxLayout->addWidget( textLabel12, 1, 0 );
    spacer38 = new QSpacerItem( 170, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    normMetalGroupBoxLayout->addItem( spacer38, 1, 1 );
    spacer34 = new QSpacerItem( 170, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    normMetalGroupBoxLayout->addItem( spacer34, 0, 1 );

    rhoNormMetalDoubleSpinBox = new QDoubleSpinBox( normMetalGroupBox, "rhoNormMetalDoubleSpinBox" );
    rhoNormMetalDoubleSpinBox->setValue( 2.69 );
    rhoNormMetalDoubleSpinBox->setDisplayPrecision( 2 );

    normMetalGroupBoxLayout->addWidget( rhoNormMetalDoubleSpinBox, 0, 2 );

    thickNormMetalDoubleSpinBox = new QDoubleSpinBox( normMetalGroupBox, "thickNormMetalDoubleSpinBox" );
    thickNormMetalDoubleSpinBox->setLineStep( 0.1 );
    thickNormMetalDoubleSpinBox->setValue( 0.4 );
    thickNormMetalDoubleSpinBox->setDisplayPrecision( 2 );

    normMetalGroupBoxLayout->addWidget( thickNormMetalDoubleSpinBox, 1, 2 );
    materialsTabLayout->addWidget( normMetalGroupBox );
    tabWidget2->insertTab( materialsTab, QString("") );

    TabPage = new QWidget( tabWidget2, "TabPage" );
    TabPageLayout = new QVBoxLayout( TabPage, 6, 6, "TabPageLayout"); 

    inputGroupBox = new QGroupBox( TabPage, "inputGroupBox" );
    inputGroupBox->setColumnLayout(0, Qt::Vertical );
    inputGroupBox->layout()->setSpacing( 6 );
    inputGroupBox->layout()->setMargin( 6 );
    inputGroupBoxLayout = new QHBoxLayout( inputGroupBox->layout() );
    inputGroupBoxLayout->setAlignment( Qt::AlignTop );

    layout14 = new QVBoxLayout( 0, 0, 6, "layout14"); 

    layout12 = new QVBoxLayout( 0, 0, 6, "layout12"); 

    textLabel6_4 = new QLabel( inputGroupBox, "textLabel6_4" );
    layout12->addWidget( textLabel6_4 );

    topInputComboBox = new QComboBox( FALSE, inputGroupBox, "topInputComboBox" );
    topInputComboBox->setMaxCount( 10 );
    layout12->addWidget( topInputComboBox );
    layout14->addLayout( layout12 );

    layout11 = new QVBoxLayout( 0, 0, 6, "layout11"); 

    textLabel7_3 = new QLabel( inputGroupBox, "textLabel7_3" );
    layout11->addWidget( textLabel7_3 );

    groundInputComboBox = new QComboBox( FALSE, inputGroupBox, "groundInputComboBox" );
    layout11->addWidget( groundInputComboBox );
    layout14->addLayout( layout11 );
    spacer43 = new QSpacerItem( 20, 31, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout14->addItem( spacer43 );
    inputGroupBoxLayout->addLayout( layout14 );

    layout40 = new QVBoxLayout( 0, 0, 6, "layout40"); 

    textLabel1_7 = new QLabel( inputGroupBox, "textLabel1_7" );
    textLabel1_7->setMinimumSize( QSize( 70, 21 ) );
    textLabel1_7->setMaximumSize( QSize( 70, 21 ) );
    textLabel1_7->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
    layout40->addWidget( textLabel1_7 );

    textLabel2_6 = new QLabel( inputGroupBox, "textLabel2_6" );
    textLabel2_6->setMinimumSize( QSize( 70, 21 ) );
    textLabel2_6->setMaximumSize( QSize( 70, 21 ) );
    textLabel2_6->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
    layout40->addWidget( textLabel2_6 );

    textLabel3_6 = new QLabel( inputGroupBox, "textLabel3_6" );
    textLabel3_6->setMinimumSize( QSize( 70, 21 ) );
    textLabel3_6->setMaximumSize( QSize( 70, 21 ) );
    textLabel3_6->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
    layout40->addWidget( textLabel3_6 );

    textLabel4_4 = new QLabel( inputGroupBox, "textLabel4_4" );
    textLabel4_4->setMinimumSize( QSize( 70, 21 ) );
    textLabel4_4->setMaximumSize( QSize( 70, 21 ) );
    textLabel4_4->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
    layout40->addWidget( textLabel4_4 );

    textLabel5_4 = new QLabel( inputGroupBox, "textLabel5_4" );
    textLabel5_4->setMinimumSize( QSize( 70, 21 ) );
    textLabel5_4->setMaximumSize( QSize( 70, 21 ) );
    textLabel5_4->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
    layout40->addWidget( textLabel5_4 );
    inputGroupBoxLayout->addLayout( layout40 );

    layout38 = new QVBoxLayout( 0, 0, 6, "layout38"); 

    widthInputDoubleSpinBox = new QDoubleSpinBox( inputGroupBox, "widthInputDoubleSpinBox" );
    widthInputDoubleSpinBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, widthInputDoubleSpinBox->sizePolicy().hasHeightForWidth() ) );
    widthInputDoubleSpinBox->setMinimumSize( QSize( 80, 21 ) );
    widthInputDoubleSpinBox->setMaximumSize( QSize( 80, 21 ) );
    widthInputDoubleSpinBox->setMinValue( 0.5 );
    widthInputDoubleSpinBox->setValue( 7 );
    widthInputDoubleSpinBox->setDisplayPrecision( 1 );
    layout38->addWidget( widthInputDoubleSpinBox );

    lengthInputDoubleSpinBox = new QDoubleSpinBox( inputGroupBox, "lengthInputDoubleSpinBox" );
    lengthInputDoubleSpinBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, lengthInputDoubleSpinBox->sizePolicy().hasHeightForWidth() ) );
    lengthInputDoubleSpinBox->setMinimumSize( QSize( 80, 21 ) );
    lengthInputDoubleSpinBox->setMaximumSize( QSize( 80, 21 ) );
    lengthInputDoubleSpinBox->setMaxValue( 1000 );
    lengthInputDoubleSpinBox->setMinValue( 1 );
    lengthInputDoubleSpinBox->setLineStep( 1 );
    lengthInputDoubleSpinBox->setValue( 48.5 );
    lengthInputDoubleSpinBox->setDisplayPrecision( 1 );
    layout38->addWidget( lengthInputDoubleSpinBox );

    heightInputDoubleSpinBox = new QDoubleSpinBox( inputGroupBox, "heightInputDoubleSpinBox" );
    heightInputDoubleSpinBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, heightInputDoubleSpinBox->sizePolicy().hasHeightForWidth() ) );
    heightInputDoubleSpinBox->setMinimumSize( QSize( 80, 21 ) );
    heightInputDoubleSpinBox->setMaximumSize( QSize( 80, 21 ) );
    heightInputDoubleSpinBox->setMinValue( 0.1 );
    heightInputDoubleSpinBox->setLineStep( 0.1 );
    heightInputDoubleSpinBox->setValue( 0.42 );
    heightInputDoubleSpinBox->setDisplayPrecision( 2 );
    layout38->addWidget( heightInputDoubleSpinBox );

    z0InputLineEdit = new QLineEdit( inputGroupBox, "z0InputLineEdit" );
    z0InputLineEdit->setMinimumSize( QSize( 80, 21 ) );
    z0InputLineEdit->setMaximumSize( QSize( 80, 21 ) );
    z0InputLineEdit->setReadOnly( TRUE );
    layout38->addWidget( z0InputLineEdit );

    phaseInputLineEdit = new QLineEdit( inputGroupBox, "phaseInputLineEdit" );
    phaseInputLineEdit->setMinimumSize( QSize( 80, 21 ) );
    phaseInputLineEdit->setMaximumSize( QSize( 80, 21 ) );
    phaseInputLineEdit->setReadOnly( TRUE );
    layout38->addWidget( phaseInputLineEdit );
    inputGroupBoxLayout->addLayout( layout38 );
    TabPageLayout->addWidget( inputGroupBox );

    tuningGroupBox = new QGroupBox( TabPage, "tuningGroupBox" );
    tuningGroupBox->setColumnLayout(0, Qt::Vertical );
    tuningGroupBox->layout()->setSpacing( 6 );
    tuningGroupBox->layout()->setMargin( 6 );
    tuningGroupBoxLayout = new QHBoxLayout( tuningGroupBox->layout() );
    tuningGroupBoxLayout->setAlignment( Qt::AlignTop );

    layout14_2 = new QVBoxLayout( 0, 0, 6, "layout14_2"); 

    layout12_2 = new QVBoxLayout( 0, 0, 6, "layout12_2"); 

    textLabel6_4_2 = new QLabel( tuningGroupBox, "textLabel6_4_2" );
    layout12_2->addWidget( textLabel6_4_2 );

    topTuningComboBox = new QComboBox( FALSE, tuningGroupBox, "topTuningComboBox" );
    topTuningComboBox->setMaxCount( 10 );
    layout12_2->addWidget( topTuningComboBox );
    layout14_2->addLayout( layout12_2 );

    layout11_2 = new QVBoxLayout( 0, 0, 6, "layout11_2"); 

    textLabel7_3_2 = new QLabel( tuningGroupBox, "textLabel7_3_2" );
    layout11_2->addWidget( textLabel7_3_2 );

    groundTuningComboBox = new QComboBox( FALSE, tuningGroupBox, "groundTuningComboBox" );
    layout11_2->addWidget( groundTuningComboBox );
    layout14_2->addLayout( layout11_2 );
    spacer43_2 = new QSpacerItem( 20, 31, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout14_2->addItem( spacer43_2 );
    tuningGroupBoxLayout->addLayout( layout14_2 );

    layout42 = new QVBoxLayout( 0, 0, 6, "layout42"); 

    textLabel1_7_2 = new QLabel( tuningGroupBox, "textLabel1_7_2" );
    textLabel1_7_2->setMinimumSize( QSize( 70, 21 ) );
    textLabel1_7_2->setMaximumSize( QSize( 70, 21 ) );
    textLabel1_7_2->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
    layout42->addWidget( textLabel1_7_2 );

    textLabel2_6_2 = new QLabel( tuningGroupBox, "textLabel2_6_2" );
    textLabel2_6_2->setMinimumSize( QSize( 70, 21 ) );
    textLabel2_6_2->setMaximumSize( QSize( 70, 21 ) );
    textLabel2_6_2->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
    layout42->addWidget( textLabel2_6_2 );

    textLabel3_6_2 = new QLabel( tuningGroupBox, "textLabel3_6_2" );
    textLabel3_6_2->setMinimumSize( QSize( 70, 21 ) );
    textLabel3_6_2->setMaximumSize( QSize( 70, 21 ) );
    textLabel3_6_2->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
    layout42->addWidget( textLabel3_6_2 );

    textLabel4_4_2 = new QLabel( tuningGroupBox, "textLabel4_4_2" );
    textLabel4_4_2->setMinimumSize( QSize( 70, 21 ) );
    textLabel4_4_2->setMaximumSize( QSize( 70, 21 ) );
    textLabel4_4_2->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
    layout42->addWidget( textLabel4_4_2 );

    textLabel5_4_2 = new QLabel( tuningGroupBox, "textLabel5_4_2" );
    textLabel5_4_2->setMinimumSize( QSize( 70, 21 ) );
    textLabel5_4_2->setMaximumSize( QSize( 70, 21 ) );
    textLabel5_4_2->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
    layout42->addWidget( textLabel5_4_2 );
    tuningGroupBoxLayout->addLayout( layout42 );

    layout41 = new QVBoxLayout( 0, 0, 6, "layout41"); 

    widthTuningDoubleSpinBox = new QDoubleSpinBox( tuningGroupBox, "widthTuningDoubleSpinBox" );
    widthTuningDoubleSpinBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, widthTuningDoubleSpinBox->sizePolicy().hasHeightForWidth() ) );
    widthTuningDoubleSpinBox->setMinimumSize( QSize( 80, 21 ) );
    widthTuningDoubleSpinBox->setMaximumSize( QSize( 80, 21 ) );
    widthTuningDoubleSpinBox->setMinValue( 0.5 );
    widthTuningDoubleSpinBox->setValue( 5 );
    widthTuningDoubleSpinBox->setDisplayPrecision( 1 );
    layout41->addWidget( widthTuningDoubleSpinBox );

    lengthTuningDoubleSpinBox = new QDoubleSpinBox( tuningGroupBox, "lengthTuningDoubleSpinBox" );
    lengthTuningDoubleSpinBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, lengthTuningDoubleSpinBox->sizePolicy().hasHeightForWidth() ) );
    lengthTuningDoubleSpinBox->setMinimumSize( QSize( 80, 21 ) );
    lengthTuningDoubleSpinBox->setMaximumSize( QSize( 80, 21 ) );
    lengthTuningDoubleSpinBox->setMaxValue( 1000 );
    lengthTuningDoubleSpinBox->setMinValue( 1 );
    lengthTuningDoubleSpinBox->setLineStep( 1 );
    lengthTuningDoubleSpinBox->setValue( 16 );
    lengthTuningDoubleSpinBox->setDisplayPrecision( 1 );
    layout41->addWidget( lengthTuningDoubleSpinBox );

    heightTuningDoubleSpinBox = new QDoubleSpinBox( tuningGroupBox, "heightTuningDoubleSpinBox" );
    heightTuningDoubleSpinBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, heightTuningDoubleSpinBox->sizePolicy().hasHeightForWidth() ) );
    heightTuningDoubleSpinBox->setMinimumSize( QSize( 80, 21 ) );
    heightTuningDoubleSpinBox->setMaximumSize( QSize( 80, 21 ) );
    heightTuningDoubleSpinBox->setMinValue( 0.1 );
    heightTuningDoubleSpinBox->setLineStep( 0.1 );
    heightTuningDoubleSpinBox->setValue( 0.2 );
    heightTuningDoubleSpinBox->setDisplayPrecision( 2 );
    layout41->addWidget( heightTuningDoubleSpinBox );

    z0TuningLineEdit = new QLineEdit( tuningGroupBox, "z0TuningLineEdit" );
    z0TuningLineEdit->setMinimumSize( QSize( 80, 21 ) );
    z0TuningLineEdit->setMaximumSize( QSize( 80, 21 ) );
    z0TuningLineEdit->setReadOnly( TRUE );
    layout41->addWidget( z0TuningLineEdit );

    phaseTuningLineEdit = new QLineEdit( tuningGroupBox, "phaseTuningLineEdit" );
    phaseTuningLineEdit->setMinimumSize( QSize( 80, 21 ) );
    phaseTuningLineEdit->setMaximumSize( QSize( 80, 21 ) );
    phaseTuningLineEdit->setReadOnly( TRUE );
    layout41->addWidget( phaseTuningLineEdit );
    tuningGroupBoxLayout->addLayout( layout41 );
    TabPageLayout->addWidget( tuningGroupBox );
    tabWidget2->insertTab( TabPage, QString("") );

    tunerTab = new QWidget( tabWidget2, "tunerTab" );
    tunerTabLayout = new QVBoxLayout( tunerTab, 6, 6, "tunerTabLayout"); 

    rfChokeCheckBox = new QCheckBox( tunerTab, "rfChokeCheckBox" );
    rfChokeCheckBox->setChecked( TRUE );
    tunerTabLayout->addWidget( rfChokeCheckBox );

    rfChokeGroupBox = new QGroupBox( tunerTab, "rfChokeGroupBox" );
    rfChokeGroupBox->setColumnLayout(0, Qt::Vertical );
    rfChokeGroupBox->layout()->setSpacing( 6 );
    rfChokeGroupBox->layout()->setMargin( 6 );
    rfChokeGroupBoxLayout = new QGridLayout( rfChokeGroupBox->layout() );
    rfChokeGroupBoxLayout->setAlignment( Qt::AlignTop );

    layout49 = new QHBoxLayout( 0, 0, 6, "layout49"); 

    textLabel5_4_2_2 = new QLabel( rfChokeGroupBox, "textLabel5_4_2_2" );
    textLabel5_4_2_2->setMinimumSize( QSize( 70, 21 ) );
    textLabel5_4_2_2->setMaximumSize( QSize( 70, 21 ) );
    textLabel5_4_2_2->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
    layout49->addWidget( textLabel5_4_2_2 );

    phaseRadialLineEdit = new QLineEdit( rfChokeGroupBox, "phaseRadialLineEdit" );
    phaseRadialLineEdit->setReadOnly( TRUE );
    layout49->addWidget( phaseRadialLineEdit );

    rfChokeGroupBoxLayout->addMultiCellLayout( layout49, 8, 8, 0, 3 );

    layout48 = new QHBoxLayout( 0, 0, 6, "layout48"); 

    textLabel4_4_2_2 = new QLabel( rfChokeGroupBox, "textLabel4_4_2_2" );
    textLabel4_4_2_2->setMinimumSize( QSize( 70, 21 ) );
    textLabel4_4_2_2->setMaximumSize( QSize( 70, 21 ) );
    textLabel4_4_2_2->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
    layout48->addWidget( textLabel4_4_2_2 );

    z0RadialLineEdit = new QLineEdit( rfChokeGroupBox, "z0RadialLineEdit" );
    z0RadialLineEdit->setReadOnly( TRUE );
    layout48->addWidget( z0RadialLineEdit );

    rfChokeGroupBoxLayout->addMultiCellLayout( layout48, 7, 7, 0, 3 );

    angleRfChokeDoubleSpinBox = new QDoubleSpinBox( rfChokeGroupBox, "angleRfChokeDoubleSpinBox" );
    angleRfChokeDoubleSpinBox->setMaxValue( 180 );
    angleRfChokeDoubleSpinBox->setMinValue( 30 );
    angleRfChokeDoubleSpinBox->setLineStep( 10 );
    angleRfChokeDoubleSpinBox->setValue( 180 );
    angleRfChokeDoubleSpinBox->setDisplayPrecision( 0 );

    rfChokeGroupBoxLayout->addWidget( angleRfChokeDoubleSpinBox, 6, 3 );

    groundRfChokeComboBox = new QComboBox( FALSE, rfChokeGroupBox, "groundRfChokeComboBox" );
    groundRfChokeComboBox->setMaxCount( 10 );

    rfChokeGroupBoxLayout->addWidget( groundRfChokeComboBox, 1, 3 );

    topRfChokeComboBox = new QComboBox( FALSE, rfChokeGroupBox, "topRfChokeComboBox" );
    topRfChokeComboBox->setMaxCount( 10 );

    rfChokeGroupBoxLayout->addWidget( topRfChokeComboBox, 0, 3 );

    heightRfChokeDoubleSpinBox = new QDoubleSpinBox( rfChokeGroupBox, "heightRfChokeDoubleSpinBox" );
    heightRfChokeDoubleSpinBox->setMaxValue( 99 );
    heightRfChokeDoubleSpinBox->setMinValue( 0.01 );
    heightRfChokeDoubleSpinBox->setValue( 0.42 );
    heightRfChokeDoubleSpinBox->setDisplayPrecision( 2 );

    rfChokeGroupBoxLayout->addWidget( heightRfChokeDoubleSpinBox, 4, 3 );

    lengthRfChokeDoubleSpinBox = new QDoubleSpinBox( rfChokeGroupBox, "lengthRfChokeDoubleSpinBox" );
    lengthRfChokeDoubleSpinBox->setMaxValue( 999 );
    lengthRfChokeDoubleSpinBox->setMinValue( 0.1 );
    lengthRfChokeDoubleSpinBox->setValue( 39 );
    lengthRfChokeDoubleSpinBox->setDisplayPrecision( 1 );

    rfChokeGroupBoxLayout->addWidget( lengthRfChokeDoubleSpinBox, 2, 3 );

    widthRfChokeDoubleSpinBox = new QDoubleSpinBox( rfChokeGroupBox, "widthRfChokeDoubleSpinBox" );
    widthRfChokeDoubleSpinBox->setMaxValue( 999 );
    widthRfChokeDoubleSpinBox->setMinValue( 0.1 );
    widthRfChokeDoubleSpinBox->setValue( 3 );
    widthRfChokeDoubleSpinBox->setDisplayPrecision( 1 );

    rfChokeGroupBoxLayout->addWidget( widthRfChokeDoubleSpinBox, 3, 3 );

    radiusRfChokeDoubleSpinBox = new QDoubleSpinBox( rfChokeGroupBox, "radiusRfChokeDoubleSpinBox" );
    radiusRfChokeDoubleSpinBox->setMaxValue( 999 );
    radiusRfChokeDoubleSpinBox->setMinValue( 0.1 );
    radiusRfChokeDoubleSpinBox->setValue( 30 );
    radiusRfChokeDoubleSpinBox->setDisplayPrecision( 1 );

    rfChokeGroupBoxLayout->addWidget( radiusRfChokeDoubleSpinBox, 5, 3 );
    spacer40_5 = new QSpacerItem( 42, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    rfChokeGroupBoxLayout->addItem( spacer40_5, 6, 2 );
    spacer40_4 = new QSpacerItem( 42, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    rfChokeGroupBoxLayout->addItem( spacer40_4, 5, 2 );
    spacer40_3 = new QSpacerItem( 42, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    rfChokeGroupBoxLayout->addItem( spacer40_3, 4, 2 );
    spacer40_2 = new QSpacerItem( 42, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    rfChokeGroupBoxLayout->addItem( spacer40_2, 3, 2 );
    spacer37 = new QSpacerItem( 42, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    rfChokeGroupBoxLayout->addItem( spacer37, 1, 2 );

    textLabel7_2 = new QLabel( rfChokeGroupBox, "textLabel7_2" );

    rfChokeGroupBoxLayout->addWidget( textLabel7_2, 0, 0 );

    textLabel8_2 = new QLabel( rfChokeGroupBox, "textLabel8_2" );

    rfChokeGroupBoxLayout->addMultiCellWidget( textLabel8_2, 1, 1, 0, 1 );

    textLabel9 = new QLabel( rfChokeGroupBox, "textLabel9" );

    rfChokeGroupBoxLayout->addMultiCellWidget( textLabel9, 3, 3, 0, 1 );

    textLabel11_2 = new QLabel( rfChokeGroupBox, "textLabel11_2" );

    rfChokeGroupBoxLayout->addMultiCellWidget( textLabel11_2, 4, 4, 0, 1 );

    textLabel13 = new QLabel( rfChokeGroupBox, "textLabel13" );

    rfChokeGroupBoxLayout->addMultiCellWidget( textLabel13, 5, 5, 0, 1 );

    textLabel14 = new QLabel( rfChokeGroupBox, "textLabel14" );

    rfChokeGroupBoxLayout->addMultiCellWidget( textLabel14, 6, 6, 0, 1 );
    spacer36 = new QSpacerItem( 50, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    rfChokeGroupBoxLayout->addMultiCell( spacer36, 0, 0, 1, 2 );
    spacer40 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    rfChokeGroupBoxLayout->addItem( spacer40, 2, 2 );

    textLabel10 = new QLabel( rfChokeGroupBox, "textLabel10" );

    rfChokeGroupBoxLayout->addMultiCellWidget( textLabel10, 2, 2, 0, 1 );
    tunerTabLayout->addWidget( rfChokeGroupBox );
    tabWidget2->insertTab( tunerTab, QString("") );
    frame4Layout->addWidget( tabWidget2 );

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

    resultsGroupBoxLayout->addWidget( unpumpLineEdit, 0, 2 );

    textLabel1_3 = new QLabel( resultsGroupBox, "textLabel1_3" );

    resultsGroupBoxLayout->addWidget( textLabel1_3, 0, 0 );

    textLabel2_3 = new QLabel( resultsGroupBox, "textLabel2_3" );

    resultsGroupBoxLayout->addWidget( textLabel2_3, 1, 0 );

    pumpedLineEdit = new QLineEdit( resultsGroupBox, "pumpedLineEdit" );
    pumpedLineEdit->setReadOnly( TRUE );

    resultsGroupBoxLayout->addWidget( pumpedLineEdit, 1, 2 );

    textLabel3_3 = new QLabel( resultsGroupBox, "textLabel3_3" );

    resultsGroupBoxLayout->addWidget( textLabel3_3, 3, 0 );
    spacer15 = new QSpacerItem( 21, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    resultsGroupBoxLayout->addItem( spacer15, 3, 1 );

    textLabel3_3_2 = new QLabel( resultsGroupBox, "textLabel3_3_2" );

    resultsGroupBoxLayout->addWidget( textLabel3_3_2, 2, 0 );

    lsbGainLineEdit = new QLineEdit( resultsGroupBox, "lsbGainLineEdit" );
    lsbGainLineEdit->setReadOnly( TRUE );

    resultsGroupBoxLayout->addWidget( lsbGainLineEdit, 2, 2 );

    usbGainLineEdit = new QLineEdit( resultsGroupBox, "usbGainLineEdit" );
    usbGainLineEdit->setReadOnly( TRUE );

    resultsGroupBoxLayout->addWidget( usbGainLineEdit, 3, 2 );

    textLabel4_3 = new QLabel( resultsGroupBox, "textLabel4_3" );

    resultsGroupBoxLayout->addWidget( textLabel4_3, 4, 0 );
    spacer16_2 = new QSpacerItem( 35, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    resultsGroupBoxLayout->addItem( spacer16_2, 5, 1 );

    textLabel4_3_2 = new QLabel( resultsGroupBox, "textLabel4_3_2" );

    resultsGroupBoxLayout->addWidget( textLabel4_3_2, 5, 0 );
    spacer15_2 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    resultsGroupBoxLayout->addItem( spacer15_2, 2, 1 );

    noiseLineEdit = new QLineEdit( resultsGroupBox, "noiseLineEdit" );
    noiseLineEdit->setReadOnly( TRUE );

    resultsGroupBoxLayout->addWidget( noiseLineEdit, 5, 2 );

    ifOutputLineEdit = new QLineEdit( resultsGroupBox, "ifOutputLineEdit" );
    ifOutputLineEdit->setReadOnly( TRUE );

    resultsGroupBoxLayout->addWidget( ifOutputLineEdit, 4, 2 );
    spacer16 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    resultsGroupBoxLayout->addItem( spacer16, 4, 1 );
    frame4Layout->addWidget( resultsGroupBox );
    spacer17_2 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
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

    frame3 = new QFrame( splitter3, "frame3" );
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
    plotLowDoubleSpinBox->setDisplayPrecision( 3 );
    layout6->addWidget( plotLowDoubleSpinBox );

    textLabel2 = new QLabel( plotControlsGroupBox, "textLabel2" );
    layout6->addWidget( textLabel2 );

    plotHighDoubleSpinBox = new QDoubleSpinBox( plotControlsGroupBox, "plotHighDoubleSpinBox" );
    plotHighDoubleSpinBox->setMaxValue( 10 );
    plotHighDoubleSpinBox->setMinValue( 0 );
    plotHighDoubleSpinBox->setLineStep( 0.1 );
    plotHighDoubleSpinBox->setValue( 5 );
    plotHighDoubleSpinBox->setDisplayPrecision( 3 );
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
    QTMixFormLayout->addWidget( splitter3 );
    languageChange();
    resize( QSize(824, 647).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( quitPushButton, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( calculatePushButton, SIGNAL( clicked() ), this, SLOT( calculate() ) );
    connect( resetPushButton, SIGNAL( clicked() ), this, SLOT( init() ) );
    connect( plotRedrawPushButton, SIGNAL( clicked() ), this, SLOT( redrawPlot() ) );
    connect( plotLeftComboBox, SIGNAL( activated(const QString&) ), this, SLOT( redrawPlot() ) );
    connect( plotRightComboBox, SIGNAL( activated(const QString&) ), this, SLOT( redrawPlot() ) );
    connect( plotLowDoubleSpinBox, SIGNAL( valueChanged(const QString&) ), this, SLOT( setPlotLimits() ) );
    connect( plotHighDoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setPlotLimits() ) );
    connect( saveDataPushButton, SIGNAL( clicked() ), this, SLOT( fileSaveAs() ) );
    connect( plot, SIGNAL( plotMousePressed(const QMouseEvent&) ), this, SLOT( calcOnMouse(const QMouseEvent&) ) );
    connect( freqDoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setOperatingPoint() ) );
    connect( z0DoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setSimParameters() ) );
    connect( IFfreqDoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setOperatingPoint() ) );
    connect( biasDoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setOperatingPoint() ) );
    connect( loPowerDoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setOperatingPoint() ) );
    connect( loTempSpinBox, SIGNAL( valueChanged(int) ), this, SLOT( setOperatingPoint() ) );
    connect( idcFileChooser, SIGNAL( fileNameChanged(const QString&) ), this, SLOT( setIVdata() ) );
    connect( ikkFileChooser, SIGNAL( fileNameChanged(const QString&) ), this, SLOT( setIVdata() ) );
    connect( harmonicsSpinBox, SIGNAL( valueChanged(int) ), this, SLOT( setHarmonics(int) ) );
    connect( capJuncDoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setJunction() ) );
    connect( rnJuncDoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setJunction() ) );
    connect( vGapJuncDoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setJunction() ) );
    connect( ifImpDoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setIFImpedance() ) );
    connect( tDoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setSimParameters() ) );
    connect( plotXAxisComboBox, SIGNAL( activated(const QString&) ), this, SLOT( setPlotXAxis() ) );
    connect( rhoNormMetalDoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setNormMetal() ) );
    connect( thickNormMetalDoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setNormMetal() ) );
    connect( tcScMetalDoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setScMetal() ) );
    connect( vGapScMetalDoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setScMetal() ) );
    connect( rhoScMetalDoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setScMetal() ) );
    connect( thickScMetalDoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setScMetal() ) );
    connect( epsSubstrateDoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setDielectric() ) );
    connect( tandSubstrateDoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setDielectric() ) );
    connect( angleRfChokeDoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setRfChokeDim() ) );
    connect( heightRfChokeDoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setRfChokeDim() ) );
    connect( lengthRfChokeDoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setRfChokeDim() ) );
    connect( radiusRfChokeDoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setRfChokeDim() ) );
    connect( widthRfChokeDoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setRfChokeDim() ) );
    connect( groundRfChokeComboBox, SIGNAL( activated(const QString&) ), this, SLOT( setRfChokeDim() ) );
    connect( topRfChokeComboBox, SIGNAL( activated(const QString&) ), this, SLOT( setRfChokeDim() ) );
    connect( groundInputComboBox, SIGNAL( activated(const QString&) ), this, SLOT( setInputDims() ) );
    connect( topInputComboBox, SIGNAL( activated(const QString&) ), this, SLOT( setInputDims() ) );
    connect( heightInputDoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setInputDims() ) );
    connect( lengthInputDoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setInputDims() ) );
    connect( widthInputDoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setInputDims() ) );
    connect( groundTuningComboBox, SIGNAL( activated(const QString&) ), this, SLOT( setTuningDims() ) );
    connect( topTuningComboBox, SIGNAL( activated(const QString&) ), this, SLOT( setTuningDims() ) );
    connect( heightTuningDoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setTuningDims() ) );
    connect( lengthTuningDoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setTuningDims() ) );
    connect( widthTuningDoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setTuningDims() ) );
    connect( rfChokeCheckBox, SIGNAL( stateChanged(int) ), this, SLOT( setRfChoke() ) );

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
    setCaption( tr( "QT Mix - Belitsky Mixer" ) );
    simulationGroupBox->setTitle( tr( "Simulation" ) );
    tDoubleSpinBox->setSuffix( tr( " K" ) );
    textLabel6_2->setText( tr( "Device Temperature:" ) );
    textLabel5_2->setText( tr( "Normalising Impedance:" ) );
    textLabel11->setText( tr( "No. of harmonics to include:" ) );
    textLabel1_5->setText( tr( "IF Impedance:" ) );
    ifImpDoubleSpinBox->setSuffix( trUtf8( "\x20\xce\xa9" ) );
    z0DoubleSpinBox->setSuffix( trUtf8( "\x20\xce\xa9" ) );
    opPointGroupBox->setTitle( tr( "Operating Point" ) );
    textLabel5->setText( tr( "Bias:" ) );
    textLabel4_2->setText( tr( "LO Frequency:" ) );
    textLabel6->setText( tr( "LO Power:" ) );
    biasDoubleSpinBox->setSuffix( tr( " mV" ) );
    textLabel8->setText( tr( "IF Frequency:" ) );
    textLabel3_4->setText( tr( "LO Noise Temperature:" ) );
    freqDoubleSpinBox->setSuffix( tr( " GHz" ) );
    loPowerDoubleSpinBox->setSuffix( tr( " nW" ) );
    loTempSpinBox->setSuffix( tr( " K" ) );
    IFfreqDoubleSpinBox->setSuffix( tr( " GHz" ) );
    tabWidget2->changeTab( globalsTab, tr( "Globals" ) );
    junctionGroupBox->setTitle( tr( "Junction" ) );
    textLabel1_2->setText( tr( "IV data file:" ) );
    textLabel2_4->setText( tr( "KK data file:" ) );
    textLabel1_4->setText( tr( "Normal Resistance:" ) );
    textLabel2_2->setText( tr( "Capacitance:" ) );
    textLabel3_2->setText( tr( "Gap Voltage:" ) );
    rnJuncDoubleSpinBox->setPrefix( QString::null );
    rnJuncDoubleSpinBox->setSuffix( trUtf8( "\x20\xce\xa9" ) );
    capJuncDoubleSpinBox->setSuffix( tr( " fF" ) );
    vGapJuncDoubleSpinBox->setSuffix( tr( " mV" ) );
    tabWidget2->changeTab( junctionTab, tr( "Junction" ) );
    substrateGroupBox->setTitle( tr( "Substrate" ) );
    textLabel1_6->setText( tr( "Dielectric Constant:" ) );
    textLabel2_5->setText( tr( "Loss Tangent:" ) );
    tandSubstrateDoubleSpinBox->setSuffix( tr( "e-4" ) );
    scMaterialGroupBox->setTitle( tr( "Superconductor" ) );
    textLabel3_5->setText( tr( "Transition Temperature:" ) );
    textLabel4_5->setText( tr( "Gap Voltage:" ) );
    textLabel5_3->setText( tr( "Normal Resistivity:" ) );
    rhoScMetalDoubleSpinBox->setSuffix( trUtf8( "\x20\xce\xbc\xce\xa9\x63\x6d" ) );
    vGapScMetalDoubleSpinBox->setSuffix( tr( " mV" ) );
    tcScMetalDoubleSpinBox->setPrefix( QString::null );
    tcScMetalDoubleSpinBox->setSuffix( tr( " K" ) );
    textLabel12_2->setText( tr( "Thickness:" ) );
    thickScMetalDoubleSpinBox->setSuffix( trUtf8( "\x20\xce\xbc\x6d" ) );
    normMetalGroupBox->setTitle( tr( "Normal Metal" ) );
    textLabel6_3->setText( tr( "Resistivity:" ) );
    textLabel12->setText( tr( "Thickness:" ) );
    rhoNormMetalDoubleSpinBox->setSuffix( trUtf8( "\x20\xce\xbc\xce\xa9\x63\x6d" ) );
    thickNormMetalDoubleSpinBox->setSuffix( trUtf8( "\x20\xce\xbc\x6d" ) );
    tabWidget2->changeTab( materialsTab, tr( "Materials" ) );
    inputGroupBox->setTitle( tr( "Input Strip" ) );
    textLabel6_4->setText( tr( "Top Metal" ) );
    textLabel7_3->setText( tr( "Ground Metal" ) );
    textLabel1_7->setText( tr( "Width : " ) );
    textLabel2_6->setText( tr( "Length : " ) );
    textLabel3_6->setText( tr( "Height : " ) );
    textLabel4_4->setText( tr( "Z0 : " ) );
    textLabel5_4->setText( tr( "Phase : " ) );
    widthInputDoubleSpinBox->setSuffix( trUtf8( "\x20\xce\xbc\x6d" ) );
    lengthInputDoubleSpinBox->setSuffix( trUtf8( "\x20\xce\xbc\x6d" ) );
    heightInputDoubleSpinBox->setSuffix( trUtf8( "\x20\xce\xbc\x6d" ) );
    tuningGroupBox->setTitle( tr( "Tuning Strip" ) );
    textLabel6_4_2->setText( tr( "Top Metal" ) );
    textLabel7_3_2->setText( tr( "Ground Metal" ) );
    textLabel1_7_2->setText( tr( "Width : " ) );
    textLabel2_6_2->setText( tr( "Length : " ) );
    textLabel3_6_2->setText( tr( "Height : " ) );
    textLabel4_4_2->setText( tr( "Z0 : " ) );
    textLabel5_4_2->setText( tr( "Phase : " ) );
    widthTuningDoubleSpinBox->setSuffix( trUtf8( "\x20\xce\xbc\x6d" ) );
    lengthTuningDoubleSpinBox->setSuffix( trUtf8( "\x20\xce\xbc\x6d" ) );
    heightTuningDoubleSpinBox->setSuffix( trUtf8( "\x20\xce\xbc\x6d" ) );
    tabWidget2->changeTab( TabPage, tr( "Tuner" ) );
    rfChokeCheckBox->setText( tr( "Include RF Choke" ) );
    rfChokeGroupBox->setTitle( tr( "RF Choke" ) );
    textLabel5_4_2_2->setText( tr( "Phase : " ) );
    textLabel4_4_2_2->setText( tr( "Z0 : " ) );
    angleRfChokeDoubleSpinBox->setSuffix( trUtf8( "\xc2\xb0" ) );
    heightRfChokeDoubleSpinBox->setSuffix( trUtf8( "\x20\xce\xbc\x6d" ) );
    lengthRfChokeDoubleSpinBox->setSuffix( trUtf8( "\x20\xce\xbc\x6d" ) );
    widthRfChokeDoubleSpinBox->setSuffix( trUtf8( "\x20\xce\xbc\x6d" ) );
    radiusRfChokeDoubleSpinBox->setSuffix( trUtf8( "\x20\xce\xbc\x6d" ) );
    textLabel7_2->setText( tr( "Top Metal:" ) );
    textLabel8_2->setText( tr( "Ground Metal:" ) );
    textLabel9->setText( tr( "Input Line Width:" ) );
    textLabel11_2->setText( tr( "Substrate Thickness:" ) );
    textLabel13->setText( tr( "Fan Radius:" ) );
    textLabel14->setText( tr( "Fan Angle:" ) );
    textLabel10->setText( tr( "Input Line Length:" ) );
    tabWidget2->changeTab( tunerTab, tr( "RF Choke" ) );
    resultsGroupBox->setTitle( tr( "Results" ) );
    textLabel1_3->setText( tr( "Unpumped DC Current:" ) );
    textLabel2_3->setText( tr( "Pumped DC Current:" ) );
    textLabel3_3->setText( tr( "Mixer Gain (USB):" ) );
    textLabel3_3_2->setText( tr( "Mixer Gain (LSB):" ) );
    textLabel4_3->setText( tr( "IF Output:" ) );
    textLabel4_3_2->setText( tr( "Mixer Noise (DSB):" ) );
    resetPushButton->setText( tr( "&Reset" ) );
    calculatePushButton->setText( tr( "Calculate" ) );
    quitPushButton->setText( tr( "&Quit" ) );
    plotControlsGroupBox->setTitle( tr( "Plot Controls" ) );
    textLabel1->setText( tr( "Plot from:" ) );
    plotLowDoubleSpinBox->setSuffix( tr( " mV" ) );
    textLabel2->setText( tr( "to:" ) );
    plotHighDoubleSpinBox->setSuffix( tr( " mV" ) );
    plotRedrawPushButton->setText( tr( "Re&draw" ) );
    saveDataPushButton->setText( tr( "&Save" ) );
    textLabel7->setText( tr( "X axis:" ) );
    textLabel3->setText( tr( "Left axis:" ) );
    textLabel4->setText( tr( "Right axis:" ) );
}

