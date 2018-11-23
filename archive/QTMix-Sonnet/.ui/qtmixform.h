/****************************************************************************
** Form interface generated from reading ui file 'qtmixform.ui'
**
** Created: Tue May 17 10:01:21 2005
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef QTMIXFORM_H
#define QTMIXFORM_H

#include <qvariant.h>
#include <qdialog.h>
#include "supermix.h"
#include "reduced_nport.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QSplitter;
class QFrame;
class QTabWidget;
class QWidget;
class QGroupBox;
class QDoubleSpinBox;
class QLabel;
class QSpinBox;
class FileChooser;
class QCheckBox;
class QLineEdit;
class QComboBox;
class QPushButton;
class QwtPlot;

class QTMixForm : public QDialog
{
    Q_OBJECT

public:
    QTMixForm( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~QTMixForm();

    QSplitter* splitter3;
    QFrame* frame4;
    QTabWidget* tabWidget2;
    QWidget* globalsTab;
    QGroupBox* simulationGroupBox;
    QDoubleSpinBox* tDoubleSpinBox;
    QLabel* textLabel6_2;
    QLabel* textLabel5_2;
    QSpinBox* harmonicsSpinBox;
    QLabel* textLabel11;
    QLabel* textLabel1_5;
    QDoubleSpinBox* ifImpDoubleSpinBox;
    QDoubleSpinBox* z0DoubleSpinBox;
    QGroupBox* opPointGroupBox;
    QLabel* textLabel5;
    QLabel* textLabel4_2;
    QLabel* textLabel6;
    QDoubleSpinBox* biasDoubleSpinBox;
    QLabel* textLabel8;
    QLabel* textLabel3_4;
    QDoubleSpinBox* freqDoubleSpinBox;
    QDoubleSpinBox* loPowerDoubleSpinBox;
    QSpinBox* loTempSpinBox;
    QDoubleSpinBox* IFfreqDoubleSpinBox;
    QWidget* junctionTab;
    QGroupBox* junctionGroupBox;
    QLabel* textLabel1_2;
    QLabel* textLabel2_4;
    FileChooser* idcFileChooser;
    FileChooser* ikkFileChooser;
    QLabel* textLabel1_4;
    QLabel* textLabel2_2;
    QLabel* textLabel3_2;
    QDoubleSpinBox* rnJuncDoubleSpinBox;
    QDoubleSpinBox* capJuncDoubleSpinBox;
    QDoubleSpinBox* vGapJuncDoubleSpinBox;
    QWidget* materialsTab;
    QGroupBox* substrateGroupBox;
    QLabel* textLabel1_6;
    QLabel* textLabel2_5;
    QDoubleSpinBox* epsSubstrateDoubleSpinBox;
    QDoubleSpinBox* tandSubstrateDoubleSpinBox;
    QGroupBox* scMaterialGroupBox;
    QLabel* textLabel3_5;
    QLabel* textLabel4_5;
    QLabel* textLabel5_3;
    QDoubleSpinBox* rhoScMetalDoubleSpinBox;
    QDoubleSpinBox* vGapScMetalDoubleSpinBox;
    QDoubleSpinBox* tcScMetalDoubleSpinBox;
    QLabel* textLabel12_2;
    QDoubleSpinBox* thickScMetalDoubleSpinBox;
    QGroupBox* normMetalGroupBox;
    QLabel* textLabel6_3;
    QLabel* textLabel12;
    QDoubleSpinBox* rhoNormMetalDoubleSpinBox;
    QDoubleSpinBox* thickNormMetalDoubleSpinBox;
    QWidget* TabPage;
    QLabel* textLabel1_7;
    FileChooser* sonnetFileChooser;
    QWidget* tunerTab;
    QCheckBox* rfChokeCheckBox;
    QGroupBox* rfChokeGroupBox;
    QLabel* textLabel5_4_2_2;
    QLineEdit* phaseRadialLineEdit;
    QLabel* textLabel4_4_2_2;
    QLineEdit* z0RadialLineEdit;
    QDoubleSpinBox* angleRfChokeDoubleSpinBox;
    QComboBox* groundRfChokeComboBox;
    QComboBox* topRfChokeComboBox;
    QDoubleSpinBox* heightRfChokeDoubleSpinBox;
    QDoubleSpinBox* lengthRfChokeDoubleSpinBox;
    QDoubleSpinBox* widthRfChokeDoubleSpinBox;
    QDoubleSpinBox* radiusRfChokeDoubleSpinBox;
    QLabel* textLabel7_2;
    QLabel* textLabel8_2;
    QLabel* textLabel9;
    QLabel* textLabel11_2;
    QLabel* textLabel13;
    QLabel* textLabel14;
    QLabel* textLabel10;
    QGroupBox* resultsGroupBox;
    QLineEdit* unpumpLineEdit;
    QLabel* textLabel1_3;
    QLabel* textLabel2_3;
    QLineEdit* pumpedLineEdit;
    QLabel* textLabel3_3;
    QLabel* textLabel3_3_2;
    QLineEdit* lsbGainLineEdit;
    QLineEdit* usbGainLineEdit;
    QLabel* textLabel4_3;
    QLabel* textLabel4_3_2;
    QLineEdit* noiseLineEdit;
    QLineEdit* ifOutputLineEdit;
    QPushButton* resetPushButton;
    QPushButton* calculatePushButton;
    QPushButton* quitPushButton;
    QFrame* frame3;
    QGroupBox* plotControlsGroupBox;
    QLabel* textLabel1;
    QDoubleSpinBox* plotLowDoubleSpinBox;
    QLabel* textLabel2;
    QDoubleSpinBox* plotHighDoubleSpinBox;
    QPushButton* plotRedrawPushButton;
    QPushButton* saveDataPushButton;
    QLabel* textLabel7;
    QComboBox* plotXAxisComboBox;
    QLabel* textLabel3;
    QComboBox* plotLeftComboBox;
    QLabel* textLabel4;
    QComboBox* plotRightComboBox;
    QwtPlot* plot;

    QString sonnetFile;
    sdata_interp sonnetDevice;
    capacitor juncCap;
    circuit rfNoChokeZemb;
    reduced_nport rfNoChoke;
    Complex TwoPiI;
    Complex Zstub;
    Complex Zemb;
    const_diel substrate;
    transformer ifWithChokeTrans;
    int LSB_PORT;
    int IF_PORT;
    int USB_PORT;
    int initialised;
    sis_device sis;
    long plotLeftCurve;
    double plotHigh;
    double plotLow;
    long plotRightCurve;
    QString m_filename;
    mixer mix;
    voltage_source bias;
    generator lo;
    ivcurve iv;
    branch rfBranch;
    radial_stub rfChoke;
    zterm rfTerm;
    const_diel superstrate;
    super_film superconductor;
    normal_film normalconductor;
    circuit rfWithChoke;
    circuit rfWithChokeZemb;
    transformer ifNoChokeTrans;
    circuit ifWithChoke;
    microstrip rfChokeStrip;

public slots:
    virtual void calculate();
    virtual void init();
    virtual void redrawPlot();
    virtual void setPlotLimits();
    virtual void calcOnMouse( const QMouseEvent & e );
    virtual void fileSaveAs();
    virtual void fileSave();
    virtual void setOperatingPoint();
    virtual void setIFImpedance();
    virtual void setHarmonics( int h );
    virtual void setIVdata();
    virtual void setJunction();
    virtual void setSimParameters();
    virtual void setRfChoke();
    virtual void setPlotXAxis();
    virtual void setNormMetal();
    virtual void setScMetal();
    virtual void setDielectric();
    virtual void setRfChokeDim();
    virtual void setTuner();
    virtual void calcStripProperties();

protected:
    QHBoxLayout* QTMixFormLayout;
    QVBoxLayout* frame4Layout;
    QSpacerItem* spacer17_2;
    QVBoxLayout* globalsTabLayout;
    QGridLayout* simulationGroupBoxLayout;
    QSpacerItem* spacer29;
    QSpacerItem* spacer30;
    QSpacerItem* spacer27;
    QSpacerItem* spacer26_2;
    QGridLayout* opPointGroupBoxLayout;
    QSpacerItem* spacer22;
    QSpacerItem* spacer21;
    QSpacerItem* spacer23;
    QSpacerItem* spacer24;
    QSpacerItem* spacer19;
    QVBoxLayout* junctionTabLayout;
    QSpacerItem* spacer28_2;
    QGridLayout* junctionGroupBoxLayout;
    QSpacerItem* spacer18_3;
    QSpacerItem* spacer17_3;
    QSpacerItem* spacer23_2;
    QSpacerItem* spacer24_2;
    QSpacerItem* spacer25;
    QVBoxLayout* materialsTabLayout;
    QGridLayout* substrateGroupBoxLayout;
    QSpacerItem* spacer29_2;
    QSpacerItem* spacer30_2;
    QGridLayout* scMaterialGroupBoxLayout;
    QSpacerItem* spacer31;
    QSpacerItem* spacer32;
    QSpacerItem* spacer33;
    QSpacerItem* spacer38_2;
    QGridLayout* normMetalGroupBoxLayout;
    QSpacerItem* spacer38;
    QSpacerItem* spacer34;
    QVBoxLayout* TabPageLayout;
    QSpacerItem* spacer44;
    QHBoxLayout* layout17;
    QSpacerItem* spacer43;
    QVBoxLayout* tunerTabLayout;
    QGridLayout* rfChokeGroupBoxLayout;
    QSpacerItem* spacer40_5;
    QSpacerItem* spacer40_4;
    QSpacerItem* spacer40_3;
    QSpacerItem* spacer40_2;
    QSpacerItem* spacer37;
    QSpacerItem* spacer36;
    QSpacerItem* spacer40;
    QHBoxLayout* layout49;
    QHBoxLayout* layout48;
    QGridLayout* resultsGroupBoxLayout;
    QSpacerItem* spacer13;
    QSpacerItem* spacer14;
    QSpacerItem* spacer15;
    QSpacerItem* spacer16_2;
    QSpacerItem* spacer15_2;
    QSpacerItem* spacer16;
    QHBoxLayout* buttonLayout;
    QVBoxLayout* frame3Layout;
    QVBoxLayout* plotControlsGroupBoxLayout;
    QHBoxLayout* layout6;
    QSpacerItem* spacer18;
    QHBoxLayout* layout19;
    QSpacerItem* spacer26;
    QHBoxLayout* layout3;
    QSpacerItem* spacer17;

protected slots:
    virtual void languageChange();

};

#endif // QTMIXFORM_H
