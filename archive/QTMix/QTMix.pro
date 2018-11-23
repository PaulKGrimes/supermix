TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release

LIBS	+= /data/collam/pxg/software/supermix/obj.i386-intel-linux/libMiX.a /data/collam/pxg/software/QDoubleSpinBox/widget/qdoublerangecontrol.o /data/collam/pxg/software/QDoubleSpinBox/widget/qdoublespinbox.o /data/collam/pxg/software/QFileChooser/widget/filechooser.o /usr/lib/libqwt.so

INCLUDEPATH	+= /data/collam/pxg/software/supermix/include /usr/include/qwt /data/collam/pxg/software/QDoubleSpinBox/widget /data/collam/pxg/software/QFileChooser/widget

HEADERS	+= arbitary_impedance.h

SOURCES	+= main.cpp

FORMS	= qtmixform.ui

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}









