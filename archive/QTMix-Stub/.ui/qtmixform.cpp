/****************************************************************************
** Form implementation generated from reading ui file 'qtmixform.ui'
**
** Created: Fri Jun 10 10:21:20 2005
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
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
    freqDoubleSpinBox->setMinValue( 10 );
    freqDoubleSpinBox->setLineStep( 5 );
    freqDoubleSpinBox->setValue( 650 );
    freqDoubleSpinBox->setDisplayPrecision( 0 );

    opPointGroupBoxLayout->addWidget( freqDoubleSpinBox, 1, 3 );

    loPowerDoubleSpinBox = new QDoubleSpinBox( opPointGroupBox, "loPowerDoubleSpinBox" );
    loPowerDoubleSpinBox->setMaxValue( 9999 );
    loPowerDoubleSpinBox->setLineStep( 10 );
    loPowerDoubleSpinBox->setValue( 100 );
    loPowerDoubleSpinBox->setDisplayPrecision( 0 );

    opPointGroupBoxLayout->addWidget( loPowerDoubleSpinBox, 2, 3 );

    loTempSpinBox = new QSpinBox( opPointGroupBox, "loTempSpinBox" );
    loTempSpinBox->setMaxValue( 999 );

    opPointGroupBoxLayout->addWidget( loTempSpinBox, 3, 3 );

    IFfreqDoubleSpinBox = new QDoubleSpinBox( opPointGroupBox, "IFfreqDoubleSpinBox" );
    IFfreqDoubleSpinBox->setMaxValue( 50 );
    IFfreqDoubleSpinBox->setLineStep( 0.5 );
    IFfreqDoubleSpinBox->setValue( 5 );
    IFfreqDoubleSpinBox->setDisplayPrecision( 2 );

    opPointGroupBoxLayout->addMultiCellWidget( IFfreqDoubleSpinBox, 4, 5, 3, 3 );
    globalsTabLayout->addWidget( opPointGroupBox );
    tabWidget2->insertTab( globalsTab, QString::fromLatin1("") );

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
    rnJuncDoubleSpinBox->setMaxValue( 1000 );
    rnJuncDoubleSpinBox->setMinValue( 1 );
    rnJuncDoubleSpinBox->setLineStep( 1 );
    rnJuncDoubleSpinBox->setValue( 20 );
    rnJuncDoubleSpinBox->setDisplayPrecision( 1 );

    junctionGroupBoxLayout->addWidget( rnJuncDoubleSpinBox, 2, 3 );

    capJuncDoubleSpinBox = new QDoubleSpinBox( junctionGroupBox, "capJuncDoubleSpinBox" );
    capJuncDoubleSpinBox->setAcceptDrops( FALSE );
    capJuncDoubleSpinBox->setMaxValue( 1999 );
    capJuncDoubleSpinBox->setLineStep( 5 );
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
    tabWidget2->insertTab( junctionTab, QString::fromLatin1("") );

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
    tabWidget2->insertTab( materialsTab, QString::fromLatin1("") );

    tunerTab = new QWidget( tabWidget2, "tunerTab" );
    tunerTabLayout = new QVBoxLayout( tunerTab, 6, 6, "tunerTabLayout"); 

    textLabel1_8 = new QLabel( tunerTab, "textLabel1_8" );
    textLabel1_8->setMinimumSize( QSize( 0, 19 ) );
    textLabel1_8->setMaximumSize( QSize( 32767, 19 ) );
    tunerTabLayout->addWidget( textLabel1_8 );

    stub1GroupBox = new QGroupBox( tunerTab, "stub1GroupBox" );
    stub1GroupBox->setColumnLayout(0, Qt::Vertical );
    stub1GroupBox->layout()->setSpacing( 6 );
    stub1GroupBox->layout()->setMargin( 6 );
    stub1GroupBoxLayout = new QGridLayout( stub1GroupBox->layout() );
    stub1GroupBoxLayout->setAlignment( Qt::AlignTop );

    topStub1ComboBox = new QComboBox( FALSE, stub1GroupBox, "topStub1ComboBox" );
    topStub1ComboBox->setMaxCount( 10 );

    stub1GroupBoxLayout->addWidget( topStub1ComboBox, 0, 3 );
    spacer36 = new QSpacerItem( 50, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    stub1GroupBoxLayout->addMultiCell( spacer36, 0, 0, 1, 2 );

    textLabel7_2 = new QLabel( stub1GroupBox, "textLabel7_2" );

    stub1GroupBoxLayout->addWidget( textLabel7_2, 0, 0 );

    textLabel8_2 = new QLabel( stub1GroupBox, "textLabel8_2" );

    stub1GroupBoxLayout->addMultiCellWidget( textLabel8_2, 1, 1, 0, 1 );

    groundStub1ComboBox = new QComboBox( FALSE, stub1GroupBox, "groundStub1ComboBox" );
    groundStub1ComboBox->setMaxCount( 10 );

    stub1GroupBoxLayout->addWidget( groundStub1ComboBox, 1, 3 );
    spacer37 = new QSpacerItem( 42, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    stub1GroupBoxLayout->addItem( spacer37, 1, 2 );

    textLabel10 = new QLabel( stub1GroupBox, "textLabel10" );

    stub1GroupBoxLayout->addMultiCellWidget( textLabel10, 2, 2, 0, 1 );
    spacer40 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    stub1GroupBoxLayout->addItem( spacer40, 2, 2 );

    lengthStub1DoubleSpinBox = new QDoubleSpinBox( stub1GroupBox, "lengthStub1DoubleSpinBox" );
    lengthStub1DoubleSpinBox->setMaxValue( 999 );
    lengthStub1DoubleSpinBox->setMinValue( 0.1 );
    lengthStub1DoubleSpinBox->setLineStep( 1 );
    lengthStub1DoubleSpinBox->setValue( 5 );
    lengthStub1DoubleSpinBox->setDisplayPrecision( 1 );

    stub1GroupBoxLayout->addWidget( lengthStub1DoubleSpinBox, 2, 3 );

    widthStub1DoubleSpinBox = new QDoubleSpinBox( stub1GroupBox, "widthStub1DoubleSpinBox" );
    widthStub1DoubleSpinBox->setMaxValue( 999 );
    widthStub1DoubleSpinBox->setMinValue( 0.1 );
    widthStub1DoubleSpinBox->setValue( 3 );
    widthStub1DoubleSpinBox->setDisplayPrecision( 1 );

    stub1GroupBoxLayout->addWidget( widthStub1DoubleSpinBox, 3, 3 );

    textLabel9 = new QLabel( stub1GroupBox, "textLabel9" );

    stub1GroupBoxLayout->addMultiCellWidget( textLabel9, 3, 3, 0, 1 );
    spacer40_2 = new QSpacerItem( 42, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    stub1GroupBoxLayout->addItem( spacer40_2, 3, 2 );

    textLabel11_2 = new QLabel( stub1GroupBox, "textLabel11_2" );

    stub1GroupBoxLayout->addMultiCellWidget( textLabel11_2, 4, 4, 0, 1 );
    spacer40_3 = new QSpacerItem( 42, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    stub1GroupBoxLayout->addItem( spacer40_3, 4, 2 );

    heightStub1DoubleSpinBox = new QDoubleSpinBox( stub1GroupBox, "heightStub1DoubleSpinBox" );
    heightStub1DoubleSpinBox->setMaxValue( 99 );
    heightStub1DoubleSpinBox->setMinValue( 0.01 );
    heightStub1DoubleSpinBox->setValue( 0.42 );
    heightStub1DoubleSpinBox->setDisplayPrecision( 2 );

    stub1GroupBoxLayout->addWidget( heightStub1DoubleSpinBox, 4, 3 );

    textLabel13 = new QLabel( stub1GroupBox, "textLabel13" );

    stub1GroupBoxLayout->addMultiCellWidget( textLabel13, 5, 5, 0, 1 );

    radiusStub1DoubleSpinBox = new QDoubleSpinBox( stub1GroupBox, "radiusStub1DoubleSpinBox" );
    radiusStub1DoubleSpinBox->setMaxValue( 999 );
    radiusStub1DoubleSpinBox->setMinValue( 0.1 );
    radiusStub1DoubleSpinBox->setValue( 30 );
    radiusStub1DoubleSpinBox->setDisplayPrecision( 1 );

    stub1GroupBoxLayout->addWidget( radiusStub1DoubleSpinBox, 5, 3 );
    spacer40_4 = new QSpacerItem( 42, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    stub1GroupBoxLayout->addItem( spacer40_4, 5, 2 );

    textLabel14 = new QLabel( stub1GroupBox, "textLabel14" );

    stub1GroupBoxLayout->addMultiCellWidget( textLabel14, 6, 6, 0, 1 );

    angleStub1DoubleSpinBox = new QDoubleSpinBox( stub1GroupBox, "angleStub1DoubleSpinBox" );
    angleStub1DoubleSpinBox->setMaxValue( 180 );
    angleStub1DoubleSpinBox->setMinValue( 30 );
    angleStub1DoubleSpinBox->setLineStep( 10 );
    angleStub1DoubleSpinBox->setValue( 180 );
    angleStub1DoubleSpinBox->setDisplayPrecision( 0 );

    stub1GroupBoxLayout->addWidget( angleStub1DoubleSpinBox, 6, 3 );
    spacer40_5 = new QSpacerItem( 42, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    stub1GroupBoxLayout->addItem( spacer40_5, 6, 2 );

    layout48 = new QHBoxLayout( 0, 0, 6, "layout48"); 

    textLabel4_4_2_2 = new QLabel( stub1GroupBox, "textLabel4_4_2_2" );
    textLabel4_4_2_2->setMinimumSize( QSize( 70, 21 ) );
    textLabel4_4_2_2->setMaximumSize( QSize( 70, 21 ) );
    textLabel4_4_2_2->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
    layout48->addWidget( textLabel4_4_2_2 );

    z0Stub1LineEdit = new QLineEdit( stub1GroupBox, "z0Stub1LineEdit" );
    z0Stub1LineEdit->setReadOnly( TRUE );
    layout48->addWidget( z0Stub1LineEdit );

    stub1GroupBoxLayout->addMultiCellLayout( layout48, 7, 7, 0, 3 );

    layout49 = new QHBoxLayout( 0, 0, 6, "layout49"); 

    textLabel5_4_2_2 = new QLabel( stub1GroupBox, "textLabel5_4_2_2" );
    textLabel5_4_2_2->setMinimumSize( QSize( 70, 21 ) );
    textLabel5_4_2_2->setMaximumSize( QSize( 70, 21 ) );
    textLabel5_4_2_2->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
    layout49->addWidget( textLabel5_4_2_2 );

    phaseStub1LineEdit = new QLineEdit( stub1GroupBox, "phaseStub1LineEdit" );
    phaseStub1LineEdit->setReadOnly( TRUE );
    layout49->addWidget( phaseStub1LineEdit );

    stub1GroupBoxLayout->addMultiCellLayout( layout49, 8, 8, 0, 3 );
    tunerTabLayout->addWidget( stub1GroupBox );
    tabWidget2->insertTab( tunerTab, QString::fromLatin1("") );

    TabPage = new QWidget( tabWidget2, "TabPage" );
    TabPageLayout = new QVBoxLayout( TabPage, 6, 6, "TabPageLayout"); 

    stub2CheckBox = new QCheckBox( TabPage, "stub2CheckBox" );
    stub2CheckBox->setChecked( FALSE );
    TabPageLayout->addWidget( stub2CheckBox );

    stub2GroupBox = new QGroupBox( TabPage, "stub2GroupBox" );
    stub2GroupBox->setEnabled( FALSE );
    stub2GroupBox->setColumnLayout(0, Qt::Vertical );
    stub2GroupBox->layout()->setSpacing( 6 );
    stub2GroupBox->layout()->setMargin( 6 );
    stub2GroupBoxLayout = new QGridLayout( stub2GroupBox->layout() );
    stub2GroupBoxLayout->setAlignment( Qt::AlignTop );

    layout49_2 = new QHBoxLayout( 0, 0, 6, "layout49_2"); 

    textLabel5_4_2_2_2 = new QLabel( stub2GroupBox, "textLabel5_4_2_2_2" );
    textLabel5_4_2_2_2->setMinimumSize( QSize( 70, 21 ) );
    textLabel5_4_2_2_2->setMaximumSize( QSize( 70, 21 ) );
    textLabel5_4_2_2_2->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
    layout49_2->addWidget( textLabel5_4_2_2_2 );

    phaseStub2LineEdit = new QLineEdit( stub2GroupBox, "phaseStub2LineEdit" );
    phaseStub2LineEdit->setReadOnly( TRUE );
    layout49_2->addWidget( phaseStub2LineEdit );

    stub2GroupBoxLayout->addMultiCellLayout( layout49_2, 8, 8, 0, 3 );

    layout48_2 = new QHBoxLayout( 0, 0, 6, "layout48_2"); 

    textLabel4_4_2_2_2 = new QLabel( stub2GroupBox, "textLabel4_4_2_2_2" );
    textLabel4_4_2_2_2->setMinimumSize( QSize( 70, 21 ) );
    textLabel4_4_2_2_2->setMaximumSize( QSize( 70, 21 ) );
    textLabel4_4_2_2_2->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
    layout48_2->addWidget( textLabel4_4_2_2_2 );

    z0Stub2LineEdit = new QLineEdit( stub2GroupBox, "z0Stub2LineEdit" );
    z0Stub2LineEdit->setReadOnly( TRUE );
    layout48_2->addWidget( z0Stub2LineEdit );

    stub2GroupBoxLayout->addMultiCellLayout( layout48_2, 7, 7, 0, 3 );

    angleStub2DoubleSpinBox = new QDoubleSpinBox( stub2GroupBox, "angleStub2DoubleSpinBox" );
    angleStub2DoubleSpinBox->setMaxValue( 180 );
    angleStub2DoubleSpinBox->setMinValue( 30 );
    angleStub2DoubleSpinBox->setLineStep( 10 );
    angleStub2DoubleSpinBox->setValue( 180 );
    angleStub2DoubleSpinBox->setDisplayPrecision( 0 );

    stub2GroupBoxLayout->addWidget( angleStub2DoubleSpinBox, 6, 3 );

    groundStub2ComboBox = new QComboBox( FALSE, stub2GroupBox, "groundStub2ComboBox" );
    groundStub2ComboBox->setMaxCount( 10 );

    stub2GroupBoxLayout->addWidget( groundStub2ComboBox, 1, 3 );

    topStub2ComboBox = new QComboBox( FALSE, stub2GroupBox, "topStub2ComboBox" );
    topStub2ComboBox->setMaxCount( 10 );

    stub2GroupBoxLayout->addWidget( topStub2ComboBox, 0, 3 );

    heightStub2DoubleSpinBox = new QDoubleSpinBox( stub2GroupBox, "heightStub2DoubleSpinBox" );
    heightStub2DoubleSpinBox->setMaxValue( 99 );
    heightStub2DoubleSpinBox->setMinValue( 0.01 );
    heightStub2DoubleSpinBox->setValue( 0.42 );
    heightStub2DoubleSpinBox->setDisplayPrecision( 2 );

    stub2GroupBoxLayout->addWidget( heightStub2DoubleSpinBox, 4, 3 );

    lengthStub2DoubleSpinBox = new QDoubleSpinBox( stub2GroupBox, "lengthStub2DoubleSpinBox" );
    lengthStub2DoubleSpinBox->setMaxValue( 999 );
    lengthStub2DoubleSpinBox->setMinValue( 0.1 );
    lengthStub2DoubleSpinBox->setValue( 39 );
    lengthStub2DoubleSpinBox->setDisplayPrecision( 1 );

    stub2GroupBoxLayout->addWidget( lengthStub2DoubleSpinBox, 2, 3 );

    widthStub2DoubleSpinBox = new QDoubleSpinBox( stub2GroupBox, "widthStub2DoubleSpinBox" );
    widthStub2DoubleSpinBox->setMaxValue( 999 );
    widthStub2DoubleSpinBox->setMinValue( 0.1 );
    widthStub2DoubleSpinBox->setValue( 3 );
    widthStub2DoubleSpinBox->setDisplayPrecision( 1 );

    stub2GroupBoxLayout->addWidget( widthStub2DoubleSpinBox, 3, 3 );

    radiusStub2DoubleSpinBox = new QDoubleSpinBox( stub2GroupBox, "radiusStub2DoubleSpinBox" );
    radiusStub2DoubleSpinBox->setMaxValue( 999 );
    radiusStub2DoubleSpinBox->setMinValue( 0.1 );
    radiusStub2DoubleSpinBox->setValue( 30 );
    radiusStub2DoubleSpinBox->setDisplayPrecision( 1 );

    stub2GroupBoxLayout->addWidget( radiusStub2DoubleSpinBox, 5, 3 );
    spacer40_5_2 = new QSpacerItem( 42, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    stub2GroupBoxLayout->addItem( spacer40_5_2, 6, 2 );
    spacer40_4_2 = new QSpacerItem( 42, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    stub2GroupBoxLayout->addItem( spacer40_4_2, 5, 2 );
    spacer40_3_2 = new QSpacerItem( 42, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    stub2GroupBoxLayout->addItem( spacer40_3_2, 4, 2 );
    spacer40_2_2 = new QSpacerItem( 42, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    stub2GroupBoxLayout->addItem( spacer40_2_2, 3, 2 );
    spacer37_2 = new QSpacerItem( 42, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    stub2GroupBoxLayout->addItem( spacer37_2, 1, 2 );

    textLabel7_2_2 = new QLabel( stub2GroupBox, "textLabel7_2_2" );

    stub2GroupBoxLayout->addWidget( textLabel7_2_2, 0, 0 );

    textLabel8_2_2 = new QLabel( stub2GroupBox, "textLabel8_2_2" );

    stub2GroupBoxLayout->addMultiCellWidget( textLabel8_2_2, 1, 1, 0, 1 );

    textLabel9_2 = new QLabel( stub2GroupBox, "textLabel9_2" );

    stub2GroupBoxLayout->addMultiCellWidget( textLabel9_2, 3, 3, 0, 1 );

    textLabel11_2_2 = new QLabel( stub2GroupBox, "textLabel11_2_2" );

    stub2GroupBoxLayout->addMultiCellWidget( textLabel11_2_2, 4, 4, 0, 1 );

    textLabel13_2 = new QLabel( stub2GroupBox, "textLabel13_2" );

    stub2GroupBoxLayout->addMultiCellWidget( textLabel13_2, 5, 5, 0, 1 );

    textLabel14_2 = new QLabel( stub2GroupBox, "textLabel14_2" );

    stub2GroupBoxLayout->addMultiCellWidget( textLabel14_2, 6, 6, 0, 1 );
    spacer36_2 = new QSpacerItem( 50, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    stub2GroupBoxLayout->addMultiCell( spacer36_2, 0, 0, 1, 2 );
    spacer40_6 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    stub2GroupBoxLayout->addItem( spacer40_6, 2, 2 );

    textLabel10_2 = new QLabel( stub2GroupBox, "textLabel10_2" );

    stub2GroupBoxLayout->addMultiCellWidget( textLabel10_2, 2, 2, 0, 1 );
    TabPageLayout->addWidget( stub2GroupBox );
    tabWidget2->insertTab( TabPage, QString::fromLatin1("") );

    TabPage_2 = new QWidget( tabWidget2, "TabPage_2" );
    TabPageLayout_2 = new QVBoxLayout( TabPage_2, 6, 6, "TabPageLayout_2"); 

    stub3CheckBox = new QCheckBox( TabPage_2, "stub3CheckBox" );
    stub3CheckBox->setChecked( FALSE );
    TabPageLayout_2->addWidget( stub3CheckBox );

    stub3GroupBox = new QGroupBox( TabPage_2, "stub3GroupBox" );
    stub3GroupBox->setEnabled( FALSE );
    stub3GroupBox->setColumnLayout(0, Qt::Vertical );
    stub3GroupBox->layout()->setSpacing( 6 );
    stub3GroupBox->layout()->setMargin( 6 );
    stub3GroupBoxLayout = new QGridLayout( stub3GroupBox->layout() );
    stub3GroupBoxLayout->setAlignment( Qt::AlignTop );

    layout49_3 = new QHBoxLayout( 0, 0, 6, "layout49_3"); 

    textLabel5_4_2_2_3 = new QLabel( stub3GroupBox, "textLabel5_4_2_2_3" );
    textLabel5_4_2_2_3->setMinimumSize( QSize( 70, 21 ) );
    textLabel5_4_2_2_3->setMaximumSize( QSize( 70, 21 ) );
    textLabel5_4_2_2_3->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
    layout49_3->addWidget( textLabel5_4_2_2_3 );

    phaseStub3LineEdit = new QLineEdit( stub3GroupBox, "phaseStub3LineEdit" );
    phaseStub3LineEdit->setReadOnly( TRUE );
    layout49_3->addWidget( phaseStub3LineEdit );

    stub3GroupBoxLayout->addMultiCellLayout( layout49_3, 8, 8, 0, 3 );

    layout48_3 = new QHBoxLayout( 0, 0, 6, "layout48_3"); 

    textLabel4_4_2_2_3 = new QLabel( stub3GroupBox, "textLabel4_4_2_2_3" );
    textLabel4_4_2_2_3->setMinimumSize( QSize( 70, 21 ) );
    textLabel4_4_2_2_3->setMaximumSize( QSize( 70, 21 ) );
    textLabel4_4_2_2_3->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
    layout48_3->addWidget( textLabel4_4_2_2_3 );

    z0Stub3LineEdit = new QLineEdit( stub3GroupBox, "z0Stub3LineEdit" );
    z0Stub3LineEdit->setReadOnly( TRUE );
    layout48_3->addWidget( z0Stub3LineEdit );

    stub3GroupBoxLayout->addMultiCellLayout( layout48_3, 7, 7, 0, 3 );

    angleStub3DoubleSpinBox = new QDoubleSpinBox( stub3GroupBox, "angleStub3DoubleSpinBox" );
    angleStub3DoubleSpinBox->setMaxValue( 180 );
    angleStub3DoubleSpinBox->setMinValue( 30 );
    angleStub3DoubleSpinBox->setLineStep( 10 );
    angleStub3DoubleSpinBox->setValue( 180 );
    angleStub3DoubleSpinBox->setDisplayPrecision( 0 );

    stub3GroupBoxLayout->addWidget( angleStub3DoubleSpinBox, 6, 3 );

    groundStub3ComboBox = new QComboBox( FALSE, stub3GroupBox, "groundStub3ComboBox" );
    groundStub3ComboBox->setMaxCount( 10 );

    stub3GroupBoxLayout->addWidget( groundStub3ComboBox, 1, 3 );

    topStub3ComboBox = new QComboBox( FALSE, stub3GroupBox, "topStub3ComboBox" );
    topStub3ComboBox->setMaxCount( 10 );

    stub3GroupBoxLayout->addWidget( topStub3ComboBox, 0, 3 );

    heightStub3DoubleSpinBox = new QDoubleSpinBox( stub3GroupBox, "heightStub3DoubleSpinBox" );
    heightStub3DoubleSpinBox->setMaxValue( 99 );
    heightStub3DoubleSpinBox->setMinValue( 0.01 );
    heightStub3DoubleSpinBox->setValue( 0.42 );
    heightStub3DoubleSpinBox->setDisplayPrecision( 2 );

    stub3GroupBoxLayout->addWidget( heightStub3DoubleSpinBox, 4, 3 );

    lengthStub3DoubleSpinBox = new QDoubleSpinBox( stub3GroupBox, "lengthStub3DoubleSpinBox" );
    lengthStub3DoubleSpinBox->setMaxValue( 999 );
    lengthStub3DoubleSpinBox->setMinValue( 0.1 );
    lengthStub3DoubleSpinBox->setValue( 39 );
    lengthStub3DoubleSpinBox->setDisplayPrecision( 1 );

    stub3GroupBoxLayout->addWidget( lengthStub3DoubleSpinBox, 2, 3 );

    widthStub3DoubleSpinBox = new QDoubleSpinBox( stub3GroupBox, "widthStub3DoubleSpinBox" );
    widthStub3DoubleSpinBox->setMaxValue( 999 );
    widthStub3DoubleSpinBox->setMinValue( 0.1 );
    widthStub3DoubleSpinBox->setValue( 3 );
    widthStub3DoubleSpinBox->setDisplayPrecision( 1 );

    stub3GroupBoxLayout->addWidget( widthStub3DoubleSpinBox, 3, 3 );

    radiusStub3DoubleSpinBox = new QDoubleSpinBox( stub3GroupBox, "radiusStub3DoubleSpinBox" );
    radiusStub3DoubleSpinBox->setMaxValue( 999 );
    radiusStub3DoubleSpinBox->setMinValue( 0.1 );
    radiusStub3DoubleSpinBox->setValue( 30 );
    radiusStub3DoubleSpinBox->setDisplayPrecision( 1 );

    stub3GroupBoxLayout->addWidget( radiusStub3DoubleSpinBox, 5, 3 );
    spacer40_5_3 = new QSpacerItem( 42, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    stub3GroupBoxLayout->addItem( spacer40_5_3, 6, 2 );
    spacer40_4_3 = new QSpacerItem( 42, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    stub3GroupBoxLayout->addItem( spacer40_4_3, 5, 2 );
    spacer40_3_3 = new QSpacerItem( 42, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    stub3GroupBoxLayout->addItem( spacer40_3_3, 4, 2 );
    spacer40_2_3 = new QSpacerItem( 42, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    stub3GroupBoxLayout->addItem( spacer40_2_3, 3, 2 );
    spacer37_3 = new QSpacerItem( 42, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    stub3GroupBoxLayout->addItem( spacer37_3, 1, 2 );

    textLabel7_2_3 = new QLabel( stub3GroupBox, "textLabel7_2_3" );

    stub3GroupBoxLayout->addWidget( textLabel7_2_3, 0, 0 );

    textLabel8_2_3 = new QLabel( stub3GroupBox, "textLabel8_2_3" );

    stub3GroupBoxLayout->addMultiCellWidget( textLabel8_2_3, 1, 1, 0, 1 );

    textLabel9_3 = new QLabel( stub3GroupBox, "textLabel9_3" );

    stub3GroupBoxLayout->addMultiCellWidget( textLabel9_3, 3, 3, 0, 1 );

    textLabel11_2_3 = new QLabel( stub3GroupBox, "textLabel11_2_3" );

    stub3GroupBoxLayout->addMultiCellWidget( textLabel11_2_3, 4, 4, 0, 1 );

    textLabel13_3 = new QLabel( stub3GroupBox, "textLabel13_3" );

    stub3GroupBoxLayout->addMultiCellWidget( textLabel13_3, 5, 5, 0, 1 );

    textLabel14_3 = new QLabel( stub3GroupBox, "textLabel14_3" );

    stub3GroupBoxLayout->addMultiCellWidget( textLabel14_3, 6, 6, 0, 1 );
    spacer36_3 = new QSpacerItem( 50, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    stub3GroupBoxLayout->addMultiCell( spacer36_3, 0, 0, 1, 2 );
    spacer40_7 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    stub3GroupBoxLayout->addItem( spacer40_7, 2, 2 );

    textLabel10_3 = new QLabel( stub3GroupBox, "textLabel10_3" );

    stub3GroupBoxLayout->addMultiCellWidget( textLabel10_3, 2, 2, 0, 1 );
    TabPageLayout_2->addWidget( stub3GroupBox );
    tabWidget2->insertTab( TabPage_2, QString::fromLatin1("") );
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
    connect( angleStub1DoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setStub1Dim() ) );
    connect( heightStub1DoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setStub1Dim() ) );
    connect( lengthStub1DoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setStub1Dim() ) );
    connect( radiusStub1DoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setStub1Dim() ) );
    connect( widthStub1DoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setStub1Dim() ) );
    connect( groundStub1ComboBox, SIGNAL( activated(const QString&) ), this, SLOT( setStub1Dim() ) );
    connect( topStub1ComboBox, SIGNAL( activated(const QString&) ), this, SLOT( setStub1Dim() ) );
    connect( stub2CheckBox, SIGNAL( stateChanged(int) ), this, SLOT( setRfCircuit() ) );
    connect( stub3CheckBox, SIGNAL( stateChanged(int) ), this, SLOT( setRfCircuit() ) );
    connect( angleStub2DoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setStub2Dim() ) );
    connect( heightStub2DoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setStub2Dim() ) );
    connect( lengthStub2DoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setStub2Dim() ) );
    connect( radiusStub2DoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setStub2Dim() ) );
    connect( widthStub2DoubleSpinBox, SIGNAL( valueChanged(const QString&) ), this, SLOT( setStub2Dim() ) );
    connect( groundStub2ComboBox, SIGNAL( activated(int) ), this, SLOT( setStub2Dim() ) );
    connect( topStub2ComboBox, SIGNAL( activated(int) ), this, SLOT( setStub2Dim() ) );
    connect( angleStub3DoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setStub3Dim() ) );
    connect( groundStub3ComboBox, SIGNAL( activated(int) ), this, SLOT( setStub3Dim() ) );
    connect( heightStub3DoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setStub3Dim() ) );
    connect( lengthStub3DoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setStub3Dim() ) );
    connect( radiusStub3DoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setStub3Dim() ) );
    connect( topStub3ComboBox, SIGNAL( activated(int) ), this, SLOT( setStub3Dim() ) );
    connect( widthStub3DoubleSpinBox, SIGNAL( valueChanged(double) ), this, SLOT( setStub3Dim() ) );

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
    setCaption( tr( "QT Mix - Radial Stub Mixer" ) );
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
    textLabel1_8->setText( tr( "Stub 1 also acts as RF choke and IF output" ) );
    stub1GroupBox->setTitle( tr( "Stub 1 Dimensions" ) );
    textLabel7_2->setText( tr( "Top Metal:" ) );
    textLabel8_2->setText( tr( "Ground Metal:" ) );
    textLabel10->setText( tr( "Input Line Length:" ) );
    lengthStub1DoubleSpinBox->setSuffix( trUtf8( "\x20\xce\xbc\x6d" ) );
    widthStub1DoubleSpinBox->setSuffix( trUtf8( "\x20\xce\xbc\x6d" ) );
    textLabel9->setText( tr( "Input Line Width:" ) );
    textLabel11_2->setText( tr( "Substrate Thickness:" ) );
    heightStub1DoubleSpinBox->setSuffix( trUtf8( "\x20\xce\xbc\x6d" ) );
    textLabel13->setText( tr( "Fan Radius:" ) );
    radiusStub1DoubleSpinBox->setSuffix( trUtf8( "\x20\xce\xbc\x6d" ) );
    textLabel14->setText( tr( "Fan Angle:" ) );
    angleStub1DoubleSpinBox->setSuffix( trUtf8( "\xc2\xb0" ) );
    textLabel4_4_2_2->setText( tr( "Z0 : " ) );
    textLabel5_4_2_2->setText( tr( "Phase : " ) );
    tabWidget2->changeTab( tunerTab, tr( "Stub 1" ) );
    stub2CheckBox->setText( tr( "Include Stub 2" ) );
    stub2GroupBox->setTitle( tr( "Stub 2 Dimensions" ) );
    textLabel5_4_2_2_2->setText( tr( "Phase : " ) );
    textLabel4_4_2_2_2->setText( tr( "Z0 : " ) );
    angleStub2DoubleSpinBox->setSuffix( trUtf8( "\xc2\xb0" ) );
    heightStub2DoubleSpinBox->setSuffix( trUtf8( "\x20\xce\xbc\x6d" ) );
    lengthStub2DoubleSpinBox->setSuffix( trUtf8( "\x20\xce\xbc\x6d" ) );
    widthStub2DoubleSpinBox->setSuffix( trUtf8( "\x20\xce\xbc\x6d" ) );
    radiusStub2DoubleSpinBox->setSuffix( trUtf8( "\x20\xce\xbc\x6d" ) );
    textLabel7_2_2->setText( tr( "Top Metal:" ) );
    textLabel8_2_2->setText( tr( "Ground Metal:" ) );
    textLabel9_2->setText( tr( "Input Line Width:" ) );
    textLabel11_2_2->setText( tr( "Substrate Thickness:" ) );
    textLabel13_2->setText( tr( "Fan Radius:" ) );
    textLabel14_2->setText( tr( "Fan Angle:" ) );
    textLabel10_2->setText( tr( "Input Line Length:" ) );
    tabWidget2->changeTab( TabPage, tr( "Stub 2" ) );
    stub3CheckBox->setText( tr( "Include Stub 3" ) );
    stub3GroupBox->setTitle( tr( "Stub 3 Dimensions" ) );
    textLabel5_4_2_2_3->setText( tr( "Phase : " ) );
    textLabel4_4_2_2_3->setText( tr( "Z0 : " ) );
    angleStub3DoubleSpinBox->setSuffix( trUtf8( "\xc2\xb0" ) );
    heightStub3DoubleSpinBox->setSuffix( trUtf8( "\x20\xce\xbc\x6d" ) );
    lengthStub3DoubleSpinBox->setSuffix( trUtf8( "\x20\xce\xbc\x6d" ) );
    widthStub3DoubleSpinBox->setSuffix( trUtf8( "\x20\xce\xbc\x6d" ) );
    radiusStub3DoubleSpinBox->setSuffix( trUtf8( "\x20\xce\xbc\x6d" ) );
    textLabel7_2_3->setText( tr( "Top Metal:" ) );
    textLabel8_2_3->setText( tr( "Ground Metal:" ) );
    textLabel9_3->setText( tr( "Input Line Width:" ) );
    textLabel11_2_3->setText( tr( "Substrate Thickness:" ) );
    textLabel13_3->setText( tr( "Fan Radius:" ) );
    textLabel14_3->setText( tr( "Fan Angle:" ) );
    textLabel10_3->setText( tr( "Input Line Length:" ) );
    tabWidget2->changeTab( TabPage_2, tr( "Stub 3" ) );
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

