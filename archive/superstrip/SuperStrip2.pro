unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}



















TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on debug thread

LIBS	+= /data/SuperMix/supermix/obj.i386-intel-linux/libMiX.a /usr/qt/3/lib/libqdoublespinbox.so /usr/lib/libqwt.so
INCLUDEPATH	+= /data/SuperMix/supermix/include /usr/include/qwt

SOURCES	+= main.cpp
FORMS	= superstrip2form.ui
