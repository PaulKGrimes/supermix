# This is a generic supermix makefile for use on pkgpc
# Modified Paul Grimes, (pkg22@mrao.cam.ac.uk) 7/11/01
#
# Set this to the directory where SuperMiX is installed.
SUPERMIXDIR = /data/collam/pxg/software/supermix

# The programs we wish to compile
EXES = sub_harmonic

# Dependencies for the programs we are compiling.
DEPEND = 

# The authors recommend compiling with g++.
CC = g++

# Set any extra compile flags here.
FLAGS = -Wall -g -O3

# Set the subdirectory containing the shared library.
OBJDIR := obj.$(shell $(SUPERMIXDIR)/makefiles/getmachtype)

# Set the location of the SuperMiX library.
LIB = libMiX.a
SUPERMIXLIB = $(SUPERMIXDIR)/$(OBJDIR)/$(LIB)

DOTCONFDIR = /data/collam/pxg/software/dotconf-1.0.13/src
DOTCONFINC = $(DOTCONFDIR)
DOTCONFLIB = $(DOTCONFDIR)/dotconf.lo

# Libraries to link to.  By default, link in SuperMiX and math libraries.
# Add extras to the end of this line.
LIBS = $(SUPERMIXLIB) $(DOTCONFLIB)

# Set the location of the SuperMiX include files.
INCLUDE = include
SUPERMIXINCLUDE = $(SUPERMIXDIR)/$(INCLUDE)

CFLAGS = $(FLAGS) -I$(SUPERMIXINCLUDE) -I$(DOTCONFINC) -I/usr/include

.SUFFIXES: .c .o

# This rule specifies how to compile files ending in ".cc"
.c:
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

all: $(EXES)

$(EXES): $(SUPERMIXLIB) $(DEPEND)

clean:
	rm -f core $(EXES)
