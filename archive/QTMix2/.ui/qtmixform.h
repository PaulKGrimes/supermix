/****************************************************************************
** Form interface generated from reading ui file 'qtmixform.ui'
**
** Created: Fri Jun 10 10:38:42 2005
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef QTMIXFORM_H
#define QTMIXFORM_H

#include <qvariant.h>
#include <qdialog.h>
#include "supermix.h"
#include "arbitary_impedance.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QSplitter;
class QFrame;
class QGroupBox;
class FileChooser;
class QLabel;
class QDoubleSpinBox;
class QSpinBox;
class QLineEdit;
class QPushButton;
class QComboBox;
class QwtPlot;

class QTMixForm : public QDialog
{
    Q_OBJECT

public:
    QTMixForm( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~QTMixForm();

    QSplitter* splitter2;
    QFrame* frame4;
    QGroupBox* junctionGroupBox;
    FileChooser* idcFileChooser;
    QLabel* textLabel1_2;
    QLabel* textLabel2_4;
    FileChooser* ikkFileChooser;
    QLabel* textLabel2_4_2;
    QDoubleSpinBox* RnSpinBox;
    QDoubleSpinBox* CapSpinBox;
    QLabel* textLabel2_4_2_2;
    QGroupBox* embImpGroupBox;
    QLabel* textLabel2_2_4;
    QDoubleSpinBox* realIfEmbImpDoubleSpinBox;
    QLabel* textLabel3_2_4;
    QDoubleSpinBox* imagIfEmbImpDoubleSpinBox;
    QLabel* textLabel2_2;
    QDoubleSpinBox* real1EmbImpDoubleSpinBox;
    QLabel* textLabel3_2;
    QDoubleSpinBox* imag1EmbImpDoubleSpinBox;
    QLabel* textLabel2_2_2;
    QDoubleSpinBox* real2EmbImpDoubleSpinBox;
    QLabel* textLabel3_2_2;
    QDoubleSpinBox* imag2EmbImpDoubleSpinBox;
    QLabel* textLabel2_2_3;
    QDoubleSpinBox* real3EmbImpDoubleSpinBox;
    QLabel* textLabel3_2_3;
    QDoubleSpinBox* imag3EmbImpDoubleSpinBox;
    QGroupBox* opPointGroupBox;
    QLabel* textLabel5;
    QLabel* textLabel4_2;
    QLabel* textLabel6;
    QDoubleSpinBox* loVoltDoubleSpinBox;
    QDoubleSpinBox* freqDoubleSpinBox;
    QDoubleSpinBox* biasDoubleSpinBox;
    QDoubleSpinBox* IFfreqDoubleSpinBox;
    QLabel* textLabel8;
    QGroupBox* simulationGroupBox;
    QLabel* textLabel11;
    QSpinBox* harmonicsSpinBox;
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
    QLabel* textLabel4_3_3;
    QLineEdit* loCurrentLineEdit;
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

    ivcurve iv;
    voltage_source lo;
    voltage_source bias;
    mixer mix;
    QString m_filename;
    long plotRightCurve;
    double plotLow;
    double plotHigh;
    long plotLeftCurve;
    arbitary_impedance embImp;
    transformer dummyIf;
    sis_device sis;
    int initialised;
    int USB_PORT;
    int IF_PORT;
    int LSB_PORT;

public slots:
    virtual void calculate();
    virtual void init();
    virtual void redrawPlot();
    virtual void setPlotLimits();
    virtual void calcOnMouse( const QMouseEvent & e );
    virtual void fileSaveAs();
    virtual void fileSave();
    virtual void setOperatingPoint();
    virtual void setEmbImpedance();
    virtual void setHarmonics( int h );
    virtual void setIVdata();
    virtual void setPlotXaxis();
    virtual void setJunction();

protected:
    QVBoxLayout* QTMixFormLayout;
    QVBoxLayout* frame4Layout;
    QSpacerItem* spacer17_2;
    QGridLayout* junctionGroupBoxLayout;
    QSpacerItem* spacer18_3;
    QSpacerItem* spacer17_3;
    QSpacerItem* spacer17_3_2;
    QSpacerItem* spacer17_3_2_2;
    QVBoxLayout* embImpGroupBoxLayout;
    QHBoxLayout* layout18;
    QSpacerItem* spacer18_2_4;
    QHBoxLayout* layout14;
    QHBoxLayout* layout16;
    QSpacerItem* spacer18_2;
    QHBoxLayout* layout15;
    QHBoxLayout* layout7_2;
    QSpacerItem* spacer18_2_2;
    QHBoxLayout* layout6_2;
    QHBoxLayout* layout7_3;
    QSpacerItem* spacer18_2_3;
    QHBoxLayout* layout6_3;
    QGridLayout* opPointGroupBoxLayout;
    QSpacerItem* spacer22;
    QSpacerItem* spacer21;
    QSpacerItem* spacer23;
    QSpacerItem* spacer24;
    QVBoxLayout* simulationGroupBoxLayout;
    QHBoxLayout* layout21;
    QSpacerItem* spacer27;
    QGridLayout* resultsGroupBoxLayout;
    QSpacerItem* spacer13;
    QSpacerItem* spacer14;
    QSpacerItem* spacer15;
    QSpacerItem* spacer16_3;
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
