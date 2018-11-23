TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release

LIBS	+= /data/SuperMix/supermix/obj.i386-intel-linux/libMiX.a /usr/qt/3/lib/libqdoublespinbox.so /usr/qt/3/lib/libfilechooser.so /usr/lib/libqwt.so

INCLUDEPATH	+= /data/SuperMix/supermix/include /usr/include/qwt

HEADERS	+= arbitary_impedance.h

SOURCES	+= main.cpp

FORMS	= qtmixform2.ui

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}




























