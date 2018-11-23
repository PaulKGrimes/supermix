# Set this to the directory where SuperMiX is installed.
SUPERMIXDIR = /d/collam/pxg/software/supermix/
MULTITONEDIR = /d/collam/pxg/software/MultiTone/

# The programs we wish to compile
EXES = MT_sub_harmonic

# Dependencies for the programs we are compiling.
DEPEND = 

# The authors recommend compiling with g++.
CC = g++

# Set any extra compile flags here.
FLAGS = -Wall -O3 -march=athlon-xp

# Set the subdirectory containing the shared library.
SMOBJDIR := obj.$(shell $(SUPERMIXDIR)/makefiles/getmachtype)
MTOBJDIR := obj.$(shell $(MULTITONEDIR)/makefiles/getmachtype)

# Set the location of the SuperMiX library.
SMLIB = libMiX.a
SUPERMIXLIB = $(SUPERMIXDIR)/$(SMOBJDIR)/$(SMLIB)
MTLIB = libMultiTone.a
MULTITONELIB = $(MULTITONEDIR)/$(MTOBJDIR)/$(MTLIB)

# Libraries to link to.  By default, link in SuperMiX and math libraries.
# Add extras to the end of this line.
LIBS =  $(MULTITONELIB) $(SUPERMIXLIB) -lm

# Set the location of the SuperMiX include files.
INCLUDE = include
SUPERMIXINCLUDE = $(SUPERMIXDIR)/$(INCLUDE)
MULTITONEINCLUDE = $(MULTITONEDIR)/$(INCLUDE)

CFLAGS = $(FLAGS) -I$(SUPERMIXINCLUDE) -I$(MULTITONEINCLUDE)

.SUFFIXES: .cc .o

# This rule specifies how to compile files ending in ".cc"
.cc:
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

all: $(EXES)

$(EXES): $(SUPERMIXLIB) $(MULTITONELIB) $(DEPEND)

clean:
	rm -f core $(EXES)
