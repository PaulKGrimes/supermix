/****************************************************************************
** QTMixForm meta object code from reading C++ file 'qtmixform2.h'
**
** Created: Fri Jun 10 13:00:45 2005
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.4   edited Jan 21 18:14 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../.ui/qtmixform2.h"
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
    static const QUMethod slot_1 = {"redrawPlot", 0, 0 };
    static const QUMethod slot_2 = {"setPlotLimits", 0, 0 };
    static const QUParameter param_slot_3[] = {
	{ "e", &static_QUType_ptr, "QMouseEvent", QUParameter::In }
    };
    static const QUMethod slot_3 = {"calcOnMouse", 1, param_slot_3 };
    static const QUMethod slot_4 = {"fileSaveAs", 0, 0 };
    static const QUMethod slot_5 = {"fileSave", 0, 0 };
    static const QUMethod slot_6 = {"setOperatingPoint", 0, 0 };
    static const QUMethod slot_7 = {"setEmbImpedance", 0, 0 };
    static const QUParameter param_slot_8[] = {
	{ "h", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_8 = {"setHarmonics", 1, param_slot_8 };
    static const QUMethod slot_9 = {"setIVdata", 0, 0 };
    static const QUMethod slot_10 = {"setPlotXaxis", 0, 0 };
    static const QUMethod slot_11 = {"setJunction", 0, 0 };
    static const QUMethod slot_12 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "calculate()", &slot_0, QMetaData::Public },
	{ "redrawPlot()", &slot_1, QMetaData::Public },
	{ "setPlotLimits()", &slot_2, QMetaData::Public },
	{ "calcOnMouse(const QMouseEvent&)", &slot_3, QMetaData::Public },
	{ "fileSaveAs()", &slot_4, QMetaData::Public },
	{ "fileSave()", &slot_5, QMetaData::Public },
	{ "setOperatingPoint()", &slot_6, QMetaData::Public },
	{ "setEmbImpedance()", &slot_7, QMetaData::Public },
	{ "setHarmonics(int)", &slot_8, QMetaData::Public },
	{ "setIVdata()", &slot_9, QMetaData::Public },
	{ "setPlotXaxis()", &slot_10, QMetaData::Public },
	{ "setJunction()", &slot_11, QMetaData::Public },
	{ "languageChange()", &slot_12, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"QTMixForm", parentObject,
	slot_tbl, 13,
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
    case 1: redrawPlot(); break;
    case 2: setPlotLimits(); break;
    case 3: calcOnMouse((const QMouseEvent&)*((const QMouseEvent*)static_QUType_ptr.get(_o+1))); break;
    case 4: fileSaveAs(); break;
    case 5: fileSave(); break;
    case 6: setOperatingPoint(); break;
    case 7: setEmbImpedance(); break;
    case 8: setHarmonics((int)static_QUType_int.get(_o+1)); break;
    case 9: setIVdata(); break;
    case 10: setPlotXaxis(); break;
    case 11: setJunction(); break;
    case 12: languageChange(); break;
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
