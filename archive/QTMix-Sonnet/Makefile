#############################################################################
# Makefile for building: QTMix-Sonnet
# Generated by qmake (1.07a) (Qt 3.3.4) on: Tue May 17 11:07:08 2005
# Project:  QTMix-Sonnet.pro
# Template: app
# Command: $(QMAKE) -o Makefile QTMix-Sonnet.pro
#############################################################################

####### Compiler, tools and options

CC       = gcc
CXX      = g++
LEX      = flex
YACC     = yacc
CFLAGS   = -pipe -Wall -W -march=pentium4 -pipe -O2  -DQT_NO_DEBUG
CXXFLAGS = -pipe -Wall -W -march=pentium4 -pipe -O2  -DQT_NO_DEBUG
LEXFLAGS = 
YACCFLAGS= -d
INCPATH  = -I/usr/qt/3/mkspecs/linux-g++ -I. -I../../SuperMix/supermix/include -I/usr/include/qwt -I$(QTDIR)/include -I.ui/ -I. -I.moc/
LINK     = g++
LFLAGS   = 
LIBS     = $(SUBLIBS) -L$(QTDIR)/lib -L/usr/X11R6/lib /data/SuperMix/supermix/obj.i386-intel-linux/libMiX.a /usr/qt/3/lib/libqdoublespinbox.so /usr/qt/3/lib/libfilechooser.so /usr/lib/libqwt.so -lqt -lXext -lX11 -lm
AR       = ar cqs
RANLIB   = 
MOC      = $(QTDIR)/bin/moc
UIC      = $(QTDIR)/bin/uic
QMAKE    = qmake
TAR      = tar -cf
GZIP     = gzip -9f
COPY     = cp -f
COPY_FILE= $(COPY)
COPY_DIR = $(COPY) -r
INSTALL_FILE= $(COPY_FILE)
INSTALL_DIR = $(COPY_DIR)
DEL_FILE = rm -f
SYMLINK  = ln -sf
DEL_DIR  = rmdir
MOVE     = mv -f
CHK_DIR_EXISTS= test -d
MKDIR    = mkdir -p

####### Output directory

OBJECTS_DIR = .obj/

####### Files

HEADERS = 
SOURCES = main.cpp
OBJECTS = .obj/main.o \
		.obj/qtmixform.o
FORMS = qtmixform.ui
UICDECLS = .ui/qtmixform.h
UICIMPLS = .ui/qtmixform.cpp
SRCMOC   = .moc/moc_qtmixform.cpp
OBJMOC = .obj/moc_qtmixform.o
DIST	   = QTMix-Sonnet.pro
QMAKE_TARGET = QTMix-Sonnet
DESTDIR  = 
TARGET   = QTMix-Sonnet

first: all
####### Implicit rules

.SUFFIXES: .c .o .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

####### Build rules

all: Makefile $(TARGET)

$(TARGET):  $(UICDECLS) $(OBJECTS) $(OBJMOC)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJMOC) $(OBJCOMP) $(LIBS)

mocables: $(SRCMOC)
uicables: $(UICDECLS) $(UICIMPLS)

$(MOC): 
	( cd $(QTDIR)/src/moc && $(MAKE) )

Makefile: QTMix-Sonnet.pro  /usr/qt/3/mkspecs/linux-g++/qmake.conf 
	$(QMAKE) -o Makefile QTMix-Sonnet.pro
qmake: 
	@$(QMAKE) -o Makefile QTMix-Sonnet.pro

dist: 
	@mkdir -p .obj/QTMix-Sonnet && $(COPY_FILE) --parents $(SOURCES) $(HEADERS) $(FORMS) $(DIST) .obj/QTMix-Sonnet/ && $(COPY_FILE) --parents qtmixform.ui.h .obj/QTMix-Sonnet/ && ( cd `dirname .obj/QTMix-Sonnet` && $(TAR) QTMix-Sonnet.tar QTMix-Sonnet && $(GZIP) QTMix-Sonnet.tar ) && $(MOVE) `dirname .obj/QTMix-Sonnet`/QTMix-Sonnet.tar.gz . && $(DEL_FILE) -r .obj/QTMix-Sonnet

mocclean:
	-$(DEL_FILE) $(OBJMOC)
	-$(DEL_FILE) $(SRCMOC)

uiclean:
	-$(DEL_FILE) $(UICIMPLS) $(UICDECLS)

yaccclean:
lexclean:
clean: mocclean uiclean
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) $(TARGET)


FORCE:

####### Compile

.obj/main.o: main.cpp .ui/qtmixform.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/main.o main.cpp

.ui/qtmixform.h: qtmixform.ui 
	$(UIC) qtmixform.ui -o .ui/qtmixform.h

.ui/qtmixform.cpp: .ui/qtmixform.h qtmixform.ui qtmixform.ui.h 
	$(UIC) qtmixform.ui -i qtmixform.h -o .ui/qtmixform.cpp

.obj/qtmixform.o: .ui/qtmixform.cpp qtmixform.ui.h \
		.ui/qtmixform.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/qtmixform.o .ui/qtmixform.cpp

.obj/moc_qtmixform.o: .moc/moc_qtmixform.cpp  .ui/qtmixform.h 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/moc_qtmixform.o .moc/moc_qtmixform.cpp

.moc/moc_qtmixform.cpp: $(MOC) .ui/qtmixform.h
	$(MOC) .ui/qtmixform.h -o .moc/moc_qtmixform.cpp

####### Install

install:  

uninstall:  

