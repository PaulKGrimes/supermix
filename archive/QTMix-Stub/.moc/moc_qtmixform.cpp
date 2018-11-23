/****************************************************************************
** QTMixForm meta object code from reading C++ file 'qtmixform.h'
**
** Created: Fri Jun 10 10:05:54 2005
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.4   edited Jan 21 18:14 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../.ui/qtmixform.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *QTMixForm::className() const
{
    return "QTMixForm";
}

QMetaObject *QTMixForm::metaObj = 0;
static QMetaObjectCleanUp cleanUp_QTMixForm( "QTMixForm", &QTMixForm::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString QTMixForm::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "QTMixForm", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString QTMixForm::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "QTMixForm", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* QTMixForm::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"calculate", 0, 0 };
    static const QUMethod slot_1 = {"init", 0, 0 };
    static const QUMethod slot_2 = {"redrawPlot", 0, 0 };
    static const QUMethod slot_3 = {"setPlotLimits", 0, 0 };
    static const QUParameter param_slot_4[] = {
	{ "e", &static_QUType_ptr, "QMouseEvent", QUParameter::In }
    };
    static const QUMethod slot_4 = {"calcOnMouse", 1, param_slot_4 };
    static const QUMethod slot_5 = {"fileSaveAs", 0, 0 };
    static const QUMethod slot_6 = {"fileSave", 0, 0 };
    static const QUMethod slot_7 = {"setOperatingPoint", 0, 0 };
    static const QUMethod slot_8 = {"setIFImpedance", 0, 0 };
    static const QUParameter param_slot_9[] = {
	{ "h", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_9 = {"setHarmonics", 1, param_slot_9 };
    static const QUMethod slot_10 = {"setIVdata", 0, 0 };
    static const QUMethod slot_11 = {"setJunction", 0, 0 };
    static const QUMethod slot_12 = {"setSimParameters", 0, 0 };
    static const QUMethod slot_13 = {"setRfCircuit", 0, 0 };
    static const QUMethod slot_14 = {"setPlotXAxis", 0, 0 };
    static const QUMethod slot_15 = {"setNormMetal", 0, 0 };
    static const QUMethod slot_16 = {"setScMetal", 0, 0 };
    static const QUMethod slot_17 = {"setDielectric", 0, 0 };
    static const QUMethod slot_18 = {"setStub1Dim", 0, 0 };
    static const QUMethod slot_19 = {"setStub2Dim", 0, 0 };
    static const QUMethod slot_20 = {"setStub3Dim", 0, 0 };
    static const QUMethod slot_21 = {"calcStripProperties", 0, 0 };
    static const QUMethod slot_22 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "calculate()", &slot_0, QMetaData::Public },
	{ "init()", &slot_1, QMetaData::Public },
	{ "redrawPlot()", &slot_2, QMetaData::Public },
	{ "setPlotLimits()", &slot_3, QMetaData::Public },
	{ "calcOnMouse(const QMouseEvent&)", &slot_4, QMetaData::Public },
	{ "fileSaveAs()", &slot_5, QMetaData::Public },
	{ "fileSave()", &slot_6, QMetaData::Public },
	{ "setOperatingPoint()", &slot_7, QMetaData::Public },
	{ "setIFImpedance()", &slot_8, QMetaData::Public },
	{ "setHarmonics(int)", &slot_9, QMetaData::Public },
	{ "setIVdata()", &slot_10, QMetaData::Public },
	{ "setJunction()", &slot_11, QMetaData::Public },
	{ "setSimParameters()", &slot_12, QMetaData::Public },
	{ "setRfCircuit()", &slot_13, QMetaData::Public },
	{ "setPlotXAxis()", &slot_14, QMetaData::Public },
	{ "setNormMetal()", &slot_15, QMetaData::Public },
	{ "setScMetal()", &slot_16, QMetaData::Public },
	{ "setDielectric()", &slot_17, QMetaData::Public },
	{ "setStub1Dim()", &slot_18, QMetaData::Public },
	{ "setStub2Dim()", &slot_19, QMetaData::Public },
	{ "setStub3Dim()", &slot_20, QMetaData::Public },
	{ "calcStripProperties()", &slot_21, QMetaData::Public },
	{ "languageChange()", &slot_22, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"QTMixForm", parentObject,
	slot_tbl, 23,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_QTMixForm.setMetaObject( metaObj );
    return metaObj;
}

void* QTMixForm::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "QTMixForm" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool QTMixForm::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: calculate(); break;
    case 1: init(); break;
    case 2: redrawPlot(); break;
    case 3: setPlotLimits(); break;
    case 4: calcOnMouse((const QMouseEvent&)*((const QMouseEvent*)static_QUType_ptr.get(_o+1))); break;
    case 5: fileSaveAs(); break;
    case 6: fileSave(); break;
    case 7: setOperatingPoint(); break;
    case 8: setIFImpedance(); break;
    case 9: setHarmonics((int)static_QUType_int.get(_o+1)); break;
    case 10: setIVdata(); break;
    case 11: setJunction(); break;
    case 12: setSimParameters(); break;
    case 13: setRfCircuit(); break;
    case 14: setPlotXAxis(); break;
    case 15: setNormMetal(); break;
    case 16: setScMetal(); break;
    case 17: setDielectric(); break;
    case 18: setStub1Dim(); break;
    case 19: setStub2Dim(); break;
    case 20: setStub3Dim(); break;
    case 21: calcStripProperties(); break;
    case 22: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool QTMixForm::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool QTMixForm::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool QTMixForm::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
